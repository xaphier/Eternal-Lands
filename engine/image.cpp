/****************************************************************************
 *            image.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "image.hpp"
#include "packtool.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	namespace
	{

		template <typename T>
		T swizzel_channels(const GLenum format, const T &data)
		{
			switch (format)
			{
				case GL_RED:
				case GL_ALPHA:
				case GL_LUMINANCE:
				case GL_RG:
				case GL_LUMINANCE_ALPHA:
				case GL_RGB:
					return data;
				case GL_BGR:
					return T(data.b, data.g, data.r,
						data.a);
				case GL_RGBA:
					return data;
				case GL_BGRA:
					return T(data.b, data.g, data.r,
						data.a);
				case GL_NONE:
				default:
					return data;
			}
		}

	}

	Uint32 Image::get_total_size() const
	{
		Uint32 i, face_count, size;

		size = 0;

		face_count = get_face_count();

		for (i = 0; i <= get_mipmap_count(); ++i)
		{
			size += get_size(i) * face_count;
		}

		return size;
	}

	Uint32 Image::get_offset(const Uint16 face, const Uint16 mipmap) const
	{
		Uint32 i, face_count, offset;

		offset = 0;

		face_count = get_face_count();

		for (i = 0; i < mipmap; ++i)
		{
			offset += get_size(i) * face_count;
		}

		offset += get_size(mipmap) * face;

		return offset;
	}

	Uint32 Image::get_size(const Uint16 mipmap) const
	{
		Uint32 width, height, depth;

		assert(mipmap <= get_mipmap_count());

		width = std::max(static_cast<Uint32>(1),
			get_width() >> mipmap);
		height = std::max(static_cast<Uint32>(1),
			get_height() >> mipmap);
		depth = std::max(static_cast<Uint32>(1),
			get_depth() >> mipmap);

		assert(width > 0);
		assert(height > 0);
		assert(depth > 0);

		return get_size(width, height, depth);
	}

	Uint32 Image::get_size(const Uint32 width, const Uint32 height,
		const Uint32 depth) const
	{
		if (get_compressed())
		{
			return get_block_size() * ((width + 3) / 4) *
				((height + 3) / 4) * depth;
		}
		else
		{
			return (get_pixel_size() / 8) * width * height * depth;
		}
	}

	Uint32 Image::get_size(const Uint32 width, const Uint32 height,
		const Uint32 depth, const TextureFormatType texture_format)
	{
		Uint32 pixel_size;

		pixel_size = TextureFormatUtil::get_size(texture_format);

		if (TextureFormatUtil::get_compressed(texture_format))
		{
			return get_block_size(pixel_size) * ((width + 3) / 4) *
				((height + 3) / 4) * depth;
		}
		else
		{
			return (pixel_size / 8) * width * height * depth;
		}
	}

	Uint32 Image::get_buffer_pixel_offset(const Uint32 x, const Uint32 y,
		const Uint32 z, const Uint16 mipmap) const
	{
		Uint32 width, height, depth;

		width = get_width(mipmap);
		height = get_height(mipmap);
		depth = get_depth(mipmap);

		assert(width > 0);
		assert(height > 0);
		assert(depth > 0);

		assert(x < width);
		assert(y < height);
		assert(z < depth);

		return (get_pixel_size() / 8) * ((z * depth + y) * width + x);
	}

	Uint32 Image::get_buffer_block_offset(const Uint32 x, const Uint32 y,
		const Uint32 z, const Uint16 mipmap) const
	{
		Uint32 width, height, depth;

		width = get_width(mipmap);
		height = get_height(mipmap);
		depth = get_depth(mipmap);

		assert(width > 0);
		assert(height > 0);
		assert(depth > 0);

		width = (width + 3) / 4;
		height = (height + 3) / 4;

		assert(x < width);
		assert(y < height);
		assert(z < depth);

		return get_block_size() * ((z * depth + y) * width + x);
	}

	String Image::get_log_str() const
	{
		BoostFormat format_string(UTF8("'%1%' %2% faces with "
			"format %3% of size %4%x%5%x%6% with %7% mipmaps and "
			"%8% bits per pixel, using color space %9%, giving a "
			"total size of %10%"));

		format_string % get_name() % get_face_count() %
			get_texture_format() % get_width() % get_height() %
			get_depth() % get_mipmap_count() % get_pixel_size() %
			(get_sRGB() ? UTF8("sRGB") : UTF8("RGB")) %
			get_total_size();

		return String(format_string.str());
	}

	Image::Image(const String &name, const bool cube_map,
		const TextureFormatType texture_format,
		const glm::uvec3 &sizes, const Uint16 mipmap_count,
		const Uint16 pixel_size, const GLenum format,
		const GLenum type, const bool sRGB)
	{
		assert(pixel_size >= 4);
		assert((pixel_size % 4) == 0);
		assert(sizes[0] > 0);
		assert(sizes[1] > 0);
		assert(sizes[2] > 0);

		m_name = name;
		m_cube_map = cube_map;
		m_texture_format = texture_format;
		m_sizes = sizes;
		m_mipmap_count = mipmap_count;
		m_pixel_size = pixel_size;
		m_format = format;
		m_type = type;
		m_sRGB = sRGB;

		LOG_DEBUG(lt_image, UTF8("Setting image %1%."), get_log_str());

		get_buffer().resize(get_total_size());
		memset(get_buffer().get_ptr(), 0, get_buffer().get_size());
	}

	Image::Image(const String &name, const bool cube_map,
		const TextureFormatType texture_format,
		const glm::uvec3 &sizes, const Uint16 mipmap_count)
	{
		assert(sizes[0] > 0);
		assert(sizes[1] > 0);
		assert(sizes[2] > 0);

		TextureFormatUtil::get_source_format(texture_format, m_format,
			m_type);

		m_name = name;
		m_cube_map = cube_map;
		m_texture_format = texture_format;
		m_sizes = sizes;
		m_mipmap_count = mipmap_count;
		m_pixel_size = TextureFormatUtil::get_size(texture_format);
		m_sRGB = TextureFormatUtil::get_sRGB(texture_format);

		LOG_DEBUG(lt_image, UTF8("Setting image %1%."), get_log_str());

		get_buffer().resize(get_total_size());
		memset(get_buffer().get_ptr(), 0, get_buffer().get_size());
	}

	Uint16 Image::get_channel_count(const GLenum format)
	{
		switch (format)
		{
			case GL_RED:
			case GL_ALPHA:
			case GL_LUMINANCE:
				return 1;
			case GL_RG:
			case GL_LUMINANCE_ALPHA:
				return 2;
			case GL_RGB:
			case GL_BGR:
				return 3;
			case GL_RGBA:
			case GL_BGRA:
				return 4;
			case GL_NONE:
				return 0;
			default:
				return 0;
		}
	}

	Uint16 Image::get_pixel_size(const GLenum type, const GLenum format)
	{
		switch (type)
		{
			case GL_UNSIGNED_BYTE:
				return get_channel_count(format) * sizeof(Uint8) * 8;
			case GL_BYTE:
				return get_channel_count(format) * sizeof(Sint8) * 8;
			case GL_UNSIGNED_SHORT:
				return get_channel_count(format) * sizeof(Uint16) * 8;
			case GL_SHORT:
				return get_channel_count(format) * sizeof(Sint16) * 8;
			case GL_UNSIGNED_INT:
				return get_channel_count(format) * sizeof(Uint32) * 8;
			case GL_INT:
				return get_channel_count(format) * sizeof(Sint32) * 8;
			case GL_FLOAT:
				return get_channel_count(format) * sizeof(float) * 8;
			case GL_HALF_FLOAT:
				return get_channel_count(format) * sizeof(Uint16) * 8;
			case GL_UNSIGNED_BYTE_3_3_2:
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				return sizeof(Uint8) * 8;
			case GL_UNSIGNED_SHORT_5_6_5:
			case GL_UNSIGNED_SHORT_5_6_5_REV:
			case GL_UNSIGNED_SHORT_4_4_4_4:
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
			case GL_UNSIGNED_SHORT_5_5_5_1:
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				return sizeof(Uint16) * 8;
			case GL_UNSIGNED_INT_8_8_8_8:
			case GL_UNSIGNED_INT_8_8_8_8_REV:
			case GL_UNSIGNED_INT_10_10_10_2:
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				return sizeof(Uint32) * 8;
			case GL_NONE:
			default:
				assert(false);
				return 0;
		}
	}

	bool Image::is_valid(const GLenum type, const GLenum format,
		const bool compressed)
	{
		switch (type)
		{
			case GL_UNSIGNED_BYTE:
			case GL_BYTE:
			case GL_UNSIGNED_SHORT:
			case GL_SHORT:
			case GL_UNSIGNED_INT:
			case GL_INT:
			case GL_FLOAT:
			case GL_HALF_FLOAT:
				return (GL_RGB == format) ||
					(GL_RGBA == format) ||
					(GL_BGR == format) ||
					(GL_BGRA == format) ||
					(GL_RED == format) ||
					(GL_RG == format) ||
					(GL_ALPHA == format) ||
					(GL_LUMINANCE == format) ||
					(GL_LUMINANCE_ALPHA == format);
			case GL_UNSIGNED_BYTE_3_3_2:
			case GL_UNSIGNED_BYTE_2_3_3_REV:
			case GL_UNSIGNED_SHORT_5_6_5:
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				return GL_RGB == format;
			case GL_UNSIGNED_SHORT_4_4_4_4:
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
			case GL_UNSIGNED_SHORT_5_5_5_1:
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
			case GL_UNSIGNED_INT_8_8_8_8:
			case GL_UNSIGNED_INT_8_8_8_8_REV:
			case GL_UNSIGNED_INT_10_10_10_2:
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				return (GL_RGBA == format) ||
					(GL_BGRA == format);
			case GL_NONE:
				return compressed && (GL_NONE == format);
			default:
				return false;
		}
	}

	glm::vec4 Image::get_pixel(const Uint32 x, const Uint32 y,
		const Uint32 z, const Uint16 face, const Uint16 mipmap) const
	{
		glm::vec4 result;
		Uint32 i, count;

		count = get_channel_count(get_format());

		assert(count > 0);
		assert(count < 5);

		const void* const value = static_cast<const Uint8* const>(
			get_buffer().get_ptr()) + get_pixel_offset(x, y, z,
			face, mipmap);

		result = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		switch (get_type())
		{
			case GL_UNSIGNED_BYTE:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Uint8*>(value)[i];
					result[i] /= std::numeric_limits<Uint8>::max();
				}
				break;
			case GL_UNSIGNED_SHORT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Uint16*>(value)[i];
					result[i] /= std::numeric_limits<Uint16>::max();
				}
				break;
			case GL_UNSIGNED_INT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Uint32*>(value)[i];
					result[i] /= std::numeric_limits<Uint32>::max();
				}
				break;
			case GL_FLOAT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const float*>(value)[i];
				}
				break;
			case GL_HALF_FLOAT:
				for (i = 0; i < count; ++i)
				{
					result[i] = glm::detail::toFloat32(
						static_cast<const Uint16*>(value)[i]);
				}
				break;
			case GL_UNSIGNED_BYTE_3_3_2:
				result = glm::vec4(PackTool::unpack_uint_3_3_2(true,
					*static_cast<const Uint8*>(value)), 1.0f);
				break;
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				result = glm::vec4(PackTool::unpack_uint_2_3_3_rev(true,
					*static_cast<const Uint8*>(value)), 1.0f);
				break;
			case GL_UNSIGNED_SHORT_5_6_5:
				result = glm::vec4(PackTool::unpack_uint_5_6_5(true,
					*static_cast<const Uint16*>(value)), 1.0f);
				break;
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				result = glm::vec4(PackTool::unpack_uint_5_6_5_rev(true,
					*static_cast<const Uint16*>(value)), 1.0f);
				break;
			case GL_UNSIGNED_SHORT_4_4_4_4:
				result = PackTool::unpack_uint_4_4_4_4(true,
					*static_cast<const Uint16*>(value));
				break;
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
				result = PackTool::unpack_uint_4_4_4_4_rev(true,
					*static_cast<const Uint16*>(value));
				break;
			case GL_UNSIGNED_SHORT_5_5_5_1:
				result = PackTool::unpack_uint_5_5_5_1(true,
					*static_cast<const Uint16*>(value));
				break;
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				result = PackTool::unpack_uint_1_5_5_5_rev(true,
					*static_cast<const Uint16*>(value));
				break;
			case GL_UNSIGNED_INT_8_8_8_8:
				for (i = 0; i < 4; ++i)
				{
					result[i] = static_cast<const Uint8*>(value)[i];
					result[i] /= std::numeric_limits<Uint8>::max();
				}
				break;
			case GL_UNSIGNED_INT_8_8_8_8_REV:
				for (i = 0; i < 4; ++i)
				{
					result[i] = static_cast<const Uint8*>(value)[3 - i];
					result[i] /= std::numeric_limits<Uint8>::max();
				}
				break;
			case GL_UNSIGNED_INT_10_10_10_2:
				result = PackTool::unpack_uint_10_10_10_2(true,
					*static_cast<const Uint32*>(value));
				break;
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				result = PackTool::unpack_uint_2_10_10_10_rev(true,
					*static_cast<const Uint32*>(value));
				break;
			case GL_BYTE:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Sint8*>(value)[i];
					result[i] /= std::numeric_limits<Sint8>::max();
				}
				break;
			case GL_SHORT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Sint16*>(value)[i];
					result[i] /= std::numeric_limits<Sint16>::max();
				}
				break;
			case GL_INT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Sint32*>(value)[i];
					result[i] /= std::numeric_limits<Sint32>::max();
				}
				break;
			case GL_NONE:
			default:
				assert(false);
				return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		return swizzel_channels(get_format(), result);
	}

	glm::uvec4 Image::get_pixel_uint(const Uint32 x, const Uint32 y,
		const Uint32 z, const Uint16 face, const Uint16 mipmap) const
	{
		glm::uvec4 result;
		Uint32 i, count;

		count = get_channel_count(get_format());

		assert(count > 0);
		assert(count < 5);

		const void* const value = static_cast<const Uint8* const>(
			get_buffer().get_ptr()) + get_pixel_offset(x, y, z,
			face, mipmap);

		result = glm::uvec4(0);

		switch (get_type())
		{
			case GL_UNSIGNED_BYTE:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Uint8*>(
						value)[i];
				}
				break;
			case GL_UNSIGNED_SHORT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Uint16*>(
						value)[i];
				}
				break;
			case GL_UNSIGNED_INT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Uint32*>(
						value)[i];
				}
				break;
			case GL_FLOAT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const float*>(
						value)[i];
				}
				break;
			case GL_HALF_FLOAT:
				for (i = 0; i < count; ++i)
				{
					result[i] = glm::detail::toFloat32(
						static_cast<const Uint16*>(
							value)[i]);
				}
				break;
			case GL_UNSIGNED_BYTE_3_3_2:
				result = glm::uvec4(PackTool::unpack_uint_3_3_2(
					false,
					*static_cast<const Uint8*>(value)), 0);
				break;
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				result = glm::uvec4(
					PackTool::unpack_uint_2_3_3_rev(false,
					*static_cast<const Uint8*>(value)), 0);
				break;
			case GL_UNSIGNED_SHORT_5_6_5:
				result = glm::uvec4(
					PackTool::unpack_uint_5_6_5(false,
					*static_cast<const Uint16*>(value)), 0);
				break;
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				result = glm::uvec4(
					PackTool::unpack_uint_5_6_5_rev(false,
					*static_cast<const Uint16*>(value)), 0);
				break;
			case GL_UNSIGNED_SHORT_4_4_4_4:
				result = glm::uvec4(
					PackTool::unpack_uint_4_4_4_4(false,
					*static_cast<const Uint16*>(value)));
				break;
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
				result = glm::uvec4(
					PackTool::unpack_uint_4_4_4_4_rev(false,
					*static_cast<const Uint16*>(value)));
				break;
			case GL_UNSIGNED_SHORT_5_5_5_1:
				result = glm::uvec4(
					PackTool::unpack_uint_5_5_5_1(false,
					*static_cast<const Uint16*>(value)));
				break;
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				result = glm::uvec4(
					PackTool::unpack_uint_1_5_5_5_rev(false,
					*static_cast<const Uint16*>(value)));
				break;
			case GL_UNSIGNED_INT_8_8_8_8:
				for (i = 0; i < 4; ++i)
				{
					result[i] = static_cast<const Uint8*>(
						value)[i];
				}
				break;
			case GL_UNSIGNED_INT_8_8_8_8_REV:
				for (i = 0; i < 4; ++i)
				{
					result[i] = static_cast<const Uint8*>(
						value)[3 - i];
				}
				break;
			case GL_UNSIGNED_INT_10_10_10_2:
				result = glm::uvec4(
					PackTool::unpack_uint_10_10_10_2(false,
					*static_cast<const Uint32*>(value)));
				break;
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				result = glm::uvec4(
					PackTool::unpack_uint_2_10_10_10_rev(
					false,
					*static_cast<const Uint32*>(value)));
				break;
			case GL_BYTE:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Sint8*>(
						value)[i];
				}
				break;
			case GL_SHORT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Sint16*>(
						value)[i];
				}
				break;
			case GL_INT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Sint32*>(
						value)[i];
				}
				break;
			case GL_NONE:
			default:
				assert(false);
				return glm::uvec4(0);
		}

		return swizzel_channels(get_format(), result);
	}

	glm::ivec4 Image::get_pixel_int(const Uint32 x, const Uint32 y,
		const Uint32 z, const Uint16 face, const Uint16 mipmap) const
	{
		glm::ivec4 result;
		Uint32 i, count;

		count = get_channel_count(get_format());

		assert(count > 0);
		assert(count < 5);

		const void* const value = static_cast<const Uint8* const>(
			get_buffer().get_ptr()) + get_pixel_offset(x, y, z,
			face, mipmap);

		result = glm::ivec4(0);

		switch (get_type())
		{
			case GL_UNSIGNED_BYTE:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Uint8*>(
						value)[i];
				}
				break;
			case GL_UNSIGNED_SHORT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Uint16*>(
						value)[i];
				}
				break;
			case GL_UNSIGNED_INT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Uint32*>(
						value)[i];
				}
				break;
			case GL_FLOAT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const float*>(
						value)[i];
				}
				break;
			case GL_HALF_FLOAT:
				for (i = 0; i < count; ++i)
				{
					result[i] = glm::detail::toFloat32(
						static_cast<const Uint16*>(
							value)[i]);
				}
				break;
			case GL_UNSIGNED_BYTE_3_3_2:
				result = glm::ivec4(PackTool::unpack_uint_3_3_2(
					false,
					*static_cast<const Uint8*>(value)), 0);
				break;
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				result = glm::ivec4(
					PackTool::unpack_uint_2_3_3_rev(false,
					*static_cast<const Uint8*>(value)), 0);
				break;
			case GL_UNSIGNED_SHORT_5_6_5:
				result = glm::ivec4(
					PackTool::unpack_uint_5_6_5(false,
					*static_cast<const Uint16*>(value)), 0);
				break;
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				result = glm::ivec4(
					PackTool::unpack_uint_5_6_5_rev(false,
					*static_cast<const Uint16*>(value)), 0);
				break;
			case GL_UNSIGNED_SHORT_4_4_4_4:
				result = glm::ivec4(
					PackTool::unpack_uint_4_4_4_4(false,
					*static_cast<const Uint16*>(value)));
				break;
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
				result = glm::ivec4(
					PackTool::unpack_uint_4_4_4_4_rev(false,
					*static_cast<const Uint16*>(value)));
				break;
			case GL_UNSIGNED_SHORT_5_5_5_1:
				result = glm::ivec4(
					PackTool::unpack_uint_5_5_5_1(false,
					*static_cast<const Uint16*>(value)));
				break;
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				result = glm::ivec4(
					PackTool::unpack_uint_1_5_5_5_rev(false,
					*static_cast<const Uint16*>(value)));
				break;
			case GL_UNSIGNED_INT_8_8_8_8:
				for (i = 0; i < 4; ++i)
				{
					result[i] = static_cast<const Uint8*>(
						value)[i];
				}
				break;
			case GL_UNSIGNED_INT_8_8_8_8_REV:
				for (i = 0; i < 4; ++i)
				{
					result[i] = static_cast<const Uint8*>(
						value)[3 - i];
				}
				break;
			case GL_UNSIGNED_INT_10_10_10_2:
				result = glm::ivec4(
					PackTool::unpack_uint_10_10_10_2(false,
					*static_cast<const Uint32*>(value)));
				break;
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				result = glm::ivec4(
					PackTool::unpack_uint_2_10_10_10_rev(
					false,
					*static_cast<const Uint32*>(value)));
				break;
			case GL_BYTE:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Sint8*>(
						value)[i];
				}
				break;
			case GL_SHORT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Sint16*>(
						value)[i];
				}
				break;
			case GL_INT:
				for (i = 0; i < count; ++i)
				{
					result[i] = static_cast<const Sint32*>(
						value)[i];
				}
				break;
			case GL_NONE:
			default:
				assert(false);
				return glm::ivec4(0);
		}

		return swizzel_channels(get_format(), result);
	}

	void Image::set_pixel(const Uint32 x, const Uint32 y, const Uint32 z,
		const Uint16 face, const Uint16 mipmap, const glm::vec4 &data)
	{
		glm::vec4 tmp;
		void* value;
		Uint32 i, count;

		count = get_channel_count(get_format());

		assert(count > 0);
		assert(count < 5);

		value = static_cast<Uint8*>(get_buffer().get_ptr()) +
			get_pixel_offset(x, y, z, face, mipmap);

		tmp = swizzel_channels(get_format(), data);

		switch (get_type())
		{
			case GL_UNSIGNED_BYTE:
				for (i = 0; i < count; ++i)
				{
					static_cast<Uint8*>(value)[i] = tmp[i] *
						std::numeric_limits<Uint8>::max();
				}
				return;
			case GL_UNSIGNED_SHORT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Uint16*>(value)[i] = tmp[i] *
						std::numeric_limits<Uint16>::max();
				}
				return;
			case GL_UNSIGNED_INT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Uint32*>(value)[i] = tmp[i] *
						std::numeric_limits<Uint32>::max();
				}
				return;
			case GL_FLOAT:
				for (i = 0; i < count; ++i)
				{
					static_cast<float*>(value)[i] = tmp[i];
				}
				return;
			case GL_HALF_FLOAT:
				for (i = 0; i < count; ++i)
				{
					static_cast<glm::detail::hdata*>(value)[i] =
						glm::detail::toFloat16(tmp[i]);
				}
				return;
			case GL_UNSIGNED_BYTE_3_3_2:
				*static_cast<Uint8*>(value) =
					PackTool::pack_uint_3_3_2(true, glm::vec3(tmp));
				return;
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				*static_cast<Uint8*>(value) =
					PackTool::pack_uint_2_3_3_rev(true, glm::vec3(tmp));
				return;
			case GL_UNSIGNED_SHORT_5_6_5:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_5_6_5(true, glm::vec3(tmp));
				return;
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_5_6_5_rev(true, glm::vec3(tmp));
				return;
			case GL_UNSIGNED_SHORT_4_4_4_4:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_4_4_4_4(true, tmp);
				return;
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_4_4_4_4_rev(true, tmp);
				return;
			case GL_UNSIGNED_SHORT_5_5_5_1:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_5_5_5_1(true, tmp);
				return;
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_1_5_5_5_rev(true, tmp);
				return;
			case GL_UNSIGNED_INT_8_8_8_8:
				for (i = 0; i < 4; ++i)
				{
					static_cast<Uint8*>(value)[i] = tmp[i] *
						std::numeric_limits<Uint8>::max();
				}
				return;
			case GL_UNSIGNED_INT_8_8_8_8_REV:
				for (i = 0; i < 4; ++i)
				{
					static_cast<Uint8*>(value)[i] = tmp[3 - i] *
						std::numeric_limits<Uint8>::max();
				}
				return;
			case GL_UNSIGNED_INT_10_10_10_2:
				*static_cast<Uint32*>(value) =
					PackTool::pack_uint_10_10_10_2(true, tmp);
				return;
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				*static_cast<Uint32*>(value) =
					PackTool::pack_uint_2_10_10_10_rev(true, tmp);
				return;
			case GL_BYTE:
				for (i = 0; i < count; ++i)
				{
					static_cast<Sint8*>(value)[i] = tmp[i] *
						std::numeric_limits<Sint8>::max();
				}
				return;
			case GL_SHORT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Sint16*>(value)[i] = tmp[i] *
						std::numeric_limits<Sint16>::max();
				}
				return;
			case GL_INT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Sint32*>(value)[i] = tmp[i] *
						std::numeric_limits<Sint32>::max();
				}
				return;
			case GL_NONE:
			default:
				assert(false);
				return;
		}
	}

	void Image::set_pixel_uint(const Uint32 x, const Uint32 y,
		const Uint32 z, const Uint16 face, const Uint16 mipmap,
		const glm::uvec4 &data)
	{
		glm::uvec4 tmp;
		void* value;
		Uint32 i, count;

		count = get_channel_count(get_format());

		assert(count > 0);
		assert(count < 5);

		value = static_cast<Uint8*>(get_buffer().get_ptr()) +
			get_pixel_offset(x, y, z, face, mipmap);

		tmp = swizzel_channels(get_format(), data);

		switch (get_type())
		{
			case GL_UNSIGNED_BYTE:
				for (i = 0; i < count; ++i)
				{
					static_cast<Uint8*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_UNSIGNED_SHORT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Uint16*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_UNSIGNED_INT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Uint32*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_FLOAT:
				for (i = 0; i < count; ++i)
				{
					static_cast<float*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_HALF_FLOAT:
				for (i = 0; i < count; ++i)
				{
					static_cast<glm::detail::hdata*>(value)[i] =
						glm::detail::toFloat16(tmp[i]);
				}
				return;
			case GL_UNSIGNED_BYTE_3_3_2:
				*static_cast<Uint8*>(value) =
					PackTool::pack_uint_3_3_2(false,
						glm::vec3(tmp));
				return;
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				*static_cast<Uint8*>(value) =
					PackTool::pack_uint_2_3_3_rev(false,
						glm::vec3(tmp));
				return;
			case GL_UNSIGNED_SHORT_5_6_5:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_5_6_5(false,
						glm::vec3(tmp));
				return;
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_5_6_5_rev(false,
						glm::vec3(tmp));
				return;
			case GL_UNSIGNED_SHORT_4_4_4_4:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_4_4_4_4(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_4_4_4_4_rev(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_SHORT_5_5_5_1:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_5_5_5_1(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_1_5_5_5_rev(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_INT_8_8_8_8:
				for (i = 0; i < 4; ++i)
				{
					static_cast<Uint8*>(value)[i] = tmp[i];
				}
				return;
			case GL_UNSIGNED_INT_8_8_8_8_REV:
				for (i = 0; i < 4; ++i)
				{
					static_cast<Uint8*>(value)[i] =
						tmp[3 - i];
				}
				return;
			case GL_UNSIGNED_INT_10_10_10_2:
				*static_cast<Uint32*>(value) =
					PackTool::pack_uint_10_10_10_2(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				*static_cast<Uint32*>(value) =
					PackTool::pack_uint_2_10_10_10_rev(
						false, glm::vec4(tmp));
				return;
			case GL_BYTE:
				for (i = 0; i < count; ++i)
				{
					static_cast<Sint8*>(value)[i] = tmp[i];
				}
				return;
			case GL_SHORT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Sint16*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_INT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Sint32*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_NONE:
			default:
				assert(false);
				return;
		}
	}

	void Image::set_pixel_int(const Uint32 x, const Uint32 y,
		const Uint32 z, const Uint16 face, const Uint16 mipmap,
		const glm::ivec4 &data)
	{
		glm::ivec4 tmp;
		void* value;
		Uint32 i, count;

		count = get_channel_count(get_format());

		assert(count > 0);
		assert(count < 5);

		value = static_cast<Uint8*>(get_buffer().get_ptr()) +
			get_pixel_offset(x, y, z, face, mipmap);

		tmp = swizzel_channels(get_format(), data);

		switch (get_type())
		{
			case GL_UNSIGNED_BYTE:
				for (i = 0; i < count; ++i)
				{
					static_cast<Uint8*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_UNSIGNED_SHORT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Uint16*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_UNSIGNED_INT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Uint32*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_FLOAT:
				for (i = 0; i < count; ++i)
				{
					static_cast<float*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_HALF_FLOAT:
				for (i = 0; i < count; ++i)
				{
					static_cast<glm::detail::hdata*>(value)[i] =
						glm::detail::toFloat16(tmp[i]);
				}
				return;
			case GL_UNSIGNED_BYTE_3_3_2:
				*static_cast<Uint8*>(value) =
					PackTool::pack_uint_3_3_2(false,
						glm::vec3(tmp));
				return;
			case GL_UNSIGNED_BYTE_2_3_3_REV:
				*static_cast<Uint8*>(value) =
					PackTool::pack_uint_2_3_3_rev(false,
						glm::vec3(tmp));
				return;
			case GL_UNSIGNED_SHORT_5_6_5:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_5_6_5(false,
						glm::vec3(tmp));
				return;
			case GL_UNSIGNED_SHORT_5_6_5_REV:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_5_6_5_rev(false,
						glm::vec3(tmp));
				return;
			case GL_UNSIGNED_SHORT_4_4_4_4:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_4_4_4_4(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_SHORT_4_4_4_4_REV:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_4_4_4_4_rev(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_SHORT_5_5_5_1:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_5_5_5_1(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_SHORT_1_5_5_5_REV:
				*static_cast<Uint16*>(value) =
					PackTool::pack_uint_1_5_5_5_rev(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_INT_8_8_8_8:
				for (i = 0; i < 4; ++i)
				{
					static_cast<Uint8*>(value)[i] = tmp[i];
				}
				return;
			case GL_UNSIGNED_INT_8_8_8_8_REV:
				for (i = 0; i < 4; ++i)
				{
					static_cast<Uint8*>(value)[i] =
						tmp[3 - i];
				}
				return;
			case GL_UNSIGNED_INT_10_10_10_2:
				*static_cast<Uint32*>(value) =
					PackTool::pack_uint_10_10_10_2(false,
						glm::vec4(tmp));
				return;
			case GL_UNSIGNED_INT_2_10_10_10_REV:
				*static_cast<Uint32*>(value) =
					PackTool::pack_uint_2_10_10_10_rev(
						false, glm::vec4(tmp));
				return;
			case GL_BYTE:
				for (i = 0; i < count; ++i)
				{
					static_cast<Sint8*>(value)[i] = tmp[i];
				}
				return;
			case GL_SHORT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Sint16*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_INT:
				for (i = 0; i < count; ++i)
				{
					static_cast<Sint32*>(value)[i] =
						tmp[i];
				}
				return;
			case GL_NONE:
			default:
				assert(false);
				return;
		}
	}

	Image::Image(const Image &image, const Uint16 face, const Uint16 mipmap,
		const bool expand_dxt1_to_dxt5)
	{
		Uint32 i, count;

		m_sizes = image.m_sizes;

		for (i = 0; i < mipmap; ++i)
		{
			m_sizes[0] = std::max(1u, m_sizes[0] / 2);
			m_sizes[1] = std::max(1u, m_sizes[1] / 2);
			m_sizes[2] = std::max(1u, m_sizes[2] / 2);
		}

		m_name = image.get_name();
		m_cube_map = false;
		m_texture_format = image.get_texture_format();
		m_mipmap_count = 0;
		m_pixel_size = image.get_pixel_size();
		m_format = image.get_format();
		m_type = image.get_type();
		m_sRGB = image.get_sRGB();

		if (expand_dxt1_to_dxt5 && ((m_texture_format == tft_rgb_dxt1)
			|| (m_texture_format == tft_srgb_dxt1)))
		{
			if (m_sRGB)
			{
				m_texture_format = tft_srgb_a_dxt5;
			}
			else
			{
				m_texture_format = tft_rgba_dxt5;
			}

			TextureFormatUtil::get_source_format(m_texture_format,
				m_format, m_type);

			m_pixel_size = TextureFormatUtil::get_size(
				m_texture_format);

			get_buffer().resize(get_total_size());
			memset(get_buffer().get_ptr(), 0xFF,
				get_buffer().get_size());

			assert((get_total_size() % 16) == 0);

			count = get_total_size() / 16;

			for (i = 0; i < count; ++i)
			{
				memcpy(static_cast<Uint8*>(
					get_buffer().get_ptr()) + i * 16,
					static_cast<const Uint8* const >(
					image.get_buffer().get_ptr()) + i * 8,
					8);
			}

			return;
		}

		get_buffer().resize(get_total_size());
		get_buffer().copy(image.get_buffer(), image.get_size(mipmap),
			image.get_offset(face, mipmap), 0);
	}

	Image::Image(const Image &image)
	{
		m_name = image.m_name;
		m_cube_map = image.m_cube_map;
		m_texture_format = image.m_texture_format;
		m_sizes = image.m_sizes;
		m_mipmap_count = image.m_mipmap_count;
		m_pixel_size = image.m_pixel_size;
		m_format = image.m_format;
		m_type = image.m_type;
		m_sRGB = image.m_sRGB;

		get_buffer().resize(get_total_size());
		get_buffer().copy(image.get_buffer());
	}

	void Image::read_framebuffer(const Uint32 x, const Uint32 y)
	{
		glReadPixels(x, y, get_width(), get_height(), get_format(),
			get_type(), get_buffer().get_ptr());
	}

}
