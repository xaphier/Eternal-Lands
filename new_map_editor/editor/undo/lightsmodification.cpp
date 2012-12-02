/****************************************************************************
 *            lightsmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "lightsmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	LightsModification::LightsModification(const LightDataVector &datas,
		const ModificationType type, const Uint32 edit_id):
		Modification(edit_id, 0, type), m_datas(datas)
	{
	}

	LightsModification::~LightsModification() throw()
	{
	}

	bool LightsModification::do_merge(Modification* modification)
	{
		return true;
	}

	bool LightsModification::undo(EditorMapData &editor)
	{
		switch (get_type())
		{
			case mt_light_added:
			case mt_light_removed:
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
				return false;
			case mt_lights_position_changed:
			case mt_lights_color_changed:
			case mt_lights_radius_changed:
				BOOST_FOREACH(const LightData &data, m_datas)
				{
					editor.modify_light(data);
				}
				return false;
			case mt_object_added:
			case mt_object_removed:
			case mt_object_translation_changed:
			case mt_object_rotation_changed:
			case mt_object_scale_changed:
			case mt_object_blend_changed:
			case mt_object_transparency_changed:
			case mt_object_selection_changed:
			case mt_object_materials_changed:
			case mt_object_walkable_changed:
			case mt_object_name_changed:
			case mt_object_description_changed:
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
			case mt_objects_description_changed:
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
