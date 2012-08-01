/****************************************************************************
 *            dungeonmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_21e5b8a5_a2f2_44e8_8045_e3084c10935e
#define	UUID_21e5b8a5_a2f2_44e8_8045_e3084c10935e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"

namespace eternal_lands
{

	class DungeonModification: public Modification
	{
		private:
			bool m_dungeon;

			virtual bool do_merge(Modification* modification);

		public:
			DungeonModification(const bool dungeon,
				const Uint32 edit_id);
			virtual ~DungeonModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_21e5b8a5_a2f2_44e8_8045_e3084c10935e */
