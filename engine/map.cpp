/****************************************************************************
 *            map.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "abstractterrain.hpp"
#include "particledata.hpp"
#include "terrain/simpleterrain.hpp"
#include "terrain/cdlodterrain.hpp"
#include "codec/codecmanager.hpp"
#include "globalvars.hpp"
#include "image.hpp"
#include "materialbuilder.hpp"
#include "materialdescription.hpp"
#include "texturecache.hpp"
#include "material.hpp"

#include "terrainbuilder.hpp"
#include "terrain/terrainmaterialdata.hpp"

namespace eternal_lands
{

	Map::Map(const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const MaterialCacheSharedPtr &material_cache,
		const TerrainBuilderWeakPtr &terrain_builder,
		const TextureCacheWeakPtr &texture_cache):
		m_global_vars(global_vars),
		m_mesh_builder(mesh_builder), m_mesh_cache(mesh_cache),
		m_material_cache(material_cache),
		m_terrain_builder(terrain_builder),
		m_texture_cache(texture_cache), m_id(0), m_dungeon(false),
		m_codec_manager(codec_manager), m_file_system(file_system)
	{
		m_light_tree.reset(new RStarTree());
		m_object_tree.reset(new RStarTree());

		set_ambient(glm::vec3(0.2f));

		m_terrain = get_terrain_builder()->get_terrain();
	}

	Map::~Map() noexcept
	{
	}

	void Map::load_data(const String& name)
	{
	}

	void Map::init_walk_height_map(const ImageSharedPtr &displacement_map)
	{
		glm::uvec2 size;
		Uint32 x, y;
		float scale, z;

		size = glm::uvec2(displacement_map->get_size());

		scale = AbstractTerrain::get_vector_scale().z;

		m_walk_height_map.resize(boost::extents[size.x][size.y]);

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				z = displacement_map->get_pixel(x, y, 0, 0,
					0).z;

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

		mesh = get_mesh_builder()->get_mesh(vft_mesh,
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
		m_terrain->clear();
		m_objects.clear();
		m_lights.clear();
		m_particles.clear();
	}

	void Map::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, BoundingBox &bounding_box) const
	{
		m_terrain->intersect(frustum, camera, bounding_box);
	}

	void Map::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, TerrainVisitor &terrain) const
	{
		m_terrain->intersect(frustum, camera, terrain);
	}

	void Map::intersect(const Frustum &frustum, ObjectVisitor &visitor)
		const
	{
		m_terrain->intersect(frustum, visitor);
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
		bounding_box.merge(m_terrain->get_bounding_box());

		return bounding_box;
	}

	void Map::add_particle(const ParticleData &particle)
	{
		m_particles.push_back(particle);
	}

	glm::vec4 Map::get_terrain_size_data() const
	{
		return m_terrain->get_terrain_size_data();
	}

	glm::vec2 Map::get_terrain_size() const
	{
		return m_terrain->get_terrain_size();
	}

	void Map::set_clipmap_terrain_texture(const TextureSharedPtr &texture)
	{
		m_terrain->set_clipmap_terrain_texture(texture);
	}

	void Map::set_terrain_geometry_maps(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map)
	{
		m_terrain->set_geometry_maps(displacement_map, normal_map,
			dudv_map);

		init_walk_height_map(displacement_map->decompress(false, true,
			false));
	}

	void Map::set_terrain_blend_map(const ImageSharedPtr &blend_map)
	{
		m_terrain->set_blend_map(blend_map, get_texture_cache());
	}

	void Map::update_terrain_geometry_maps(
		const ImageUpdate &displacement_map,
		const ImageUpdate &normal_map, const ImageUpdate &dudv_map)
	{
		m_terrain->update_geometry_maps(displacement_map, normal_map,
			dudv_map);
	}

	void Map::update_terrain_blend_map(const ImageUpdate &blend_map)
	{
		m_terrain->update_blend_map(blend_map);
	}

	void Map::set_terrain_material(const StringVector &albedo_maps,
		const StringVector &extra_maps,
		const TerrainMaterialData &material_data)
	{
		EffectSharedPtr effect;

		m_terrain->set_texture_maps(albedo_maps, extra_maps,
			material_data.get_use_blend_size_samplers(),
			material_data.get_use_extra_maps(),
			get_texture_cache());

		effect = get_terrain_builder()->get_effect(get_name(),
			material_data);

		m_terrain->set_effect(effect);
	}

	bool Map::get_terrain() const
	{
		return !m_terrain->get_empty();
	}

	const MaterialSharedPtr &Map::get_clipmap_terrain_material() const
	{
		return m_terrain->get_clipmap_terrain_material();
	}

	void Map::set_terrain_dudv_scale_offset(
		const glm::vec4 &dudv_scale_offset)
	{
		m_terrain->set_dudv_scale_offset(dudv_scale_offset);
	}

	const glm::vec4 &Map::get_terrain_dudv_scale_offset() const
	{
		return m_terrain->get_dudv_scale_offset();
	}

}
