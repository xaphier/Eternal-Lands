/****************************************************************************
 *            lightmodification.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "lightmodification.hpp"

namespace eternal_lands
{

	LightModification::LightModification(const LightData &data,
		const ModificationType type): m_data(data), m_type(type)
	{
	}

	LightModification::~LightModification()
	{
	}

	ModificationType LightModification::get_type() const
	{
		return m_type;
	}

	bool LightModification::merge(Modification* modification)
	{
		if (get_type() == modification->get_type())
		{
			assert(dynamic_cast<LightModification*>(modification) != 0);

			return m_id == dynamic_cast<LightModification*>(modification)->m_id;
		}
		else
		{
			return false;
		}
	}

	bool LightModification::undo(Editor &editor)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;

		editor.get_scene().get_scene_page_read_write(m_id.get_page_id(),
			scene_page_read_write);

		switch (get_type())
		{
			case mt_light_added:
				scene_page_read_write->remove_light(m_id.get_id());
				return editor.get_light_selected(m_id);
			case mt_light_removed:
				scene_page_read_write->add_light(m_data);
				return false;
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
				scene_page_read_write->modify_light(m_id.get_id(), m_data);
				return false;
			case mt_object_added:
			case mt_object_removed:
			case mt_object_translation_changed:
			case mt_object_rotation_changed:
			case mt_object_scale_changed:
			case mt_object_blending_changed:
			case mt_object_color_changed:
			case mt_object_type_changed:
			case mt_object_server_id_changed:
			case mt_terrain_diffuse_texture_changed:
			case mt_terrain_normal_texture_changed:
			case mt_terrain_scale_offset_changed:
			case mt_tile_texture_changed:
			case mt_scene_ambient_changed:
			case mt_terrain_height_changed:
			case mt_height_changed:
			case mt_blend_values_changed:
			case mt_terrain_height_map_changed:
			case mt_height_map_changed:
			case mt_terrain_height_scale_changed:
				return false;
		}

		return false;
	}

}
