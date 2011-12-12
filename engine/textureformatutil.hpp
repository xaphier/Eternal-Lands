/****************************************************************************
 *            textureformatutil.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a9fa1f4e_be28_429d_bf79_551076987253
#define	UUID_a9fa1f4e_be28_429d_bf79_551076987253

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "packtool.hpp"

/**
 * @file
 * @brief The @c class TextureFormatUtil.
 * This file contains the @c class TextureFormatUtil.
 */
namespace eternal_lands
{

#define EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(type)	\
	tft_##type##_ui,	\
	tft_##type##_i,	\
	tft_##type,	\
	tft_##type##_snorm

	/**
	 * Basic types of the shader texture format types.
	 * @{
	 */
	enum TextureFormatType
	{
		/**
		 * Red 4 bit, green 4 bit, blue 4 bit, alpha 4 bit
		 */
		tft_rgba4 = 0,
		/**
		 * Red 3 bit, green 3 bit, blue 2 bit, no alpha
		 */
		tft_r3g3b2,
		/**
		 * Red 5 bit, green 6 bit, blue 5 bit, no alpha
		 */
		tft_r5g6b5,
		/**
		 * Red 5 bit, green 5 bit, blue 5 bit, alpha 1
		 */
		tft_rgb5_a1,
		/**
		 * Red 10 bit, green 10 bit, blue 10 bit, alpha 2
		 */
		tft_rgb10_a2,
		/**
		 * Red 10 bit, green 10 bit, blue 10 bit, alpha 2
		 */
		tft_rgb10_a2_ui,
		/**
		 * Red 8 bit, no green, no blue, no alpha
		 */
		EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(r8),
		/**
		 * Red 8 bit, green 8 bit, no blue, no alpha
		 */
		EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(rg8),
		/**
		 * Red 8 bit, green 8 bit, blue 8 bit, no alpha
		 */
		EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(rgb8),
		/**
		 * Red 8 bit, green 8 bit, blue 8 bit, alpha 8 bit
		 */
		EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(rgba8),
		/**
		 * Red 16 bit, no green, no blue, no alpha
		 */
		EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(r16),
		/**
		 * Red 16 bit, green 16 bit, no blue, no alpha
		 */
		EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(rg16),
		/**
		 * Red 16 bit, green 16 bit, blue 16 bit, no alpha
		 */
		EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(rgb16),
		/**
		 * Red 16 bit, green 16 bit, blue 16 bit, alpha 16 bit
		 */
		EXPAND_INTEGER_TEXTURE_FORMAT_TYPE(rgba16),
		/**
		 * Red 32 bit, no green, no blue, no alpha
		 */
		tft_r32_ui,
		/**
		 * Red 32 bit, no green, no blue, no alpha
		 */
		tft_r32_i,
		/**
		 * Red 32 bit, green 32 bit, no blue, no alpha
		 */
		tft_rg32_ui,
		/**
		 * Red 32 bit, green 32 bit, no blue, no alpha
		 */
		tft_rg32_i,
		/**
		 * Red 32 bit, green 32 bit, blue 32 bit, no alpha
		 */
		tft_rgb32_ui,
		/**
		 * Red 32 bit, green 32 bit, blue 32 bit, no alpha
		 */
		tft_rgb32_i,
		/**
		 * Red 32 bit, green 32 bit, blue 32 bit, alpha 32 bit
		 */
		tft_rgba32_ui,
		/**
		 * Red 32 bit, green 32 bit, blue 32 bit, alpha 32 bit
		 */
		tft_rgba32_i,
		/**
		 * Red 16 bit float, no green, no blue, no alpha
		 */
		tft_r16f,
		/**
		 * Red 16 bit float, green 16 bit float, no blue, no alpha
		 */
		tft_rg16f,
		/**
		 * Red 16 bit float, green 16 bit float, blue 16 bit float, no alpha
		 */
		tft_rgb16f,
		/**
		 * Red 16 bit float, green 16 bit float, blue 16 bit float, alpha 16 bit float
		 */
		tft_rgba16f,
		/**
		 * Red 32 bit float, no green, no blue, no alpha
		 */
		tft_r32f,
		/**
		 * Red 32 bit float, green 32 bit float, no blue, no alpha
		 */
		tft_rg32f,
		/**
		 * Red 32 bit float, green 32 bit float, blue 32 bit float, no alpha
		 */
		tft_rgb32f,
		/**
		 * Red 32 bit float, green 32 bit float, blue 32 bit float, alpha 32 bit float
		 */
		tft_rgba32f,
		/**
		 * DXT1 compression without alpha
		 */
		tft_rgb_dxt1,
		/**
		 * DXT1 compression with alpha
		 */
		tft_rgba_dxt1,
		/**
		 * DXT3 compression
		 */
		tft_rgba_dxt3,
		/**
		 * DXT5 compression
		 */
		tft_rgba_dxt5,
		/**
		 * RGTC1 compression
		 */
		tft_r_rgtc1,
		/**
		 * RGTC2 compression
		 */
		tft_rg_rgtc2,
		/**
		 * Signed RGTC1 compression
		 */
		tft_signed_r_rgtc1,
		/**
		 * Signed RGTC2 compression
		 */
		tft_signed_rg_rgtc2,
		/**
		 * Depth 16 bit
		 */
		tft_depth16,
		/**
		 * Depth 24 bit
		 */
		tft_depth24,
		/**
		 * Depth 32 bit
		 */
		tft_depth32,
		/**
		 * Depth compononet 24 bit and 8 bits stencil
		 */
		tft_depth24_stencil8,
		/**
		 * Depth 32 bit float
		 */
		tft_depth32f,
		/**
		 * Depth 32 bit float and 8 bits stencil
		 */
		tft_depth32f_stencil8,
		/**
		 * Red, green and blue sRGB color space values
		 */
		tft_srgb8,
		/**
		 * Red, green and blue sRGB color space values and 8 bit alpha
		 */
		tft_srgb8_a8,
		/**
		 * Red, green and blue sRGB color space values compressed with DXT1 without alpha
		 */
		tft_srgb_dxt1,
		/**
		 * Red, green and blue sRGB color space values and alpha compressed with DXT1
		 */
		tft_srgb_a_dxt1,
		/**
		 * Red, green and blue sRGB color space values and alpha compressed with DXT3
		 */
		tft_srgb_a_dxt3,
		/**
		 * Red, green and blue sRGB color space values and alpha compressed with DXT5
		 */
		tft_srgb_a_dxt5
	};
	/**
	 * @}
	 */

#undef EXPAND_INTEGER_TEXTURE_FORMAT_TYPE

