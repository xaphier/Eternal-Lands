/****************************************************************************
 *            objectsmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "objectsmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	ObjectsModification::ObjectsModification(
		const EditorObjectDescriptionVector &object_descriptions,
		const ModificationType type, const Uint32 edit_id):
		Modification(edit_id, 0, type),
		m_object_descriptions(object_descriptions)
	{
	}

	ObjectsModification::~ObjectsModification() throw()
	{
	}

	bool ObjectsModification::do_merge(Modification* modification)
	{
		return true;
	}

	bool ObjectsModification::undo(EditorMapData &editor)
	{
		switch (get_type())
		{
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
				return false;
			case mt_objects_removed:
				BOOST_FOREACH(const EditorObjectDescription
					&object_description,
					m_object_descriptions)
				{
					editor.use_object_id(
						object_description.get_id());
					editor.add_object(object_description,
						sct_no);
				}
				return false;
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
				BOOST_FOREACH(const EditorObjectDescription
					&object_description,
					m_object_descriptions)
				{
					editor.modify_object(
						object_description);
				}
				return false;
			case mt_light_added:
			case mt_light_removed:
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
			case mt_lights_position_changed:
			case mt_lights_color_changed:
			case mt_lights_radius_changed:
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
