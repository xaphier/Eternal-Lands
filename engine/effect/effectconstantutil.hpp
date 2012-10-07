/****************************************************************************
 *            effectconstantutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_d7d2dfcc_9d06_43ee_b658_f89ad358e2ed
#define	UUID_d7d2dfcc_9d06_43ee_b658_f89ad358e2ed

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectConstantUtil.
 * This file contains the @c class EffectConstantUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the effect constants.
	 * @{
	 */
	enum EffectConstantType
	{
		ect_color_rgb,
		ect_direction_xy,
		ect_float,
		ect_vec2,
		ect_vec3,
		ect_vec4
	};
	/**
	 * @}
	 */

	class EffectConstantUtil
	{
		public:
			static const String &get_str(
				const EffectConstantType effect_constant);
			static Uint16 get_count(
				const EffectConstantType effect_constant);
			static EffectConstantType get_effect_constant(
				const String &str);
			static bool get_effect_constant(const String &str,
				EffectConstantType &effect_constant) noexcept;
			static Uint32 get_effect_constant_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const EffectConstantType value);
	InStream &operator>>(InStream &str, EffectConstantType &value);

}

#endif	/* UUID_d7d2dfcc_9d06_43ee_b658_f89ad358e2ed */
