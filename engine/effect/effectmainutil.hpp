/****************************************************************************
 *            effectmainutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b1e2ca87_3fe1_42c2_b9c2_84b49efc9424
#define	UUID_b1e2ca87_3fe1_42c2_b9c2_84b49efc9424

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	/**
	 * Types of effect main.
	 * @{
	 */
	enum EffectMainType
	{
		emt_source = 0,
		emt_nodes,
		emt_terrain
	};

	class EffectMainUtil
	{
		public:
			static const String &get_str(
				const EffectMainType effect_main);
			static EffectMainType get_effect_main(
				const String &str);
			static Uint32 get_effect_main_count() noexcept;

	};

	OutStream &operator<<(OutStream &str, const EffectMainType value);
	InStream &operator>>(InStream &str, EffectMainType &value);

}

#endif	/* UUID_b1e2ca87_3fe1_42c2_b9c2_84b49efc9424 */
