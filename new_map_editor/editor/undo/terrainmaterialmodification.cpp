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
		const String &albedo_map, const String &extra_map,
		const float blend_size, const bool use_blend_size_sampler,
		const bool use_blend_size, const bool use_extra_map,
		const Uint16 index, const ModificationType type,
		const Uint32 edit_id): Modification(edit_id, index, type),
		m_albedo_map(albedo_map), m_extra_map(extra_map),
		m_blend_size(blend_size),
		m_use_blend_size_sampler(use_blend_size_sampler),
		m_use_blend_size(use_blend_size),
		m_use_extra_map(use_extra_map), m_index(index)
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
				break;
			case mt_terrain_material_changed:
				editor.set_terrain_material(m_albedo_map,
					m_extra_map, m_blend_size,
					m_use_blend_size_sampler,
					m_use_blend_size, m_use_extra_map,
					m_index);
				break;
			case mt_terrain_scale_offset_changed:
			case mt_tile_texture_changed:
			case mt_scene_ambient_changed:
			case mt_dungeon_changed:
			case mt_displacement_value_changed:
			case mt_height_changed:
			case mt_blend_values_changed:
				return false;
		}

		return false;
	}

}
