/****************************************************************************
 *            effectnodeutil.hpp
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
 * @brief The @c class EffectNodeUtil.
 * This file contains the @c class EffectNodeUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the effect nodes.
	 * @{
	 */
	enum EffectNodeType
	{
		ent_add = 0,
		ent_sub,
		ent_mul,
		ent_div,
		ent_mad,
		ent_swizzle,
		ent_merge,
		ent_split,
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
		ent_color,
		ent_constant,
		ent_paramter,
		ent_texture,
		ent_output,
		ent_inout
	};
	/**
	 * @}
	 */

	class EffectNodeUtil
	{
		public:
			static const String &get_str(
				const EffectNodeType effect_node);
			static EffectNodeType get_effect_node(
				const String &str);
			static bool get_effect_node(const String &str,
				EffectNodeType &effect_node) noexcept;
			static Uint32 get_effect_node_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const EffectNodeType value);
	InStream& operator>>(InStream &str, EffectNodeType &value);

}

#endif	/* UUID_42bf6e54_e167_4745_9e03_df97db5ddc4b */
