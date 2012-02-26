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
	}

	void EditorMapData::add_light(const LightData &light)
	{
		m_lights.insert(std::pair<Uint32,
			LightData>(light.get_id(), light));
	}

	void EditorMapData::add_particle(const ParticleData &particle)
	{
		m_particles.insert(std::pair<Uint32,
			ParticleData>(particle.get_id(), particle));
	}

	void EditorMapData::modify_object(const EditorObjectData &object)
	{
		m_objects.erase(object.get_id());
		m_objects.insert(std::pair<Uint32,
			EditorObjectData>(object.get_id(), object));
	}

	void EditorMapData::modify_light(const LightData &light)
	{
		m_lights.erase(light.get_id());
		m_lights.insert(std::pair<Uint32,
			LightData>(light.get_id(), light));
	}

	void EditorMapData::modify_particle(const ParticleData &particle)
	{
		m_particles.erase(particle.get_id());
		m_particles.insert(std::pair<Uint32,
			ParticleData>(particle.get_id(), particle));
	}

	bool EditorMapData::remove_object(const Uint32 id)
	{
		m_objects.erase(id);

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

		if (id == m_id)
		{
			m_selected = false;

			return true;
		}

		return false;
	}

	void EditorMapData::set_ambient_color(const glm::vec3 &color)
	{
	}

	void EditorMapData::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
		m_tiles[x][y] = tile;
	}

	void EditorMapData::set_heights(const HeightVector &heights)
	{
	}

	void EditorMapData::set_terrain_heights(const HeightVector &heights,
		const Uint16 index)
	{
		glm::uvec4 data;

		BOOST_FOREACH(const Height &height, heights)
		{
			data = m_terrain_heights->get_pixel_uint(height.get_x(),
				height.get_y(), 0, 0, 0);

			data[index] = height.get_value();

			m_terrain_heights->set_pixel_uint(height.get_x(),
				height.get_y(), 0, 0, 0, data);
		}
	}

	void EditorMapData::set_terrain_albedo_map(const String &name,
		const Uint16 index)
	{
	}

	void EditorMapData::set_terrain_blend_map(const String &name)
	{
	}

	void EditorMapData::set_terrain_height_map(const String &name)
	{
	}

	void EditorMapData::set_terrain_dvdu_map(const String &name)
	{
	}

}
