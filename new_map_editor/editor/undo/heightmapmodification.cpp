/****************************************************************************
 *            heightmapmodification.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "heightmapmodification.hpp"
#include "editor.hpp"
#include "scenepagereadonly.hpp"
#include "scenepagereadwrite.hpp"

namespace eternal_lands
{

	HeightMapModification::HeightMapModification(
		const Uint8MultiArray2 &height_map, const Uint16 page_id):
		m_height_map(height_map), m_page_id(page_id)
	{
	}

	HeightMapModification::~HeightMapModification()
	{
	}

	ModificationType HeightMapModification::get_type() const
	{
		return mt_height_map_changed;
	}

	bool HeightMapModification::merge(Modification* modification)
	{
		return false;
	}

	bool HeightMapModification::undo(Editor &editor)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;

		editor.get_scene().get_scene_page_read_write(m_page_id,
			scene_page_read_write);

		scene_page_read_write->set_height_map(m_height_map);

		return false;
	}

}
