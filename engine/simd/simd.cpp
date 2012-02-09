/****************************************************************************
 *            simd.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "simd.hpp"
#ifdef	USE_SSE2
#include <emmintrin.h>
#endif	/* USE_SSE2 */

namespace eternal_lands
{

	void SIMD::float_to_half_4(const float* source, const Uint32 count,
		const Uint32 stride, unsigned char* dest)
	{
#ifdef	USE_SSE2
		__m128i t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
		__m128i t10, t11, t12, t13, t14, t15, t16, t17, t18;
		__m128i t19, t20, t21, t22, t23, t24, t25, abs, infinity;
		__m128i denormal, mantissa, one_dot, exponent, fixup;
		__m128i round1, round2, sign, base, adjust;
		Uint32 i;

		abs = _mm_set1_epi32(0x7FFFFFFF);
		infinity = _mm_set1_epi32(0x47FFEFFF);
		denormal = _mm_set1_epi32(0x38800000);
		mantissa = _mm_set1_epi32(0x007FFFFF);
		one_dot = _mm_set1_epi32(0x00800000);
		exponent = _mm_set1_epi32(0x7F800000);
		fixup = _mm_set1_epi32(0x48000000);
		round1 = _mm_set1_epi32(0x00000001);
		round2 = _mm_set1_epi32(0x00000FFF);
		sign = _mm_set1_epi32(0x80000000);
		base = _mm_set1_epi32(0x00007FFF);
		adjust = _mm_set1_epi32(0x07000000);

		for (i = 0; i < count; ++i)
		{
			t0 = (__m128i)_mm_load_ps(source + i * 4);

			// Compute masks
			t1 = _mm_and_si128(t0, abs);
			t2 = (__m128i)_mm_cmpnlt_ps((__m128)t1, (__m128)infinity);
			t3 = (__m128i)_mm_cmplt_ps((__m128)t1, (__m128)denormal);

			// Denormal case
			t4 = _mm_and_si128(t1, exponent);
			t5 = _mm_add_epi32(t4, adjust);
			t6 = _mm_and_si128(t1, mantissa);
			t7 = _mm_or_si128(t6, one_dot);
			t8 = (__m128i)_mm_cvtepi32_ps(t7);
			t9 = (__m128i)_mm_mul_ps((__m128)t8, (__m128)t5);
			t10 = _mm_cvttps_epi32((__m128)t9);

			// Normal case and combine
			t11 = _mm_add_epi32(t1, fixup);
			t12 = _mm_and_si128(t10, t3);
			t13 = _mm_andnot_si128(t3, t11);
			t14 = _mm_or_si128(t12, t13);

			// Correct rounding
			t15 = _mm_srli_epi32(t14, 13);
			t16 = _mm_and_si128(t15, round1);
			t17 = _mm_add_epi32(t16, round2);
			t18 = _mm_add_epi32(t17, t14);

			// Combine with sign and infinity
			t19 = _mm_srli_epi32(t18, 13);
			t20 = _mm_and_si128(t0, sign);
			t21 = _mm_srli_epi32(t20, 16);
			t22 = _mm_or_si128(t19, t2);
			t23 = _mm_and_si128(t22, base);
			t24 = _mm_or_si128(t23, t21);

			t24 = _mm_shufflelo_epi16(t24, _MM_SHUFFLE(3, 1, 2, 0));
			t24 = _mm_shufflehi_epi16(t24, _MM_SHUFFLE(3, 1, 2, 0));
			t25 = _mm_shuffle_epi32(t24, _MM_SHUFFLE(2, 0, 2, 0));

			_mm_storel_pd((double*)(dest + i * stride),
				(__m128d)t25);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_half_2(const float* source, const Uint32 count,
		const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128i t0, t1, t2, t3, t4, t5, t6, t7, t8, t9;
		__m128i t10, t11, t12, t13, t14, t15, t16, t17, t18;
		__m128i t19, t20, t21, t22, t23, t24, t25, abs, infinity;
		__m128i denormal, mantissa, one_dot, exponent, fixup;
		__m128i round1, round2, sign, base, adjust;
		Uint32 i;

		abs = _mm_set1_epi32(0x7FFFFFFF);
		infinity = _mm_set1_epi32(0x47FFEFFF);
		denormal = _mm_set1_epi32(0x38800000);
		mantissa = _mm_set1_epi32(0x007FFFFF);
		one_dot = _mm_set1_epi32(0x00800000);
		exponent = _mm_set1_epi32(0x7F800000);
		fixup = _mm_set1_epi32(0x48000000);
		round1 = _mm_set1_epi32(0x00000001);
		round2 = _mm_set1_epi32(0x00000FFF);
		sign = _mm_set1_epi32(0x80000000);
		base = _mm_set1_epi32(0x00007FFF);
		adjust = _mm_set1_epi32(0x07000000);

		for (i = 0; i < (count / 2); ++i)
		{
			t0 = (__m128i)_mm_load_sd((double*)(source + i * 8 + 0));
			t0 = (__m128i)_mm_loadh_pd((__m128d)t0, (double*)(source + i * 8 + 4));

			// Compute masks
			t1 = _mm_and_si128(t0, abs);
			t2 = (__m128i)_mm_cmpnlt_ps((__m128)t1, (__m128)infinity);
			t3 = (__m128i)_mm_cmplt_ps((__m128)t1, (__m128)denormal);

			// Denormal case
			t4 = _mm_and_si128(t1, exponent);
			t5 = _mm_add_epi32(t4, adjust);
			t6 = _mm_and_si128(t1, mantissa);
			t7 = _mm_or_si128(t6, one_dot);
			t8 = (__m128i)_mm_cvtepi32_ps(t7);
			t9 = (__m128i)_mm_mul_ps((__m128)t8, (__m128)t5);
			t10 = _mm_cvttps_epi32((__m128)t9);

			// Normal case and combine
			t11 = _mm_add_epi32(t1, fixup);
			t12 = _mm_and_si128(t10, t3);
			t13 = _mm_andnot_si128(t3, t11);
			t14 = _mm_or_si128(t12, t13);

			// Correct rounding
			t15 = _mm_srli_epi32(t14, 13);
			t16 = _mm_and_si128(t15, round1);
			t17 = _mm_add_epi32(t16, round2);
			t18 = _mm_add_epi32(t17, t14);

			// Combine with sign and infinity
			t19 = _mm_srli_epi32(t18, 13);
			t20 = _mm_and_si128(t0, sign);
			t21 = _mm_srli_epi32(t20, 16);
			t22 = _mm_or_si128(t19, t2);
			t23 = _mm_and_si128(t22, base);
			t24 = _mm_or_si128(t23, t21);

			t24 = _mm_shufflelo_epi16(t24, _MM_SHUFFLE(3, 1, 2, 0));
			t24 = _mm_shufflehi_epi16(t24, _MM_SHUFFLE(3, 1, 2, 0));
			t25 = _mm_shuffle_epi32(t24, _MM_SHUFFLE(2, 2, 2, 2));

			_mm_store_ss((float*)(dest + (i * 2 + 0) * stride),
				(__m128)t24);
			_mm_store_ss((float*)(dest + (i * 2 + 1) * stride),
				(__m128)t25);
		}

		if ((count % 2) == 0)
		{
			return;
		}

		t0 = (__m128i)_mm_load_sd((double*)(source + i * 8 + 0));

		// Compute masks
		t1 = _mm_and_si128(t0, abs);
		t2 = (__m128i)_mm_cmpnlt_ps((__m128)t1, (__m128)infinity);
		t3 = (__m128i)_mm_cmplt_ps((__m128)t1, (__m128)denormal);

		// Denormal case
		t4 = _mm_and_si128(t1, exponent);
		t5 = _mm_add_epi32(t4, adjust);
		t6 = _mm_and_si128(t1, mantissa);
		t7 = _mm_or_si128(t6, one_dot);
		t8 = (__m128i)_mm_cvtepi32_ps(t7);
		t9 = (__m128i)_mm_mul_ps((__m128)t8, (__m128)t5);
		t10 = _mm_cvttps_epi32((__m128)t9);

		// Normal case and combine
		t11 = _mm_add_epi32(t1, fixup);
		t12 = _mm_and_si128(t10, t3);
		t13 = _mm_andnot_si128(t3, t11);
		t14 = _mm_or_si128(t12, t13);

		// Correct rounding
		t15 = _mm_srli_epi32(t14, 13);
		t16 = _mm_and_si128(t15, round1);
		t17 = _mm_add_epi32(t16, round2);
		t18 = _mm_add_epi32(t17, t14);

		// Combine with sign and infinity
		t19 = _mm_srli_epi32(t18, 13);
		t20 = _mm_and_si128(t0, sign);
		t21 = _mm_srli_epi32(t20, 16);
		t22 = _mm_or_si128(t19, t2);
		t23 = _mm_and_si128(t22, base);
		t24 = _mm_or_si128(t23, t21);

		t24 = _mm_shufflelo_epi16(t24, _MM_SHUFFLE(3, 1, 2, 0));

		_mm_store_ss((float*)(dest + (i * 2 + 0) * stride),
			(__m128)t24);
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_unsigned_short_4(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1;
		__m128i t2, t3, t4;
		Uint32 i;

		for (i = 0; i < (count / 2); ++i)
		{
			t0 = _mm_load_ps(source + i * 8 + 0);
			t1 = _mm_load_ps(source + i * 8 + 4);
			t2 = _mm_cvttps_epi32(t0);
			t3 = _mm_cvttps_epi32(t1);
			t2 = _mm_shufflelo_epi16(t2, _MM_SHUFFLE(3, 1, 2, 0));
			t2 = _mm_shufflehi_epi16(t2, _MM_SHUFFLE(3, 1, 2, 0));
			t3 = _mm_shufflelo_epi16(t3, _MM_SHUFFLE(3, 1, 2, 0));
			t3 = _mm_shufflehi_epi16(t3, _MM_SHUFFLE(3, 1, 2, 0));
			t4 = (__m128i)_mm_shuffle_ps((__m128)t2, (__m128)t3,
				_MM_SHUFFLE(2, 0, 2, 0));

			_mm_storel_pd((double*)(dest + (i * 2 + 0) * stride),
				(__m128d)t4);
			_mm_storeh_pd((double*)(dest + (i * 2 + 1) * stride),
				(__m128d)t4);
		}

		if ((count % 2) == 0)
		{
			return;
		}

		t0 = _mm_load_ps(source + i * 8 + 0);
		t2 = _mm_cvttps_epi32(t0);
		t2 = _mm_shufflelo_epi16(t2, _MM_SHUFFLE(3, 1, 2, 0));
		t2 = _mm_shufflehi_epi16(t2, _MM_SHUFFLE(3, 1, 2, 0));
		t4 = (__m128i)_mm_shuffle_ps((__m128)t2, (__m128)t2,
			_MM_SHUFFLE(2, 0, 2, 0));

		_mm_storel_pd((double*)(dest + (i * 2 + 0) * stride),
			(__m128d)t4);
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_unsigned_short_2(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1;
		__m128i t2, t3, t4, t5;
		Uint32 i, j, remains;

		for (i = 0; i < (count / 4); ++i)
		{
			t0 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + 0));
			t0 = (__m128)_mm_loadh_pd(
				(__m128d)t0, (double*)(source + i * 16 + 4));
			t1 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + 8));
			t1 = (__m128)_mm_loadh_pd(
				(__m128d)t1, (double*)(source + i * 16 + 12));

			t2 = _mm_cvttps_epi32(t0);
			t3 = _mm_cvttps_epi32(t1);
			t2 = _mm_shufflelo_epi16(t2, _MM_SHUFFLE(3, 1, 2, 0));
			t2 = _mm_shufflehi_epi16(t2, _MM_SHUFFLE(3, 1, 2, 0));
			t3 = _mm_shufflelo_epi16(t3, _MM_SHUFFLE(3, 1, 2, 0));
			t3 = _mm_shufflehi_epi16(t3, _MM_SHUFFLE(3, 1, 2, 0));

			t4 = _mm_shuffle_epi32(t2, _MM_SHUFFLE(2, 2, 2, 2));
			t5 = _mm_shuffle_epi32(t3, _MM_SHUFFLE(2, 2, 2, 2));

			_mm_store_ss((float*)(dest + (i * 4 + 0) * stride),
				(__m128)t2);
			_mm_store_ss((float*)(dest + (i * 4 + 1) * stride),
				(__m128)t4);
			_mm_store_ss((float*)(dest + (i * 4 + 2) * stride),
				(__m128)t3);
			_mm_store_ss((float*)(dest + (i * 4 + 3) * stride),
				(__m128)t5);
		}

		remains = count - i * 4;

		for (j = 0; j < remains; ++j)
		{
			t0 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + j * 4));

