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
#include "terrain/cdlodterrainmanager.hpp"
#include "codec/codecmanager.hpp"
#include "globalvars.hpp"
#include "image.hpp"

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
		ImageCompressionTypeSet compressions;
		ImageSharedPtr vector_map, normal_map, dudv_map;
		MaterialSharedPtrVector materials;
		String file_name, vector_map_name, normal_map_name;
		String dudv_map_name;

		m_light_tree.reset(new RStarTree());
		m_object_tree.reset(new RStarTree());

		set_ambient(glm::vec3(0.2f));

		file_name = FileSystem::get_name_without_extension(name);

		vector_map_name = String(file_name.get() + UTF8("_vector.png"));
		normal_map_name = String(file_name.get() + UTF8("_normal.dds"));
		dudv_map_name = String(file_name.get() + UTF8("_dudv.dds"));

		if (!file_system->get_file_readable(vector_map_name) ||
			!file_system->get_file_readable(normal_map_name) ||
			!file_system->get_file_readable(dudv_map_name))
		{
			return;
		}

		vector_map = codec_manager->load_image(vector_map_name,
			file_system, ImageCompressionTypeSet(), true, false);

		init_walk_height_map(vector_map);

		if (global_vars->get_opengl_3_2())
		{
			compressions.insert(ict_rgtc);

			normal_map = codec_manager->load_image(normal_map_name,
				file_system, compressions, true, false);

			dudv_map = codec_manager->load_image(dudv_map_name,
				file_system, compressions, true, false);

			m_terrain.reset(new CdLodTerrainManager(vector_map,
				normal_map, dudv_map, global_vars,
				mesh_cache, get_material_cache()->get_material(
				String(UTF8("cdlod-terrain")))));

			return;
		}

		normal_map = codec_manager->load_image(normal_map_name,
			file_system, ImageCompressionTypeSet(), true, false);

		dudv_map = codec_manager->load_image(dudv_map_name,
			file_system, ImageCompressionTypeSet(), true, false);

		m_terrain.reset(new SimpleTerrainManager(vector_map,
			normal_map, dudv_map, global_vars, mesh_builder,
			get_material_cache()->get_material(
				String(UTF8("simple-terrain")))));
	}

	Map::~Map() noexcept
	{
	}

	void Map::init_walk_height_map(const ImageSharedPtr &vector_map)
	{
		glm::uvec2 size;
		Uint32 x, y;
		float scale, z;

		size = glm::uvec2(vector_map->get_sizes());

		scale = AbstractTerrainManager::get_vector_scale().z;

		m_walk_height_map.resize(boost::extents[size.x][size.y]);

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				z = vector_map->get_pixel(x, y, 0, 0, 0).z;

				m_walk_height_map[x][y] = z * scale;
			}
		}
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

	void Map::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, BoundingBox &bounding_box) const
	{
		if (m_terrain.get() != nullptr)
		{
			m_terrain->intersect(frustum, camera, bounding_box);
		}
	}

	void Map::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, TerrainVisitor &terrain) const
	{
		if (m_terrain.get() != nullptr)
		{
			m_terrain->intersect(frustum, camera, terrain);
		}
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

	BoundingBox Map::get_bounding_box() const
	{
		BoundingBox bounding_box;

		bounding_box = m_object_tree->get_bounding_box();

		if (m_terrain.get() != nullptr)
		{
			bounding_box.merge(m_terrain->get_bounding_box());
		}

		return bounding_box;
	}

	void Map::add_terrain(const AbstractTerrainManagerSharedPtr &terrain)
	{
		m_terrain = terrain;
	}

	void Map::add_particle(const ParticleData &particle)
	{
		m_particles.push_back(particle);
	}

	glm::vec4 Map::get_terrain_size_data() const
	{
		if (m_terrain.get() != nullptr)
		{
			return m_terrain->get_terrain_size_data();
		}

		return glm::vec4(0.0f);
	}

	glm::vec2 Map::get_terrain_size() const
	{
		if (m_terrain.get() != nullptr)
		{
			return m_terrain->get_terrain_size();
		}

		return glm::vec2(0.0f);
	}

	void Map::set_clipmap_texture(const TextureSharedPtr &texture)
	{
		if (m_terrain.get() != nullptr)
		{
			m_terrain->set_clipmap_texture(texture);
		}
	}

}
