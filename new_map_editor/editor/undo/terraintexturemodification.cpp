/****************************************************************************
 *            terraintexturemodification.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terraintexturemodification.hpp"
#include "material.hpp"
#include "editor.hpp"
#include "scenepagereadonly.hpp"
#include "scenepagereadwrite.hpp"

namespace eternal_lands
{

	TerrainTextureModification::TerrainTextureModification(const String &texture,
		const GlobalId global_id, const Uint16 index): m_texture(texture),
		m_global_id(global_id), m_index(index)
	{
	}

	TerrainTextureModification::~TerrainTextureModification()
	{
	}

	ModificationType TerrainTextureModification::get_type() const
	{
		return mt_terrain_diffuse_texture_changed;
	}

	bool TerrainTextureModification::merge(Modification* modification)
	{
		TerrainTextureModification* terrain_texture_modification;

		if (get_type() == modification->get_type())
		{
			terrain_texture_modification = dynamic_cast<TerrainTextureModification*>(modification);

			assert(terrain_texture_modification != 0);

			return (m_index == terrain_texture_modification->m_index) &&
				(m_global_id == terrain_texture_modification->m_global_id);
		}
		else
		{
			return false;
		}
	}

	bool TerrainTextureModification::undo(Editor &editor)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MaterialSharedPtr material;

		editor.get_scene().get_scene_page_read_write(m_global_id.get_page_id(),
			scene_page_read_write);

		material = scene_page_read_write->get_object_materials(
			m_global_id.get_id())[0];

		switch (get_type())
		{
			case mt_light_added:
			case mt_light_removed:
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
			case mt_object_added:
			case mt_object_removed:
			case mt_object_translation_changed:
			case mt_object_rotation_changed:
			case mt_object_scale_changed:
			case mt_object_blending_changed:
			case mt_object_color_changed:
			case mt_object_type_changed:
			case mt_object_server_id_changed:
				break;
			case mt_terrain_diffuse_texture_changed:
				material->set_diffuse_texture(m_texture, m_index);
			case mt_terrain_normal_texture_changed:
				material->set_normal_texture(m_texture, m_index);
				break;
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
