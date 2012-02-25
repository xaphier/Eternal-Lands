/****************************************************************************
 *            map.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
#include "materialeffectdescription.hpp"
#include "lightvisitor.hpp"
#include "objectvisitor.hpp"
#include "filesystem.hpp"
#include "objectvisitor.hpp"
#include "renderobjectdata.hpp"
#include "materialdescriptioncache.hpp"
#include "abstractterrainmanager.hpp"
#include "particledata.hpp"

namespace eternal_lands
{

	Map::Map(const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache,
		const MaterialDescriptionCacheSharedPtr
			&material_description_cache, const String &name):
		m_mesh_builder(mesh_builder), m_mesh_cache(mesh_cache),
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_material_description_cache(material_description_cache),
		m_name(name), m_id(0), m_dungeon(false)
	{
//		String file_name;

		m_light_tree.reset(new RStarTree());
		m_object_tree.reset(new RStarTree());

		set_ambient(glm::vec3(0.2f));
/*
		file_name = FileSystem::get_name_without_extension(name);

		file_name = String(file_name.get() + UTF8(".xml"));

		if (file_system->get_file_readable(file_name))
		{
//			m_terrains.reset(new SimpleTerrainManager(codec_manager,
//				file_system, global_vars, mesh_builder,
//				effect_cache, texture_cache, file_name));
		}
*/
	}

	Map::~Map() throw()
	{
	}

	void Map::add_object(const ObjectData &object_data)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;
		MaterialEffectDescriptionVector materials;

		get_mesh_cache()->get_mesh(object_data.get_name(), mesh,
			materials);

		object = boost::make_shared<Object>(object_data, mesh,
			materials, get_effect_cache(), get_texture_cache());

		temp = m_objects.insert(Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

		assert(temp.second);

		m_object_tree->add(object);
	}

	void Map::add_object(const ObjectData &object_data,
		const MaterialEffectDescriptionVector &materials)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;

		get_mesh_cache()->get_mesh(object_data.get_name(), mesh);

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
			instance_data.get_mesh_data_tool(),
			instance_data.get_name());

		object = boost::make_shared<Object>(instance_data, mesh,
			instance_data.get_materials(), get_effect_cache(),
			get_texture_cache(),
			instance_data.get_instanced_objects());
		temp = m_objects.insert(Uint32ObjectSharedPtrMap::value_type(
			instance_data.get_id(), object));

		assert(temp.second);

		m_object_tree->add(object);
	}

	void Map::add_object(const ObjectData &object_data,
		const StringVector &material_names)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;
		MaterialEffectDescriptionVector materials;
		Uint32 i, count;

		get_mesh_cache()->get_mesh(object_data.get_name(), mesh,
			materials);

		count = std::min(material_names.size(), materials.size());

		for (i = 0; i < count; ++i)
		{
			if (material_names[i].get().empty())
			{
				continue;
			}

			materials[i].set_material_descriptiont(
				get_material_description_cache(
					)->get_material_description(
						material_names[i]));
		}

		object = boost::make_shared<Object>(object_data, mesh,
			materials, get_effect_cache(), get_texture_cache());

		temp = m_objects.insert(Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

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

		position = found->second->get_world_transformation(
			).get_translation();

		return true;
	}

	void Map::add_light(const LightData &light_data)
	{
		std::pair<Uint32LightSharedPtrMap::iterator, bool> temp;
		LightSharedPtr light;
		

		light = boost::make_shared<Light>(light_data);

		temp = m_lights.insert(Uint32LightSharedPtrMap::value_type(
			light_data.get_id(), light));

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

	void Map::clear()
	{
		m_light_tree->clear();
		m_object_tree->clear();
		m_objects.clear();
		m_lights.clear();
		m_terrains.clear();
		m_particles.clear();
	}

	void Map::intersect(const Frustum &frustum, ObjectVisitor &visitor)
		const
	{
		Uint32 i, count;

		count = m_terrains.size();

		for (i = 0; i < count; ++i)
		{
			m_terrains[i].intersect(frustum, visitor);
		}

		m_object_tree->intersect(frustum, visitor);
	}

	void Map::intersect(const Frustum &frustum, LightVisitor &visitor)
		const
	{
		m_light_tree->intersect(frustum, visitor);
	}

	const BoundingBox &Map::get_bounding_box() const
	{
		return m_object_tree->get_bounding_box();
	}

	void Map::add_terrain(AbstractTerrainManagerAutoPtr &terrain)
	{
		m_terrains.push_back(terrain);
	}

	void Map::add_particle(const ParticleData &particle)
	{
		m_particles.push_back(particle);
	}

}
