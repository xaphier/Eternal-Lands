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
#include "scene.hpp"

namespace eternal_lands
{

	EditorMapData::EditorMapData()
	{
	}

	EditorMapData::~EditorMapData() throw()
	{
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

		if (id == m_id)
		{
			m_selected = false;

			return true;
		}

		return false;
	}

	bool EditorMapData::remove_light(const Uint32 id)
	{
		m_lights.erase(id);
		m_scene->remove_light(id);

		if (id == m_id)
		{
			m_selected = false;

			return true;
		}

		return false;
	}

	bool EditorMapData::remove_particle(const Uint32 id)
	{
		m_particles.erase(id);
//		m_scene->remove_particle(id);

		if (id == m_id)
		{
			m_selected = false;

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

	void EditorMapData::set_ambient_color(const glm::vec3 &color)
	{
	}

	void EditorMapData::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
		m_tiles[x][y] = tile;
	}

	Uint16 EditorMapData::get_tile(const Uint16 x, const Uint16 y) const
	{
		return m_tiles[x][y];
	}

	void EditorMapData::set_heights(const HeightVector &heights,
		const Uint16 id)
	{
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

	const String &EditorMapData::get_terrain_albedo_map(const Uint16 index,
		const Uint16 id) const;
	const String &EditorMapData::get_terrain_blend_map(const Uint16 id)
		const;
	const String &EditorMapData::get_terrain_height_map(const Uint16 id)
		const;
	const String &EditorMapData::get_terrain_dudv_map(const Uint16 id)
		const;
	Uint32 EditorMapData::get_free_object_id() const;
	Uint32 EditorMapData::get_free_light_id() const;

}
