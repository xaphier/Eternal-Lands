/****************************************************************************
 *            lightmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "lightmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	LightModification::LightModification(const LightData &data,
		const ModificationType type, const Uint32 edit_id):
		Modification(edit_id), m_data(data), m_type(type)
	{
	}

	LightModification::~LightModification() throw()
	{
	}

	ModificationType LightModification::get_type() const
	{
		return m_type;
	}

	bool LightModification::do_merge(Modification* modification)
	{
		assert(dynamic_cast<LightModification*>(modification) != 0);

		return m_data.get_id() == dynamic_cast<LightModification*>(
			modification)->m_data.get_id();
	}

	bool LightModification::undo(EditorMapData &editor)
	{
		switch (get_type())
		{
			case mt_light_added:
				return editor.remove_light(m_data.get_id());
			case mt_light_removed:
				editor.add_light(m_data);
				return false;
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
				editor.modify_light(m_data);
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
			case mt_terrain_albedo_map_changed:
			case mt_terrain_blend_map_changed:
			case mt_terrain_vector_map_changed:
			case mt_terrain_dudv_map_changed:
			case mt_terrain_scale_offset_changed:
			case mt_tile_texture_changed:
			case mt_scene_ambient_changed:
			case mt_terrain_value_changed:
			case mt_height_changed:
			case mt_blend_values_changed:
				return false;
		}

		return false;
	}

}
