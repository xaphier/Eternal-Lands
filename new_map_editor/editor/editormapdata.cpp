/****************************************************************************
 *            editormapdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editormapdata.hpp"
#include "lightdata.hpp"
#include "objectdata.hpp"
#include "exceptions.hpp"
#include "particledata.hpp"
#include "image.hpp"
#include "editorscene.hpp"
#include "freeidsmanager.hpp"
#include "materialdescription.hpp"
#include "materialdescriptioncache.hpp"
#include "meshdatacache.hpp"
#include "abstractmaploader.hpp"

namespace eternal_lands
{

	EditorMapData::EditorMapData(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system)
	{
		m_scene.reset(new EditorScene(global_vars, file_system));
	}

	EditorMapData::~EditorMapData() throw()
	{
	}

	void EditorMapData::set_draw_lights(const bool draw_lights)
	{
		m_scene->set_draw_lights(draw_lights);
	}

	void EditorMapData::set_draw_light_spheres(
		const bool draw_light_spheres)
	{
		m_scene->set_draw_light_spheres(draw_light_spheres);
	}

	void EditorMapData::init()
	{
		m_scene->init();
	}

	void EditorMapData::set_view_matrix(const glm::mat4 &view_matrix)
	{
		m_scene->set_view_matrix(view_matrix);
	}

	void EditorMapData::set_perspective(const float fov, const float aspect,
		const float z_near, const float z_far)
	{
		m_scene->set_perspective(fov, aspect, z_near, z_far);
	}

	void EditorMapData::set_view_port(const glm::uvec4 &view_port)
	{
		m_scene->set_view_port(view_port);
	}

	const glm::mat4 &EditorMapData::get_projection_matrix() const
	{
		return m_scene->get_projection_matrix();
	}

	void EditorMapData::set_ambient(const glm::vec3 &color)
	{
		m_scene->set_ambient(color);
	}

	const glm::vec3 &EditorMapData::get_ambient() const
	{
		return m_scene->get_ambient();
	}

	void EditorMapData::add_object(const EditorObjectData &object)
	{
		m_objects.insert(std::pair<Uint32,
			EditorObjectData>(object.get_id(), object));
		m_scene->add_object(object, object.get_material_names());
	}

	void EditorMapData::add_light(const LightData &light)
	{
		m_lights.insert(std::pair<Uint32,
			LightData>(light.get_id(), light));
		m_scene->add_light(light);
	}

	void EditorMapData::add_particle(const ParticleData &particle)
	{
		m_particles.insert(std::pair<Uint32,
			ParticleData>(particle.get_id(), particle));
//		m_scene->add_particle(particle);
	}

	void EditorMapData::modify_object(const EditorObjectData &object)
	{
		m_objects.erase(object.get_id());
		m_scene->remove_object(object.get_id());

		m_objects.insert(std::pair<Uint32,
			EditorObjectData>(object.get_id(), object));
		m_scene->add_object(object, object.get_material_names());
	}

	void EditorMapData::modify_light(const LightData &light)
	{
		m_lights.erase(light.get_id());
		m_scene->remove_light(light.get_id());

		m_lights.insert(std::pair<Uint32,
			LightData>(light.get_id(), light));
		m_scene->add_light(light);
	}

	void EditorMapData::modify_particle(const ParticleData &particle)
	{
		m_particles.erase(particle.get_id());
//		m_scene->remove_particle(particle.get_id());

		m_particles.insert(std::pair<Uint32,
			ParticleData>(particle.get_id(), particle));
//		m_scene->add_particle(particle);
	}

	bool EditorMapData::remove_object(const Uint32 id)
	{
		m_objects.erase(id);
		m_scene->remove_object(id);

		if ((id == m_id) && (m_renderable == rt_object))
		{
			m_renderable = rt_none;

			return true;
		}

		return false;
	}

	bool EditorMapData::remove_light(const Uint32 id)
	{
		m_lights.erase(id);
		m_scene->remove_light(id);

		if ((id == m_id) && (m_renderable == rt_light))
		{
			m_renderable = rt_none;

			return true;
		}

		return false;
	}

	bool EditorMapData::remove_particle(const Uint32 id)
	{
		m_particles.erase(id);
//		m_scene->remove_particle(id);

		if ((id == m_id) && (m_renderable == rt_particle))
		{
			m_renderable = rt_none;

			return true;
		}

		return false;
	}

	bool EditorMapData::get_object(const Uint32 id,
		EditorObjectData &object) const
	{
		std::map<Uint32, EditorObjectData>::const_iterator found;

		found = m_objects.find(id);

		if (found == m_objects.end())
		{
			return false;
		}

		object = found->second;
		return true;
	}

	bool EditorMapData::get_light(const Uint32 id,
		LightData &light) const
	{
		std::map<Uint32, LightData>::const_iterator found;

		found = m_lights.find(id);

		if (found == m_lights.end())
		{
			return false;
		}

		light = found->second;
		return true;
	}

	bool EditorMapData::get_particle(const Uint32 id,
		ParticleData &particle) const
	{
		std::map<Uint32, ParticleData>::const_iterator found;

		found = m_particles.find(id);

		if (found == m_particles.end())
		{
			return false;
		}

		particle = found->second;
		return true;
	}

	void EditorMapData::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
		MaterialDescriptionVector materials;
		StringStream str;
		String file_name;
		Transformation transformation;
		glm::vec3 offset;
		Uint32 id;

		RANGE_CECK(x, m_tile_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK(y, m_tile_map.shape()[1],
			UTF8("index value too big"));

		id = m_scene->get_free_ids()->get_object_id(x + (y << 10),
			it_tile_object);

		m_scene->remove_object(id);

		m_tile_map[x][y] = tile;

		if (tile == 255)
		{
			return;
		}

		m_scene->get_free_ids()->use_object_id(id);

		offset.x = x * AbstractMapLoader::get_tile_size();
		offset.y = y * AbstractMapLoader::get_tile_size();

		if ((tile == 0) || (tile > 230))
		{
			offset.z = -0.2501f;
		}
		else
		{
			offset.z = -0.0011f;
		}

		assert(glm::all(glm::lessThanEqual(glm::abs(offset),
			glm::vec3(1e7f))));

		transformation.set_translation(offset);
		transformation.set_scale(3.0f);

		if ((tile != 0) && (tile != 240))
		{
			str << UTF8("tile") << static_cast<Uint16>(tile);
		}
		else
		{
			if (tile == 240)
			{
				str << UTF8("lava");
			}
			else
			{
				str << UTF8("water");
			}
		}

		materials.push_back(m_scene->get_scene_resources(
			).get_material_description_cache(
			)->get_material_description(String(str.str())));

		m_scene->add_object(ObjectData(transformation,
			String(UTF8("plane_4")), 0.0f, id, st_none,
			bt_disabled), materials);
	}

	Uint16 EditorMapData::get_tile(const Uint16 x, const Uint16 y) const
	{
		RANGE_CECK(x, m_tile_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK(y, m_tile_map.shape()[1],
			UTF8("index value too big"));

		return m_tile_map[x][y];
	}

	Uint16Array2 EditorMapData::get_tile_offset(const glm::vec2 &point)
		const
	{
		Uint16Array2 result;

		result[0] = std::max(0.0f, point.x / 3);
		result[1] = std::max(0.0f, point.y / 3);

		return result;
	}

	void EditorMapData::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		RANGE_CECK(x, m_height_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK(y, m_height_map.shape()[1],
			UTF8("index value too big"));

		m_height_map[x][y] = height;
	}

	void EditorMapData::set_heights(const HeightVector &heights)
	{
		BOOST_FOREACH(const Height &height, heights)
		{
			set_height(height.get_x(), height.get_y(),
				height.get_value());
		}
	}

	void EditorMapData::set_terrain_heights(const HeightVector &heights,
		const Uint16 id)
	{
		glm::uvec4 data;

		BOOST_FOREACH(const Height &height, heights)
		{
			data = m_terrain_heights->get_pixel_uint(height.get_x(),
				height.get_y(), 0, 0, 0);

			data.x = height.get_value();

			m_terrain_heights->set_pixel_uint(height.get_x(),
				height.get_y(), 0, 0, 0, data);
		}
	}

	void EditorMapData::set_terrain_albedo_map(const String &name,
		const Uint16 index, const Uint16 id)
	{
	}

	void EditorMapData::set_terrain_blend_map(const String &name,
		const Uint16 id)
	{
	}

	void EditorMapData::set_terrain_height_map(const String &name,
		const Uint16 id)
	{
	}

	void EditorMapData::set_terrain_dudv_map(const String &name,
		const Uint16 id)
	{
	}

	String tmp;

	const String &EditorMapData::get_terrain_albedo_map(const Uint16 index,
		const Uint16 id) const
	{
		return tmp;
	}

	const String &EditorMapData::get_terrain_blend_map(const Uint16 id)
		const
	{
		return tmp;
	}

	const String &EditorMapData::get_terrain_height_map(const Uint16 id)
		const
	{
		return tmp;
	}

	const String &EditorMapData::get_terrain_dudv_map(const Uint16 id)
		const
	{
		return tmp;
	}

	Uint32 EditorMapData::get_free_object_id() const
	{
		return m_scene->get_free_ids()->get_next_free_object_id(
			it_3d_object);
	}

	Uint32 EditorMapData::get_free_light_id() const
	{
		return m_scene->get_free_ids()->get_next_free_light_id();
	}

	void EditorMapData::load_map(const String &name)
	{
		m_scene->load_map(name, *this);
	}

	void EditorMapData::draw()
	{
		m_scene->set_main_light_ambient(glm::vec3(0.2f));
		m_scene->set_main_light_color(glm::vec3(0.8f));
		m_scene->set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		m_scene->set_night(false);
		m_scene->cull();
		m_scene->draw();
	}

	void EditorMapData::select(const Uint16Array2 &position,
		const Uint16Array2 &half_size)
	{
		Uint32 typeless_id;
		SelectionType selection;
		IdType type;
		bool selected;

		m_id = m_scene->pick(glm::vec2(position[0], position[1]),
			glm::vec2(half_size[0], half_size[1]), selection);

		selected = FreeIdsManager::get_id_type(m_id, typeless_id, type);
		m_renderable = rt_none;

		if (!selected)
		{
			return;
		}

		switch (type)
		{
			case it_3d_object:
			case it_2d_object:
				m_renderable = rt_object;
				break;
			case it_tile_object:
			case it_water_object:
			case it_dynamic_object:
				break;
			case it_light_object:
				m_id = typeless_id;
				m_renderable = rt_light;
				break;
			case it_particle_object:
				m_id = typeless_id;
				m_renderable = rt_particle;
				break;
			default:
				break;
		}
	}

	StringVector EditorMapData::get_materials() const
	{
		return m_scene->get_scene_resources(
			).get_material_description_cache(
			)->get_material_description_names();
	}

	StringVector EditorMapData::get_default_materials(const String &name)
		const
	{
		MaterialDescriptionVector materials;
		StringVector result;

		materials = m_scene->get_scene_resources(
			).get_mesh_data_cache()->get_mesh_materials(name);

		BOOST_FOREACH(const MaterialDescription &material, materials)
		{
			result.push_back(material.get_name());
		}

		return result;
	}

}
