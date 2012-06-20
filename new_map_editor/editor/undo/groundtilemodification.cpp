/****************************************************************************
 *            groundtilemodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "groundtilemodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	GroundTileModification::GroundTileModification(
		const glm::uvec2 &offset, const Uint16 material,
		const Uint32 edit_id): Modification(edit_id), m_offset(offset),
		m_material(material)
	{
	}

	GroundTileModification::~GroundTileModification() throw()
	{
	}

	ModificationType GroundTileModification::get_type() const
	{
		return mt_tile_texture_changed;
	}

	bool GroundTileModification::do_merge(Modification* modification)
	{
		GroundTileModification* tile_modification;

		tile_modification = dynamic_cast<GroundTileModification*>(
			modification);

		assert(tile_modification != 0);

		return m_offset == tile_modification->m_offset;
	}

	bool GroundTileModification::undo(EditorMapData &editor)
	{
		editor.set_tile(m_offset.x, m_offset.y, m_material);

		return false;
	}

}
