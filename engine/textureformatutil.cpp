/****************************************************************************
 *            textureformatutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

			public:
				inline TextureFormatTypeData(const String &name,
					const IntegerType integer_type,
					const GLenum gl_type,
					const GLenum source_type,
					const GLenum format_type,
					const Uint16 count,
					const Uint16 size,
					const bool compressed = false,
					const bool sRGB = false): m_name(name),
					m_integer_type(integer_type),
					m_gl_type(gl_type),
					m_source_type(source_type),
					m_format_type(format_type),
					m_count(count), m_size(size),
					m_compressed(compressed), m_sRGB(sRGB),
					m_depth(false), m_stencil(false),
					m_integer(true)
				{
				}

				inline TextureFormatTypeData(const String &name,
					const GLenum gl_type,
					const GLenum source_type,
					const GLenum format_type,
					const Uint16 count,
					const Uint16 size,
					const bool compressed = false,
					const bool sRGB = false,
					const bool depth = false,
					const bool stencil = false):
					m_name(name),
					m_integer_type(it_unsigned),
					m_gl_type(gl_type),
					m_source_type(source_type),
					m_format_type(format_type),
					m_count(count), m_size(size),
					m_compressed(compressed), m_sRGB(sRGB),
					m_depth(depth), m_stencil(stencil),
					m_integer(false)
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
					m_gl_type(gl_type),
					m_source_type(GL_NONE),
					m_format_type(gl_type),
					m_count(count), m_size(size),
					m_compressed(true), m_sRGB(sRGB),
					m_depth(false), m_stencil(false),
					m_integer(true)
				{
				}

				inline TextureFormatTypeData(const String &name,
					const GLenum gl_type,
					const Uint16 count,
					const Uint16 size,
					const bool sRGB = false):
					m_name(name),
					m_integer_type(it_unsigned),
					m_gl_type(gl_type),
					m_source_type(GL_NONE),
					m_format_type(gl_type),
					m_count(count), m_size(size),
					m_compressed(true), m_sRGB(sRGB),
					m_depth(false), m_stencil(false),
					m_integer(false)
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline const IntegerType get_integer_type()
					const
				{
					return m_integer_type;
				}

				inline const GLenum get_gl_type() const
				{
					return m_gl_type;
				}

				inline const GLenum get_source_type() const
				{
					return m_source_type;
				}

				inline const GLenum get_format_type() const
				{
					return m_format_type;
				}

				inline const Uint16 get_count() const
				{
					return m_count;
				}

				inline const Uint16 get_size() const
				{
					return m_size;
				}

				inline const bool get_compressed() const
				{
					return m_compressed;
				}

				inline const bool get_sRGB() const
				{
					return m_sRGB;
				}

				inline const bool get_depth() const
				{
					return m_depth;
				}

				inline const bool get_stencil() const
				{
					return m_stencil;
				}

				inline const bool get_integer() const
				{
					return m_integer;
				}

		};

#define EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(name, gl_type, source_type, source_format, count, size)	\
	TextureFormatTypeData(String(L##name "ui"), it_unsigned,	\
		gl_type##UI, GL_UNSIGNED_##source_type, source_format,	\
		count, size),	\
	TextureFormatTypeData(String(L##name "i"), it_signed, gl_type##I,	\
		GL_##source_type, source_format, count, size),	\
	TextureFormatTypeData(String(L##name), it_unsigned, gl_type,	\
		GL_UNSIGNED_##source_type, source_format, count, size),	\
	TextureFormatTypeData(String(L##name "snorm"), it_signed,	\
		gl_type##_SNORM, GL_##source_type, source_format, count, size)

		const TextureFormatTypeData texture_format_type_datas[] =
		{
			TextureFormatTypeData(String(L"rgba4"),
				it_unsigned_normalized, GL_RGBA4,
				GL_UNSIGNED_SHORT_4_4_4_4, GL_RGBA, 4, 16),
			TextureFormatTypeData(String(L"r3g3b2"),
				it_unsigned_normalized, GL_R3_G3_B2,
				GL_UNSIGNED_BYTE_3_3_2, GL_RGB, 3, 8),
			TextureFormatTypeData(String(L"r5g6b5"),
				it_unsigned_normalized, GL_RGB5,
				GL_UNSIGNED_SHORT_5_6_5, GL_RGB, 3, 16),
			TextureFormatTypeData(String(L"rgb5_a1"),
				it_unsigned_normalized, GL_RGB5_A1,
				GL_UNSIGNED_SHORT_5_5_5_1, GL_RGBA, 4, 16),
			TextureFormatTypeData(String(L"rgb10_a2"),
				it_unsigned_normalized, GL_RGB10_A2,
				GL_UNSIGNED_INT_10_10_10_2, GL_RGBA, 4, 32),
			TextureFormatTypeData(String(L"rgb10_a2_ui"),
				it_unsigned, GL_RGB10_A2UI,
				GL_UNSIGNED_INT_10_10_10_2, GL_RGBA, 4, 32),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("r8", GL_R8,
				BYTE, GL_RED, 1, 8),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rg8", GL_RG8,
				BYTE, GL_RG, 2, 16),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rgb8", GL_RGB8,
				BYTE, GL_RGB, 3, 24),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rgba8", GL_RGBA8,
				BYTE, GL_RGBA, 4, 32),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("r16", GL_R16,
				SHORT, GL_RED, 1, 16),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rg16", GL_RG16,
				SHORT, GL_RG, 2, 32),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rgb16", GL_RGB16,
				SHORT, GL_RGB, 3, 48),
			EXPAND_INTEGER_TEXTURE_FORMAT_TYPE("rgba16", GL_RGBA16,
				SHORT, GL_RGBA, 4, 64),
			TextureFormatTypeData(String(L"r32ui"), GL_R32UI,
				GL_UNSIGNED_INT, GL_RED, 1, 32),
			TextureFormatTypeData(String(L"r32i"), GL_R32I,
				GL_INT, GL_RED, 1, 32),
			TextureFormatTypeData(String(L"rg32ui"), GL_RG32UI,
				GL_UNSIGNED_INT, GL_RG, 2, 64),
			TextureFormatTypeData(String(L"rg32i"), GL_RG32I,
				GL_INT, GL_RG, 2, 64),
			TextureFormatTypeData(String(L"rgb32ui"),
				GL_RGB32UI, GL_UNSIGNED_INT, GL_RGB, 3, 96),
			TextureFormatTypeData(String(L"rgb32i"), GL_RGB32I,
				GL_INT, GL_RGB, 3, 96),
			TextureFormatTypeData(String(L"rgba32ui"),
				GL_RGBA32UI, GL_UNSIGNED_INT, GL_RGBA, 4, 128),
			TextureFormatTypeData(String(L"rgba32i"),
				GL_RGBA32I, GL_INT, GL_RGBA, 4, 128),
			TextureFormatTypeData(String(L"r16f"), GL_R16F,
				GL_HALF_FLOAT, GL_RED, 1, 16),
			TextureFormatTypeData(String(L"rg16f"), GL_RG16F,
				GL_HALF_FLOAT, GL_RG, 2, 32),
			TextureFormatTypeData(String(L"rgb16f"), GL_RGB16F,
				GL_HALF_FLOAT, GL_RGB, 3, 48),
			TextureFormatTypeData(String(L"rgba16f"),
				GL_RGBA16F, GL_HALF_FLOAT, GL_RGBA, 4, 64),
			TextureFormatTypeData(String(L"r32f"), GL_R32F,
				GL_FLOAT, GL_RED, 1, 32),
			TextureFormatTypeData(String(L"rg32f"), GL_RG32F,
				GL_FLOAT, GL_RG, 2, 64),
			TextureFormatTypeData(String(L"rgb32f"),
				GL_RGB32F, GL_FLOAT, GL_RGB, 3, 96),
			TextureFormatTypeData(String(L"rgba32f"),
				GL_RGBA32F, GL_FLOAT, GL_RGBA, 4, 128),
			TextureFormatTypeData(String(L"rgb_dxt1"),
				it_unsigned_normalized,
				GL_COMPRESSED_RGB_S3TC_DXT1_EXT, 3, 4),
			TextureFormatTypeData(String(L"rgba_dxt1"),
				it_unsigned_normalized,
				GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 4, 4),
			TextureFormatTypeData(String(L"rgba_dxt3"),
				it_unsigned_normalized,
				GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 4, 8),
			TextureFormatTypeData(String(L"rgba_dxt5"),
				it_unsigned_normalized,
				GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 4, 8),
			TextureFormatTypeData(String(L"r_rgtc1"),
				it_unsigned_normalized,
				GL_COMPRESSED_RED_RGTC1, 1, 4),
			TextureFormatTypeData(String(L"signed_r_rgtc1"),
				it_signed_normalized,
				GL_COMPRESSED_SIGNED_RED_RGTC1, 1, 4),
			TextureFormatTypeData(String(L"rg_rgtc2"),
				it_unsigned_normalized,
				GL_COMPRESSED_RG_RGTC2, 2, 8),
			TextureFormatTypeData(String(L"signed_rg_rgtc2"),
				it_signed_normalized,
				GL_COMPRESSED_SIGNED_RG_RGTC2, 2, 8),
			TextureFormatTypeData(String(L"depth16"),
				GL_DEPTH_COMPONENT16, GL_UNSIGNED_SHORT,
				GL_DEPTH_COMPONENT, 0, 16, false, false, true),
			TextureFormatTypeData(String(L"depth24"),
				GL_DEPTH_COMPONENT24, GL_UNSIGNED_INT,
				GL_DEPTH_COMPONENT, 0, 24, false, false, true),
			TextureFormatTypeData(String(L"depth32"),
				GL_DEPTH_COMPONENT32, GL_UNSIGNED_INT,
				GL_DEPTH_COMPONENT, 0, 32, false, false, true),
			TextureFormatTypeData(String(L"depth24_stencil8"),
				GL_DEPTH24_STENCIL8, GL_UNSIGNED_INT_24_8,
				GL_DEPTH_COMPONENT, 0, 32, false, false, true,
				true),
			TextureFormatTypeData(String(L"depth32f"),
				GL_DEPTH_COMPONENT32F, GL_FLOAT,
				GL_DEPTH_COMPONENT, 0, 32, false, false, true),
			TextureFormatTypeData(String(L"depth32f_stencil8"),
				GL_DEPTH32F_STENCIL8,
				GL_FLOAT_32_UNSIGNED_INT_24_8_REV,
				GL_DEPTH_COMPONENT, 0, 64, false, false, true,
				true),
			TextureFormatTypeData(String(L"srgb8"),
				it_unsigned_normalized, GL_SRGB8,
				GL_UNSIGNED_BYTE, GL_RGB, 3, 24),
			TextureFormatTypeData(String(L"srgb8_a8"),
				it_unsigned_normalized, GL_SRGB8_ALPHA8,
				GL_UNSIGNED_BYTE, GL_RGBA, 4, 32),
			TextureFormatTypeData(String(L"srgb_dxt1"),
				it_unsigned_normalized,
				GL_COMPRESSED_SRGB_S3TC_DXT1_EXT, 3, 4, true),
			TextureFormatTypeData(String(L"srgb_a_dxt1"),
				it_unsigned_normalized,
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT, 4, 4,
				true),
			TextureFormatTypeData(String(L"srgb_a_dxt3"),
				it_unsigned_normalized,
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT, 4, 8,
				true),
			TextureFormatTypeData(String(L"srgb_a_dxt5"),
				it_unsigned_normalized,
				GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT, 4, 8,
				true),
		};

#undef EXPAND_INTEGER_TEXTURE_FORMAT_TYPE

		const Uint32 texture_format_type_datas_count =
			sizeof(texture_format_type_datas) /
			sizeof(TextureFormatTypeData);

	};

	void TextureFormatUtil::get_source_format_type(
		const TextureFormatType texture_format, GLenum &format,
		GLenum &type)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		type = texture_format_type_datas[
			texture_format].get_source_type();
		format = texture_format_type_datas[
			texture_format].get_format_type();
	}

	Uint16 TextureFormatUtil::get_count(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		return texture_format_type_datas[texture_format].get_count();
	}

	const String &TextureFormatUtil::get_str(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		return texture_format_type_datas[texture_format].get_name();
	}

	bool TextureFormatUtil::get_sRGB(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		return texture_format_type_datas[texture_format].get_sRGB();
	}


	bool TextureFormatUtil::get_depth(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		return texture_format_type_datas[texture_format].get_depth();
	}


	bool TextureFormatUtil::get_stencil(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		return texture_format_type_datas[texture_format].get_stencil();
	}


	bool TextureFormatUtil::get_compressed(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		return texture_format_type_datas[
			texture_format].get_compressed();
	}

	bool TextureFormatUtil::get_format_supported(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		switch (texture_format)
		{
			case tft_rgb10_a2_ui:
				return GLEW_VERSION_3_3 ||
					GLEW_ARB_texture_rgb10_a2ui;
			case tft_rgb16f:
			case tft_rgba16f:
			case tft_rgb32f:
			case tft_rgba32f:
				return GLEW_VERSION_3_0 ||
					GLEW_ARB_texture_float;
			case tft_rgb_dxt1:
			case tft_rgba_dxt1:
			case tft_rgba_dxt3:
			case tft_rgba_dxt5:
				return GLEW_EXT_texture_compression_s3tc;
			case tft_r_rgtc1:
			case tft_signed_r_rgtc1:
			case tft_rg_rgtc2:
			case tft_signed_rg_rgtc2:
				return GLEW_VERSION_3_0 ||
					GLEW_EXT_texture_compression_rgtc ||
					GLEW_ARB_texture_compression_rgtc;
			case tft_depth24_stencil8:
				return GLEW_VERSION_3_0 ||
					GLEW_EXT_packed_depth_stencil ||
					GLEW_ARB_framebuffer_object;
			case tft_depth32f:
			case tft_depth32f_stencil8:
				return GLEW_VERSION_3_0 ||
					GLEW_ARB_depth_buffer_float;
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
			texture_format_type_datas[texture_format];

		if (format.get_integer())
		{
			switch (format.get_integer_type())
			{
				case it_unsigned:
				case it_signed:
					if (format.get_count() > 2)
					{
						return GLEW_VERSION_3_0 ||
							GLEW_ARB_texture_rg;
					}
					else
					{
						return GLEW_VERSION_3_0 ||
							GLEW_EXT_texture_integer;
					}
				case it_unsigned_normalized:
					if (format.get_count() > 2)
					{
						return true;
					}
					else
					{
						return GLEW_VERSION_3_0 ||
							GLEW_ARB_texture_rg;
					}
				case it_signed_normalized:
					return GLEW_VERSION_3_1 ||
						GLEW_EXT_texture_snorm;
			}
		}

		return true;
	}

	Uint32 TextureFormatUtil::get_size(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		return texture_format_type_datas[texture_format].get_size();
	}


	GLenum TextureFormatUtil::get_gl_format(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		return texture_format_type_datas[texture_format].get_gl_type();
	}


	IntegerType TextureFormatUtil::get_integer_type(
		const TextureFormatType texture_format)
	{
		if (texture_format_type_datas_count <= texture_format)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					texture_format_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture_format))
				<< boost::errinfo_type_info_name(
					"TextureFormatType"));
		}

		return texture_format_type_datas[
			texture_format].get_integer_type();
	}

	TextureFormatType TextureFormatUtil::get_type(const String &str)
	{
		Uint32 i;
		TextureFormatType texture_format_type;

		for (i = 0; i < texture_format_type_datas_count; i++)
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
			<< boost::errinfo_type_info_name(
				"TextureFormatUtil"));
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

		value = TextureFormatUtil::get_type(String(string));

		return str;
	}

}
