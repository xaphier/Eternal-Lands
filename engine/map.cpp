/****************************************************************************
 *            map.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "map.hpp"
#include "light.hpp"
#include "object.hpp"
#include "exceptions.hpp"
#include "rstartree.hpp"
#include "meshcache.hpp"
#include "instancedata.hpp"
#include "objectdescription.hpp"
#include "meshbuilder.hpp"
#include "materialdescription.hpp"

namespace eternal_lands
{

	Map::Map(const MeshBuilderSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache):
		m_mesh_builder(mesh_builder), m_mesh_cache(mesh_cache),
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_name(UTF8("")), m_id(0), m_dungeon(false)
	{
		m_light_tree.reset(new RStarTree());
		m_object_tree.reset(new RStarTree());

		set_ambient(glm::vec3(0.2f));
	}

	Map::~Map() throw()
	{
	}

	void Map::add_object(const ObjectData &object_data)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;
		MaterialDescriptionVector materials;

		get_mesh_cache()->get_mesh(object_data.get_name(), mesh,
			materials);

		object = boost::make_shared<Object>(object_data, mesh,
			materials, get_effect_cache(), get_texture_cache());

		temp = m_objects.insert(Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

		assert(temp.second);

		m_object_tree->add(object);
	}

	void Map::add_object(const InstanceData &instance_data)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;

		mesh = get_mesh_builder()->get_mesh(vft_instanced_mesh,
			instance_data.get_mesh_data_tool());

		object = boost::make_shared<Object>(instance_data, mesh,
			instance_data.get_materials(), get_effect_cache(),
			get_texture_cache(),
			instance_data.get_instanced_objects());
		temp = m_objects.insert(Uint32ObjectSharedPtrMap::value_type(
			instance_data.get_id(), object));

		assert(temp.second);

		m_object_tree->add(object);
	}

	void Map::remove_object(const Uint32 id)
	{
		Uint32ObjectSharedPtrMap::iterator found;

		found = m_objects.find(id);

		if (found != m_objects.end())
		{
			m_object_tree->remove(found->second);

			m_objects.erase(found);
		}
	}

	bool Map::get_object_position(const Uint32 id, glm::vec3 &position)
	{
		Uint32ObjectSharedPtrMap::iterator found;

		found = m_objects.find(id);

		if (found == m_objects.end())
		{
			return false;
		}

		position = found->second->get_world_matrix()[3];

		return true;
	}

	void Map::add_light(const glm::vec3 &position, const glm::vec3 &color,
		const float ambient, const float radius, const Uint32 id)
	{
		std::pair<Uint32LightSharedPtrMap::iterator, bool> temp;
		LightSharedPtr light;

		light = boost::make_shared<Light>(position, color, ambient,
			radius, id);
		assert(light->get_radius() == radius);
		temp = m_lights.insert(Uint32LightSharedPtrMap::value_type(id,
			light));

		assert(temp.second);

		m_light_tree->add(light);
	}

	void Map::remove_light(const Uint32 id)
	{
		Uint32LightSharedPtrMap::iterator found;

		found = m_lights.find(id);

		if (found != m_lights.end())
		{
			m_light_tree->remove(found->second);

			m_lights.erase(found);
		}
	}

	void Map::load(const String &name, const glm::vec3 &ambient,
		const bool dungeon)
	{
		m_name = name;

		set_dungeon(dungeon);
		set_ambient(ambient);

		clear();
	}

	void Map::clear()
	{
		m_light_tree->clear();
		m_object_tree->clear();
		m_objects.clear();
		m_lights.clear();
	}

}