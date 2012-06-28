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
#include "clipmapdata.hpp"
#include "materialbuilder.hpp"
#include "materialdescription.hpp"
#include "texturecache.hpp"
#include "material.hpp"
#include "effect.hpp"

namespace eternal_lands
{

	Map::Map(const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const MaterialCacheSharedPtr &material_cache,
		const MaterialBuilderWeakPtr &material_builder,
		const TextureCacheWeakPtr &texture_cache,
		const String &name): m_mesh_builder(mesh_builder),
		m_mesh_cache(mesh_cache), m_material_cache(material_cache),
		m_material_builder(material_builder),
		m_texture_cache(texture_cache), m_name(name), m_id(0),
		m_dungeon(false)
	{
		ImageCompressionTypeSet compressions;
		ImageSharedPtr vector_map, normal_map, dudv_map;
		MaterialSharedPtrVector materials;
		String file_name, vector_map_name, normal_map_name;
		String dudv_map_name;

		m_light_tree.reset(new RStarTree());
		m_object_tree.reset(new RStarTree());

		set_ambient(glm::vec3(0.2f));

		init_terrain(global_vars, mesh_builder, mesh_cache,	
			material_cache);

		file_name = FileSystem::get_name_without_extension(name);

		{
			ClipmapData clipmap_data;
			StringArray16 albedo_maps;
			StringArray4 blend_maps;

			albedo_maps[0] = String(UTF8("3dobjects/tile1.dds"));
			albedo_maps[1] = String(UTF8("3dobjects/tile2.dds"));
			albedo_maps[2] = String(UTF8("textures/tile3.dds"));
			albedo_maps[3] = String(UTF8("3dobjects/tile3.dds"));

			blend_maps[0] = String(UTF8("textures/blend0.dds"));

			clipmap_data.set_albedo_maps(albedo_maps);
			clipmap_data.set_blend_maps(blend_maps);

			build_clipmap_material(clipmap_data);
		}

		vector_map_name = String(file_name.get() + UTF8("_vector.png"));
		normal_map_name = String(file_name.get() + UTF8("_normal.dds"));
		dudv_map_name = String(file_name.get() + UTF8("_dudv.dds"));

		if (!file_system->get_file_readable(vector_map_name) ||
			!file_system->get_file_readable(normal_map_name) ||
			!file_system->get_file_readable(dudv_map_name))
		{
			return;
		}

		if (global_vars->get_opengl_3_0())
		{
			compressions.insert(ict_rgtc);
		}

		vector_map = codec_manager->load_image(vector_map_name,
			file_system, compressions, true, false);

		normal_map = codec_manager->load_image(normal_map_name,
			file_system, compressions, true, false);

		dudv_map = codec_manager->load_image(dudv_map_name,
			file_system, compressions, true, false);

		init_walk_height_map(vector_map->decompress(false, true));

		set_terrain(vector_map, normal_map, dudv_map);
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
		m_terrain_manager->clear();
		m_objects.clear();
		m_lights.clear();
		m_particles.clear();
	}

