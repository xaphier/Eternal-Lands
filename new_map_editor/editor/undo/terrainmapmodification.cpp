/****************************************************************************
 *            terrainmapmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainmapmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	TerrainMapModification::TerrainMapModification(const String &map,
		const Uint16 index, const Uint16 id,
		const ModificationType type): m_map(map), m_index(index),
		m_id(id), m_type(type)
	{
	}

	TerrainMapModification::~TerrainMapModification() throw()
	{
	}

	ModificationType TerrainMapModification::get_type() const
	{
		return m_type;
	}

	bool TerrainMapModification::merge(Modification* modification)
	{
		TerrainMapModification* terrain_map_modification;

		if (get_type() == modification->get_type())
		{
			terrain_map_modification =
				dynamic_cast<TerrainMapModification*>(
					modification);

			assert(terrain_map_modification != 0);

			return (m_index == terrain_map_modification->m_index)
				&& (m_id == terrain_map_modification->m_id);
		}
		else
		{
			return false;
		}
	}

	bool TerrainMapModification::undo(EditorMapData &editor)
	{
		switch (get_type())
		{
			case mt_light_added:
			case mt_light_removed:
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
			case mt_object_added:
			case mt_object_removed:
			case mt_object_translation_changed:
			case mt_object_rotation_changed:
			case mt_object_scale_changed:
			case mt_object_blend_changed:
			case mt_object_selection_changed:
			case mt_object_materials_changed:
				break;
			case mt_terrain_albedo_map_changed:
				editor.set_terrain_albedo_map(m_map, m_index,
					m_id);
			case mt_terrain_blend_map_changed:
				editor.set_terrain_blend_map(m_map, m_id);
				break;
			case mt_terrain_height_map_changed:
				editor.set_terrain_height_map(m_map, m_id);
				break;
			case mt_terrain_dudv_map_changed:
				editor.set_terrain_dudv_map(m_map, m_id);
				break;
			case mt_terrain_scale_offset_changed:
			case mt_tile_texture_changed:
			case mt_scene_ambient_changed:
			case mt_terrain_height_changed:
			case mt_height_changed:
			case mt_blend_values_changed:
				return false;
		}

		return false;
	}

}
