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
				editor.modify_object(m_object_description);
				return false;
			case mt_light_added:
			case mt_light_removed:
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
			case mt_terrain_albedo_map_changed:
			case mt_terrain_blend_map_changed:
			case mt_terrain_vector_map_changed:
			case mt_terrain_dudv_map_changed:
			case mt_terrain_scale_offset_changed:
			case mt_tile_texture_changed:
			case mt_scene_ambient_changed:
			case mt_dungeon_changed:
			case mt_terrain_value_changed:
			case mt_height_changed:
			case mt_blend_values_changed:
				return false;
		}

		return false;
	}

}
