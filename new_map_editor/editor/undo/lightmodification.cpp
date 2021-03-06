/****************************************************************************
 *            lightmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "lightmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	LightModification::LightModification(const LightData &data,
		const ModificationType type, const Uint32 edit_id):
		Modification(edit_id, data.get_id(), type), m_data(data)
	{
	}

	LightModification::~LightModification() throw()
	{
	}

	bool LightModification::do_merge(Modification* modification)
	{
		return true;
	}

	bool LightModification::undo(EditorMapData &editor)
	{
		switch (get_type())
		{
			case mt_light_added:
				return editor.remove_light(m_data.get_id());
			case mt_light_removed:
				editor.use_light_id(m_data.get_id());
				editor.add_light(m_data, sct_select);
				return false;
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
				editor.modify_light(m_data);
				return false;
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
			case mt_terrain_blend_layer_moved:
				return false;
		}

		return false;
	}

}
