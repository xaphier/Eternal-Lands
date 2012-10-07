/****************************************************************************
 *            effectparameterutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_66d70394_4ad8_4cac_9524_7ed61ab4d747
#define	UUID_66d70394_4ad8_4cac_9524_7ed61ab4d747

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectParameterUtil.
 * This file contains the @c class EffectParameterUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the effect Parameters.
	 * @{
	 */
	enum EffectParameterType
	{
		ept_position = 0,
		ept_normal,
		ept_tangent,
		ept_view_direction,
		ept_uv,
		ept_fragment_coordinate,
		ept_time,
		ept_camera
	};
	/**
	 * @}
	 */

	class EffectParameterUtil
	{
		public:
			static const String &get_str(
				const EffectParameterType effect_parameter);
			static EffectParameterType get_effect_parameter(
				const String &str);
			static bool get_effect_parameter(const String &str,
				EffectParameterType &effect_parameter) noexcept;
			static Uint32 get_effect_parameter_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const EffectParameterType value);
	InStream &operator>>(InStream &str, EffectParameterType &value);

}

#endif	/* UUID_42bf6e54_e167_4745_9e03_df97db5ddc4b */
