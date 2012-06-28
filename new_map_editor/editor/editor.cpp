/****************************************************************************
 *            editor.cpp
 *
 * Author: 2010  Daniel Jungmann <dsj@gmx.net>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editor.hpp"
#include "height.hpp"
#include "undo/ambientmodification.hpp"
#include "undo/blendmodification.hpp"
#include "undo/heightmodification.hpp"
#include "undo/lightmodification.hpp"
#include "undo/objectmodification.hpp"
#include "undo/heightmodification.hpp"
#include "undo/terrainmapmodification.hpp"
#include "undo/groundtilemodification.hpp"
#include "undo/terrainvaluemodification.hpp"
#include "scene.hpp"
#include "codec/codecmanager.hpp"
#include "logging.hpp"
#include "image.hpp"

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

		init_logging("log");
	}

	bool Editor::undo()
	{
		return m_undo.undo(m_data);
	}

	void Editor::change_object(const ModificationType type,
		const EditorObjectDescription &object_description)
	{
		ModificationAutoPtr modification(new ObjectModification(
			object_description, type, m_edit_id));

		m_undo.add(modification);
	}

	void Editor::change_light(const ModificationType type,
		const LightData &light_data)
	{
		ModificationAutoPtr modification(new LightModification(
			light_data, type, m_edit_id));

		m_undo.add(modification);
	}

	void Editor::set_terrain_albedo_map(const String &str,
		const Uint16 index)
	{
		String tmp;

		tmp = m_data.get_terrain_albedo_map(index);

		if (str == tmp)
		{
			return;
		}

		ModificationAutoPtr modification(new TerrainMapModification(
			tmp, index, mt_terrain_albedo_map_changed,
			get_edit_id()));

		m_undo.add(modification);

		m_data.set_terrain_albedo_map(str, index);
	}

	void Editor::set_ground_tile(const glm::vec2 &point,
		const Uint16 tile)
	{
		glm::uvec2 offset;
		Uint16 tmp;

		offset = m_data.get_tile_offset(point);
		tmp = m_data.get_tile(offset[0], offset[1]);

		if (tile != tmp)
		{
			ModificationAutoPtr modification(
				new GroundTileModification(offset, tmp,
					get_edit_id()));

			m_undo.add(modification);

			m_data.set_tile(offset[0], offset[1], tile);
		}
	}

	void Editor::add_3d_object(const glm::vec3 &position,
		const String &mesh, const SelectionType selection)
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
		object_description.set_scale(scale);
		object_description.set_selection(selection);
		object_description.set_name(mesh);
		object_description.set_id(id);

		m_data.add_object(object_description);

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

		m_data.add_light(light_data);

		change_light(mt_light_added, light_data);
	}

	void Editor::set_ambient(const glm::vec3 &color)
	{
		ModificationAutoPtr modification(new AmbientModification(
			m_data.get_ambient(), get_edit_id()));

		m_undo.add(modification);

		m_data.set_ambient(color);
	}

/*
	void Editor::terrain_edit(const Uint16Array2 &vertex,
		const float strength, const float radius,
		const EditorBrushType brush_type, const Uint16 id)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		HeightVector heights;

		get_scene().get_scene_page_read_write(get_page_id(),
			scene_page_read_write);

		get_heights(scene_page_read_write, heights, vertex, radius);

		{
			ModificationAutoPtr modification(new HeightModification(heights,
				id, get_page_id(), true));

			m_undo.add(modification);
		}

		change_heights(heights, vertex, strength, radius, get_brush_type(
			brush_type));

		scene_page_read_write->set_terrain_heights(heights);
	}

	void Editor::blend_image_edit(const Uint32 id, const Uint16Array2 vertex,
		const Uint32 index, const float strength, const float radius,
		const EditorBrushType brush_type)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		ImageValueVector blend_values;

		get_scene().get_scene_page_read_write(get_page_id(),
			scene_page_read_write);

		get_blend_values(blend_values, vertex, radius);

		{
			ModificationAutoPtr modification(new BlendModification(
				blend_values, m_blend_image, id, get_page_id()));

			m_undo.add(modification);
		}

		change_blend_values(blend_values, vertex, index, strength, radius,
			brush_type);

		BOOST_FOREACH(const ImageValue &value, blend_values)
		{
			m_blend_image->set_pixel(value.get_x(), value.get_y(), 0, 0, 0,
				value.get_value());
		}

		m_texture->unload();
	}

	void Editor::terrain_height_edit(const Uint32 id, const glm::vec3 &p0,
		const glm::vec3 &p1, const float strength, const float radius,
		const int brush_type)
	{
		Ray ray;
		Uint32Array2 size;
		Uint16Array2 vertex;

		ray = Ray(p0, glm::normalize(p1 - p0));

		{
			ScenePageReadOnlyIntrusivePtr scene_page_read_only;

			get_scene().get_scene_page_read_only(get_page_id(),
				scene_page_read_only);

			size[0] = scene_page_read_only->get_terrain_width();
			size[1] = scene_page_read_only->get_terrain_height();
		}

		if (get_scene().intersect_terrain(get_page_id(), ray, size, vertex))
		{
			terrain_edit(id, vertex, strength, radius, get_brush_type(
				brush_type));

			get_scene().set_view_changed();
		}
	}

	void Editor::terrain_layer_edit(const Uint32 id, const glm::vec3 &p0,
		const glm::vec3 &p1, const Uint32 index, const float strength,
		const float radius, const int brush_type)
	{
		Ray ray;
		Uint32Array2 size;
		Uint16Array2 vertex;

		ray = Ray(p0, glm::normalize(p1 - p0));

		size[0] = m_blend_image->get_width();
		size[1] = m_blend_image->get_height();

		if (get_scene().intersect_terrain(get_page_id(), ray, size, vertex))
		{
			blend_image_edit(id, vertex, index, strength, radius,
				get_brush_type(brush_type));

			get_scene().set_view_changed();
		}
	}
*/
	void Editor::ground_tile_edit(const glm::vec3 &point,
		const Uint8 height)
	{
		set_ground_tile(glm::vec2(point), height);
	}

	void Editor::water_tile_edit(const glm::vec3 &point, const Uint8 water)
	{
//		set_ground_tile(glm::vec2(point), water);
	}

	void Editor::height_edit(const glm::vec3 &point, const Uint8 height)
	{
/*		Ray ray;

		ray = Ray(p0, glm::normalize(p1 - p0));

		{

			get_scene().set_view_changed();
		}
*/	}
/*
	void Editor::set_terrain(const MaterialData &terrain_material,
		const Uint16Array2 map_size, const Uint16Array2 blend_image_size)
	{
		ImageSharedPtr terrain_map;
		Uint32Array3 size;

		size[0] = blend_image_size[0];
		size[1] = blend_image_size[1];
		size[2] = 1;

		m_blend_image = boost::make_shared<Image>("blend.dds", false,
			tft_rgba8, size, 0);

		m_texture.reset(new ImageTexture(m_blend_image));
		m_texture->set_wrap_r(twt_clamp);
		m_texture->set_wrap_s(twt_clamp);
		m_texture->set_wrap_t(twt_clamp);

		size[0] = map_size[0] + 1;
		size[1] = map_size[1] + 1;

		terrain_map = boost::make_shared<Image>("terrain", false,
			tft_l8, size, 0);

		get_scene().set_terrain(terrain_map, terrain_material, m_texture);

		m_undo.clear();
	}

	void Editor::get_terrain_material_data(MaterialData &terrain_material)
		const
	{
		ScenePageReadOnlyIntrusivePtr scene_page_read_only;
		MaterialSharedPtr material;

		get_scene().get_scene_page_read_only(get_global_id().get_page_id(),
			scene_page_read_only);

		material = scene_page_read_only->get_object_materials(
			get_global_id().get_id())[0];

		terrain_material = material->get_material_data();
	}

	void Editor::save(const String &name) const
	{
		String path;

		path = AbstractFile::get_path_str(name);

		if (m_blend_image.get() != 0)
		{
			m_scene.save(name, m_blend_image->get_name());

			m_blend_image->save(path + "/" + m_blend_image->get_name(),
				"dds");
		}
		else
		{
			m_scene.save(name, "");
		}
	}
*/
	void Editor::load_map(const String &name)
	{
		m_data.load_map(name);

		m_undo.clear();
	}

	void Editor::remove_object(const Uint32 id)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		change_object(mt_object_removed, object_description);

		m_data.remove_object(id);
	}

	void Editor::set_object_blend(const Uint32 id, const BlendType blend)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_blend() != blend)
		{
			change_object(mt_object_blend_changed,
				object_description);

			object_description.set_blend(blend);

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

	void Editor::set_object_scale(const Uint32 id, const float scale)
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

	void Editor::set_object_materials(const Uint32 id,
		const StringVector &materials)
	{
		EditorObjectDescription object_description;

		m_data.get_object(id, object_description);

		if (object_description.get_material_names() != materials)
		{
			change_object(mt_object_materials_changed, object_description);

			object_description.set_material_names(materials);

			m_data.modify_object(object_description);
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

	const glm::vec3 &Editor::get_ambient() const
	{
		return m_data.get_ambient();
	}

	void Editor::change_terrain_values(const glm::vec3 &position,
		const glm::vec3 &data, const glm::bvec3 &mask,
		const glm::vec2 &size, const float attenuation_size,
		const int attenuation, const int shape, const int effect)
	{
		TerrainValueVector terrain_values;
		glm::uvec2 vertex;

		vertex = m_data.get_vertex(position);

		m_data.get_terrain_values(vertex, size, attenuation_size,
			static_cast<BrushAttenuationType>(attenuation),
			static_cast<BrushShapeType>(shape), terrain_values);

		ModificationAutoPtr modification(new TerrainValueModification(
			terrain_values, get_edit_id()));

		m_undo.add(modification);

		m_data.change_terrain_values(data, mask, size, vertex,
			attenuation_size,
			static_cast<BrushAttenuationType>(attenuation),
			static_cast<BrushShapeType>(shape),
			static_cast<BrushEffectType>(effect),
			terrain_values);

		m_data.set_terrain_values(terrain_values);
	}

	void Editor::terrain_layer_edit(const glm::vec3 &position,
		const Uint32 index, const float strength, const float radius,
		const int brush_type)
	{
	}

/*
	void Editor::export_blend_image(const String &file_name,
		const String &type) const
	{
		SceneResources::get_codec_manager().save_image(*m_blend_image,
			file_name, type);
	}

	void Editor::export_terrain_map(const String &file_name,
		const String &type) const
	{
		ScenePageReadOnlyIntrusivePtr scene_page_read_only;
		ImageSharedPtr image;

		get_scene().get_scene_page_read_only(get_page_id(),
			scene_page_read_only);

		export_terrain(image, scene_page_read_only);

		SceneResources::get_codec_manager().save_image(*image, file_name,
			type);
	}

	void Editor::import_terrain_map(const String &file_name)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		ImageSharedPtr image;

		image = SceneResources::get_codec_manager().load_image(file_name,
			tft_l8);

		get_scene().get_scene_page_read_write(get_page_id(),
			scene_page_read_write);
#warning "Missing implementation"

		ModificationAutoPtr modification(new HeightMapModification(
			scene_page_read_write->get_terrain_height_map(), get_page_id(),
			true));

		import_terrain(image, scene_page_read_write);

//		m_undo.add(modification);

		get_scene().set_view_changed();
	}

	void Editor::set_terrain(const MaterialData &terrain_material,
		const String &image_name, const Uint16Array2 blend_image_size)
	{
		ImageSharedPtr image;
		Uint32Array3 size;

		size[0] = blend_image_size[0];
		size[1] = blend_image_size[1];
		size[2] = 1;

		m_blend_image = boost::make_shared<Image>("blend.dds", false,
			tft_rgba8, size, 0);

		m_texture.reset(new ImageTexture(m_blend_image));
		m_texture->set_wrap_r(twt_clamp);
		m_texture->set_wrap_s(twt_clamp);
		m_texture->set_wrap_t(twt_clamp);

		image = SceneResources::get_codec_manager().load_image(image_name,
			tft_l8);

		get_scene().set_terrain(image, terrain_material, m_texture);

		m_undo.clear();
	}
*/

}
