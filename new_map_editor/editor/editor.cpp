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
#include "undo/terraintexturemodification.hpp"
#include "undo/groundtilemodification.hpp"
#include "undo/terrainheightscalemodification.hpp"
#include "sceneresources.hpp"
#include "texturemanager.hpp"
#include "codec/codecmanager.hpp"
#include "image.hpp"
#include "file/abstractfile.hpp"

namespace eternal_lands
{

	Editor::Editor(): m_undo(100), m_uni_dist(0.0f, 1.0f), m_random(m_gen,
		m_uni_dist)
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
		m_page_id = 0;
	}

	bool Editor::undo()
	{
		return m_undo.undo(*this);
	}

	void Editor::change_object(const ModificationType type, const GlobalId id,
		const MeshObjectData &mesh_object_data)
	{
		ModificationAutoPtr modification(new ObjectModification(
			mesh_object_data, id, type));

		m_undo.add(modification);

		get_scene().set_view_changed();
	}

	void Editor::change_light(const ModificationType type, const GlobalId id,
		const LightData &light_data)
	{
		ModificationAutoPtr modification(new LightModification(light_data,
			id, type));

		m_undo.add(modification);

		get_scene().set_view_changed();
	}

	bool Editor::do_set_terrain_diffuse_texture(const GlobalId id,
		const String &str, const Uint16 index)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MaterialSharedPtrVector materials;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		materials = scene_page_read_write->get_object_materials(id.get_id());

		if (materials[0]->get_diffuse_texture(index) != str)
		{
			materials[0] = boost::make_shared<Material>(
				materials[0]->get_name(), materials[0]->get_material_data());

			materials[0]->set_diffuse_texture(str, index);
			scene_page_read_write->set_object_materials(id.get_id(),
				materials);

			return true;
		}
		else
		{
			return false;
		}
	}

	void Editor::set_terrain_diffuse_texture(const GlobalId id,
		const String &texture, const Uint16 index)
	{
		MaterialSharedPtr material;
		String str;

		{
			ScenePageReadOnlyIntrusivePtr scene_page_read_only;

			get_scene().get_scene_page_read_only(id.get_page_id(),
				scene_page_read_only);

			material = scene_page_read_only->get_object_materials(
				id.get_id())[0];
		}

		str = material->get_diffuse_texture(index);

		if (do_set_terrain_diffuse_texture(id, texture, index))
		{
			ModificationAutoPtr modification(new TerrainTextureModification(
				str, id, index));

			m_undo.add(modification);

			get_scene().set_view_changed();
		}
	}

	bool Editor::do_set_terrain_normal_texture(const GlobalId id,
		const String &str, const Uint16 index)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MaterialSharedPtrVector materials;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		materials = scene_page_read_write->get_object_materials(id.get_id());

		if (materials[0]->get_normal_texture(index) != str)
		{
			materials[0] = boost::make_shared<Material>(
				materials[0]->get_name(), materials[0]->get_material_data());

			materials[0]->set_diffuse_texture(str, index);
			scene_page_read_write->set_object_materials(id.get_id(),
				materials);

			return true;
		}
		else
		{
			return false;
		}
	}

	void Editor::set_terrain_normal_texture(const GlobalId id,
		const String &texture, const Uint16 index)
	{
		MaterialSharedPtr material;
		String str;

		{
			ScenePageReadOnlyIntrusivePtr scene_page_read_only;

			get_scene().get_scene_page_read_only(id.get_page_id(),
				scene_page_read_only);

			material = scene_page_read_only->get_object_materials(
				id.get_id())[0];
		}

		str = material->get_normal_texture(index);

		if (do_set_terrain_normal_texture(id, texture, index))
		{
			ModificationAutoPtr modification(new TerrainTextureModification(
				str, id, index));

			m_undo.add(modification);

			get_scene().set_view_changed();
		}
	}

	void Editor::set_ground_tile_texture(const glm::vec2 &point,
		const Uint8 index)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		Uint32Array2 offset;
		Uint8 material;

		material = index;

		get_scene().get_scene_page_read_write(get_page_id(),
			scene_page_read_write);

		offset = scene_page_read_write->get_ground_tile_offset(point);

		scene_page_read_write->modify_ground_tile(offset, material);

		if (material != index)
		{
			ModificationAutoPtr modification(new GroundTileModification(
				get_page_id(), offset, material));

			m_undo.add(modification);

			get_scene().set_view_changed();
		}
	}

	void Editor::add_3d_object(const glm::vec3 &position,
		const glm::vec4 &color, const Uint16 type, const Uint16 server_id,
		const String &mesh)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		Selection selection;
		MeshObjectData mesh_object_data;
		glm::vec3 translation, rotation, vrandom;
		GlobalId id;
		float scale;
		Uint32 i;

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

		rotation = glm::mix(m_random_rotation_min, m_random_rotation_max,
			vrandom);

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

		mesh_object_data.set_translation(position + translation);
		mesh_object_data.set_rotation(rotation);
		mesh_object_data.set_scale(scale);
		mesh_object_data.set_color(color);
		mesh_object_data.set_type(type);
		mesh_object_data.set_server_id(server_id);

		get_scene().get_scene_page_read_write(get_page_id(),
			scene_page_read_write);

		mesh_object_data.set_mesh(mesh);

		id = scene_page_read_write->add_object(mesh_object_data);

		change_object(mt_object_added, id, mesh_object_data);

		selection.set_global_id(id);
		m_scene.set_renderable(rt_mesh);
		selection.set_valid(true);

		m_scene.set_selection(selection);
	}

	void Editor::add_light(const glm::vec3 &position)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		Selection selection;
		LightData light_data;
		GlobalId id;

		light_data.set_position(position);
		light_data.set_color(glm::vec4(1.0f));
		light_data.set_radius(5.0f);

		get_scene().get_scene_page_read_write(get_page_id(),
			scene_page_read_write);

		id = scene_page_read_write->add_light(light_data);

		change_light(mt_light_added, id, light_data);

		selection.set_global_id(id);
		m_scene.set_renderable(rt_light);
		selection.set_valid(true);

		m_scene.set_selection(selection);
	}

	void Editor::set_scene_ambient_color(const glm::vec4 &color)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;

		get_scene().get_scene_page_read_write(get_page_id(),
			scene_page_read_write);

		ModificationAutoPtr modification(new AmbientModification(
			get_page_id(), scene_page_read_write->get_ambient_color()));

		m_undo.add(modification);

		scene_page_read_write->set_ambient_color(color);

		get_scene().set_view_changed();
	}

	void Editor::terrain_edit(const Uint32 id, const Uint16Array2 vertex,
		const float strength, const float radius,
		const EditorBrushType brush_type)
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

	void Editor::ground_tile_edit(const glm::vec3 &p0, const glm::vec3 &p1,
		const Uint8 height)
	{
		glm::vec3 dir;
		glm::vec2 point;
		float distance;

		dir = p1 - p0;

		if (std::abs(dir.z) > EPSILON)
		{
			distance = -p0.z / dir.z;

			if (distance >= 0.0f)
			{
				point = glm::vec2(p0 + dir * distance);

				set_ground_tile_texture(point, height);

				get_scene().set_view_changed();
			}
		}
	}

	void Editor::water_tile_edit(const glm::vec3 &p0, const glm::vec3 &p1,
		const Uint8 water)
	{
		glm::vec3 dir;
		glm::vec2 point;
		float distance;

		dir = p1 - p0;

		if (std::abs(dir.z) > EPSILON)
		{
			distance = -p0.z / dir.z;

			if (distance >= 0.0f)
			{
				point = glm::vec2(p0 + dir * distance);

				set_ground_tile_texture(point, water);

				get_scene().set_view_changed();
			}
		}
	}

	void Editor::height_edit(const glm::vec3 &p0, const glm::vec3 &p1,
		const Uint8 height)
	{
		Ray ray;

		ray = Ray(p0, glm::normalize(p1 - p0));

		{

			get_scene().set_view_changed();
		}
	}

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

	void Editor::remove_object(const GlobalId id)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MeshObjectData mesh_object_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		mesh_object_data = scene_page_read_write->get_mesh_data(id.get_id());

		scene_page_read_write->remove_object(id.get_id());
		change_object(mt_object_removed, id, mesh_object_data);

		get_scene().set_view_changed();
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

	void Editor::load_map(const String &name)
	{
		ScenePageReadOnlyIntrusivePtr scene_page_read_only;
		ImageSharedPtr image;
		String blend_texture;
		Uint32Array3 size;

		m_scene.load_map(name);

		size[0] = 256;
		size[1] = 256;
		size[2] = 1;

		m_blend_image = boost::make_shared<Image>("blend.dds", false,
			tft_rgba8, size, 0);

		get_scene().get_scene_page_read_only(get_page_id(),
			scene_page_read_only);

		blend_texture = scene_page_read_only->get_blend_texture();

		if (!blend_texture.empty())
		{
			image = SceneResources::get_codec_manager().load_image(
				blend_texture, tft_rgba8);

			m_blend_image = boost::make_shared<Image>(*image, 0, 0);

			m_texture.reset(new ImageTexture(m_blend_image));
			m_texture->set_wrap_r(twt_clamp);
			m_texture->set_wrap_s(twt_clamp);
			m_texture->set_wrap_t(twt_clamp);

			if (SceneResources::get_texture_manager().has_texture(
				blend_texture))
			{
				SceneResources::get_texture_manager().remove_texture(
					blend_texture);
			}
			SceneResources::get_texture_manager().add_texture(m_texture);
		}

		m_undo.clear();
	}

	void Editor::set_object_blending(const GlobalId id,
		const BlendType blending)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MeshObjectData mesh_object_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		mesh_object_data = scene_page_read_write->get_object_data(
			id.get_id());

		if (mesh_object_data.get_blending() != blending)
		{
			change_object(mt_object_blending_changed, id, mesh_object_data);

			mesh_object_data.set_blending(blending);

			scene_page_read_write->modify_object(id.get_id(),
				mesh_object_data);
		}
	}

	void Editor::set_object_translation(const GlobalId id,
		const glm::vec3 &translation)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MeshObjectData mesh_object_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		mesh_object_data = scene_page_read_write->get_object_data(
			id.get_id());

		if (glm::any(glm::notEqual(mesh_object_data.get_translation(),
			translation)))
		{
			change_object(mt_object_translation_changed, id,
				mesh_object_data);

			mesh_object_data.set_translation(translation);

			scene_page_read_write->modify_object(id.get_id(),
				mesh_object_data);
		}
	}

	void Editor::set_object_rotation(const GlobalId id,
		const glm::vec3 &rotation)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MeshObjectData mesh_object_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		mesh_object_data = scene_page_read_write->get_object_data(
			id.get_id());

		if (glm::any(glm::notEqual(mesh_object_data.get_rotation(),
			rotation)))
		{
			change_object(mt_object_rotation_changed, id, mesh_object_data);

			mesh_object_data.set_rotation(rotation);

			scene_page_read_write->modify_object(id.get_id(),
				mesh_object_data);
		}
	}

	void Editor::set_object_scale(const GlobalId id, const float scale)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MeshObjectData mesh_object_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		mesh_object_data = scene_page_read_write->get_object_data(
			id.get_id());

		if (mesh_object_data.get_scale() != scale)
		{
			change_object(mt_object_scale_changed, id, mesh_object_data);

			mesh_object_data.set_scale(scale);

			scene_page_read_write->modify_object(id.get_id(),
				mesh_object_data);
		}
	}

	void Editor::set_object_color(const GlobalId id, const glm::vec4 &color)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MeshObjectData mesh_object_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		mesh_object_data = scene_page_read_write->get_object_data(
			id.get_id());

		if (glm::any(glm::notEqual(mesh_object_data.get_color(), color)))
		{
			change_object(mt_object_color_changed, id, mesh_object_data);

			mesh_object_data.set_color(color);

			scene_page_read_write->modify_object(id.get_id(),
				mesh_object_data);
		}
	}

	void Editor::set_object_type(const GlobalId id, const Uint16 type)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MeshObjectData mesh_object_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		mesh_object_data = scene_page_read_write->get_object_data(
			id.get_id());

		if (mesh_object_data.get_type() != type)
		{
			change_object(mt_object_type_changed, id, mesh_object_data);

			mesh_object_data.set_type(type);

			scene_page_read_write->modify_object(id.get_id(),
				mesh_object_data);
		}
	}

	void Editor::set_object_server_id(const GlobalId id,
		const Uint16 server_id)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		MeshObjectData mesh_object_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		mesh_object_data = scene_page_read_write->get_object_data(
			id.get_id());

		if (mesh_object_data.get_server_id() != server_id)
		{
			change_object(mt_object_server_id_changed, id, mesh_object_data);

			mesh_object_data.set_server_id(server_id);

			scene_page_read_write->modify_object(id.get_id(),
				mesh_object_data);
		}
	}

	void Editor::remove_light(const GlobalId id)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		LightData light_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		scene_page_read_write->get_light_data(id.get_id(), light_data);

		change_light(mt_light_removed, id, light_data);

		scene_page_read_write->remove_light(id.get_id());
	}

	void Editor::set_light_position(const GlobalId id,
		const glm::vec3 &position)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		LightData light_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		scene_page_read_write->get_light_data(id.get_id(), light_data);

		if (glm::any(glm::notEqual(light_data.get_position(), position)))
		{
			change_light(mt_light_position_changed, id, light_data);

			light_data.set_position(position);

			scene_page_read_write->modify_light(id.get_id(), light_data);
		}
	}

	void Editor::set_light_radius(const GlobalId id, const float radius)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		LightData light_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		scene_page_read_write->get_light_data(id.get_id(), light_data);

		if (light_data.get_radius() != radius)
		{
			change_light(mt_light_radius_changed, id, light_data);

			light_data.set_radius(radius);

			scene_page_read_write->modify_light(id.get_id(), light_data);
		}
	}

	void Editor::set_light_color(const GlobalId id, const glm::vec4 &color)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;
		LightData light_data;

		get_scene().get_scene_page_read_write(id.get_page_id(),
			scene_page_read_write);

		scene_page_read_write->get_light_data(id.get_id(), light_data);

		if (glm::any(glm::notEqual(light_data.get_color(), color)))
		{
			change_light(mt_light_color_changed, id, light_data);

			light_data.set_color(color);

			scene_page_read_write->modify_light(id.get_id(), light_data);
		}
	}

	void Editor::get_object_data(MeshObjectData &mesh_object_data) const
	{
		ScenePageReadOnlyIntrusivePtr scene_page_read_only;

		assert(get_object_selected());

		get_scene().get_scene_page_read_only(get_global_id().get_page_id(),
			scene_page_read_only);

		mesh_object_data = scene_page_read_only->get_object_data(
			get_global_id().get_id());
	}

	void Editor::get_light_data(LightData &light) const
	{
		ScenePageReadOnlyIntrusivePtr scene_page_read_only;

		assert(get_light_selected());

		get_scene().get_scene_page_read_only(get_global_id().get_page_id(),
			scene_page_read_only);
		scene_page_read_only->get_light_data(get_global_id().get_id(), light);
	}

	const glm::vec4 &Editor::get_ambient_color() const
	{
		ScenePageReadOnlyIntrusivePtr scene_page_read_only;

		get_scene().get_scene_page_read_only(get_page_id(),
			scene_page_read_only);

		return scene_page_read_only->get_ambient_color();
	}

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
/*
		ModificationAutoPtr modification(new HeightMapModification(
			scene_page_read_write->get_terrain_height_map(), get_page_id(),
			true));
*/
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

	void Editor::set_terrain_height_scale(const float terrain_height_scale)
	{
		ModificationAutoPtr modification(new TerrainHeightScaleModification(
			get_terrain_height_scale()));

		m_undo.add(modification);

		get_scene().set_terrain_height_scale(terrain_height_scale);

		get_scene().set_view_changed();
	}

	float Editor::get_terrain_height_scale() const
	{
		return get_scene().get_terrain_height_scale();
	}

}
