/****************************************************************************
 *            objectmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "objectmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	ObjectModification::ObjectModification(
		const EditorObjectDescription &object_description,
		const ModificationType type, const Uint32 edit_id):
		Modification(edit_id, object_description.get_id(), type),
		m_object_description(object_description)
	{
	}

	ObjectModification::~ObjectModification() throw()
	{
	}

	bool ObjectModification::do_merge(Modification* modification)
	{
		return true;
	}

	bool ObjectModification::undo(EditorMapData &editor)
	{
		switch (get_type())
		{
			case mt_object_added:
				return editor.remove_object(
					m_object_description.get_id());
			case mt_object_removed:
				editor.use_object_id(
					m_object_description.get_id());
				editor.add_object(m_object_description,
					sct_select);
				return false;
			case mt_object_translation_changed:
			case mt_object_rotation_changed:
			case mt_object_scale_changed:
			case mt_object_blend_changed:
			case mt_object_transparency_changed:
			case mt_object_selection_changed:
			case mt_object_materials_changed:
			case mt_object_walkable_changed:
			case mt_object_name_changed:
				editor.modify_object(m_object_description);
				return false;
			case mt_objects_removed:
			case mt_objects_translation_changed:
			case mt_objects_rotation_changed:
			case mt_objects_scale_changed:
			case mt_objects_blend_changed:
			case mt_objects_transparency_changed:
			case mt_objects_selection_changed:
			case mt_objects_materials_changed:
			case mt_objects_walkable_changed:
			case mt_objects_name_changed:
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
			case mt_tile_texture_changed:
			case mt_scene_ground_hemisphere_changed:
			case mt_dungeon_changed:
			case mt_displacement_value_changed:
			case mt_height_changed:
			case mt_blend_values_changed:
				return false;
		}

		return false;
	}

}
