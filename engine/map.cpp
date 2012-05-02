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
#include "materialdescription.hpp"
#include "lightvisitor.hpp"
#include "objectvisitor.hpp"
#include "filesystem.hpp"
#include "objectvisitor.hpp"
#include "renderobjectdata.hpp"
#include "materialcache.hpp"
#include "abstractterrainmanager.hpp"
#include "particledata.hpp"
#include "terrain/simpleterrainmanager.hpp"
#include "codec/codecmanager.hpp"

namespace eternal_lands
{

	Map::Map(const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const MaterialCacheSharedPtr &material_cache,
		const String &name): m_mesh_builder(mesh_builder),
		m_mesh_cache(mesh_cache), m_material_cache(material_cache),
		m_name(name), m_id(0), m_dungeon(false)
	{
		ImageSharedPtr vector_map, normal_map, dudv_map;
		MaterialSharedPtrVector materials;
		String file_name, vector_map_name, normal_map_name;
		String dudv_map_name;

		m_light_tree.reset(new RStarTree());
		m_object_tree.reset(new RStarTree());

		set_ambient(glm::vec3(0.2f));

		file_name = FileSystem::get_name_without_extension(name);

		vector_map_name = String(file_name.get() + UTF8("_vector.dds"));
		normal_map_name = String(file_name.get() + UTF8("_normal.dds"));
		dudv_map_name = String(file_name.get() + UTF8("_dudv.dds"));

		if (file_system->get_file_readable(vector_map_name) &&
			file_system->get_file_readable(normal_map_name) &&
			file_system->get_file_readable(dudv_map_name))
		{
			vector_map = codec_manager->load_image(vector_map_name,
				file_system, ImageCompressionTypeSet(), true);

			normal_map = codec_manager->load_image(normal_map_name,
				file_system, ImageCompressionTypeSet(), true);

			dudv_map = codec_manager->load_image(dudv_map_name,
				file_system, ImageCompressionTypeSet(), true);

			materials.push_back(get_material_cache()->get_material(
				String(UTF8("terrain"))));

			m_terrain.reset(new SimpleTerrainManager(vector_map,
				normal_map, dudv_map, global_vars,
				mesh_builder, materials));

			std::cout << "terrain build" << std::endl;
		}
	}

	Map::~Map() throw()
	{
	}

	void Map::add_object(const ObjectDescription &object_description)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;
		StringVector material_names;
		MaterialSharedPtrVector materials;
		Uint32 i, count;

		get_mesh_cache()->get_mesh(object_description.get_name(), mesh,
			material_names);

		count = std::min(material_names.size(),
			object_description.get_material_names().size());

		for (i = 0; i < count; ++i)
		{
			if (object_description.get_material_names()[i].get(
				).empty())
			{
				continue;
			}

			material_names[i] =
				object_description.get_material_names()[i];
		}

		BOOST_FOREACH(const String &material_name, material_names)
		{
			materials.push_back(get_material_cache()->get_material(
				material_name));
		}

		object = boost::make_shared<Object>(object_description, mesh,
			materials);

		temp = m_objects.insert(Uint32ObjectSharedPtrMap::value_type(
			object_description.get_id(), object));

		assert(temp.second);

		m_object_tree->add(object);
	}

	void Map::add_object(const InstanceData &instance_data)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;
		MaterialSharedPtrVector materials;

		mesh = get_mesh_builder()->get_mesh(vft_instanced_mesh,
			instance_data.get_mesh_data_tool(),
			instance_data.get_name());

		BOOST_FOREACH(const String &material_name,
			instance_data.get_material_names())
		{
			materials.push_back(get_material_cache()->get_material(
				material_name));
		}

		object = boost::make_shared<Object>(instance_data, mesh,
			materials, instance_data.get_instanced_objects());
		temp = m_objects.insert(Uint32ObjectSharedPtrMap::value_type(
			instance_data.get_id(), object));

		assert(temp.second);

		m_object_tree->add(object);
	}

	void Map::add_object(const ObjectData &object_data,
		const MaterialSharedPtrVector &materials)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;

		get_mesh_cache()->get_mesh(object_data.get_name(), mesh);

		object = boost::make_shared<Object>(object_data, mesh,
			materials);

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
		m_terrain.reset();
		m_particles.clear();
	}

	void Map::intersect(const Frustum &frustum, ObjectVisitor &visitor)
		const
	{
		if (m_terrain.get() != nullptr)
		{
			m_terrain->intersect(frustum, visitor);
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

	void Map::add_terrain(const AbstractTerrainManagerSharedPtr &terrain)
	{
		m_terrain = terrain;
	}

	void Map::add_particle(const ParticleData &particle)
	{
		m_particles.push_back(particle);
	}

}
