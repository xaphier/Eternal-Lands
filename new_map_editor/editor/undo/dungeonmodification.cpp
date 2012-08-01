/****************************************************************************
 *            dungeonmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "dungeonmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	DungeonModification::DungeonModification(const bool dungeon,
		const Uint32 edit_id):
		Modification(edit_id, 0, mt_dungeon_changed),
		m_dungeon(dungeon)
	{
	}

	DungeonModification::~DungeonModification() throw()
	{
	}

	bool DungeonModification::do_merge(Modification* modification)
	{
		return true;
	}

	bool DungeonModification::undo(EditorMapData &editor)
	{
		editor.set_dungeon(m_dungeon);

		return false;
	}

}
