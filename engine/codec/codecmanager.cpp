/****************************************************************************
 *            codecmanager.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

namespace eternal_lands
{

	Uint32 CodecManager::GlPackedPixelType::get_size() const
	{
		return __builtin_popcount(m_red_mask | m_green_mask |
			m_blue_mask | m_alpha_mask);
	}

	CodecManager::GlPackedPixelType::GlPackedPixelType(
		const Uint32 red_mask, const Uint32 green_mask,
		const Uint32 blue_mask, const Uint32 alpha_mask,
		const GLenum type)
	{
		assert((red_mask & green_mask) == 0);
		assert((red_mask & blue_mask) == 0);
		assert((red_mask & alpha_mask) == 0);
		assert((green_mask & blue_mask) == 0);
		assert((green_mask & alpha_mask) == 0);
		assert((blue_mask & alpha_mask) == 0);

		m_red_mask = red_mask;
		m_green_mask = green_mask;
		m_blue_mask = blue_mask;
		m_alpha_mask = alpha_mask;
		m_type = type;
	}

	bool CodecManager::GlPackedPixelType::has_alpha() const
	{
		return __builtin_popcount(m_alpha_mask) > 0;
	}

	CodecManager::GlFastLoadInfo CodecManager::GlPackedPixelType::build(
		const GLenum format, const bool swap_colors,
		const bool ignore_alpha) const
	{
		GlFastLoadInfo info;
		Uint32 size;

		if (swap_colors)
		{
			info.set_red_mask(m_blue_mask);
			info.set_blue_mask(m_red_mask);
		}
		else
		{
			info.set_red_mask(m_red_mask);
			info.set_blue_mask(m_blue_mask);
		}

		info.set_green_mask(m_green_mask);

		if (ignore_alpha)
		{
			info.set_alpha_mask(0x00000000);
		}
		else
		{
			info.set_alpha_mask(m_alpha_mask);
		}

		info.set_type(m_type);
		info.set_format(format);
		size = get_size();
		assert((size % 8) == 0);
		info.set_size(size / 8);
		info.set_swap_size(size / 8);
		info.set_rg_format(false);

		return info;
	}

	void CodecManager::add_packed_gl_pixel_type(
		const GlPackedPixelType &pixel_type)
	{
		GlFastLoadInfo info;

		if (pixel_type.has_alpha())
		{
			info = pixel_type.build(GL_RGBA, false, false);
			info.set_channels(4);
			m_flast_load_infos.push_back(info);
			info = pixel_type.build(GL_BGRA, true, false);
			info.set_channels(4);
			m_flast_load_infos.push_back(info);
		}
		else
		{
			info = pixel_type.build(GL_RGB, false, true);
			info.set_channels(3);
			m_flast_load_infos.push_back(info);
		}
	}

	void CodecManager::add_red_gl_pixel_type(const GLenum type,
		const Uint32 mask)
	{
		GlFastLoadInfo info;
		Uint32 bits;

		info.set_channels(1);
		info.set_rg_format(true);

		info.set_type(type);
		info.set_format(GL_RED);
		bits = __builtin_popcount(mask);
		assert((bits % 8) == 0);
		info.set_size(bits / 8);
		info.set_swap_size(bits / 8);

		info.set_red_mask(mask);
		info.set_blue_mask(0);
		info.set_green_mask(0);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(0);
		info.set_blue_mask(mask);
		info.set_green_mask(0);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(0);
		info.set_blue_mask(0);
		info.set_green_mask(mask);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(mask);
		info.set_blue_mask(mask);
		info.set_green_mask(mask);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(mask);
		info.set_blue_mask(mask);
		info.set_green_mask(mask);
		info.set_alpha_mask(mask);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(0);
		info.set_blue_mask(0);
		info.set_green_mask(0);
		info.set_alpha_mask(mask);

		m_flast_load_infos.push_back(info);
	}

	void CodecManager::add_red_green_gl_pixel_type(const GLenum type,
		const Uint32 mask)
	{
		GlFastLoadInfo info;
		Uint32 bits, green_mask;

		info.set_channels(2);
		info.set_rg_format(true);

		bits = __builtin_popcount(mask);
		green_mask = mask << bits;

		info.set_type(type);
		info.set_format(GL_RG);
		assert((bits % 8) == 0);
		info.set_size((bits / 8) * 2);
		info.set_swap_size(bits / 8);

		info.set_red_mask(mask);
		info.set_green_mask(0);
		info.set_blue_mask(0);
		info.set_alpha_mask(green_mask);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(0);
		info.set_green_mask(mask);
		info.set_blue_mask(0);
		info.set_alpha_mask(green_mask);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(0);
		info.set_green_mask(0);
		info.set_blue_mask(mask);
		info.set_alpha_mask(green_mask);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(mask);
		info.set_green_mask(mask);
		info.set_blue_mask(mask);
		info.set_alpha_mask(green_mask);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(mask);
		info.set_green_mask(green_mask);
		info.set_blue_mask(0);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);
	}

	void CodecManager::add_luminance_gl_pixel_type(const GLenum type,
		const Uint32 mask)
	{
		GlFastLoadInfo info;
		Uint32 bits;

		info.set_channels(1);
		info.set_rg_format(false);

		info.set_type(type);
		info.set_format(GL_LUMINANCE);
		bits = __builtin_popcount(mask);
		assert((bits % 8) == 0);
		info.set_size(bits / 8);
		info.set_swap_size(bits / 8);

		info.set_red_mask(mask);
		info.set_blue_mask(0);
		info.set_green_mask(0);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(0);
		info.set_blue_mask(mask);
		info.set_green_mask(0);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(0);
		info.set_blue_mask(0);
		info.set_green_mask(mask);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(mask);
		info.set_blue_mask(mask);
		info.set_green_mask(mask);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(mask);
		info.set_blue_mask(mask);
		info.set_green_mask(mask);
		info.set_alpha_mask(mask);

		m_flast_load_infos.push_back(info);
	}

	void CodecManager::add_alpha_gl_pixel_type(const GLenum type,
		const Uint32 mask)
	{
		GlFastLoadInfo info;
		Uint32 bits;

		info.set_channels(1);
		info.set_rg_format(false);

		info.set_type(type);
		info.set_format(GL_ALPHA);
		bits = __builtin_popcount(mask);
		assert((bits % 8) == 0);
		info.set_size(bits / 8);
		info.set_swap_size(bits / 8);

		info.set_red_mask(0);
		info.set_blue_mask(0);
		info.set_green_mask(0);
		info.set_alpha_mask(mask);

		m_flast_load_infos.push_back(info);
	}

	void CodecManager::add_luminance_alpha_gl_pixel_type(const GLenum type,
		const Uint32 mask)
	{
		GlFastLoadInfo info;
		Uint32 bits, green_mask;

		info.set_channels(2);
		info.set_rg_format(false);

		bits = __builtin_popcount(mask);
		green_mask = mask << bits;

		info.set_type(type);
		info.set_format(GL_LUMINANCE_ALPHA);
		assert((bits % 8) == 0);
		info.set_size((bits / 8) * 2);
		info.set_swap_size(bits / 8);

		info.set_red_mask(mask);
		info.set_green_mask(0);
		info.set_blue_mask(0);
		info.set_alpha_mask(green_mask);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(0);
		info.set_green_mask(mask);
		info.set_blue_mask(0);
		info.set_alpha_mask(green_mask);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(0);
		info.set_green_mask(0);
		info.set_blue_mask(mask);
		info.set_alpha_mask(green_mask);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(mask);
		info.set_green_mask(mask);
		info.set_blue_mask(mask);
		info.set_alpha_mask(green_mask);

		m_flast_load_infos.push_back(info);

		info.set_red_mask(mask);
		info.set_green_mask(green_mask);
		info.set_blue_mask(0);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);
	}

	void CodecManager::add_color_gl_pixel_type(const GLenum type,
		const Uint32 mask)
	{
		GlFastLoadInfo info;
		Uint32 bits;

		info.set_channels(3);
		info.set_rg_format(false);

		info.set_type(type);
		info.set_format(GL_RGB);
		bits = __builtin_popcount(mask);
		assert((bits % 8) == 0);
		info.set_size((bits / 8) * 3);
		info.set_swap_size(bits / 8);

		info.set_red_mask(mask);
		info.set_green_mask(mask << bits);
		info.set_blue_mask(mask << (bits * 2));
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);

		info.set_format(GL_BGR);

		info.set_red_mask(mask << (bits * 2));
		info.set_green_mask(mask << bits);
		info.set_blue_mask(mask);
		info.set_alpha_mask(0);

		m_flast_load_infos.push_back(info);
	}

	void CodecManager::add_color_alpha_gl_pixel_type(const GLenum type,
		const Uint32 mask)
	{
		GlFastLoadInfo info;
		Uint32 bits;

		info.set_channels(4);
		info.set_rg_format(false);

		info.set_type(type);
		info.set_format(GL_RGBA);
		bits = __builtin_popcount(mask);
		assert((bits % 8) == 0);
		info.set_size((bits / 8) * 4);
		info.set_swap_size(bits / 8);

		info.set_red_mask(mask);
		info.set_green_mask(mask << bits);
		info.set_blue_mask(mask << (bits * 2));
		info.set_alpha_mask(mask << (bits * 3));

		m_flast_load_infos.push_back(info);

		info.set_format(GL_BGRA);

		info.set_red_mask(mask << (bits * 2));
		info.set_green_mask(mask << bits);
		info.set_blue_mask(mask);
		info.set_alpha_mask(mask << (bits * 3));

		m_flast_load_infos.push_back(info);
	}

	void CodecManager::add_gl_pixel_types()
	{
		add_packed_gl_pixel_type(GlPackedPixelType(0xFFC00000, 0x003FF000, 0x00000FFC, 0x00000003, GL_UNSIGNED_INT_10_10_10_2));
		add_packed_gl_pixel_type(GlPackedPixelType(0x000003FF, 0x000FFC00, 0x3FF00000, 0xC0000000, GL_UNSIGNED_INT_2_10_10_10_REV));
		add_packed_gl_pixel_type(GlPackedPixelType(0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF, GL_UNSIGNED_INT_8_8_8_8));
		add_packed_gl_pixel_type(GlPackedPixelType(0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000, GL_UNSIGNED_INT_8_8_8_8_REV));
		add_packed_gl_pixel_type(GlPackedPixelType(0xF000, 0x0F00, 0x00F0, 0x000F, GL_UNSIGNED_SHORT_4_4_4_4));
		add_packed_gl_pixel_type(GlPackedPixelType(0x000F, 0x00F0, 0x0F00, 0xF000, GL_UNSIGNED_SHORT_4_4_4_4_REV));
		add_packed_gl_pixel_type(GlPackedPixelType(0xF800, 0x07C0, 0x003E, 0x0001, GL_UNSIGNED_SHORT_5_5_5_1));
		add_packed_gl_pixel_type(GlPackedPixelType(0x001F, 0x03E0, 0x7C00, 0x8000, GL_UNSIGNED_SHORT_1_5_5_5_REV));
		add_packed_gl_pixel_type(GlPackedPixelType(0xF800, 0x07E0, 0x001F, 0x0000, GL_UNSIGNED_SHORT_5_6_5));
		add_packed_gl_pixel_type(GlPackedPixelType(0x001F, 0x07E0, 0xF800, 0x0000, GL_UNSIGNED_SHORT_5_6_5_REV));
		add_packed_gl_pixel_type(GlPackedPixelType(0xE0, 0x1C, 0x03, 0x00, GL_UNSIGNED_BYTE_3_3_2));
		add_packed_gl_pixel_type(GlPackedPixelType(0x07, 0x38, 0xC0, 0x00, GL_UNSIGNED_BYTE_2_3_3_REV));
		add_color_gl_pixel_type(GL_UNSIGNED_BYTE, 0xFF);
		add_color_alpha_gl_pixel_type(GL_UNSIGNED_BYTE, 0xFF);
		add_red_gl_pixel_type(GL_UNSIGNED_BYTE, 0xFF);
		add_red_green_gl_pixel_type(GL_UNSIGNED_BYTE, 0xFF);
		add_luminance_gl_pixel_type(GL_UNSIGNED_BYTE, 0xFF);
		add_alpha_gl_pixel_type(GL_UNSIGNED_BYTE, 0xFF);
		add_luminance_alpha_gl_pixel_type(GL_UNSIGNED_BYTE, 0xFF);
	}

	CodecManager::CodecManager()
	{
		add_gl_pixel_types();
		assert(m_flast_load_infos.size() > 0);
	}

	CodecManager::~CodecManager() throw()
	{
	}

	bool CodecManager::has_color_bit_mask(const GLenum type,
		const GLenum format, Uint32 &red_mask, Uint32 &green_mask,
		Uint32 &blue_mask, Uint32 &alpha_mask, Uint32 &size,
		Uint32 &swap_size) const
	{
		assert(m_flast_load_infos.size() > 0);

		BOOST_FOREACH(const GlFastLoadInfo &info, m_flast_load_infos)
		{
			if ((info.get_type() == type) &&
				(info.get_format() == format))
			{
				red_mask = info.get_red_mask();
				green_mask = info.get_green_mask();
				blue_mask = info.get_blue_mask();
				alpha_mask = info.get_alpha_mask();
				size = info.get_size();
				swap_size = info.get_swap_size();

				return true;
			}
		}
		return false;
	}

	bool CodecManager::is_fast_load_supported(const Uint32 red_mask,
		const Uint32 green_mask, const Uint32 blue_mask,
		const Uint32 alpha_mask, const bool rg_formats, GLenum &type,
		GLenum &format, Uint32 &size, Uint32 &swap_size) const
	{
		assert(m_flast_load_infos.size() > 0);

		BOOST_FOREACH(const GlFastLoadInfo &info, m_flast_load_infos)
		{
			if ((info.get_red_mask() == red_mask) &&
				(info.get_green_mask() == green_mask) &&
				(info.get_blue_mask() == blue_mask) &&
				(info.get_alpha_mask() == alpha_mask) &&
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

	bool CodecManager::is_fast_load_supported(const Uint32 red_mask,
		const Uint32 green_mask, const Uint32 blue_mask,
		const Uint32 alpha_mask, const bool rg_formats) const
	{
		assert(m_flast_load_infos.size() > 0);

		BOOST_FOREACH(const GlFastLoadInfo &info, m_flast_load_infos)
		{
			if ((info.get_red_mask() == red_mask) &&
				(info.get_green_mask() == green_mask) &&
				(info.get_blue_mask() == blue_mask) &&
				(info.get_alpha_mask() == alpha_mask) &&
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
		const bool rg_formats) const
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
					rg_formats);
			}
		}

		EL_THROW_MESSAGE_EXCEPTION(UTF8("Can't find file '%1%' or "
			"'%2%'[.dds|.png|.jpg|.jpeg]"), name % base_name,
			FileNotFoundException()
				<< boost::errinfo_file_name(name));
	}

	ImageSharedPtr CodecManager::load_image(const ReaderSharedPtr &reader,
		const ImageCompressionTypeSet &compressions,
		const bool rg_formats) const
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
			return DdsImage::load_image(*this, reader,
				compressions, rg_formats);
		}

		if (PngImage::check_load(magic))
		{
			return PngImage::load_image(reader, rg_formats);
		}

		if (JpegImage::check_load(magic))
		{
			return JpegImage::load_image(reader, rg_formats);
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
		const bool rg_formats, TextureFormatType &texture_format,
		Uint32Array3 &sizes, Uint16 &mipmaps) const
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
				get_image_information(reader, rg_formats,
					texture_format, sizes, mipmaps);
				return;
			}
		}

		EL_THROW_MESSAGE_EXCEPTION(UTF8("Can't find file '%1%' or "
			"'%2%'[.dds|.png|.jpg|.jpeg]"), name % base_name,
			FileNotFoundException()
				<< boost::errinfo_file_name(name));
	}

	void CodecManager::get_image_information(const ReaderSharedPtr &reader,
		const bool rg_formats, TextureFormatType &texture_format,
		Uint32Array3 &sizes, Uint16 &mipmaps) const
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
				texture_format, sizes, mipmaps);

			return;
		}

		if (PngImage::check_load(magic))
		{
			PngImage::get_image_information(reader, rg_formats,
				texture_format, sizes, mipmaps);

			return;
		}

		if (JpegImage::check_load(magic))
		{
			JpegImage::get_image_information(reader, rg_formats,
				texture_format, sizes, mipmaps);

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
		StringVector &extensions) const
	{
		extensions.clear();

		extensions.push_back(String(UTF8("*.dds")));
		extensions.push_back(String(UTF8("*.png")));
		extensions.push_back(String(UTF8("*.jpg")));
		extensions.push_back(String(UTF8("*.jpeg")));
	}

}
