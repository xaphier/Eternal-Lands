/****************************************************************************
 *            effectprogramutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a7b7d08c_d0b4_4ad6_8f80_566440f96106
#define	UUID_a7b7d08c_d0b4_4ad6_8f80_566440f96106

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	/**
	 * Types of effect programs.
	 * @{
	 */
	enum EffectProgramType
	{
		ept_default = 0,
		ept_shadow,
		ept_depth
	};


	class EffectProgramUtil
	{
		public:
			static const String &get_str(
				const EffectProgramType effect_program);
			static EffectProgramType get_effect_program(
				const String &str);
			static Uint32 get_effect_program_count() noexcept;

	};

	OutStream& operator<<(OutStream &str, const EffectProgramType value);
	InStream& operator>>(InStream &str, EffectProgramType &value);

}

#endif	/* UUID_a7b7d08c_d0b4_4ad6_8f80_566440f96106 */
