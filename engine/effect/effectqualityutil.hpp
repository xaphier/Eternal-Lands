/****************************************************************************
 *            effectqualityutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_295db973_284e_47c7_8913_8e864ce4adb8
#define	UUID_295db973_284e_47c7_8913_8e864ce4adb8

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class EffectQualityUtil.
 * This file contains the @c class EffectQualityUtil.
 */
namespace eternal_lands
{

	/**
	 * Basic types of the effect qualities.
	 * @{
	 */
	enum EffectQualityType
	{
		eqt_low = 0,
		eqt_medium,
		eqt_high
	};
	/**
	 * @}
	 */

	class EffectQualityUtil
	{
		public:
			static const String &get_str(
				const EffectQualityType effect_quality);
			static EffectQualityType get_effect_quality(
				const String &str);
			static bool get_effect_quality(const String &str,
				EffectQualityType &effect_quality) noexcept;
			static Uint32 get_effect_quality_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const EffectQualityType value);
	InStream &operator>>(InStream &str, EffectQualityType &value);

}

#endif	/* UUID_295db973_284e_47c7_8913_8e864ce4adb8 */
