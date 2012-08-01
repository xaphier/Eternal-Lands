/****************************************************************************
 *            groundtilemodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "groundtilemodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	GroundTileModification::GroundTileModification(
		const glm::uvec2 &offset, const Uint16 material,
		const Uint32 edit_id): Modification(edit_id,
			offset.x + (offset.y << 16), mt_tile_texture_changed),
		m_offset(offset), m_material(material)
	{
	}

	GroundTileModification::~GroundTileModification() throw()
	{
	}

	bool GroundTileModification::do_merge(Modification* modification)
	{
		return true;
	}

	bool GroundTileModification::undo(EditorMapData &editor)
	{
		editor.set_tile(m_offset.x, m_offset.y, m_material);

		return false;
	}

}
