/****************************************************************************
 *            terrainmaterialmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainmaterialmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	TerrainMaterialModification::TerrainMaterialModification(
		const String &albedo_map, const String &specular_map,
		const String &gloss_map, const String &height_map,
		const glm::vec3 &default_specular, const float default_gloss,
		const float default_height, const float blend_size,
		const bool use_blend_size_texture, const bool use_specular_map,
		const bool use_gloss_map, const bool use_height_map,
		const Uint16 index, const ModificationType type,
		const Uint32 edit_id): Modification(edit_id, index, type),
		m_albedo_map(albedo_map), m_specular_map(specular_map),
		m_gloss_map(gloss_map), m_height_map(height_map),
		m_default_specular(default_specular),
		m_default_gloss(default_gloss),
		m_default_height(default_height), m_blend_size(blend_size),
		m_use_blend_size_texture(use_blend_size_texture),
		m_use_specular_map(use_specular_map),
		m_use_gloss_map(use_gloss_map),
		m_use_height_map(use_height_map), m_index(index)
	{
	}

	TerrainMaterialModification::~TerrainMaterialModification() throw()
	{
	}

	bool TerrainMaterialModification::do_merge(Modification* modification)
	{
		return true;
	}

	bool TerrainMaterialModification::undo(EditorMapData &editor)
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
				break;
			case mt_terrain_material_changed:
				editor.set_terrain_material(m_albedo_map,
					m_specular_map, m_gloss_map,
					m_height_map, m_default_specular,
					m_default_gloss, m_default_height,
					m_blend_size, m_use_blend_size_texture,
					m_use_specular_map, m_use_gloss_map,
					m_use_height_map, m_index);
				break;
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
