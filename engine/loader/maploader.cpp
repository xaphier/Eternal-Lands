/****************************************************************************
 *            maploader.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "maploader.hpp"
#include "instancedata.hpp"
#include "objectdescription.hpp"
#include "filesystem.hpp"
#include "particledata.hpp"
#include "reader.hpp"
#include "logging.hpp"
#include "map.hpp"
#include "materialdescription.hpp"
#include "instancesbuilder.hpp"
#include "materialdescriptioncache.hpp"
#include "globalvars.hpp"
#include "lightdata.hpp"
#include "exceptions.hpp"
#include "freeidsmanager.hpp"
#include "glmutil.hpp"
#include "simplexnoise.hpp"

namespace eternal_lands
{

	MapLoader::MapLoader(const CodecManagerWeakPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const EffectCacheWeakPtr &effect_cache,
		const MeshBuilderWeakPtr &mesh_builder,
		const MeshCacheWeakPtr &mesh_cache,
		const MeshDataCacheWeakPtr &mesh_data_cache,
		const MaterialCacheWeakPtr &material_cache,
		const MaterialDescriptionCacheWeakPtr
			&material_description_cache,
		const TerrainBuilderWeakPtr &terrain_builder,
		const TextureCacheWeakPtr &texture_cache,
		const FreeIdsManagerSharedPtr &free_ids):
		AbstractMapLoader(file_system, free_ids),
		m_codec_manager(codec_manager), m_global_vars(global_vars),
		m_effect_cache(effect_cache), m_mesh_builder(mesh_builder),
		m_mesh_cache(mesh_cache), m_mesh_data_cache(mesh_data_cache),
		m_material_cache(material_cache),
		m_material_description_cache(material_description_cache),
		m_terrain_builder(terrain_builder),
		m_texture_cache(texture_cache)
	{
		assert(!m_codec_manager.expired());
		assert(m_global_vars.get() != nullptr);
		assert(!m_effect_cache.expired());
		assert(!m_mesh_builder.expired());
		assert(!m_mesh_cache.expired());
		assert(!m_mesh_data_cache.expired());
		assert(!m_material_cache.expired());
		assert(!m_material_description_cache.expired());
		assert(!m_terrain_builder.expired());
		assert(!m_texture_cache.expired());
	}

	MapLoader::~MapLoader() noexcept
	{
	}

	MapSharedPtr MapLoader::load(const String &name)
	{
		m_instances_builder.reset(new InstancesBuilder(
			get_effect_cache(), get_mesh_data_cache(),
			get_material_description_cache(), 16.0f,
			get_global_vars()->get_use_simd(),
			get_global_vars()->get_opengl_3_2()));

		m_map = boost::make_shared<Map>(get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_mesh_builder(), get_mesh_cache(),
			get_material_cache(), get_terrain_builder(),
			get_texture_cache());

		m_map->set_name(name);

		read(name);

		return m_map;
	}

	void MapLoader::instance()
	{
		InstanceDataVector instances;
		ObjectDescriptionVector uninstanced;

		m_instances_builder->build(
			get_free_ids().get_free_object_ids(it_3d_object),
			instances, uninstanced);

		BOOST_FOREACH(const InstanceData &instance_data, instances)
		{
			m_map->add_object(instance_data);
		}

		BOOST_FOREACH(const ObjectDescription &object_descritption,
			uninstanced)
		{
			m_map->add_object(object_descritption);
		}
	}

	void MapLoader::add_object(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const glm::vec3 &scale,
		const String &name, const float transparency, const Uint32 id,
		const SelectionType selection, const BlendType blend,
		const bool walkable, const StringVector &material_names)
	{
		if (blend != bt_disabled)
		{
			m_map->add_object(get_object_description(translation,
				rotation_angles, scale, material_names,
				name, id, selection, blend));
		}
		else
		{
			m_instances_builder->add(get_object_description(
				translation, rotation_angles, scale,
				material_names, name, id, selection, blend));
		}
	}

	void MapLoader::add_light(const glm::vec3 &position,
		const glm::vec3 &color, const float radius, const Uint32 id)
	{
		m_map->add_light(LightData(position, color, radius, id));
	}

	void MapLoader::add_particle(const glm::vec3 &position,
		const String &name, const Uint32 id)
	{
		m_map->add_particle(ParticleData(position, name, id));
	}

	void MapLoader::add_decal(const glm::vec2 &position,
		const glm::vec2 &scale, const float rotation,
		const String &texture, const Uint32 id)
	{
		glm::mat4x4 matrix;

		matrix = glm::scale(glm::vec3(scale, 1.0f));
		matrix = glm::rotate(matrix, rotation,
			glm::vec3(0.0f, 0.0f, 1.0f));
		matrix = glm::translate(matrix, glm::vec3(position, 0.0f));

//		m_map->add_decal(DecalDescription(matrix, texture));
	}

	void MapLoader::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
		StringVector materials;
		StringStream str;
		String file_name;
		Transformation transformation;
		glm::vec3 offset;
		Uint32 id;

		m_map->set_tile(x, y, 255);

		if (tile == 255)
		{
			return;
		}

		offset.x = x * get_tile_size();
		offset.y = y * get_tile_size();

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
		transformation.set_scale(glm::vec3(1.0f));

		switch (tile)
		{
			case 0:
				str << UTF8("water");
				break;
			case 240:
				str << UTF8("lava");
				break;
			default:
				str << UTF8("tile") <<
					static_cast<Uint16>(tile);
				break;
		}

		materials.push_back(String(str.str()));

		id = get_free_ids().use_typeless_object_id(x + (y << 10),
			it_tile_object);

		m_instances_builder->add(ObjectDescription(transformation,
			materials, String(UTF8("tile")), 0.0f, id, st_none,
			bt_disabled));

		m_map->set_tile(x, y, tile);
	}

	void MapLoader::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		m_map->set_height(x, y, height);
	}

	void MapLoader::set_ambient(const glm::vec3 &ambient)
	{
		m_map->set_ambient(ambient);
	}

	void MapLoader::set_height_map_size(const Uint16 width,
		const Uint16 height)
	{
		m_map->set_height_map_size(width, height);
	}

	void MapLoader::set_tile_map_size(const Uint16 width,
		const Uint16 height)
	{
		m_map->set_tile_map_size(width, height);
	}

	void MapLoader::set_dungeon(const bool dungeon)
	{
		m_map->set_dungeon(dungeon);
	}

}
