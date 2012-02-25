/****************************************************************************
 *            selectionutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_35bae9ed_f963_43f5_a2f8_074813e9c727
#define	UUID_35bae9ed_f963_43f5_a2f8_074813e9c727

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class SelectionUtil.
 * This file contains the @c class SelectionUtil.
 */
namespace eternal_lands
{

	enum SelectionType
	{
		st_none = 0,
		st_select = 1,
		st_harvest = 2,
		st_pick = 3,
		st_enter = 4,
		st_npc = 5,
		st_player = 6,
		st_animal = 7
	};

	/**
	 * @brief @c class for selection util functions.
	 *
	 * @c class for selection util functions.
	 */
	class SelectionUtil
	{
		public:
			static const String &get_str(
				const SelectionType selection);
			static SelectionType get_selection_type(
				const String &str);

	};

	OutStream& operator<<(OutStream &str, const SelectionType value);
	InStream& operator>>(InStream &str, SelectionType &value);

}

#endif	/* UUID_35bae9ed_f963_43f5_a2f8_074813e9c727 */
