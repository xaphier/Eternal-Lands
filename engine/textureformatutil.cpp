/****************************************************************************
 *            textureformatutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "textureformatutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class TextureFormatTypeData
		{
			private:
				const String m_name;
				const IntegerType m_integer_type;
				const PackFormatType m_pack_format_type;
				const GLenum m_gl_type;
				const GLenum m_source_type;
				const GLenum m_format_type;
				const Uint16 m_count;
				const Uint16 m_size;
				const bool m_compressed;
				const bool m_sRGB;
				const bool m_depth;
				const bool m_stencil;
				const bool m_integer;
				const bool m_pack_format;

			public:
				inline TextureFormatTypeData(const String &name,
					const IntegerType integer_type,
					const PackFormatType pack_format_type,
					const GLenum gl_type,
					const GLenum source_type,
					const GLenum format_type,
					const Uint16 count,
					const Uint16 size,
					const bool sRGB = false): m_name(name),
					m_integer_type(integer_type),
					m_pack_format_type(pack_format_type),
					m_gl_type(gl_type),
					m_source_type(source_type),
					m_format_type(format_type),
					m_count(count), m_size(size),
					m_compressed(false), m_sRGB(sRGB),
					m_depth(false), m_stencil(false),
					m_integer(true), m_pack_format(true)
				{
				}

				inline TextureFormatTypeData(const String &name,
					const PackFormatType pack_format_type,
					const GLenum gl_type,
					const GLenum source_type,
					const GLenum format_type,
					const Uint16 count,
					const Uint16 size):
					m_name(name),
					m_integer_type(it_unsigned),
					m_pack_format_type(pack_format_type),
					m_gl_type(gl_type),
					m_source_type(source_type),
					m_format_type(format_type),
					m_count(count), m_size(size),
					m_compressed(false), m_sRGB(false),
					m_depth(false), m_stencil(false),
					m_integer(false), m_pack_format(true)
				{
				}

				inline TextureFormatTypeData(const String &name,
					const GLenum gl_type,
					const GLenum source_type,
					const GLenum format_type,
					const Uint16 count,
					const Uint16 size,
					const bool stencil = false):
					m_name(name),
					m_integer_type(it_unsigned),
					m_pack_format_type(pft_unsigned_byte_1),
					m_gl_type(gl_type),
					m_source_type(source_type),
					m_format_type(format_type),
					m_count(count), m_size(size),
					m_compressed(false), m_sRGB(false),
					m_depth(true), m_stencil(stencil),
					m_integer(false), m_pack_format(false)
				{
				}

				inline TextureFormatTypeData(const String &name,
					const IntegerType integer_type,
					const GLenum gl_type,
					const Uint16 count,
					const Uint16 size,
					const bool sRGB = false):
					m_name(name),
					m_integer_type(integer_type),
					m_pack_format_type(pft_unsigned_byte_1),
					m_gl_type(gl_type),
					m_source_type(GL_NONE),
					m_format_type(gl_type),
					m_count(count), m_size(size),
					m_compressed(true), m_sRGB(sRGB),
					m_depth(false), m_stencil(false),
					m_integer(true), m_pack_format(false)
				{
				}

				inline TextureFormatTypeData(const String &name,
					const GLenum gl_type,
					const Uint16 count,
					const Uint16 size,
					const bool sRGB = false):
					m_name(name),
					m_integer_type(it_unsigned),
					m_pack_format_type(pft_unsigned_byte_1),
					m_gl_type(gl_type),
					m_source_type(GL_NONE),
					m_format_type(gl_type),
					m_count(count), m_size(size),
					m_compressed(true), m_sRGB(sRGB),
					m_depth(false), m_stencil(false),
					m_integer(false), m_pack_format(false)
				{
				}

				inline ~TextureFormatTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline IntegerType get_integer_type()
					const noexcept
				{
					return m_integer_type;
				}

				inline GLenum get_gl_type() const noexcept
				{
					return m_gl_type;
				}

				inline GLenum get_source_type() const
					noexcept
				{
					return m_source_type;
				}

				inline GLenum get_format_type() const
					noexcept
				{
					return m_format_type;
				}

				inline Uint16 get_count() const noexcept
				{
					return m_count;
				}

				inline Uint16 get_size() const noexcept
				{
					return m_size;
				}

				inline bool get_compressed() const
					noexcept
				{
					return m_compressed;
				}

				inline bool get_sRGB() const noexcept
				{
					return m_sRGB;
				}

				inline bool get_depth() const noexcept
				{
					return m_depth;
				}

				inline bool get_stencil() const noexcept
				{
					return m_stencil;
				}

				inline bool get_integer() const noexcept
				{
					return m_integer;
				}

		};

#define EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(name, packed, gl_type, source_type, source_format, count, size)	\
	TextureFormatTypeData(String(UTF8(name "ui")), it_unsigned,	\
		pft_unsigned_##packed##_##count, gl_type##UI, 	\
		GL_UNSIGNED_##source_type, source_format##_INTEGER, count, size),	\
	TextureFormatTypeData(String(UTF8(name "i")), it_signed,	\
		pft_signed_##packed##_##count, gl_type##I, GL_##source_type,	\
		source_format##_INTEGER, count, size),	\
	TextureFormatTypeData(String(UTF8(name)), it_unsigned,	\
		pft_unsigned_normalized_##packed##_##count, gl_type,	\
		GL_UNSIGNED_##source_type, source_format, count, size),	\
	TextureFormatTypeData(String(UTF8(name "snorm")), it_signed,	\
		pft_signed_normalized_##packed##_##count, gl_type##_SNORM,	\
		GL_##source_type, source_format, count, size)

		const TextureFormatTypeData texture_format_datas[] =
		{
			TextureFormatTypeData(String(UTF8("rgba4")),
				it_unsigned_normalized,
				pft_unsigned_short_4_4_4_4, GL_RGBA4,
				GL_UNSIGNED_SHORT_4_4_4_4, GL_RGBA, 4, 16),
			TextureFormatTypeData(String(UTF8("r3g3b2")),
				it_unsigned_normalized,
				pft_unsigned_normalized_byte_3_3_2, GL_R3_G3_B2,
				GL_UNSIGNED_BYTE_3_3_2, GL_RGB, 3, 8),
			TextureFormatTypeData(String(UTF8("r5g6b5")),
				it_unsigned_normalized,
				pft_unsigned_normalized_short_5_6_5, GL_RGB5,
				GL_UNSIGNED_SHORT_5_6_5, GL_RGB, 3, 16),
			TextureFormatTypeData(String(UTF8("rgb5_a1")),
				it_unsigned_normalized,
				pft_unsigned_normalized_short_5_5_5_1,
				GL_RGB5_A1, GL_UNSIGNED_SHORT_5_5_5_1, GL_RGBA,
				4, 16),
			TextureFormatTypeData(String(UTF8("rgb10_a2")),
				it_unsigned_normalized,
				pft_unsigned_normalized_int_10_10_10_2,
				GL_RGB10_A2, GL_UNSIGNED_INT_10_10_10_2,
				GL_RGBA, 4, 32),
			TextureFormatTypeData(String(UTF8("rgb10_a2_ui")),
				it_unsigned, pft_unsigned_int_10_10_10_2,
				GL_RGB10_A2UI, GL_UNSIGNED_INT_10_10_10_2,
				GL_RGBA_INTEGER, 4, 32),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("r8", byte, GL_R8,
				BYTE, GL_RED, 1, 8),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rg8", byte, GL_RG8,
				BYTE, GL_RG, 2, 16),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rgb8", byte,
				GL_RGB8, BYTE, GL_RGB, 3, 24),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rgba8", byte,
				GL_RGBA8, BYTE, GL_RGBA, 4, 32),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("r16", short, GL_R16,
				SHORT, GL_RED, 1, 16),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rg16", short,
				GL_RG16, SHORT, GL_RG, 2, 32),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rgb16", short,
				GL_RGB16, SHORT, GL_RGB, 3, 48),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rgba16", short,
				GL_RGBA16, SHORT, GL_RGBA, 4, 64),
			TextureFormatTypeData(String(UTF8("r32ui")),
				pft_unsigned_int_1, GL_R32UI, GL_UNSIGNED_INT,
				GL_RED_INTEGER, 1, 32),
			TextureFormatTypeData(String(UTF8("r32i")),
				pft_signed_int_1, GL_R32I, GL_INT,
				GL_RED_INTEGER, 1, 32),
			TextureFormatTypeData(String(UTF8("rg32ui")),
				pft_unsigned_int_2, GL_RG32UI, GL_UNSIGNED_INT,
				GL_RG_INTEGER, 2, 64),
			TextureFormatTypeData(String(UTF8("rg32i")),
				pft_signed_int_2, GL_RG32I, GL_INT,
				GL_RG_INTEGER, 2, 64),
			TextureFormatTypeData(String(UTF8("rgb32ui")),
				pft_unsigned_int_3, GL_RGB32UI, GL_UNSIGNED_INT,
				GL_RGB_INTEGER, 3, 96),
			TextureFormatTypeData(String(UTF8("rgb32i")),
				pft_signed_int_3, GL_RGB32I, GL_INT,
				GL_RGB_INTEGER, 3, 96),
			TextureFormatTypeData(String(UTF8("rgba32ui")),
				pft_unsigned_int_4, GL_RGBA32UI,
				GL_UNSIGNED_INT, GL_RGBA_INTEGER, 4, 128),
			TextureFormatTypeData(String(UTF8("rgba32i")),
				pft_signed_int_4, GL_RGBA32I, GL_INT,
				GL_RGBA_INTEGER, 4, 128),
			TextureFormatTypeData(String(UTF8("r16f")), pft_half_1,
				GL_R16F, GL_HALF_FLOAT, GL_RED, 1, 16),
			TextureFormatTypeData(String(UTF8("rg16f")), pft_half_2,
				GL_RG16F, GL_HALF_FLOAT, GL_RG, 2, 32),
			TextureFormatTypeData(String(UTF8("rgb16f")),
				pft_half_3, GL_RGB16F, GL_HALF_FLOAT, GL_RGB,
				3, 48),
			TextureFormatTypeData(String(UTF8("rgba16f")),
				pft_half_4, GL_RGBA16F, GL_HALF_FLOAT, GL_RGBA,
				4, 64),
			TextureFormatTypeData(String(UTF8("r32f")),
				pft_float_1, GL_R32F, GL_FLOAT, GL_RED, 1, 32),
			TextureFormatTypeData(String(UTF8("rg32f")),
				pft_float_2, GL_RG32F, GL_FLOAT, GL_RG, 2, 64),
			TextureFormatTypeData(String(UTF8("rgb32f")),
				pft_float_3, GL_RGB32F, GL_FLOAT, GL_RGB, 3,
				96),
			TextureFormatTypeData(String(UTF8("rgba32f")),
				pft_float_4, GL_RGBA32F, GL_FLOAT, GL_RGBA, 4,
				128),
			TextureFormatTypeData(String(UTF8("rgb9_e5")),
				pft_float_4, GL_RGB9_E5,
				GL_UNSIGNED_INT_5_9_9_9_REV, GL_RGB, 3,
				32),
			TextureFormatTypeData(String(UTF8("r10f_g11f_b10f")),
				pft_float_4, GL_R11F_G11F_B10F,
				GL_UNSIGNED_INT_10F_11F_11F_REV, GL_RGB, 3,
				32),
			TextureFormatTypeData(String(UTF8("rgb_dxt1")),
				it_unsigned_normalized,
				GL_COMPRESSED_RGB_S3TC_DXT1_EXT, 3, 4),
			TextureFormatTypeData(String(UTF8("rgba_dxt1")),
				it_unsigned_normalized,
				GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 4, 4),
			TextureFormatTypeData(String(UTF8("rgba_dxt3")),
				it_unsigned_normalized,
				GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 4, 8),
			TextureFormatTypeData(String(UTF8("rgba_dxt5")),
				it_unsigned_normalized,
				GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 4, 8),
			TextureFormatTypeData(String(UTF8("r_rgtc1")),
				it_unsigned_normalized,
				GL_COMPRESSED_RED_RGTC1, 1, 4),
			TextureFormatTypeData(String(UTF8("signed_r_rgtc1")),
				it_signed_normalized,
				GL_COMPRESSED_SIGNED_RED_RGTC1, 1, 4),
			TextureFormatTypeData(String(UTF8("rg_rgtc2")),
				it_unsigned_normalized,
				GL_COMPRESSED_RG_RGTC2, 2, 8),
			TextureFormatTypeData(String(UTF8("signed_rg_rgtc2")),
				it_signed_normalized,
				GL_COMPRESSED_SIGNED_RG_RGTC2, 2, 8),
			TextureFormatTypeData(String(UTF8("depth16")),
				GL_DEPTH_COMPONENT16, GL_UNSIGNED_SHORT,
				GL_DEPTH_COMPONENT, 0, 16, false),
			TextureFormatTypeData(String(UTF8("depth24")),
				GL_DEPTH_COMPONENT24, GL_UNSIGNED_INT,
				GL_DEPTH_COMPONENT, 0, 24, false),
			TextureFormatTypeData(String(UTF8("depth32")),
				GL_DEPTH_COMPONENT32, GL_UNSIGNED_INT,
				GL_DEPTH_COMPONENT, 0, 32, false),
			TextureFormatTypeData(String(UTF8("depth24_stencil8")),
				GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8,
				GL_DEPTH_STENCIL, 0, 32, true),
			TextureFormatTypeData(String(UTF8("depth32f")),
				GL_DEPTH_COMPONENT32F, GL_FLOAT,
				GL_DEPTH_COMPONENT, 0, 32, false),
			TextureFormatTypeData(String(UTF8("depth32f_stencil8")),
				GL_DEPTH32F_STENCIL8,
				GL_FLOAT_32_UNSIGNED_INT_24_8_REV,
				GL_DEPTH_COMPONENT, 0, 64, true),
			TextureFormatTypeData(String(UTF8("srgb8")),
				it_unsigned_normalized,
				pft_unsigned_normalized_byte_3, GL_SRGB8,
				GL_UNSIGNED_BYTE, GL_RGB, 3, 24),
			TextureFormatTypeData(String(UTF8("srgb8_a8")),
				it_unsigned_normalized,
				pft_unsigned_normalized_byte_4, GL_SRGB8_ALPHA8,
				GL_UNSIGNED_BYTE, GL_RGBA, 4, 32),
			TextureFormatTypeData(String(UTF8("srgb_dxt1")),
				it_unsigned_normalized,
				GL_COMPRESSED_SRGB_S3TC_DXT1_EXT, 3, 4, true),
			TextureFormatTypeData(String(UTF8("srgb_a_dxt1")),
				it_unsigned_normalized,
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, 4, 4,
				true),
			TextureFormatTypeData(String(UTF8("srgb_a_dxt3")),
				it_unsigned_normalized,
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT, 4, 8,
				true),
			TextureFormatTypeData(String(UTF8("srgb_a_dxt5")),
				it_unsigned_normalized,
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, 4, 8,
				true),
			TextureFormatTypeData(String(UTF8("l8")),
				it_unsigned_normalized,
				pft_unsigned_normalized_byte_1, GL_LUMINANCE8,
				GL_UNSIGNED_BYTE, GL_LUMINANCE, 1, 8),
			TextureFormatTypeData(String(UTF8("a8")),
				it_unsigned_normalized,
				pft_unsigned_normalized_byte_1, GL_ALPHA8,
				GL_UNSIGNED_BYTE, GL_ALPHA, 1, 8),
			TextureFormatTypeData(String(UTF8("la8")),
				it_unsigned_normalized,
				pft_unsigned_normalized_byte_2,
				GL_LUMINANCE8_ALPHA8,
				GL_UNSIGNED_BYTE, GL_LUMINANCE_ALPHA, 2, 16),
			TextureFormatTypeData(String(UTF8("l16")),
				it_unsigned_normalized,
				pft_unsigned_normalized_short_1, GL_LUMINANCE16,
				GL_UNSIGNED_SHORT, GL_LUMINANCE, 1, 16),
			TextureFormatTypeData(String(UTF8("la16")),
				it_unsigned_normalized,
				pft_unsigned_normalized_short_2,
				GL_LUMINANCE16_ALPHA16,
				GL_UNSIGNED_SHORT, GL_LUMINANCE_ALPHA, 2, 32)
		};

#undef EXPAND_INTEGER_TEXTURE_FORMAT_TYPE

		const Uint32 texture_format_datas_count =
			sizeof(texture_format_datas) /
			sizeof(TextureFormatTypeData);

	};

	void TextureFormatUtil::get_source_format(
		const TextureFormatType texture_format, GLenum &format,
		GLenum &type)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		type = texture_format_datas[
			texture_format].get_source_type();
		format = texture_format_datas[
			texture_format].get_format_type();
	}

	Uint16 TextureFormatUtil::get_count(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		return texture_format_datas[texture_format].get_count();
	}

	const String &TextureFormatUtil::get_str(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		return texture_format_datas[texture_format].get_name();
	}

	bool TextureFormatUtil::get_sRGB(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		return texture_format_datas[texture_format].get_sRGB();
	}


	bool TextureFormatUtil::get_depth(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		return texture_format_datas[texture_format].get_depth();
	}


	bool TextureFormatUtil::get_stencil(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		return texture_format_datas[texture_format].get_stencil();
	}


	bool TextureFormatUtil::get_compressed(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		return texture_format_datas[
			texture_format].get_compressed();
	}

	bool TextureFormatUtil::get_format_supported(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		switch (texture_format)
		{
			case tft_rgb10_a2_ui:
				return GLEW_VERSION_3_3;
			case tft_rgb16f:
			case tft_rgba16f:
			case tft_rgb32f:
			case tft_rgba32f:
				return GLEW_VERSION_3_0;
			case tft_rgb_dxt1:
			case tft_rgba_dxt1:
			case tft_rgba_dxt3:
			case tft_rgba_dxt5:
				return GLEW_EXT_texture_compression_s3tc;
			case tft_r_rgtc1:
			case tft_signed_r_rgtc1:
			case tft_rg_rgtc2:
			case tft_signed_rg_rgtc2:
				return GLEW_VERSION_3_0;
			case tft_depth24_stencil8:
				return GLEW_VERSION_3_0 ||
					GLEW_EXT_packed_depth_stencil;
			case tft_depth32f:
			case tft_depth32f_stencil8:
				return GLEW_VERSION_3_0;
			case tft_srgb8:
			case tft_srgb8_a8:
				return GLEW_VERSION_2_1 ||
					GLEW_EXT_texture_sRGB;
			case tft_srgb_dxt1:
			case tft_srgb_a_dxt1:
			case tft_srgb_a_dxt3:
			case tft_srgb_a_dxt5:
				return GLEW_EXT_texture_sRGB;
			default:
				break;
		}

		const TextureFormatTypeData &format =
			texture_format_datas[texture_format];

		if (format.get_integer())
		{
			switch (format.get_integer_type())
			{
				case it_unsigned:
				case it_signed:
					if (format.get_count() > 2)
					{
						return GLEW_VERSION_3_0;
					}
					else
					{
						return GLEW_VERSION_3_0;
					}
				case it_unsigned_normalized:
					if (format.get_count() > 2)
					{
						return true;
					}
					else
					{
						return GLEW_VERSION_3_0;
					}
				case it_signed_normalized:
					return GLEW_VERSION_3_1;
			}
		}

		return true;
	}

	Uint32 TextureFormatUtil::get_size(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		return texture_format_datas[texture_format].get_size();
	}


	GLenum TextureFormatUtil::get_gl_format(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		return texture_format_datas[texture_format].get_gl_type();
	}


	IntegerType TextureFormatUtil::get_integer(
		const TextureFormatType texture_format)
	{
		if (texture_format_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(UTF8(
					"TextureFormatType")));
		}

		return texture_format_datas[
			texture_format].get_integer_type();
	}

	TextureFormatType TextureFormatUtil::get_texture_format(
		const String &str)
	{
		Uint32 i;
		TextureFormatType texture_format_type;

		for (i = 0; i < texture_format_datas_count; ++i)
		{
			texture_format_type =
				static_cast<TextureFormatType>(i);

			if (str == get_str(texture_format_type))
			{
				return texture_format_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"TextureFormatUtil")));
	}

	Uint32 TextureFormatUtil::get_texture_format_count() noexcept
	{
		return texture_format_datas_count;
	}

	OutStream& operator<<(OutStream &str, const TextureFormatType value)
	{
		str << TextureFormatUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, TextureFormatType &value)
	{
		StringType string;

		str >> string;

		value = TextureFormatUtil::get_texture_format(String(string));

		return str;
	}

}
