/****************************************************************************
 *            groundtilemodification.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "groundtilemodification.hpp"
#include "editor.hpp"
#include "scenepagereadonly.hpp"
#include "scenepagereadwrite.hpp"

namespace eternal_lands
{

	GroundTileModification::GroundTileModification(const Uint16 page_id,
		const Uint32Array2 &offset, const Uint8 material): m_offset(offset),
		m_page_id(page_id), m_material(material)
	{
	}

	GroundTileModification::~GroundTileModification()
	{
	}

	ModificationType GroundTileModification::get_type() const
	{
		return mt_tile_texture_changed;
	}

	bool GroundTileModification::merge(Modification* modification)
	{
		GroundTileModification* tile_modification;

		if (get_type() == modification->get_type())
		{
			tile_modification = dynamic_cast<GroundTileModification*>(modification);

			assert(tile_modification != 0);

			return (m_offset == tile_modification->m_offset) &&
				(m_page_id == tile_modification->m_page_id);
		}
		else
		{
			return false;
		}
	}

	bool GroundTileModification::undo(Editor &editor)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;

		editor.get_scene().get_scene_page_read_write(m_page_id, scene_page_read_write);

		scene_page_read_write->modify_ground_tile(m_offset, m_material);

		return false;
	}

}
