/****************************************************************************
 *            editor.cpp
 *
 * Author: 2010  Daniel Jungmann <dsj@gmx.net>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editor.hpp"
#include "height.hpp"
#include "undo/groundhemispheremodification.hpp"
#include "undo/blendvaluemodification.hpp"
#include "undo/blendvaluesmodification.hpp"
#include "undo/dungeonmodification.hpp"
#include "undo/heightmodification.hpp"
#include "undo/lightmodification.hpp"
#include "undo/objectmodification.hpp"
#include "undo/heightmodification.hpp"
#include "undo/terrainmaterialmodification.hpp"
#include "undo/tilevaluemodification.hpp"
#include "undo/tilelayerheightmodification.hpp"
#include "undo/displacementvaluemodification.hpp"
#include "undo/lightsmodification.hpp"
#include "undo/objectsmodification.hpp"
#include "undo/terraintranslationmodification.hpp"
#include "scene.hpp"
#include "codec/codecmanager.hpp"
#include "logging.hpp"
#include "image.hpp"
#include "tilebuilder.hpp"

namespace eternal_lands
{

	Editor::Editor(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system):
		m_data(global_vars, file_system), m_undo(1000),
		m_uni_dist(0.0f, 1.0f), m_random(m_gen, m_uni_dist)
	{
		m_random_translation_min = glm::vec3(0.0f);
		m_random_translation_max = glm::vec3(0.0f);
		m_random_rotation_min = glm::vec3(0.0f);
		m_random_rotation_max = glm::vec3(0.0f);
		m_random_scale_min = 1.0f;
		m_random_scale_max = 1.0f;
		m_random_translation[0] = false;
		m_random_translation[1] = false;
		m_random_translation[2] = false;
		m_random_rotation[0] = false;
		m_random_rotation[1] = false;
		m_random_rotation[2] = false;
		m_random_scale = false;

		m_edit_id = 0;
	}

	bool Editor::undo()
	{
		return m_undo.undo(m_data);
	}

	bool Editor::add_needed(const Uint32 id, const ModificationType type)
	{
		return m_undo.add_needed(m_edit_id, id, type);
	}

	void Editor::change_object(const ModificationType type,
		const EditorObjectDescription &object_description)
	{
		if (!add_needed(object_description.get_id(), type))
		{
			return;
		}

		ModificationAutoPtr modification(new ObjectModification(
			object_description, type, m_edit_id));

		m_undo.add(modification);
	}

	void Editor::change_objects(const ModificationType type,
		const EditorObjectDescriptionVector &object_descriptions)
	{
		if (!add_needed(0, type))
		{
			return;
		}

		ModificationAutoPtr modification(new ObjectsModification(
			object_descriptions, type, m_edit_id));

		m_undo.add(modification);
	}

	void Editor::change_light(const ModificationType type,
		const LightData &light_data)
	{
		if (!add_needed(light_data.get_id(), type))
		{
			return;
		}

		ModificationAutoPtr modification(new LightModification(
			light_data, type, m_edit_id));

		m_undo.add(modification);
	}

	void Editor::change_lights(const ModificationType type,
		const LightDataVector &light_datas)
	{
		if (!add_needed(0, type))
		{
			return;
		}

		ModificationAutoPtr modification(new LightsModification(
			light_datas, type, m_edit_id));

		m_undo.add(modification);
	}

	void Editor::set_terrain_translation(const glm::vec3 &translation)
	{
		if (translation == get_terrain_translation())
		{
			return;
		}

		if (add_needed(0, mt_terrain_translation_changed))
		{
			ModificationAutoPtr modification(new
				TerrainTranslationModification(
					get_terrain_translation(),
					mt_terrain_translation_changed,
					get_edit_id()));

			m_undo.add(modification);
		}

		m_data.set_terrain_translation(translation);
	}

	void Editor::set_terrain_material(const String &albedo_map,
		const String &specular_map, const String &gloss_map,
		const String &height_map, const glm::vec3 &default_specular,
		const float default_gloss, const float default_height,
		const float blend_size, const bool use_blend_size_texture,
		const bool use_specular_map, const bool use_gloss_map,
		const bool use_height_map, const Uint16 index)
	{
		String old_albedo_map, old_specular_map, old_gloss_map;
		String old_height_map;
		glm::vec3 old_default_specular;
		float old_default_gloss, old_default_height, old_blend_size;
		bool old_use_blend_size_texture, old_use_specular_map;
		bool old_use_gloss_map, old_use_height_map;

		get_terrain_material(old_albedo_map, old_specular_map,
			old_gloss_map, old_height_map, old_default_specular,
			old_default_gloss, old_default_height, old_blend_size,
			old_use_blend_size_texture, old_use_specular_map,
			old_use_gloss_map, old_use_height_map, index);

		if ((old_albedo_map == albedo_map) &&
			(old_specular_map == specular_map) &&
			(old_gloss_map == gloss_map) &&
			(old_height_map == height_map) &&
			(old_default_specular == default_specular) &&
			(old_default_gloss == default_gloss) &&
			(old_default_height == default_height) &&
			(old_blend_size == blend_size) &&
			(old_use_blend_size_texture ==
				use_blend_size_texture) &&
			(old_use_specular_map == use_specular_map) &&
			(old_use_gloss_map == use_gloss_map) &&
			(old_use_height_map == use_height_map))
		{
			return;
		}

		if (add_needed(index, mt_terrain_material_changed))
		{
			ModificationAutoPtr modification(new
				TerrainMaterialModification(old_albedo_map,
					old_specular_map, old_gloss_map,
					old_height_map, old_default_specular,
					old_default_gloss, old_default_height,
					old_blend_size,
					old_use_blend_size_texture,
					old_use_specular_map,
					old_use_gloss_map, old_use_height_map,
					index, mt_terrain_material_changed,
					get_edit_id()));

			m_undo.add(modification);
		}

		m_data.set_terrain_material(albedo_map, specular_map,
			gloss_map, height_map, default_specular, default_gloss,
			default_height, blend_size, use_blend_size_texture,
			use_specular_map, use_gloss_map, use_height_map,
			index);
	}

	void Editor::set_tile(const glm::uvec3 &offset, const Uint16 size,
		const Uint16 tile)
	{
		ImageValueVector tile_values;

		m_data.get_tile_values(offset, size, tile_values);

		ModificationAutoPtr modification(new TileValueModification(
			tile_values, offset.z, get_edit_id()));

		m_undo.add(modification);

		BOOST_FOREACH(ImageValue &tile_value, tile_values)
		{
			tile_value.set_value(tile);
		}

		m_data.set_tile_values(tile_values, offset.z);
	}

	void Editor::add_3d_object(const glm::vec3 &position,
		const String &name, const SelectionType selection)
	{
		EditorObjectDescription object_description;
		glm::vec3 translation, rotation, vrandom;
		float scale;
		Uint32 i,id;

		id = m_data.get_free_object_id();

		for (i = 0; i < 3; i++)
		{
			vrandom[i] = m_random();
		}

		translation = glm::mix(m_random_translation_min,
			m_random_translation_max, vrandom);

		for (i = 0; i < 3; i++)
		{
			vrandom[i] = m_random();
		}

		rotation = glm::mix(m_random_rotation_min,
			m_random_rotation_max, vrandom);

		for (i = 0; i < 3; i++)
		{
			if (!m_random_translation[i])
			{
				translation[i] = 0.0f;
			}

			if (!m_random_rotation[i])
			{
				rotation[i] = 0.0f;
			}
		}

		if (m_random_scale)
		{
			scale = glm::mix(m_random_scale_min, m_random_scale_max,
				m_random());
		}
		else
		{
			scale = 1.0f;
		}

		object_description.set_translation(position + translation);
		object_description.set_rotation_angles(rotation);
		object_description.set_scale(glm::vec3(scale));
		object_description.set_selection(selection);
		object_description.set_name(name);
		object_description.set_id(id);

		m_data.add_object(object_description, sct_no);

		change_object(mt_object_added, object_description);
	}

	void Editor::add_light(const glm::vec3 &position, const float radius)
	{
		LightData light_data;
		Uint32 id;

		id = m_data.get_free_light_id();

		light_data.set_position(position);
		light_data.set_color(glm::vec3(1.0f));
		light_data.set_radius(radius);
		light_data.set_id(id);

		m_data.add_light(light_data, sct_force_select);

		change_light(mt_light_added, light_data);
	}

	void Editor::set_dungeon(const bool dungeon)
	{
		if (add_needed(0, mt_dungeon_changed))
		{
			ModificationAutoPtr modification(new
				DungeonModification(m_data.get_dungeon(),
				get_edit_id()));

			m_undo.add(modification);
		}

		m_data.set_dungeon(dungeon);
	}

	bool Editor::get_dungeon() const
	{
		return m_data.get_dungeon();
	}

	void Editor::set_ground_hemisphere(const glm::vec4 &ground_hemisphere)
	{
		if (add_needed(0, mt_scene_ground_hemisphere_changed))
		{
			ModificationAutoPtr modification(new
				GroundHemisphereModification(
					m_data.get_ground_hemisphere(),
					get_edit_id()));

			m_undo.add(modification);
		}

		m_data.set_ground_hemisphere(ground_hemisphere);
	}

	const glm::vec4 &Editor::get_ground_hemisphere() const
	{
		return m_data.get_ground_hemisphere();
	}

	bool Editor::get_tile_edit(const glm::vec3 &min_position,
		const glm::vec3 &max_position, const Uint16 layer,
		glm::uvec3 &offset) const
	{
		glm::vec3 dir, position, camera;
		float d;

		dir = max_position - min_position;

		if (std::abs(dir.z) < epsilon)
		{
			return false;
		}

		d = (m_data.get_tile_layer_height(layer) - min_position.z) /
			dir.z;

		if ((d > 1.0f) || (d < 0.0f))
		{
			return false;
		}

		position = min_position + dir * d;
		position /= TileBuilder::get_tile_world_scale();

		if ((position.x < 0.0f) || (position.y < 0.0f))
		{
			return false;
		}

		offset = position;

		if ((offset.x >= m_data.get_tile_map_size().x) ||
			(offset.y >= m_data.get_tile_map_size().x))
		{
			return false;
		}

		offset.z = layer;

		return true;
	}

	float Editor::get_tile_layer_height(const Uint16 index) const
	{
		return m_data.get_tile_layer_height(index);
	}

	void Editor::set_tile_layer_height(const float value,
		const Uint16 layer)
	{
		float height;

		height = m_data.get_tile_layer_height(layer);

		if (value == height)
		{
			return;
		}

		if (add_needed(layer, mt_tile_layer_height_changed))
		{
			ModificationAutoPtr modification(new
				TileLayerHeightModification(height,
				layer, get_edit_id()));

			m_undo.add(modification);
		}

		m_data.set_tile_layer_height(value, layer);
	}

	void Editor::height_edit(const glm::vec3 &point, const Uint8 height)
	{
/*		Ray ray;

		ray = Ray(p0, glm::normalize(p1 - p0));

		{

			get_scene().set_view_changed();
		}
*/	}

	void Editor::load_map(const String &name, const bool load_2d_objects,
		const bool load_3d_objects, const bool load_lights,
		const bool load_particles, const bool load_materials,
		const bool load_height_map, const bool load_tile_map,
		const bool load_walk_map, const bool load_terrain)
	{
		MapItemsTypeSet skip_items;

		if (!load_2d_objects)
		{
			skip_items.insert(mit_2d_objects);
		}

		if (!load_3d_objects)
		{
			skip_items.insert(mit_3d_objects);
		}

		if (!load_lights)
		{
			skip_items.insert(mit_lights);
		}

		if (!load_particles)
		{
			skip_items.insert(mit_particles);
		}

		if (!load_materials)
		{
			skip_items.insert(mit_materials);
		}

		if (!load_height_map)
		{
			skip_items.insert(mit_height_map);
		}

		if (!load_tile_map)
		{
			skip_items.insert(mit_tile_map);
		}

		if (!load_walk_map)
		{
			skip_items.insert(mit_walk_map);
		}

		if (!load_terrain)
		{
			skip_items.insert(mit_terrain);
		}

		m_data.load_map(name, skip_items);

		m_undo.clear();
	}

	void Editor::remove_object(const Uint32 id)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		change_object(mt_object_removed, object_description);

		m_data.remove_object(id);
	}

	void Editor::remove_all_copies_of_object(const Uint32 id)
	{
		EditorObjectDescriptionVector object_descriptions;

		m_data.get_objects(id, object_descriptions);

		change_objects(mt_objects_removed, object_descriptions);

		BOOST_FOREACH(
			const EditorObjectDescription &object_description,
			object_descriptions)
		{
			m_data.remove_object(object_description.get_id());
		}
	}

	void Editor::set_object_blend(const Uint32 id, const BlendType blend,
		const BitSet64 blend_mask)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if ((object_description.get_blend() != blend) ||
			(object_description.get_blend_mask() != blend_mask))
		{
			change_object(mt_object_blend_changed,
				object_description);

			object_description.set_blend(blend);
			object_description.set_blend_mask(blend_mask);

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_object_blend_mask(const Uint32 id,
		const BitSet64 blend_mask)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_blend_mask() != blend_mask)
		{
			change_object(mt_object_blend_changed,
				object_description);

			object_description.set_blend_mask(blend_mask);

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_object_transparency(const Uint32 id,
		const float transparency)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_transparency() != transparency)
		{
			change_object(mt_object_transparency_changed,
				object_description);

			object_description.set_transparency(transparency);

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_object_glow(const Uint32 id, const float glow)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_glow() != glow)
		{
			change_object(mt_object_glow_changed,
				object_description);

			object_description.set_glow(glow);

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_object_translation(const Uint32 id,
		const glm::vec3 &translation)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (glm::any(glm::notEqual(object_description.get_translation(),
			translation)))
		{
			change_object(mt_object_translation_changed,
				object_description);

			object_description.set_translation(translation);

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_object_rotation(const Uint32 id,
		const glm::vec3 &rotation)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (glm::any(glm::notEqual(
			object_description.get_rotation_angles(), rotation)))
		{
			change_object(mt_object_rotation_changed,
				object_description);

			object_description.set_rotation_angles(rotation);

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_object_scale(const Uint32 id, const glm::vec3 &scale)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_scale() != scale)
		{
			change_object(mt_object_scale_changed,
				object_description);

			object_description.set_scale(scale);

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_object_selection(const Uint32 id,
		const SelectionType selection)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_selection() != selection)
		{
			change_object(mt_object_selection_changed,
				object_description);

			object_description.set_selection(selection);

			m_data.modify_object(object_description);
		}
	}

	bool Editor::check_default_materials(const String &name,
		const StringVector &materials) const
	{
		StringVector default_materials;
		Uint32 i, count;

		default_materials = get_default_materials(name);

		count = std::min(default_materials.size(), materials.size());

		for (i = 0; i < count; ++i)
		{
			if (default_materials[i] != materials[i])
			{
				return false;
			}
		}

		return true;
	}

	void Editor::set_object_materials(const Uint32 id,
		const StringVector &materials)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_material_names() != materials)
		{
			change_object(mt_object_materials_changed,
				object_description);

			if (check_default_materials(
				object_description.get_name(), materials))
			{
				object_description.set_material_names(
					StringVector());
			}
			else
			{
				object_description.set_material_names(
					materials);
			}

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_object_name(const Uint32 id, const String &name)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_name() != name)
		{
			change_object(mt_object_name_changed,
				object_description);

			object_description.set_name(name);

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_object_description(const Uint32 id,
		const String &description)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_description() != description)
		{
			change_object(mt_object_description_changed,
				object_description);

			object_description.set_description(description);

			m_data.modify_object(object_description);
		}
	}

	void Editor::set_all_copies_of_object_name(const Uint32 id,
		const String &name)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, count;

		m_data.get_objects(id, object_descriptions);

		change_objects(mt_objects_name_changed, object_descriptions);

		count = object_descriptions.size();

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_name(name);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_blend(const Uint32Set &ids,
		const BlendType blend)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_blend_changed, object_descriptions);

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_blend(blend);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_transparency(const Uint32Set &ids,
		const float transparency)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_transparency_changed,
			object_descriptions);

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_transparency(transparency);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_glow(const Uint32Set &ids,
		const float glow)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_glow_changed, object_descriptions);

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_glow(glow);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_translation(const Uint32Set &ids,
		const glm::vec3 &translation)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_translation_changed,
			object_descriptions);

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_translation(translation);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_rotation(const Uint32Set &ids,
		const glm::vec3 &rotation)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_rotation_changed,
			object_descriptions);

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_rotation_angles(rotation);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_scale(const Uint32Set &ids,
		const glm::vec3 &scale)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_scale_changed,
			object_descriptions);

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_scale(scale);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_selection(const Uint32Set &ids,
		const SelectionType selection)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_selection_changed,
			object_descriptions);

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_selection(selection);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_materials(const Uint32Set &ids,
		const StringVector &materials)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_materials_changed,
			object_descriptions);

		for (i = 0; i < count; ++i)
		{
			if (check_default_materials(
				object_descriptions[i].get_name(), materials))
			{
				object_descriptions[i].set_material_names(
					StringVector());
			}
			else
			{
				object_descriptions[i].set_material_names(
					materials);
			}

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_description(const Uint32Set &ids,
		const String &description)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_description_changed,
			object_descriptions);

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_description(description);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::set_objects_name(const Uint32Set &ids, const String &name)
	{
		EditorObjectDescriptionVector object_descriptions;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		object_descriptions.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_object(id, object_descriptions[index]);
			index++;
		}

		change_objects(mt_objects_name_changed, object_descriptions);

		for (i = 0; i < count; ++i)
		{
			object_descriptions[i].set_name(name);

			m_data.modify_object(object_descriptions[i]);
		}
	}

	void Editor::remove_light(const Uint32 id)
	{
		LightData light_data;

		m_data.get_light(id, light_data);

		change_light(mt_light_removed, light_data);

		m_data.remove_light(id);
	}

	void Editor::set_light_position(const Uint32 id,
		const glm::vec3 &position)
	{
		LightData light_data;

		m_data.get_light(id, light_data);

		if (glm::any(glm::notEqual(light_data.get_position(), position)))
		{
			change_light(mt_light_position_changed, light_data);

			light_data.set_position(position);

			m_data.modify_light(light_data);
		}
	}

	void Editor::set_light_radius(const Uint32 id, const float radius)
	{
		LightData light_data;

		m_data.get_light(id, light_data);

		if (light_data.get_radius() != radius)
		{
			change_light(mt_light_radius_changed, light_data);

			light_data.set_radius(radius);

			m_data.modify_light(light_data);
		}
	}

	void Editor::set_light_color(const Uint32 id, const glm::vec3 &color)
	{
		LightData light_data;

		m_data.get_light(id, light_data);

		if (glm::any(glm::notEqual(light_data.get_color(), color)))
		{
			change_light(mt_light_color_changed, light_data);

			light_data.set_color(color);

			m_data.modify_light(light_data);
		}
	}

	void Editor::set_lights_position(const Uint32Set &ids,
		const glm::vec3 &position)
	{
		LightDataVector light_datas;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		light_datas.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_light(id, light_datas[index]);
			index++;
		}

		change_lights(mt_lights_position_changed, light_datas);

		for (i = 0; i < count; ++i)
		{
			light_datas[i].set_position(position);

			m_data.modify_light(light_datas[i]);
		}
	}

	void Editor::set_lights_radius(const Uint32Set &ids,
		const float radius)
	{
		LightDataVector light_datas;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		light_datas.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_light(id, light_datas[index]);
			index++;
		}

		change_lights(mt_lights_radius_changed, light_datas);

		for (i = 0; i < count; ++i)
		{
			light_datas[i].set_radius(radius);

			m_data.modify_light(light_datas[i]);
		}
	}

	void Editor::set_lights_color(const Uint32Set &ids,
		const glm::vec3 &color)
	{
		LightDataVector light_datas;
		Uint32 i, index, count;

		count = ids.size();

		if (count == 0)
		{
			return;
		}

		light_datas.resize(count);

		index = 0;

		BOOST_FOREACH(const Uint32 id, ids)
		{
			m_data.get_light(id, light_datas[index]);
			index++;
		}

		change_lights(mt_lights_color_changed, light_datas);

		for (i = 0; i < count; ++i)
		{
			light_datas[i].set_color(color);

			m_data.modify_light(light_datas[i]);
		}
	}

	void Editor::get_object_description(const Uint32 id,
		EditorObjectDescription &object_description) const
	{
		m_data.get_object(id, object_description);
	}

	void Editor::get_light_data(const Uint32 id, LightData &light_data)
		const
	{
		m_data.get_light(id, light_data);
	}

	void Editor::change_terrain_displacement_values(
		const glm::vec3 &position, const glm::vec3 &data,
		const glm::bvec3 &mask, const glm::vec2 &size,
		const float attenuation_size, const int attenuation,
		const int shape, const int effect)
	{
		DisplacementValueVector displacement_values;
		glm::uvec2 vertex;

		if (!m_data.get_terrain_vertex(position, vertex))
		{
			return;
		}

		m_data.get_terrain_displacement_values(vertex, size,
			attenuation_size,
			static_cast<BrushAttenuationType>(attenuation),
			static_cast<BrushShapeType>(shape),
			displacement_values);

		ModificationAutoPtr modification(
			new DisplacementValueModification(displacement_values,
			get_edit_id()));

		m_undo.add(modification);

		m_data.change_terrain_displacement_values(data, mask, size,
			vertex, attenuation_size,
			static_cast<BrushAttenuationType>(attenuation),
			static_cast<BrushShapeType>(shape),
			static_cast<DisplacementEffectType>(effect),
			displacement_values);

		m_data.set_terrain_displacement_values(displacement_values);
	}

	void Editor::change_terrain_blend_values(const glm::vec3 &position,
		const glm::vec2 &size, const float attenuation_size,
		const float data, const int attenuation, const int shape,
		const int effect, const int layer)
	{
		ImageValueVector blend_values;
		glm::uvec2 vertex;

		if (!m_data.get_terrain_vertex(position, vertex))
		{
			return;
		}

		m_data.get_terrain_blend_values(vertex, size, attenuation_size,
			static_cast<BrushAttenuationType>(attenuation),
			static_cast<BrushShapeType>(shape), layer,
			blend_values);

		ModificationAutoPtr modification(new BlendValueModification(
			blend_values, layer, get_edit_id()));

		m_undo.add(modification);

		m_data.change_terrain_blend_values(size, vertex,
			attenuation_size, data,
			static_cast<BrushAttenuationType>(attenuation),
			static_cast<BrushShapeType>(shape),
			static_cast<BlendEffectType>(effect), layer,
			blend_values);

		m_data.set_terrain_blend_values(blend_values, layer);
	}

	void Editor::import_terrain_height_map(const String &name)
	{
		DisplacementValueVector displacement_values;

		m_data.get_all_terrain_displacement_values(
			displacement_values);

		ModificationAutoPtr modification(
			new DisplacementValueModification(displacement_values,
			get_edit_id()));

		m_undo.add(modification);

		m_data.import_terrain_height_map(name);
	}

	void Editor::import_terrain_blend_map(const String &name)
	{
		ImageValuesVector blend_values;

		m_data.get_all_terrain_blend_values(blend_values);

		ModificationAutoPtr modification(new BlendValuesModification(
			blend_values, get_edit_id()));

		m_undo.add(modification);

		m_data.import_terrain_blend_map(name);
	}

	void Editor::relax_terrain_uv(
		const AbstractProgressSharedPtr &progress, const Uint16 count)
	{
#ifdef	USE_SSE2
		m_data.relax_terrain_uv(progress, count, SDL_HasSSE2());
#else	/* USE_SSE2 */
		m_data.relax_terrain_uv(progress, count, false);
#endif	/* USE_SSE2 */
	}

	void Editor::update_terrain_dudv()
	{
		m_data.update_terrain_dudv();
	}

	void Editor::get_terrain_layers_usage(Uint32Vector &use_layer_pixels,
		Uint32 &pixels) const
	{
		return m_data.get_terrain_layers_usage(use_layer_pixels,
			pixels);
	}

	void Editor::clear_invisible_terrain_layers()
	{
		m_data.clear_invisible_terrain_layers();
	}

	void Editor::pack_terrain_layers()
	{
		m_data.pack_terrain_layers();
	}

	void Editor::deselect()
	{
		m_data.deselect();
	}

	void Editor::clear()
	{
		m_undo.clear();
		m_data.clear();
	}

	bool Editor::get_terrain_displacement(const glm::vec3 &position,
		glm::vec3 &displacement) const
	{
		glm::uvec2 vertex;

		if (!m_data.get_terrain_vertex(position, vertex))
		{
			return false;
		}

		displacement = m_data.get_terrain_displacement(vertex);

		return true;
	}

	bool Editor::get_terrain_normal(const glm::vec3 &position,
		glm::vec3 &normal) const
	{
		glm::uvec2 vertex;

		if (!m_data.get_terrain_vertex(position, vertex))
		{
			return false;
		}

		normal = m_data.get_terrain_normal(vertex);

		return true;
	}

	void Editor::fill_terrain_blend_layer(const float strength,
		const int effect, const int layer)
	{
		ImageValueVector blend_values;

		m_data.get_all_terrain_blend_values(layer, blend_values);

		ModificationAutoPtr modification(new BlendValueModification(
			blend_values, layer, get_edit_id()));

		m_undo.add(modification);

		m_data.fill_terrain_blend_layer(strength,
			static_cast<BlendEffectType>(effect), layer);
	}

}
