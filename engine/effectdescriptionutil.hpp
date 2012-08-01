/****************************************************************************
 *            effectdescriptionutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_cf75c25d_852c_4a92_8d15_7e95a0bc232a
#define	UUID_cf75c25d_852c_4a92_8d15_7e95a0bc232a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	/**
	 * Types of effect descriptions.
	 * @{
	 */
	enum EffectDescriptionType
	{
		edt_default = 0,
		edt_screen_quad_1,
		edt_screen_quad_2,
		edt_screen_quad_3,
		edt_screen_quad_4
	};

	class EffectDescriptionUtil
	{
		public:
			static const String &get_str(
				const EffectDescriptionType effect_description);
			static EffectDescriptionType get_effect_description(
				const String &str);
			static Uint16 get_render_targets_count(
				const EffectDescriptionType effect_description);
			static Uint32 get_effect_description_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const EffectDescriptionType value);
	InStream& operator>>(InStream &str, EffectDescriptionType &value);

}

#endif	/* UUID_cf75c25d_852c_4a92_8d15_7e95a0bc232a */
