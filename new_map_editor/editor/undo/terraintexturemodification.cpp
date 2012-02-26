/****************************************************************************
 *            terraintexturemodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terraintexturemodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	TerrainTextureModification::TerrainTextureModification(
		const String &texture, const Uint16 index): m_texture(texture),
		m_index(index)
	{
	}

	TerrainTextureModification::~TerrainTextureModification()
	{
	}

	ModificationType TerrainTextureModification::get_type() const
	{
		return mt_terrain_albedo_map_changed;
	}

	bool TerrainTextureModification::merge(Modification* modification)
	{
		TerrainTextureModification* terrain_texture_modification;

		if (get_type() == modification->get_type())
		{
			terrain_texture_modification =
				dynamic_cast<TerrainTextureModification*>(
					modification);

			assert(terrain_texture_modification != 0);

			return m_index == terrain_texture_modification->m_index;
		}
		else
		{
			return false;
		}
	}

	bool TerrainTextureModification::undo(EditorMapData &editor)
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
			case mt_object_blending_changed:
			case mt_object_selection_changed:
			case mt_object_materials_changed:
				break;
			case mt_terrain_albedo_map_changed:
				editor.set_terrain_albedo_map(m_texture,
					m_index);
			case mt_terrain_blend_map_changed:
				editor.set_terrain_blend_map(m_texture);
				break;
			case mt_terrain_height_map_changed:
				editor.set_terrain_height_map(m_texture);
				break;
			case mt_terrain_dvdu_map_changed:
				editor.set_terrain_dvdu_map(m_texture);
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