	enum IntegerType
	{
		it_unsigned = 0,
		it_signed,
		it_unsigned_normalized,
		it_signed_normalized
	};

	enum ImageCompressionType
	{
		ict_s3tc = 0,
		ict_rgtc = 1
	};

	typedef std::set<ImageCompressionType> ImageCompressionTypeSet;

	class TextureFormatUtil
	{
		public:
			static void get_source_format_type(
				const TextureFormatType texture_format,
				GLenum &format, GLenum &type);
			static Uint16 get_count(
				const TextureFormatType texture_format);
			static const String &get_str(
				const TextureFormatType texture_format);
			static bool get_sRGB(
				const TextureFormatType texture_format);
			static bool get_depth(
				const TextureFormatType texture_format);
			static bool get_stencil(
				const TextureFormatType texture_format);
			static bool get_compressed(
				const TextureFormatType texture_format);
			static bool get_format_supported(
				const TextureFormatType texture_format);
			static Uint32 get_size(
				const TextureFormatType texture_format);
			static GLenum get_gl_format(
				const TextureFormatType texture_format);
			static IntegerType get_integer_type(
				const TextureFormatType texture_format);
			static TextureFormatType get_texture_format(
				const String &str);
			static PackFormatType get_pack_format_type(
				const TextureFormatType texture_format);
			static bool get_has_format_type(
				const TextureFormatType texture_format);

	};

	OutStream& operator<<(OutStream &str, const TextureFormatType value);
	InStream& operator>>(InStream &str, TextureFormatType &value);

}

#endif	/* UUID_a9fa1f4e_be28_429d_bf79_551076987253 */
