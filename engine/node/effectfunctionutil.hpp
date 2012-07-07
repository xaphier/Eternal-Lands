/****************************************************************************
 *            effectfunctionutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
		ent_add = 0,
		ent_sub,
		ent_mul,
		ent_div,
		ent_mad,
		ent_radians,
		ent_degrees,
		ent_sin,
		ent_cos,
		ent_tan,
		ent_asin,
		ent_acos,
		ent_atan,
		ent_atan2,
		ent_sinh,
		ent_cosh,
		ent_tanh,
		ent_asinh,
		ent_acosh,
		ent_atanh,
		ent_pow,
		ent_exp,
		ent_log,
		ent_exp2,
		ent_log2,
		ent_sqrt,
		ent_inversesqrt,
		ent_abs,
		ent_sign,
		ent_round,
		ent_trunc,
		ent_floor,
		ent_ceil,
		ent_fract,
		ent_mod,
		ent_min,
		ent_max,
		ent_clamp,
		ent_mix,
		ent_step,
		ent_smoothstep,
		ent_reflect,
		ent_length,
		ent_distance,
		ent_dot,
		ent_normalize,
		ent_saturate,
		ent_refract,
		ent_dFdx,
		ent_dFdy,
		ent_fresnel
	};
	/**
	 * @}
	 */

	class EffectFunctionUtil
	{
		public:
			static const String &get_str(
				const EffectFunctionType effect_function);
			static EffectFunctionType get_effect_function(
				const String &str);
			static bool get_effect_function(const String &str,
				EffectFunctionType &effect_function) noexcept;
			static Uint32 get_effect_function_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const EffectFunctionType value);
	InStream& operator>>(InStream &str, EffectFunctionType &value);

}

#endif	/* UUID_42bf6e54_e167_4745_9e03_df97db5ddc4b */
