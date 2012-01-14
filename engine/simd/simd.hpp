/****************************************************************************
 *            simd.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_fa217cb6_0bb7_4566_8f14_0e5865b3bd9b
#define	UUID_fa217cb6_0bb7_4566_8f14_0e5865b3bd9b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class SIMD.
 * This file contains the @c class SIMD.
 */
namespace eternal_lands
{

	class SIMD
	{
		public:
			/**
			 * Converts an array of floats to half floats using
			 * SSE2. Floats are processed in blocks of four each
			 * and are stored as blocks of four half floats.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source memory must be 16 byte
			 * aligned.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the four half float
			 * blocks.
			 * @param dest The dest memory where the half floats
			 * are stored.
			 */
			static void float_to_half_4(const float* source,
				const Uint32 count, const Uint32 stride,
				Uint8* dest);

			/**
			 * Converts an array of floats to half floats using
			 * SSE2. Floats are processed in blocks of four each
			 * and are stored as blocks of two half floats. Only
			 * the two lower floats are processed.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the two half float
			 * blocks.
			 * @param dest The dest memory where the half floats
			 * are stored.
			 */
			static void float_to_half_2(const float* source,
				const Uint32 count, const Uint32 stride,
				Uint8* dest);

			/**
			 * Converts an array of floats to unsigned shorts using
			 * SSE2. Floats are processed in blocks of four each
			 * and are stored as blocks of four unsigned shorts.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source memory must be 16 byte
			 * aligned.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the four unsigned
			 * shorts blocks.
			 * @param dest The dest memory where the unsigned shorts
			 * are stored.
			 */
			static void float_to_unsigned_short_4(
				const float* source, const Uint32 count,
				const Uint32 stride, Uint8* dest);

			/**
			 * Converts an array of floats to unsigned shorts using
			 * SSE2. Floats are processed in blocks of four each
			 * and are stored as blocks of two unsigned shorts.
			 * Only the two lower floats are processed.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the two unsigned
			 * shorts 
			 * @param dest The dest memory where the unsigned shorts
			 * are stored.
			 */
			static void float_to_unsigned_short_2(
				const float* source, const Uint32 count,
				const Uint32 stride, Uint8* dest);

			/**
			 * Converts an array of floats to unsigned normalized
			 * shorts using SSE2. Floats are processed in blocks of
			 * four each and are stored as blocks of four unsigned
			 * normalized shorts.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source memory must be 16 byte
			 * aligned.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the four unsigned
			 * normalized shorts .
			 * @param dest The dest memory where the unsigned
			 * normalized shorts are stored.
			 */
			static void float_to_unsigned_normalized_short_4(
				const float* source, const Uint32 count,
				const Uint32 stride, Uint8* dest);

			/**
			 * Converts an array of floats to unsigned normalized
			 * shorts using SSE2. Floats are processed in blocks of
			 * four each and are stored as blocks of two unsigned
			 * normalized shorts. Only the two lower floats are
			 * processed.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the two unsigned
			 * normalized shorts blocks.
			 * @param dest The dest memory where the unsigned
			 * normalized shorts are stored.
			 */
			static void float_to_unsigned_normalized_short_2(
				const float* source, const Uint32 count,
				const Uint32 stride, Uint8* dest);

			/**
			 * Converts an array of floats to signed shorts using
			 * SSE2. Floats are processed in blocks of four each
			 * and are stored as blocks of four signed shorts.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source memory must be 16 byte
			 * aligned.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the four signed
			 * shorts blocks.
			 * @param dest The dest memory where the signed shorts
			 * are stored.
			 */
			static void float_to_signed_short_4(const float* source,
				const Uint32 count, const Uint32 stride,
				Uint8* dest);

			/**
			 * Converts an array of floats to signed shorts using
			 * SSE2. Floats are processed in blocks of four each
			 * and are stored as blocks of two signed shorts.
			 * Only the two lower floats are processed.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the two signed
			 * shorts 
			 * @param dest The dest memory where the signed shorts
			 * are stored.
			 */
			static void float_to_signed_short_2(const float* source,
				const Uint32 count, const Uint32 stride,
				Uint8* dest);

			/**
			 * Converts an array of floats to signed normalized
			 * shorts using SSE2. Floats are processed in blocks of
			 * four each and are stored as blocks of four signed
			 * normalized shorts.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source memory must be 16 byte
			 * aligned.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the four signed
			 * normalized shorts .
			 * @param dest The dest memory where the signed
			 * normalized shorts are stored.
			 */
			static void float_to_signed_normalized_short_4(
				const float* source, const Uint32 count,
				const Uint32 stride, Uint8* dest);

			/**
			 * Converts an array of floats to signed normalized
			 * shorts using SSE2. Floats are processed in blocks of
			 * four each and are stored as blocks of two signed
			 * normalized shorts. Only the two lower floats are
			 * processed.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the two signed
			 * normalized shorts blocks.
			 * @param dest The dest memory where the signed
			 * normalized shorts are stored.
			 */
			static void float_to_signed_normalized_short_2(
				const float* source, const Uint32 count,
				const Uint32 stride, Uint8* dest);

			/**
			 * Converts an array of floats to unsigned bytes using
			 * SSE2. Floats are processed in blocks of four each
			 * and are stored as blocks of four unsigned bytes.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source memory must be 16 byte
			 * aligned.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the four unsigned
			 * bytes blocks.
			 * @param dest The dest memory where the unsigned bytes
			 * are stored.
			 */
			static void float_to_unsigned_byte_4(
				const float* source, const Uint32 count,
				const Uint32 stride, Uint8* dest);

			/**
			 * Converts an array of floats to unsigned normalized
			 * bytes using SSE2. Floats are processed in blocks of
			 * four each and are stored as blocks of four unsigned
			 * normalized bytes.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source memory must be 16 byte
			 * aligned.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param stride The stride between the four unsigned
			 * normalized bytes .
			 * @param dest The dest memory where the unsigned
			 * normalized bytes are stored.
			 */
			static void float_to_unsigned_normalized_byte_4(
				const float* source, const Uint32 count,
				const Uint32 stride, Uint8* dest);

			/**
			 * Transforms an array of floats using SSE2. Floats are
			 * processed in blocks of four each and transformed
			 * using the matrix and are stored as blocks of four
			 * floats.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source memory must be 16 byte
			 * aligned.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param matrix The transformation matrix to use.
			 * @param dest The dest memory must be 16 byte
			 * aligned.
			 */
			static void transform(const float* source,
				const Uint32 count, const glm::mat4x3 &matrix,
				float* dest);

			/**
			 * Transforms an array of floats using SSE2. Floats are
			 * processed in blocks of three each and transformed
			 * using the matrix and are stored as blocks of four
			 * floats.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param source The source memory must be 16 byte
			 * aligned.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param matrix The transformation matrix to use.
			 * @param dest The dest memory must be 16 byte
			 * aligned.
			 */
			static void transform(const float* source,
				const Uint32 count, const glm::mat3x3 &matrix,
				float* dest);

			/**
			 * Fills the memory using the given data.
			 * Be carefull when using these functions, no error
			 * checking is done!
			 * @param data The data to use for filling the memory.
			 * @param count The Number of four float blocks to
			 * process.
			 * @param dest The dest memory must be 16 byte
			 * aligned.
			 */
			static void fill(const glm::vec4 &data,
				const Uint32 count, float* dest);

	};

}

#endif	/* UUID_fa217cb6_0bb7_4566_8f14_0e5865b3bd9b */
