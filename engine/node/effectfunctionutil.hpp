/****************************************************************************
 *            effectfunctionutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_42bf6e54_e167_4745_9e03_df97db5ddc4b
#define	UUID_42bf6e54_e167_4745_9e03_df97db5ddc4b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectFunctionUtil.
 * This file contains the @c class EffectFunctionUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the effect functions.
	 * @{
	 */
	enum EffectFunctionType
	{
		eft_add = 0,
		eft_sub,
		eft_mul,
		eft_div,
		eft_mad,
		eft_radians,
		eft_degrees,
		eft_sin,
		eft_cos,
		eft_tan,
		eft_asin,
		eft_acos,
		eft_atan,
		eft_atan2,
		eft_sinh,
		eft_cosh,
		eft_tanh,
		eft_asinh,
		eft_acosh,
		eft_atanh,
		eft_pow,
		eft_exp,
		eft_log,
		eft_exp2,
		eft_log2,
		eft_sqrt,
		eft_inversesqrt,
		eft_abs,
		eft_sign,
		eft_round,
		eft_roundEven,
		eft_trunc,
		eft_floor,
		eft_ceil,
		eft_fract,
		eft_mod,
		eft_min,
		eft_max,
		eft_clamp,
		eft_mix,
		eft_step,
		eft_smoothstep,
		eft_reflect,
		eft_length,
		eft_distance,
		eft_dot,
		eft_normalize,
		eft_saturate,
		eft_refract,
		eft_derivatives,
		eft_fwidth
	};
	/**
	 * @}
	 */

	class EffectFunctionUtil
	{
		public:
			static const String &get_str(
				const EffectFunctionType effect_function);
			static const String &get_description(
				const EffectFunctionType effect_function);
			static EffectFunctionType get_effect_function(
				const String &str);
			static bool get_effect_function(const String &str,
				EffectFunctionType &effect_function) noexcept;
			static bool get_geometric(
				const EffectFunctionType effect_function);
			static bool get_trigonometric(
				const EffectFunctionType effect_function);
			static Uint32 get_effect_function_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const EffectFunctionType value);
	InStream& operator>>(InStream &str, EffectFunctionType &value);

}

#endif	/* UUID_42bf6e54_e167_4745_9e03_df97db5ddc4b */