	void Map::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, BoundingBox &bounding_box) const
	{
		m_terrain_manager->intersect(frustum, camera, bounding_box);
	}

	void Map::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, TerrainVisitor &terrain) const
	{
		m_terrain_manager->intersect(frustum, camera, terrain);
	}

	void Map::intersect(const Frustum &frustum, ObjectVisitor &visitor)
		const
	{
		m_terrain_manager->intersect(frustum, visitor);
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
		bounding_box.merge(m_terrain_manager->get_bounding_box());

		return bounding_box;
	}

	void Map::add_particle(const ParticleData &particle)
	{
		m_particles.push_back(particle);
	}

	glm::vec4 Map::get_terrain_size_data() const
	{
		return m_terrain_manager->get_terrain_size_data();
	}

	glm::vec2 Map::get_terrain_size() const
	{
		return m_terrain_manager->get_terrain_size();
	}

	void Map::set_clipmap_texture(const TextureSharedPtr &texture)
	{
		m_terrain_manager->set_clipmap_texture(texture);
	}

	void Map::init_terrain(const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const MaterialCacheSharedPtr &material_cache)
	{
		if (global_vars->get_opengl_3_2())
		{
			m_terrain_manager.reset(new CdLodTerrainManager(
				mesh_cache, get_material_cache()->get_material(
					String(UTF8("cdlod-terrain")))));

			return;
		}

		m_terrain_manager.reset(new SimpleTerrainManager(global_vars,
			mesh_builder, get_material_cache()->get_material(
				String(UTF8("simple-terrain")))));
	}

	void Map::set_terrain(const ImageSharedPtr &vector_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map)
	{
		m_terrain_manager->update(vector_map, normal_map, dudv_map);
	}

	bool Map::get_terrain() const
	{
		return !m_terrain_manager->get_empty();
	}

	void Map::build_clipmap_material(const ClipmapData &clipmap_data)
	{
/*		if (global_vars->get_opengl_3_0())
		{
			build_clipmap_material_with_texture_arrays(
				clipmap_data);

			return;
		}
*/
		build_clipmap_material_simple(clipmap_data);
	}

	void Map::build_clipmap_material_simple(
		const ClipmapData &clipmap_data)
	{
		MaterialDescription material;

		material.set_name(String(UTF8("clipmap")));
		material.set_texture(clipmap_data.get_albedo_map(0),
			spt_albedo_0);
		material.set_texture(clipmap_data.get_albedo_map(1),
			spt_albedo_1);
		material.set_texture(clipmap_data.get_albedo_map(2),
			spt_albedo_2);
		material.set_texture(clipmap_data.get_albedo_map(3),
			spt_albedo_3);
		material.set_texture(clipmap_data.get_blend_map(0),
			spt_blend);
		material.set_blend_sizes(clipmap_data.get_blend_sizes());
		material.set_effect(
			String(UTF8("clipmap-simple-blend-4-screen-quad")));

		m_clipmap_material = get_material_builder()->get_material(
			material);
	}

	void Map::build_clipmap_material_with_texture_arrays(
		const ClipmapData &clipmap_data)
	{
		StringVector albedo_names, normal_names, specular_names;
		StringVector blend_names;
		MaterialDescription material;
		TextureSharedPtr texture;
		String effect;
		Uint16 i, count;

		material.set_name(String(UTF8("clipmap")));

		count = clipmap_data.get_blend_count() * 4;
		count = std::min(count, clipmap_data.get_albedo_count());

		if (count <= 4)
		{
			effect = String(UTF8("clipmap-blend-4-screen-quad"));
		}

		if (count <= 8)
		{
			effect = String(UTF8("clipmap-blend-8-screen-quad"));
		}

		if (count <= 12)
		{
			effect = String(UTF8("clipmap-blend-12-screen-quad"));
		}

		if (count <= 16)
		{
			effect = String(UTF8("clipmap-blend-16-screen-quad"));
		}

		material.set_effect(effect);

		m_clipmap_material = get_material_builder()->get_material(
			material);

		for (i = 0; i < count; ++i)
		{
			albedo_names.push_back(clipmap_data.get_albedo_map(i));
			normal_names.push_back(clipmap_data.get_normal_map(i));
			specular_names.push_back(
				clipmap_data.get_specular_map(i));
		}

		for (i = 0; i < (count / 4); ++i)
		{
			blend_names.push_back(clipmap_data.get_blend_map(i));
		}

		texture = get_texture_cache()->get_texture_array(albedo_names,
			String(UTF8("albedo clipmap")));

		m_clipmap_material->set_texture(texture, spt_albedo_0);
		m_clipmap_material->set_blend_sizes(clipmap_data.get_blend_sizes());

/*
		texture = get_texture_cache()->get_texture_array(normal_names,
			String(UTF8("normal clipmap")));

		m_clipmap_material->set_texture(texture, spt_normal);

		texture = get_texture_cache()->get_texture_array(specular_names,
			String(UTF8("specular clipmap")));

		m_clipmap_material->set_texture(texture, spt_specular);
*/
		texture = get_texture_cache()->get_texture_array(blend_names,
			String(UTF8("blend clipmap")));

		m_clipmap_material->set_texture(texture, spt_blend);
	}

}
