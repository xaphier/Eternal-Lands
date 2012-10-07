/****************************************************************************
 *            effectchangeutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ed720325_2bc3_426c_9b69_3534b1fa22b8
#define	UUID_ed720325_2bc3_426c_9b69_3534b1fa22b8

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectChangeUtil.
 * This file contains the @c class EffectChangeUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the effect changes.
	 * @{
	 */
	enum EffectChangeType
	{
		ect_undefined = 0,
		ect_constant,
		ect_vertex,
		ect_fragment
	};
	/**
	 * @}
	 */

	class EffectChangeUtil
	{
		public:
			static const String &get_str(
				const EffectChangeType effect_change);
			static EffectChangeType get_effect_change(
				const String &str);
			static bool get_effect_change(const String &str,
				EffectChangeType &effect_change) noexcept;
			static Uint32 get_effect_change_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const EffectChangeType value);
	InStream &operator>>(InStream &str, EffectChangeType &value);

}

#endif	/* UUID_ed720325_2bc3_426c_9b69_3534b1fa22b8 */