			t2 = _mm_cvttps_epi32(t0);

			t2 = _mm_shufflelo_epi16(t2, _MM_SHUFFLE(3, 1, 2, 0));

			_mm_store_ss((float*)(dest + (i * 4 + j) * stride),
				(__m128)t2);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_unsigned_normalized_short_4(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1, t2, t3, scale;
		__m128i t4, t5, t6;
		Uint32 i;

		scale = _mm_set1_ps(65535.0f);

		for (i = 0; i < (count / 2); ++i)
		{
			t0 = _mm_load_ps(source + i * 8 + 0);
			t1 = _mm_load_ps(source + i * 8 + 4);

			t2 = _mm_mul_ps(t0, scale);
			t3 = _mm_mul_ps(t1, scale);
			t4 = _mm_cvttps_epi32(t2);
			t5 = _mm_cvttps_epi32(t3);
			t4 = _mm_shufflelo_epi16(t4, _MM_SHUFFLE(3, 1, 2, 0));
			t4 = _mm_shufflehi_epi16(t4, _MM_SHUFFLE(3, 1, 2, 0));
			t5 = _mm_shufflelo_epi16(t5, _MM_SHUFFLE(3, 1, 2, 0));
			t5 = _mm_shufflehi_epi16(t5, _MM_SHUFFLE(3, 1, 2, 0));
			t6 = (__m128i)_mm_shuffle_ps((__m128)t4, (__m128)t5,
				_MM_SHUFFLE(2, 0, 2, 0));

			_mm_storel_pd((double*)(dest + (i * 2 + 0) * stride),
				(__m128d)t6);
			_mm_storeh_pd((double*)(dest + (i * 2 + 1) * stride),
				(__m128d)t6);
		}

		if ((count % 2) == 0)
		{
			return;
		}

		t0 = _mm_load_ps(source + i * 8 + 0);
		t2 = _mm_mul_ps(t0, scale);
		t4 = _mm_cvttps_epi32(t2);
		t4 = _mm_shufflelo_epi16(t4, _MM_SHUFFLE(3, 1, 2, 0));
		t4 = _mm_shufflehi_epi16(t4, _MM_SHUFFLE(3, 1, 2, 0));
		t6 = (__m128i)_mm_shuffle_ps((__m128)t4, (__m128)t4,
			_MM_SHUFFLE(2, 0, 2, 0));

		_mm_storel_pd((double*)(dest + (i * 2 + 0) * stride),
			(__m128d)t6);
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_unsigned_normalized_short_2(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1, t2, t3, scale;
		__m128i t4, t5, t6, t7;
		Uint32 i, j, remains;

		scale = _mm_set1_ps(65535.0f);

		for (i = 0; i < (count / 4); ++i)
		{
			t0 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + 0));
			t0 = (__m128)_mm_loadh_pd(
				(__m128d)t0, (double*)(source + i * 16 + 4));
			t1 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + 8));
			t1 = (__m128)_mm_loadh_pd((__m128d)t1,
				(double*)(source + i * 16 + 12));

			t2 = _mm_mul_ps(t0, scale);
			t3 = _mm_mul_ps(t1, scale);
			t4 = _mm_cvttps_epi32(t2);
			t5 = _mm_cvttps_epi32(t3);
			t4 = _mm_shufflelo_epi16(t4, _MM_SHUFFLE(3, 1, 2, 0));
			t4 = _mm_shufflehi_epi16(t4, _MM_SHUFFLE(3, 1, 2, 0));
			t5 = _mm_shufflelo_epi16(t5, _MM_SHUFFLE(3, 1, 2, 0));
			t5 = _mm_shufflehi_epi16(t5, _MM_SHUFFLE(3, 1, 2, 0));

			t6 = _mm_shuffle_epi32(t4, _MM_SHUFFLE(2, 2, 2, 2));
			t7 = _mm_shuffle_epi32(t5, _MM_SHUFFLE(2, 2, 2, 2));

			_mm_store_ss((float*)(dest + (i * 4 + 0) * stride),
				(__m128)t4);
			_mm_store_ss((float*)(dest + (i * 4 + 1) * stride),
				(__m128)t6);
			_mm_store_ss((float*)(dest + (i * 4 + 2) * stride),
				(__m128)t5);
			_mm_store_ss((float*)(dest + (i * 4 + 3) * stride),
				(__m128)t7);
		}

		remains = count - i * 4;

		for (j = 0; j < remains; ++j)
		{
			t0 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + j * 4));

			t2 = _mm_mul_ps(t0, scale);
			t4 = _mm_cvttps_epi32(t2);
			t4 = _mm_shufflelo_epi16(t4, _MM_SHUFFLE(3, 1, 2, 0));

			_mm_store_ss((float*)(dest + (i * 4 + j) * stride),
				(__m128)t4);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_signed_short_4(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1;
		__m128i t2, t3, t4;
		Uint32 i;

		for (i = 0; i < (count / 2); ++i)
		{
			t0 = _mm_load_ps(source + i * 8 + 0);
			t1 = _mm_load_ps(source + i * 8 + 4);

			t2 = _mm_cvttps_epi32(t0);
			t3 = _mm_cvttps_epi32(t1);
			t4 = _mm_packs_epi32(t2, t3);

			_mm_storel_pd((double*)(dest + (i * 2 + 0) * stride),
				(__m128d)t4);
			_mm_storeh_pd((double*)(dest + (i * 2 + 1) * stride),
				(__m128d)t4);
		}

		if ((count % 2) == 0)
		{
			return;
		}

		t0 = _mm_load_ps(source + i * 8 + 0);

		t2 = _mm_cvttps_epi32(t0);
		t4 = _mm_packs_epi32(t2, t2);

		_mm_storel_pd((double*)(dest + (i * 2 + 0) * stride),
			(__m128d)t4);
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_signed_short_2(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1;
		__m128i t2, t3, t4, t5, t6, t7;
		Uint32 i, j, remains;

		for (i = 0; i < (count / 4); ++i)
		{
			t0 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + 0));
			t0 = (__m128)_mm_loadh_pd((__m128d)t0,
				(double*)(source + i * 16 + 4));
			t1 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + 8));
			t1 = (__m128)_mm_loadh_pd((__m128d)t1,
				(double*)(source + i * 16 + 12));

			t2 = _mm_cvttps_epi32(t0);
			t3 = _mm_cvttps_epi32(t1);
			t4 = _mm_packs_epi32(t2, t3);

			t5 = _mm_shuffle_epi32(t4, _MM_SHUFFLE(1, 1, 1, 1));
			t6 = _mm_shuffle_epi32(t4, _MM_SHUFFLE(2, 2, 2, 2));
			t7 = _mm_shuffle_epi32(t4, _MM_SHUFFLE(3, 3, 3, 3));

			_mm_store_ss((float*)(dest + (i * 4 + 0) * stride),
				(__m128)t4);
			_mm_store_ss((float*)(dest + (i * 4 + 1) * stride),
				(__m128)t5);
			_mm_store_ss((float*)(dest + (i * 4 + 2) * stride),
				(__m128)t6);
			_mm_store_ss((float*)(dest + (i * 4 + 3) * stride),
				(__m128)t7);
		}

		remains = count - i * 4;

		for (j = 0; j < remains; ++j)
		{
			t0 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + j * 4));

			t2 = _mm_cvttps_epi32(t0);

			t4 = _mm_packs_epi32(t2, t2);

			_mm_store_ss((float*)(dest + (i * 4 + j) * stride),
				(__m128)t4);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_signed_normalized_short_4(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1, t2, t3, scale;
		__m128i t4, t5, t6;
		Uint32 i;

		scale = _mm_set1_ps(32767.0f);

		for (i = 0; i < (count / 2); ++i)
		{
			t0 = _mm_load_ps(source + i * 8 + 0);
			t1 = _mm_load_ps(source + i * 8 + 4);

			t2 = _mm_mul_ps(t0, scale);
			t3 = _mm_mul_ps(t1, scale);
			t4 = _mm_cvttps_epi32(t2);
			t5 = _mm_cvttps_epi32(t3);
			t6 = _mm_packs_epi32(t4, t5);

			_mm_storel_pd((double*)(dest + (i * 2 + 0) * stride),
				(__m128d)t6);
			_mm_storeh_pd((double*)(dest + (i * 2 + 1) * stride),
				(__m128d)t6);
		}

		if ((count % 2) == 0)
		{
			return;
		}

		t0 = _mm_load_ps(source + i * 8 + 0);
		t2 = _mm_mul_ps(t0, scale);
		t4 = _mm_cvttps_epi32(t2);
		t6 = _mm_packs_epi32(t4, t4);

		_mm_storel_pd((double*)(dest + (i * 2 + 0) * stride),
			(__m128d)t6);
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_signed_normalized_short_2(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1, t2, t3, scale;
		__m128i t4, t5, t6, t7, t8, t9;
		Uint32 i, j, remains;

		scale = _mm_set1_ps(32767.0f);

		for (i = 0; i < (count / 4); ++i)
		{
			t0 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + 0));
			t0 = (__m128)_mm_loadh_pd((__m128d)t0,
				(double*)(source + i * 16 + 4));
			t1 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + 8));
			t1 = (__m128)_mm_loadh_pd((__m128d)t1,
				(double*)(source + i * 16 + 12));

			t2 = _mm_mul_ps(t0, scale);
			t3 = _mm_mul_ps(t1, scale);
			t4 = _mm_cvttps_epi32(t2);
			t5 = _mm_cvttps_epi32(t3);
			t6 = _mm_packs_epi32(t4, t5);

			t7 = _mm_shuffle_epi32(t6, _MM_SHUFFLE(1, 1, 1, 1));
			t8 = _mm_shuffle_epi32(t6, _MM_SHUFFLE(2, 2, 2, 2));
			t9 = _mm_shuffle_epi32(t6, _MM_SHUFFLE(3, 3, 3, 3));

			_mm_store_ss((float*)(dest + (i * 4 + 0) * stride),
				(__m128)t6);
			_mm_store_ss((float*)(dest + (i * 4 + 1) * stride),
				(__m128)t7);
			_mm_store_ss((float*)(dest + (i * 4 + 2) * stride),
				(__m128)t8);
			_mm_store_ss((float*)(dest + (i * 4 + 3) * stride),
				(__m128)t9);
		}

		remains = count - i * 4;

		for (j = 0; j < remains; ++j)
		{
			t0 = (__m128)_mm_load_sd(
				(double*)(source + i * 16 + j * 4));

			t2 = _mm_mul_ps(t0, scale);
			t4 = _mm_cvttps_epi32(t2);
			t6 = _mm_packs_epi32(t4, t4);

			_mm_store_ss((float*)(dest + (i * 4 + j) * stride),
				(__m128)t6);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_unsigned_byte_4(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1, t2, t3;
		__m128i t4, t5, t6, t7, t8, t9, t10, t11, t12, t13;
		Uint32 i, j, remains;

		for (i = 0; i < (count / 4); ++i)
		{
			t0 = _mm_load_ps(source + i * 16 + 0);
			t1 = _mm_load_ps(source + i * 16 + 4);
			t2 = _mm_load_ps(source + i * 16 + 8);
			t3 = _mm_load_ps(source + i * 16 + 12);

			t4 = _mm_cvttps_epi32(t0);
			t5 = _mm_cvttps_epi32(t1);
			t6 = _mm_cvttps_epi32(t2);
			t7 = _mm_cvttps_epi32(t3);
			t8 = _mm_packs_epi32(t4, t5);
			t9 = _mm_packs_epi32(t6, t7);
			t10 = _mm_packus_epi16(t8, t9);

			t11 = _mm_shuffle_epi32(t10, _MM_SHUFFLE(1, 1, 1, 1));
			t12 = _mm_shuffle_epi32(t10, _MM_SHUFFLE(2, 2, 2, 2));
			t13 = _mm_shuffle_epi32(t10, _MM_SHUFFLE(3, 3, 3, 3));

			_mm_store_ss((float*)(dest + (i * 4 + 0) * stride),
				(__m128)t10);
			_mm_store_ss((float*)(dest + (i * 4 + 1) * stride),
				(__m128)t11);
			_mm_store_ss((float*)(dest + (i * 4 + 2) * stride),
				(__m128)t12);
			_mm_store_ss((float*)(dest + (i * 4 + 3) * stride),
				(__m128)t13);
		}

		remains = count - i * 4;

		for (j = 0; j < remains; ++j)
		{
			t0 = _mm_load_ps(source + i * 16 + j * 4);

			t4 = _mm_cvttps_epi32(t0);
			t8 = _mm_packs_epi32(t4, t4);
			t10 = _mm_packus_epi16(t8, t8);

			_mm_store_ss((float*)(dest + (i * 4 + j) * stride),
				(__m128)t10);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::float_to_unsigned_normalized_byte_4(const float* source,
		const Uint32 count, const Uint32 stride, Uint8* dest)
	{
#ifdef	USE_SSE2
		__m128 t0, t1, t2, t3, t4, t5, t6, t7, scale;
		__m128i t8, t9, t10, t11, t12, t13, t14, t15, t16, t17;
		Uint32 i, j, remains;

		scale = _mm_set1_ps(255.0f);

		for (i = 0; i < (count / 4); ++i)
		{
			t0 = _mm_load_ps(source + i * 16 + 0);
			t1 = _mm_load_ps(source + i * 16 + 4);
			t2 = _mm_load_ps(source + i * 16 + 8);
			t3 = _mm_load_ps(source + i * 16 + 12);

			t4 = _mm_mul_ps(t0, scale);
			t5 = _mm_mul_ps(t1, scale);
			t6 = _mm_mul_ps(t2, scale);
			t7 = _mm_mul_ps(t3, scale);
			t8 = _mm_cvttps_epi32(t4);
			t9 = _mm_cvttps_epi32(t5);
			t10 = _mm_cvttps_epi32(t6);
			t11 = _mm_cvttps_epi32(t7);
			t12 = _mm_packs_epi32(t8, t9);
			t13 = _mm_packs_epi32(t10, t11);
			t14 = _mm_packus_epi16(t12, t13);

			t15 = _mm_shuffle_epi32(t14, _MM_SHUFFLE(1, 1, 1, 1));
			t16 = _mm_shuffle_epi32(t14, _MM_SHUFFLE(2, 2, 2, 2));
			t17 = _mm_shuffle_epi32(t14, _MM_SHUFFLE(3, 3, 3, 3));

			_mm_store_ss((float*)(dest + (i * 4 + 0) * stride),
				(__m128)t14);
			_mm_store_ss((float*)(dest + (i * 4 + 1) * stride),
				(__m128)t15);
			_mm_store_ss((float*)(dest + (i * 4 + 2) * stride),
				(__m128)t16);
			_mm_store_ss((float*)(dest + (i * 4 + 3) * stride),
				(__m128)t17);
		}

		remains = count - i * 4;

		for (j = 0; j < remains; ++j)
		{
			t0 = _mm_load_ps(source + i * 16 + j * 4);

			t4 = _mm_mul_ps(t0, scale);
			t8 = _mm_cvttps_epi32(t4);
			t12 = _mm_packs_epi32(t8, t8);
			t14 = _mm_packus_epi16(t12, t12);

			_mm_store_ss((float*)(dest + (i * 4 + j) * stride),
				(__m128)t14);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::fill(const glm::vec4 &data, const Uint32 count, float* dest)
	{
#ifdef	USE_SSE2
		__m128 t0;
		Uint32 i;

		t0 = _mm_loadu_ps(glm::value_ptr(data));

		for (i = 0; i < count; ++i)
		{
			_mm_stream_ps(dest + 4 * i, t0);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::transform(const float* source, const Uint32 count,
		const glm::mat4x3 &matrix, float* dest)
	{
#ifdef	USE_SSE2
		__m128 tmp, m0, m1, m2, m3, x, y, z;
		Uint32 i;

		m0 = _mm_setr_ps(matrix[0].x, matrix[0].y, matrix[0].z, 0.0f);
		m1 = _mm_setr_ps(matrix[1].x, matrix[1].y, matrix[1].z, 0.0f);
		m2 = _mm_setr_ps(matrix[2].x, matrix[2].y, matrix[2].z, 0.0f);
		m3 = _mm_setr_ps(matrix[3].x, matrix[3].y, matrix[3].z, 1.0f);

		for (i = 0; i < count; ++i)
		{
			tmp = _mm_load_ps(source + 4 * i);

			x = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(0, 0, 0, 0));
			y = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(1, 1, 1, 1));
			z = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(2, 2, 2, 2));

			x = _mm_mul_ps(x, m0);
			y = _mm_mul_ps(y, m1);
			z = _mm_mul_ps(z, m2);

			tmp = _mm_add_ps(m3, x);
			tmp = _mm_add_ps(tmp, y);
			tmp = _mm_add_ps(tmp, z);

			_mm_stream_ps(dest + 4 * i, tmp);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::transform(const float* source, const Uint32 count,
		const glm::mat3x3 &matrix, float* dest)
	{
#ifdef	USE_SSE2
		__m128 tmp, m0, m1, m2, x, y, z, w, mask, normal;
		Uint32 i;

		m0 = _mm_setr_ps(matrix[0].x, matrix[0].y, matrix[0].z, 0.0f);
		m1 = _mm_setr_ps(matrix[1].x, matrix[1].y, matrix[1].z, 0.0f);
		m2 = _mm_setr_ps(matrix[2].x, matrix[2].y, matrix[2].z, 0.0f);
		mask = (__m128)_mm_setr_epi32(0x00000000, 0x00000000,
			0x00000000, 0xFFFFFFFF);

		for (i = 0; i < count; ++i)
		{
			tmp = _mm_load_ps(source + 4 * i);

			x = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(0, 0, 0, 0));
			y = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(1, 1, 1, 1));
			z = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(2, 2, 2, 2));
			w = _mm_and_ps(tmp, mask);

			x = _mm_mul_ps(x, m0);
			y = _mm_mul_ps(y, m1);
			z = _mm_mul_ps(z, m2);

			normal = _mm_add_ps(x, y);
			normal = _mm_add_ps(normal, z);

			tmp = _mm_mul_ps(normal, normal);

			x = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(0, 0, 0, 0));
			y = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(1, 1, 1, 1));
			z = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(2, 2, 2, 2));

			tmp = _mm_add_ss(x, y);
			tmp = _mm_add_ss(tmp, z);
			tmp = _mm_rsqrt_ss(tmp);

			tmp = (__m128)_mm_shuffle_epi32((__m128i)tmp,
				_MM_SHUFFLE(0, 0, 0, 0));

			normal = _mm_mul_ps(normal, tmp);

			normal = _mm_or_ps(normal, w);

			_mm_stream_ps(dest + 4 * i, normal);
		}
#endif	/* USE_SSE2 */
	}

	void SIMD::transform(const float* source, const Uint32 count,
		const glm::vec4 &scale_offset, float* dest)
	{
#ifdef	USE_SSE2
		__m128 tmp, offset, scale;
		Uint32 i;

		scale = _mm_setr_ps(scale_offset.x, scale_offset.y, 1.0f,
			1.0f);
		offset = _mm_setr_ps(scale_offset.z, scale_offset.w, 0.0f,
			0.0f);

		for (i = 0; i < count; ++i)
		{
			tmp = _mm_load_ps(source + 4 * i);

			tmp = _mm_mul_ps(tmp, scale);
			tmp = _mm_add_ps(tmp, offset);

			_mm_stream_ps(dest + 4 * i, tmp);
		}
#endif	/* USE_SSE2 */
	}

}
