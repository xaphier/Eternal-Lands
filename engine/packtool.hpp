/****************************************************************************
 *            packtool.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_3afdaee9_6721_4536_9aeb_bf4558d8a4b9
#define	UUID_3afdaee9_6721_4536_9aeb_bf4558d8a4b9

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "alignedarrays.hpp"

/**
 * @file
 * @brief The @c class PackTool.
 * This file contains the @c class PackTool.
 */
namespace eternal_lands
{

#define PACK_FORMAT(type)	\
	pft_##type##_1,	\
	pft_##type##_2,	\
	pft_##type##_3,	\
	pft_##type##_4

#define PACK_FORMAT_INT(type)	\
	PACK_FORMAT(unsigned_##type),	\
	PACK_FORMAT(signed_##type),	\
	PACK_FORMAT(unsigned_normalized_##type),	\
	PACK_FORMAT(signed_normalized_##type)

#define PACK_FORMAT_PACKED_3(type, x, y, z)	\
	pft_unsigned_##type##_##x##_##y##_##z,	\
	pft_signed_##type##_##x##_##y##_##z,	\
	pft_unsigned_normalized_##type##_##x##_##y##_##z,	\
	pft_signed_normalized_##type##_##x##_##y##_##z,	\
	pft_unsigned_##type##_rev_##z##_##y##_##x,	\
	pft_signed_##type##_rev_##z##_##y##_##x,	\
	pft_unsigned_normalized_##type##_rev_##z##_##y##_##x,	\
	pft_signed_normalized_##type##_rev_##z##_##y##_##x

#define PACK_FORMAT_PACKED_4(type, x, y, z, w)	\
	pft_unsigned_##type##_##x##_##y##_##z##_##w,	\
	pft_signed_##type##_##x##_##y##_##z##_##w,	\
	pft_unsigned_normalized_##type##_##x##_##y##_##z##_##w,	\
	pft_signed_normalized_##type##_##x##_##y##_##z##_##w,	\
	pft_unsigned_##type##_rev_##w##_##z##_##y##_##x,	\
	pft_signed_##type##_rev_##w##_##z##_##y##_##x,	\
	pft_unsigned_normalized_##type##_rev_##w##_##z##_##y##_##x,	\
	pft_signed_normalized_##type##_rev_##w##_##z##_##y##_##x

	enum PackFormatType
	{
		PACK_FORMAT_INT(byte),
		PACK_FORMAT_INT(short),
		PACK_FORMAT_INT(int),
		PACK_FORMAT(float),
		PACK_FORMAT(half),
		PACK_FORMAT_PACKED_3(byte, 3, 3, 2),
		PACK_FORMAT_PACKED_4(short, 4, 4, 4, 4),
		PACK_FORMAT_PACKED_3(short, 5, 6, 5),
		PACK_FORMAT_PACKED_4(short, 5, 5, 5, 1),
		PACK_FORMAT_PACKED_4(int, 10, 10, 10, 2),
		pft_rgb9_e5,
		pft_r10f_g11f_b10f
	};

#undef	PACK_FORMAT
#undef	PACK_FORMAT_INT
#undef	PACK_FORMAT_PACKED_3
#undef	PACK_FORMAT_PACKED_4

	class PackTool
	{
		public:
			/**
			 * Compress a normalized vector into a 16 bit unsigned
			 * int.
			 * @param value The normalized vector to compress.
			 * @return The compressed normalized vector.
			 */
			static Uint16 compress_normalized(
				const glm::vec3 &value);

			/**
			 * Uncompress a 16 bit unsigned int into a normalized
			 * vector.
			 * @param value The 16 bit unsigned int to uncompress.
			 * @return The normalized vector.
			 */
			static glm::vec3 uncompress_normalized(
				const Uint16 value);

			/**
			 * Converts to Lambert Azimuthal Equal-Area projection.
			 * @param value The normalized vector to use.
			 * @return The Lambert Azimuthal Equal-Area projection
			 * vector (0..1).
			 */
			static glm::vec2 encode_normal(const glm::vec3 &value);

			/**
			 * Converts from Lambert Azimuthal Equal-Area
			 * projection.
			 * @param value The Lambert Azimuthal Equal-Area
			 * projection vector to use (0..1).
			 * @return The normalized vector to use
			 */
			static glm::vec3 decode_normal(const glm::vec2 &value);

			/**
			 * Converts to Lambert Azimuthal Equal-Area projection.
			 * Optimized for normalized Uint8 values.
			 * @param value The normalized vector to use.
			 * @return The Lambert Azimuthal Equal-Area projection
			 * vector (0..1).
			 */
			static glm::vec2 encode_normal_optimized(
				const glm::vec3 &value);

			/**
			 * Converts from Lambert Azimuthal Equal-Area
			 * projection. Optimized for normalized Uint8 values.
			 * @param value The Lambert Azimuthal Equal-Area
			 * projection vector to use (0..1).
			 * @return The normalized vector.
			 */
			static glm::vec3 decode_normal_optimized(
				const glm::vec2 &value);

			/**
			 * Converts to Lambert Azimuthal Equal-Area projection.
			 * Optimized for Uint8 values (0..255)
			 * @param value The normalized vector to use.
			 * @return The Lambert Azimuthal Equal-Area projection
			 * vector as Uint8 values (0..255)
			 */
			static glm::uvec2 encode_normal_optimized_uint8(
				const glm::vec3 &value);

			/**
			 * Converts from Lambert Azimuthal Equal-Area
			 * projection. Optimized for Uint8 values (0..255)
			 * @param value The Lambert Azimuthal Equal-Area
			 * projection vector to use as Uint8 values (0..255)
			 * @return The normalized vector to use
			 */
			static glm::vec3 decode_normal_optimized_uint8(
				const glm::uvec2 &value);

			/**
			 * Unpacks an 8 bit 3_3_2-value into three unsigned
			 * normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 8 bit unsigned int to unpack.
			 * @return Vector of three unsigned normalized floats.
			 */
			static glm::vec3 unpack_uint_3_3_2(const bool normalize,
				const Uint8 value);

			/**
			 * Unpacks an 8 bit 2_3_3_rev-value into three unsigned
			 * normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 8 bit unsigned int to unpack.
			 * @return Vector of three unsigned normalized floats.
			 */
			static glm::vec3 unpack_uint_2_3_3_rev(
				const bool normalize, const Uint8 value);

			/**
			 * Unpacks a 16 bit 5_6_5-value into three unsigned
			 * normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 16 bit unsigned int to unpack.
			 * @return Vector of three unsigned normalized floats.
			 */
			static glm::vec3 unpack_uint_5_6_5(const bool normalize,
				const Uint16 value);

			/**
			 * Unpacks a 16 bit 5_6_5_rev-value into three unsigned
			 * normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 16 bit unsigned int to unpack.
			 * @return Vector of three unsigned normalized floats.
			 */
			static glm::vec3 unpack_uint_5_6_5_rev(
				const bool normalize, const Uint16 value);

			/**
			 * Unpacks a 16 bit 5_5_5_1-value into four unsigned
			 * normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 16 bit unsigned int to unpack.
			 * @return Vector of four unsigned normalized floats.
			 */
			static glm::vec4 unpack_uint_5_5_5_1(
				const bool normalize, const Uint16 value);

			/**
			 * Unpacks a 16 bit 1_5_5_5_rev-value into four
			 * unsigned normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 16 bit unsigned int to unpack.
			 * @return Vector of four unsigned normalized floats.
			 */
			static glm::vec4 unpack_uint_1_5_5_5_rev(
				const bool normalize, const Uint16 value);

			/**
			 * Unpacks a 16 bit 4_4_4_4-value into four unsigned
			 * normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 16 bit unsigned int to unpack.
			 * @return Vector of four unsigned normalized floats.
			 */
			static glm::vec4 unpack_uint_4_4_4_4(
				const bool normalize, const Uint16 value);

			/**
			 * Unpacks a 16 bit 4_4_4_4_rev-value into four
			 * unsigned normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 16 bit unsigned int to unpack.
			 * @return Vector of four unsigned normalized floats.
			 */
			static glm::vec4 unpack_uint_4_4_4_4_rev(
				const bool normalize, const Uint16 value);

			/**
			 * Unpacks a 32 bit 8_8_8_8-value into four unsigned
			 * normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 32 bit unsigned int to unpack.
			 * @return Vector of four unsigned normalized floats.
			 */
			static glm::vec4 unpack_uint_8_8_8_8(
				const bool normalize, const Uint32 value);

			/**
			 * Unpacks a 32 bit 8_8_8_8_rev-value into four
			 * unsigned normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 32 bit unsigned int to unpack.
			 * @return Vector of four unsigned normalized floats.
			 */
			static glm::vec4 unpack_uint_8_8_8_8_rev(
				const bool normalize, const Uint32 value);

			/**
			 * Unpacks a 32 bit 10_10_10_2-value into four unsigned
			 * normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 32 bit unsigned int to unpack.
			 * @return Vector of four unsigned normalized floats.
			 */
			static glm::vec4 unpack_uint_10_10_10_2(
				const bool normalize, const Uint32 value);

			/**
			 * Unpacks a 32 bit 2_10_10_10_rev-value into four
			 * unsigned normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 32 bit unsigned int to unpack.
			 * @return Vector of four unsigned normalized floats.
			 */
			static glm::vec4 unpack_uint_2_10_10_10_rev(
				const bool normalize, const Uint32 value);

			/**
			 * Unpacks a 32 bit 10_10_10_2-value into four signed
			 * normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 32 bit unsigned int to unpack.
			 * @return Vector of four signed normalized floats.
			 */
			static glm::vec4 unpack_sint_10_10_10_2(
				const bool normalize, const Uint32 value);

			/**
			 * Unpacks a 32 bit 2_10_10_10_rev-value into four
			 * signed normalized floats.
			 * @param normalize When true the result values get
			 * normalized.
			 * @param value The 32 bit unsigned int to unpack.
			 * @return Vector of four signed normalized floats.
			 */
			static glm::vec4 unpack_sint_2_10_10_10_rev(
				const bool normalize, const Uint32 value);

			/**
			 * Packs three unsigned normalized floats into an 8 bit
			 * 3_3_2-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param Vector of three unsigned normalized floats to
			 * pack.
			 * @return The packed 8 bit unsigned int.
			 */
			static Uint8 pack_uint_3_3_2(const bool normalize,
				const glm::vec3 &value);

			/**
			 * Packs three unsigned normalized floats into an 8 bit
			 * 2_3_3_rev-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param Vector of three unsigned normalized floats to
			 * pack.
			 * @return The packed 8 bit unsigned int.
			 */
			static Uint8 pack_uint_2_3_3_rev(const bool normalize,
				const glm::vec3 &value);

			/**
			 * Packs three unsigned normalized floats into a 16 bit
			 * 5_6_5-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param Vector of three unsigned normalized floats to
			 * pack.
			 * @return The packed 16 bit unsigned int.
			 */
			static Uint16 pack_uint_5_6_5(const bool normalize,
				const glm::vec3 &value);

			/**
			 * Packs three unsigned normalized floats into a 16 bit
			 * 5_6_5_rev-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param Vector of three unsigned normalized floats to
			 * pack.
			 * @return The packed 16 bit unsigned int.
			 */
			static Uint16 pack_uint_5_6_5_rev(const bool normalize,
				const glm::vec3 &value);

			/**
			 * Packs four unsigned normalized floats into a 16 bit
			 * 5_5_5_1-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param Vector of four unsigned normalized floats to
			 * pack.
			 * @return The packed 16 bit unsigned int.
			 */
			static Uint16 pack_uint_5_5_5_1(const bool normalize,
				const glm::vec4 &value);

			/**
			 * Packs four unsigned normalized floats into a 16 bit
			 * 1_5_5_5_rev-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param Vector of four unsigned normalized floats to
			 * pack.
			 * @return The packed 16 bit unsigned int.
			 */
			static Uint16 pack_uint_1_5_5_5_rev(
				const bool normalize, const glm::vec4 &value);

			/**
			 * Packs four unsigned normalized floats into a 16 bit
			 * 4_4_4_4-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param Vector of four unsigned normalized floats to
			 * pack.
			 * @return The packed 16 bit unsigned int.
			 */
			static Uint16 pack_uint_4_4_4_4(const bool normalize,
				const glm::vec4 &value);

			/**
			 * Packs four unsigned normalized floats into a 16 bit
			 * 4_4_4_4_rev-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param Vector of four unsigned normalized floats to
			 * pack.
			 * @return The packed 16 bit unsigned int.
			 */
			static Uint16 pack_uint_4_4_4_4_rev(
				const bool normalize, const glm::vec4 &value);

			/**
			 * Packs four unsigned normalized floats into a 32 bit
			 * 8_8_8_8-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param value Vector of four unsigned normalized
			 * floats to pack.
			 * @return The packed 32 bit unsigned int.
			 */
			static Uint32 pack_uint_8_8_8_8(const bool normalize,
				const glm::vec4 &value);

			/**
			 * Packs four unsigned normalized floats into a 32 bit
			 * 8_8_8_8_rev-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param value Vector of four unsigned normalized
			 * floats to pack.
			 * @return The packed 32 bit unsigned int.
			 */
			static Uint32 pack_uint_8_8_8_8_rev(
				const bool normalize, const glm::vec4 &value);

			/**
			 * Packs four unsigned normalized floats into a 32 bit
			 * 10_10_10_2-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param value Vector of four unsigned normalized
			 * floats to pack.
			 * @return The packed 32 bit unsigned int.
			 */
			static Uint32 pack_uint_10_10_10_2(const bool normalize,
				const glm::vec4 &value);

			/**
			 * Packs four unsigned normalized floats into a 32 bit
			 * 2_10_10_10_rev-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param value Vector of four unsigned normalized
			 * floats to pack.
			 * @return The packed 32 bit unsigned int.
			 */
			static Uint32 pack_uint_2_10_10_10_rev(
				const bool normalize, const glm::vec4 &value);

			/**
			 * Packs four signed normalized floats into a 32 bit
			 * 10_10_10_2-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param value Vector of four signed normalized floats
			 * to pack.
			 * @return The packed 32 bit unsigned int.
			 */
			static Uint32 pack_sint_10_10_10_2(const bool normalize,
				const glm::vec4 &value);

			/**
			 * Packs four signed normalized floats into a 32 bit
			 * 2_10_10_10_rev-value.
			 * @param normalize When true the values are treated as
			 * normalized.
			 * @param value Vector of four signed normalized floats
			 * to pack.
			 * @return The packed 32 bit unsigned int.
			 */
			static Uint32 pack_sint_2_10_10_10_rev(
				const bool normalize, const glm::vec4 &value);

			/**
			 * Encodes as color with shared exponent, used for
			 * RGB9E5 textures. Useful for HDR textures, because
			 * values from 0 up to 65408 can be stored.
			 * @param value Vector of three floats.
			 * @return The 32 bit unsigned int representing the
			 * RGB9E5 value.
			 */
			static Uint32 encode_rgb9e5(const glm::vec3 &value);

			/**
			 * Decodes from a color with shared exponent, used for
			 * RGB9E5 textures. Useful for HDR textures, because
			 * values from 0 up to 65408 can be stored.
			 * @param value The 32 bit unsigned int representing
			 * the RGB9E5 value.
			 * @return Vector of three floats.
			 */
			static glm::vec3 decode_rgb9e5(const Uint32 value);

			static glm::vec4 unpack_vec4(
				const AbstractReadMemory &buffer,
				const Uint64 offset, const PackFormatType type);

			static void pack(const glm::vec4 &data,
				const Uint64 offset,
				const PackFormatType pack_format,
				AbstractWriteMemory &buffer);

			static void pack(const Vec4Vector &data,
				const Uint64 offset, const Uint32 stride,
				const Uint32 count,
				const PackFormatType pack_format,
				AbstractWriteMemory &buffer);

			static void pack(const AlignedVec4Array &data,
				const Uint64 offset, const Uint32 stride,
				const Uint32 count,
				const PackFormatType pack_format,
				const bool use_simd,
				AbstractWriteMemory &buffer);

			static String get_str(const PackFormatType pack_format);

	};

	OutStream &operator<<(OutStream &str, const PackFormatType value);

}

#endif	/* UUID_3afdaee9_6721_4536_9aeb_bf4558d8a4b9 */
