/****************************************************************************
 *            terrainblendlayermovemodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainblendlayermovemodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	TerrainBlendLayerMoveModification::TerrainBlendLayerMoveModification(
		const Uint16 idx0, const Uint16 idx1, const Uint32 edit_id):
		Modification(edit_id, 0, mt_terrain_blend_layer_moved),
		m_idx0(idx0), m_idx1(idx1)
	{
	}

	TerrainBlendLayerMoveModification::~TerrainBlendLayerMoveModification()
		throw()
	{
	}

	bool TerrainBlendLayerMoveModification::do_merge(
		Modification* modification)
	{
		return false;
	}

	bool TerrainBlendLayerMoveModification::undo(EditorMapData &editor)
	{
		switch (get_type())
		{
			case mt_light_added:
			case mt_light_removed:
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
			case mt_lights_position_changed:
			case mt_lights_color_changed:
			case mt_lights_radius_changed:
			case mt_object_added:
			case mt_object_removed:
			case mt_object_translation_changed:
			case mt_object_rotation_changed:
			case mt_object_scale_changed:
			case mt_object_blend_changed:
			case mt_object_transparency_changed:
			case mt_object_glow_changed:
			case mt_object_selection_changed:
			case mt_object_materials_changed:
			case mt_object_name_changed:
			case mt_object_description_changed:
			case mt_object_id_changed:
			case mt_objects_removed:
			case mt_objects_translation_changed:
			case mt_objects_rotation_changed:
			case mt_objects_scale_changed:
			case mt_objects_blend_changed:
			case mt_objects_transparency_changed:
			case mt_objects_glow_changed:
			case mt_objects_selection_changed:
			case mt_objects_materials_changed:
			case mt_objects_name_changed:
			case mt_objects_description_changed:
			case mt_terrain_material_changed:
			case mt_terrain_scale_offset_changed:
			case mt_terrain_translation_changed:
			case mt_scene_ground_hemisphere_changed:
			case mt_dungeon_changed:
			case mt_displacement_value_changed:
			case mt_height_changed:
			case mt_tile_value_changed:
			case mt_tile_layer_height_changed:
			case mt_blend_value_changed:
			case mt_blend_values_changed:
			case mt_terrain_blend_layers_swapped:
				return false;
			case mt_terrain_blend_layer_moved:
				editor.move_terrain_blend_layer(m_idx0,
					m_idx1);
				return false;
		}

		return false;
	}

}