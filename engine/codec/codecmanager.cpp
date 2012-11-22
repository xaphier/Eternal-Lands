/****************************************************************************
 *            codecmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "codecmanager.hpp"
#include "image.hpp"
#include "reader.hpp"
#include "logging.hpp"
#include "exceptions.hpp"
#include "ddsimage.hpp"
#include "pngimage.hpp"
#include "jpegimage.hpp"
#include "filesystem.hpp"
#include "writer.hpp"

namespace eternal_lands
{

	namespace
	{

		class GlFastLoadInfo
		{
			private:
				glm::uvec4 m_masks;
				Uint32 m_size;
				Uint32 m_swap_size;
				GLenum m_type;
				GLenum m_format;
				Uint16 m_channels;
				bool m_rg_format;
				bool m_integer_format;

			public:
				GlFastLoadInfo(const glm::uvec4 &masks,
					const GLenum type, const GLenum format,
					const bool swap_red_blue,
					const bool integer_format);
				GlFastLoadInfo(const glm::uvec4 &masks,
					const GLenum type, const GLenum format,
					const bool swap_red_blue,
					const bool rg_format,
					const bool integer_format);

				inline const glm::uvec4 &get_masks() const
					noexcept
				{
					return m_masks;
				}

				inline Uint32 get_size() const noexcept
				{
					return m_size;
				}

				inline Uint32 get_swap_size() const noexcept
				{
					return m_swap_size;
				}

				inline GLenum get_type() const noexcept
				{
					return m_type;
				}

				inline GLenum get_format() const noexcept
				{
					return m_format;
				}

				inline Uint16 get_channels() const noexcept
				{
					return m_channels;
				}

				inline bool get_rg_format() const noexcept
				{
					return m_rg_format;
				}

				inline bool get_integer_format() const noexcept
				{
					return m_integer_format;
				}

				inline void set_masks(const glm::uvec4 &masks)
					noexcept
				{
					m_masks = masks;
				}

				inline void set_size(const Uint32 size)
					noexcept
				{
					m_size = size;
				}

				inline void set_swap_size(
					const Uint32 swap_size) noexcept
				{
					m_swap_size = swap_size;
				}

				inline void set_type(const GLenum type)
					noexcept
				{
					m_type = type;
				}

				inline void set_format(const GLenum format)
					noexcept
				{
					m_format = format;
				}

				inline void set_rg_format(const bool rg_format)
					noexcept
				{
					m_rg_format = rg_format;
				}

				inline void set_integer_format(
					const bool integer_format) noexcept
				{
					m_integer_format = integer_format;
				}

				inline void set_channels(const Uint16 channels)
					noexcept
				{
					m_channels = channels;
				}

		};

		GlFastLoadInfo::GlFastLoadInfo(const glm::uvec4 &masks,
			const GLenum type, const GLenum format,
			const bool swap_red_blue, const bool integer_format)
		{
			Uint32 i;

			m_masks = masks;

			if (swap_red_blue)
			{
				std::swap(m_masks.r, m_masks.b);
			}

			assert(((m_masks[0] & m_masks[1]) == 0) || (m_masks[0] == m_masks[1]));
			assert(((m_masks[0] & m_masks[2]) == 0) || (m_masks[0] == m_masks[2]));
			assert(((m_masks[0] & m_masks[3]) == 0) || (m_masks[0] == m_masks[3]));
			assert(((m_masks[1] & m_masks[2]) == 0) || (m_masks[1] == m_masks[2]));
			assert(((m_masks[1] & m_masks[3]) == 0) || (m_masks[1] == m_masks[3]));
			assert(((m_masks[2] & m_masks[3]) == 0) || (m_masks[2] == m_masks[3]));

			assert(m_masks[0] != 0);
			assert(m_masks[1] != 0);
			assert(m_masks[2] != 0);

			assert((__builtin_popcount(m_masks[0] | m_masks[1] |
				m_masks[2] | m_masks[3]) % 8) == 0);

			assert(__builtin_popcount(m_masks[0] | m_masks[1] |
				m_masks[2] | m_masks[3]) != 0);

			m_size = __builtin_popcount(m_masks[0] | m_masks[1] |
				m_masks[2] | m_masks[3]) / 8;

			m_swap_size = m_size;
			m_type = type;
			m_format = format;
			m_channels = 0;
			m_rg_format = false;
			m_integer_format = integer_format;

			for (i = 0; i < 4; ++i)
			{
				if (m_masks[i] != 0)
				{
					m_channels++;
				}
			}
		}

		GlFastLoadInfo::GlFastLoadInfo(const glm::uvec4 &masks,
			const GLenum type, const GLenum format,
			const bool swap_red_blue, const bool rg_format,
			const bool integer_format)
		{
			Uint32 i;

			m_masks = masks;

			if (swap_red_blue)
			{
				std::swap(m_masks.r, m_masks.b);
			}

			assert(((m_masks[0] & m_masks[1]) == 0) || (m_masks[0] == m_masks[1]));
			assert(((m_masks[0] & m_masks[2]) == 0) || (m_masks[0] == m_masks[2]));
			assert(((m_masks[0] & m_masks[3]) == 0) || (m_masks[0] == m_masks[3]));
			assert(((m_masks[1] & m_masks[2]) == 0) || (m_masks[1] == m_masks[2]));
			assert(((m_masks[1] & m_masks[3]) == 0) || (m_masks[1] == m_masks[3]));
			assert(((m_masks[2] & m_masks[3]) == 0) || (m_masks[2] == m_masks[3]));

			assert((__builtin_popcount(m_masks[0] | m_masks[1] |
				m_masks[2] | m_masks[3]) % 8) == 0);

			assert(__builtin_popcount(m_masks[0] | m_masks[1] |
				m_masks[2] | m_masks[3]) != 0);

			m_size = __builtin_popcount(m_masks[0] | m_masks[1] |
				m_masks[2] | m_masks[3]) / 8;

			m_type = type;
			m_format = format;
			m_channels = 0;
			m_rg_format = rg_format;
			m_integer_format = integer_format;

			for (i = 0; i < 4; ++i)
			{
				if (m_masks[i] != 0)
				{
					m_channels++;
				}
			}

			m_swap_size = m_size / m_channels;
		}

		const GlFastLoadInfo flast_load_infos[] =
		{
			GlFastLoadInfo(glm::uvec4(0xFFC00000, 0x003FF000, 0x00000FFC, 0x00000003), GL_UNSIGNED_INT_10_10_10_2, GL_RGBA, false, false),
			GlFastLoadInfo(glm::uvec4(0xFFC00000, 0x003FF000, 0x00000FFC, 0x00000003), GL_UNSIGNED_INT_10_10_10_2, GL_BGRA, true, false),
			GlFastLoadInfo(glm::uvec4(0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000), GL_UNSIGNED_INT_2_10_10_10_REV, GL_RGBA, false, false),
			GlFastLoadInfo(glm::uvec4(0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000), GL_UNSIGNED_INT_2_10_10_10_REV, GL_BGRA, true, false),
			GlFastLoadInfo(glm::uvec4(0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF), GL_UNSIGNED_INT_8_8_8_8, GL_RGBA, false, false),
			GlFastLoadInfo(glm::uvec4(0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF), GL_UNSIGNED_INT_8_8_8_8, GL_BGRA, true, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000), GL_UNSIGNED_INT_8_8_8_8_REV, GL_RGBA, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000), GL_UNSIGNED_INT_8_8_8_8_REV, GL_BGRA, true, false),
			GlFastLoadInfo(glm::uvec4(0xF000, 0x0F00, 0x00F0, 0x000F), GL_UNSIGNED_SHORT_4_4_4_4, GL_RGBA, false, false),
			GlFastLoadInfo(glm::uvec4(0xF000, 0x0F00, 0x00F0, 0x000F), GL_UNSIGNED_SHORT_4_4_4_4, GL_BGRA, true, false),
			GlFastLoadInfo(glm::uvec4(0x000F, 0x00F0, 0x0F00, 0xF000), GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_RGBA, false, false),
			GlFastLoadInfo(glm::uvec4(0x000F, 0x00F0, 0x0F00, 0xF000), GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_BGRA, true, false),
			GlFastLoadInfo(glm::uvec4(0xF800, 0x07C0, 0x003E, 0x0001), GL_UNSIGNED_SHORT_5_5_5_1, GL_RGBA, false, false),
			GlFastLoadInfo(glm::uvec4(0xF800, 0x07C0, 0x003E, 0x0001), GL_UNSIGNED_SHORT_5_5_5_1, GL_BGRA, true, false),
			GlFastLoadInfo(glm::uvec4(0x001F, 0x03E0, 0x7C00, 0x8000), GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGBA, false, false),
			GlFastLoadInfo(glm::uvec4(0x001F, 0x03E0, 0x7C00, 0x8000), GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_BGRA, true, false),
			GlFastLoadInfo(glm::uvec4(0xF800, 0x07E0, 0x001F, 0x0000), GL_UNSIGNED_SHORT_5_6_5, GL_RGB, false, false),
			GlFastLoadInfo(glm::uvec4(0x001F, 0x07E0, 0xF800, 0x0000), GL_UNSIGNED_SHORT_5_6_5_REV, GL_RGB, false, false),
			GlFastLoadInfo(glm::uvec4(0xE0, 0x1C, 0x03, 0x00), GL_UNSIGNED_BYTE_3_3_2, GL_RGB, false, false),
			GlFastLoadInfo(glm::uvec4(0x07, 0x38, 0xC0, 0x00), GL_UNSIGNED_BYTE_2_3_3_REV, GL_RGB, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x00000000, 0x00000000, 0x00000000), GL_UNSIGNED_BYTE, GL_RED, false, true, false),
			GlFastLoadInfo(glm::uvec4(0x00000000, 0x00000000, 0x00000000, 0x000000FF), GL_UNSIGNED_BYTE, GL_RED, false, true, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF), GL_UNSIGNED_BYTE, GL_RED, false, true, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x000000FF, 0x000000FF, 0x00000000), GL_UNSIGNED_BYTE, GL_RED, false, true, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x00000000, 0x00000000, 0x00000000), GL_UNSIGNED_BYTE, GL_LUMINANCE, false, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x000000FF, 0x000000FF, 0x00000000), GL_UNSIGNED_BYTE, GL_LUMINANCE, false, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF), GL_UNSIGNED_BYTE, GL_LUMINANCE, false, false, false),
			GlFastLoadInfo(glm::uvec4(0x00000000, 0x00000000, 0x00000000, 0x000000FF), GL_UNSIGNED_BYTE, GL_ALPHA, false, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00000000, 0x00000000), GL_UNSIGNED_BYTE, GL_RG, false, true, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x00000000, 0x00000000, 0x0000FF00), GL_UNSIGNED_BYTE, GL_RG, false, true, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x000000FF, 0x000000FF, 0x0000FF00), GL_UNSIGNED_BYTE, GL_RG, false, true, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00000000, 0x00000000), GL_UNSIGNED_BYTE, GL_LUMINANCE_ALPHA, false, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x00000000, 0x00000000, 0x0000FF00), GL_UNSIGNED_BYTE, GL_LUMINANCE_ALPHA, false, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x000000FF, 0x000000FF, 0x0000FF00), GL_UNSIGNED_BYTE, GL_LUMINANCE_ALPHA, false, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000), GL_UNSIGNED_BYTE, GL_RGB, false, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000), GL_UNSIGNED_BYTE, GL_BGR, true, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000), GL_UNSIGNED_BYTE, GL_RGBA, false, false, false),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000), GL_UNSIGNED_BYTE, GL_BGRA, true, false, false),
			GlFastLoadInfo(glm::uvec4(0xFFC00000, 0x003FF000, 0x00000FFC, 0x00000003), GL_UNSIGNED_INT_10_10_10_2, GL_RGBA_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0xFFC00000, 0x003FF000, 0x00000FFC, 0x00000003), GL_UNSIGNED_INT_10_10_10_2, GL_BGRA_INTEGER, true, true),
			GlFastLoadInfo(glm::uvec4(0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000), GL_UNSIGNED_INT_2_10_10_10_REV, GL_RGBA_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000), GL_UNSIGNED_INT_2_10_10_10_REV, GL_BGRA_INTEGER, true, true),
			GlFastLoadInfo(glm::uvec4(0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF), GL_UNSIGNED_INT_8_8_8_8, GL_RGBA_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF), GL_UNSIGNED_INT_8_8_8_8, GL_BGRA_INTEGER, true, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000), GL_UNSIGNED_INT_8_8_8_8_REV, GL_RGBA_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000), GL_UNSIGNED_INT_8_8_8_8_REV, GL_BGRA_INTEGER, true, true),
			GlFastLoadInfo(glm::uvec4(0xF000, 0x0F00, 0x00F0, 0x000F), GL_UNSIGNED_SHORT_4_4_4_4, GL_RGBA_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0xF000, 0x0F00, 0x00F0, 0x000F), GL_UNSIGNED_SHORT_4_4_4_4, GL_BGRA_INTEGER, true, true),
			GlFastLoadInfo(glm::uvec4(0x000F, 0x00F0, 0x0F00, 0xF000), GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_RGBA_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0x000F, 0x00F0, 0x0F00, 0xF000), GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_BGRA_INTEGER, true, true),
			GlFastLoadInfo(glm::uvec4(0xF800, 0x07C0, 0x003E, 0x0001), GL_UNSIGNED_SHORT_5_5_5_1, GL_RGBA_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0xF800, 0x07C0, 0x003E, 0x0001), GL_UNSIGNED_SHORT_5_5_5_1, GL_BGRA_INTEGER, true, true),
			GlFastLoadInfo(glm::uvec4(0x001F, 0x03E0, 0x7C00, 0x8000), GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_RGBA_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0x001F, 0x03E0, 0x7C00, 0x8000), GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_BGRA_INTEGER, true, true),
			GlFastLoadInfo(glm::uvec4(0xF800, 0x07E0, 0x001F, 0x0000), GL_UNSIGNED_SHORT_5_6_5, GL_RGB_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0x001F, 0x07E0, 0xF800, 0x0000), GL_UNSIGNED_SHORT_5_6_5_REV, GL_RGB_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0xE0, 0x1C, 0x03, 0x00), GL_UNSIGNED_BYTE_3_3_2, GL_RGB_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0x07, 0x38, 0xC0, 0x00), GL_UNSIGNED_BYTE_2_3_3_REV, GL_RGB_INTEGER, false, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x00000000, 0x00000000, 0x00000000), GL_UNSIGNED_BYTE, GL_RED_INTEGER, false, true, true),
			GlFastLoadInfo(glm::uvec4(0x00000000, 0x00000000, 0x00000000, 0x000000FF), GL_UNSIGNED_BYTE, GL_RED_INTEGER, false, true, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF), GL_UNSIGNED_BYTE, GL_RED_INTEGER, false, true, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x000000FF, 0x000000FF, 0x00000000), GL_UNSIGNED_BYTE, GL_RED_INTEGER, false, true, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00000000, 0x00000000), GL_UNSIGNED_BYTE, GL_RG_INTEGER, false, true, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x00000000, 0x00000000, 0x0000FF00), GL_UNSIGNED_BYTE, GL_RG_INTEGER, false, true, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x000000FF, 0x000000FF, 0x0000FF00), GL_UNSIGNED_BYTE, GL_RG_INTEGER, false, true, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000), GL_UNSIGNED_BYTE, GL_RGB_INTEGER, false, false, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0x00000000), GL_UNSIGNED_BYTE, GL_BGR_INTEGER, true, false, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000), GL_UNSIGNED_BYTE, GL_RGBA_INTEGER, false, false, true),
			GlFastLoadInfo(glm::uvec4(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000), GL_UNSIGNED_BYTE, GL_BGRA_INTEGER, true, false, true)
		};

	}

	bool CodecManager::has_color_bit_mask(const GLenum type,
		const GLenum format, glm::uvec4 &masks, Uint32 &size,
		Uint32 &swap_size, bool &integer_format)
	{
		BOOST_FOREACH(const GlFastLoadInfo &info, flast_load_infos)
		{
			if ((info.get_type() == type) &&
				(info.get_format() == format))
			{
				masks = info.get_masks();
				size = info.get_size();
				swap_size = info.get_swap_size();
				integer_format = info.get_integer_format();

				return true;
			}
		}
		return false;
	}

	bool CodecManager::is_fast_load_supported(const glm::uvec4 &masks,
		const bool rg_formats, const bool integer_format, GLenum &type,
		GLenum &format, Uint32 &size, Uint32 &swap_size)
	{
		BOOST_FOREACH(const GlFastLoadInfo &info, flast_load_infos)
		{
			if ((info.get_integer_format() == integer_format) &&
				(info.get_masks() == masks) &&
				((info.get_rg_format() == rg_formats) ||
				(info.get_channels() > 2)))
				
			{
				type = info.get_type();
				format = info.get_format();
				size = info.get_size();
				swap_size = info.get_swap_size();

				return true;
			}
		}
		return false;
	}

	bool CodecManager::is_fast_load_supported(const glm::uvec4 &masks,
		const bool rg_formats, const bool integer_format)
	{
		BOOST_FOREACH(const GlFastLoadInfo &info, flast_load_infos)
		{
			if ((info.get_integer_format() == integer_format) &&
				(info.get_masks() == masks) &&
				((info.get_rg_format() == rg_formats) ||
				(info.get_channels() > 2)))
			{
				return true;
			}
		}
		return false;
	}

	ImageSharedPtr CodecManager::load_image(const String &name,
		const FileSystemSharedPtr &file_system,
		const ImageCompressionTypeSet &compressions,
		const bool rg_formats, const bool sRGB,
		const bool merge_layers)
	{
		Uint8Array32 magic;
		boost::array<String, 5> file_names;
		String base_name;
		ReaderSharedPtr reader;

		base_name = file_system->get_name_without_extension(name);

		file_names[0] = name;
		file_names[1] = base_name.get() + UTF8(".dds");
		file_names[2] = base_name.get() + UTF8(".png");
		file_names[3] = base_name.get() + UTF8(".jpg");
		file_names[4] = base_name.get() + UTF8(".jpeg");

		BOOST_FOREACH(const String &file_name, file_names)
		{
			if (!file_system->get_file_readable(file_name))
			{
				continue;
			}

			reader = file_system->get_file(file_name);

			reader->set_position(0);

			BOOST_FOREACH(Uint8 &value, magic)
			{
				value = reader->read_u8();
			}

			reader->set_position(0);

			if (DdsImage::check_load(magic) ||
				PngImage::check_load(magic) ||
				JpegImage::check_load(magic))
			{
				return load_image(reader, compressions,
					rg_formats, sRGB, merge_layers);
			}
		}

		EL_THROW_MESSAGE_EXCEPTION(UTF8("Can't find file '%1%' or "
			"'%2%'[.dds|.png|.jpg|.jpeg]"), name % base_name,
			FileNotFoundException()
				<< boost::errinfo_file_name(name));
	}

	ImageSharedPtr CodecManager::load_image(const ReaderSharedPtr &reader,
		const ImageCompressionTypeSet &compressions,
		const bool rg_formats, const bool sRGB,
		const bool merge_layers)
	{
		Uint8Array32 magic;
		StringStream str;

		reader->set_position(0);

		BOOST_FOREACH(Uint8 &value, magic)
		{
			value = reader->read_u8();
		}

		reader->set_position(0);

		if (DdsImage::check_load(magic))
		{
			return DdsImage::load_image(reader, compressions,
				rg_formats, sRGB, merge_layers);
		}

		if (PngImage::check_load(magic))
		{
			return PngImage::load_image(reader, rg_formats, sRGB);
		}

		if (JpegImage::check_load(magic))
		{
			return JpegImage::load_image(reader, rg_formats, sRGB);
		}

		BOOST_FOREACH(const Uint8 value, magic)
		{
			str << value << UTF8("(0x") << std::hex;
			str << static_cast<Uint16>(value) << UTF8(") ");
			str << std::dec;
		}

		EL_THROW_EXCEPTION(UnkownFormatException()
			<< boost::errinfo_file_name(reader->get_name())
			<< errinfo_string_value(str.str()));
	}

	void CodecManager::get_image_information(const String &name,
		const FileSystemSharedPtr &file_system,
		const bool rg_formats, const bool sRGB,
		TextureFormatType &texture_format, glm::uvec3 &size,
		Uint16 &mipmaps, bool &cube_map, bool &array)
	{
		Uint8Array32 magic;
		boost::array<String, 5> file_names;
		String base_name;
		ReaderSharedPtr reader;

		base_name = file_system->get_name_without_extension(name);

		file_names[0] = name;
		file_names[1] = base_name.get() + UTF8(".dds");
		file_names[2] = base_name.get() + UTF8(".png");
		file_names[3] = base_name.get() + UTF8(".jpg");
		file_names[4] = base_name.get() + UTF8(".jpeg");

		BOOST_FOREACH(const String &file_name, file_names)
		{
			if (!file_system->get_file_readable(file_name))
			{
				continue;
			}

			reader = file_system->get_file(file_name);

			reader->set_position(0);

			BOOST_FOREACH(Uint8 &value, magic)
			{
				value = reader->read_u8();
			}

			reader->set_position(0);

			if (DdsImage::check_load(magic) ||
				PngImage::check_load(magic) ||
				JpegImage::check_load(magic))
			{
				get_image_information(reader, rg_formats, sRGB,
					texture_format, size, mipmaps,
					cube_map, array);

				return;
			}
		}

		EL_THROW_MESSAGE_EXCEPTION(UTF8("Can't find file '%1%' or "
			"'%2%'[.dds|.png|.jpg|.jpeg]"), name % base_name,
			FileNotFoundException()
				<< boost::errinfo_file_name(name));
	}

	void CodecManager::get_image_information(const ReaderSharedPtr &reader,
		const bool rg_formats, const bool sRGB,
		TextureFormatType &texture_format, glm::uvec3 &size,
		Uint16 &mipmaps, bool &cube_map, bool &array)
	{
		Uint8Array32 magic;
		StringStream str;

		reader->set_position(0);

		BOOST_FOREACH(Uint8 &value, magic)
		{
			value = reader->read_u8();
		}

		reader->set_position(0);

		if (DdsImage::check_load(magic))
		{
			DdsImage::get_image_information(reader, rg_formats,
				sRGB, texture_format, size, mipmaps,
				cube_map, array);

			return;
		}

		if (PngImage::check_load(magic))
		{
			PngImage::get_image_information(reader, rg_formats,
				sRGB, texture_format, size, mipmaps, cube_map,
				array);

			return;
		}

		if (JpegImage::check_load(magic))
		{
			JpegImage::get_image_information(reader, rg_formats,
				sRGB, texture_format, size, mipmaps, cube_map,
				array);

			return;
		}

		BOOST_FOREACH(const Uint8 value, magic)
		{
			str << value << UTF8("(0x") << std::hex;
			str << static_cast<Uint16>(value) << UTF8(") ");
			str << std::dec;
		}

		EL_THROW_EXCEPTION(UnkownFormatException()
			<< boost::errinfo_file_name(reader->get_name())
			<< errinfo_string_value(str.str()));
	}

	void CodecManager::get_supported_file_extensions(
		StringVector &extensions)
	{
		extensions.clear();

		extensions.push_back(String(UTF8("*.dds")));
		extensions.push_back(String(UTF8("*.png")));
		extensions.push_back(String(UTF8("*.jpg")));
		extensions.push_back(String(UTF8("*.jpeg")));
	}

	void CodecManager::save_image_as_png(const ImageSharedPtr &image,
		const WriterSharedPtr &writer)
	{
		PngImage::save_image(image, writer);
	}

	void CodecManager::save_image_as_jpeg(const ImageSharedPtr &image,
		const WriterSharedPtr &writer)
	{
		JpegImage::save_image(image, writer);
	}

	void CodecManager::save_image_as_dds(const ImageSharedPtr &image,
		const WriterSharedPtr &writer)
	{
		DdsImage::save_image(image, writer);
	}

	void CodecManager::save_image_as_dds_dxt10(const ImageSharedPtr &image,
		const WriterSharedPtr &writer)
	{
		DdsImage::save_image_dxt10(image, writer);
	}

	void CodecManager::save_image_as_png(const ImageSharedPtr &image,
		const String &name)
	{
		WriterSharedPtr writer;

		writer = boost::make_shared<Writer>(name);

		save_image_as_png(image, writer);
	}

	void CodecManager::save_image_as_jpeg(const ImageSharedPtr &image,
		const String &name)
	{
		WriterSharedPtr writer;

		writer = boost::make_shared<Writer>(name);

		save_image_as_jpeg(image, writer);
	}

	void CodecManager::save_image_as_dds(const ImageSharedPtr &image,
		const String &name)
	{
		WriterSharedPtr writer;

		writer = boost::make_shared<Writer>(name);

		save_image_as_dds(image, writer);
	}

	void CodecManager::save_image_as_dds_dxt10(const ImageSharedPtr &image,
		const String &name)
	{
		WriterSharedPtr writer;

		writer = boost::make_shared<Writer>(name);

		save_image_as_dds_dxt10(image, writer);
	}

}
