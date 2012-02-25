/****************************************************************************
 *            maploader.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
#include "materialeffectdescription.hpp"
#include "instancesbuilder.hpp"
#include "materialdescriptioncache.hpp"
#include "terrain/simpleterrainmanager.hpp"
#include "globalvars.hpp"
#include "lightdata.hpp"
#include "exceptions.hpp"
#include "freeidsmanager.hpp"
#include "glmutil.hpp"
#include "simplexnoise.hpp"

namespace eternal_lands
{

	MapLoader::MapLoader(const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const MeshDataCacheSharedPtr &mesh_data_cache,
		const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache,
		const MaterialDescriptionCacheSharedPtr
			&material_description_cache,
		const FreeIdsManagerSharedPtr &free_ids):
		AbstractMapLoader(file_system, free_ids),
		m_codec_manager(codec_manager), m_global_vars(global_vars),
		m_mesh_builder(mesh_builder), m_mesh_cache(mesh_cache),
		m_mesh_data_cache(mesh_data_cache),
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_material_description_cache(material_description_cache)
	{
	}

	MapLoader::~MapLoader() throw()
	{
	}

	MapSharedPtr MapLoader::load(const String &name)
	{
		m_instances_builder.reset(new InstancesBuilder(
			get_mesh_data_cache(), 8.0f,
			get_global_vars()->get_use_simd(),
			get_global_vars()->get_opengl_3_2()));

		m_reader = get_file_system()->get_file(name);

		m_map = boost::make_shared<Map>(get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_mesh_builder(), get_mesh_cache(),
			get_effect_cache(), get_texture_cache(),
			get_material_description_cache(), name);

		read();

		return m_map;
	}

	void MapLoader::instance()
	{
		InstanceDataVector instances;
		ObjectDataVector uninstanced;

		m_instances_builder->build(
			get_free_ids().get_free_ids(it_3d_object), instances,
			uninstanced);

		BOOST_FOREACH(const InstanceData &instance_data, instances)
		{
			m_map->add_object(instance_data);
		}

		BOOST_FOREACH(const ObjectData &object_data, uninstanced)
		{
			m_map->add_object(object_data);
		}
	}

	void MapLoader::add_object(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const String &name,
		const float scale, const float transparency,
		const Uint32 id, const bool blended,
		const SelectionType selection,
		const StringVector &material_names)
	{
		if (blended)
		{
			m_map->add_object(get_object_data(translation,
				rotation_angles, name, scale, true, id,
				selection));
		}
		else
		{
			m_instances_builder->add(get_object_data(translation,
				rotation_angles, name, scale, false, id,
				selection));
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

	void MapLoader::add_terrain(const StringArray4 &albedo,
		const String &blend, const String &height, const String &dudv,
		const glm::vec3 &translation, const glm::vec3 &rotation_angles,
		const float scale)
	{
		AbstractTerrainManagerAutoPtr terrain;
		Transformation transformation;

		terrain.reset(new SimpleTerrainManager(get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_mesh_builder(), get_effect_cache(),
			get_texture_cache(), transformation, albedo, blend,
			height, dudv));

		m_map->add_terrain(terrain);
	}

	void MapLoader::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
		MaterialEffectDescriptionVector materials;
		MaterialEffectDescription material;
		StringStream str;
		String file_name;
		Transformation transformation;
		glm::vec3 offset;
		Uint32 id, i, j;

		m_map->set_tile(x, y, tile);

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
		transformation.set_scale(1.0f);

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

		material.set_material_descriptiont(
			get_material_description_cache(
				)->get_material_description(String(str.str())));

		material.set_world_transformation(String(UTF8("default")));

		materials.push_back(material);

		id = get_free_ids().use_typeless_id(x + (y << 10),
			it_tile_object);

		m_instances_builder->add(ObjectData(transformation,
			String(UTF8("tile")), 0.0f, id, st_none, false, false,
			0), materials);

//		if (tile != 1)
		{
			return;
		}

		material.set_material_descriptiont(
			get_material_description_cache(
				)->get_material_description(String(UTF8("grass-billboard"))));

		material.set_world_transformation(String(UTF8("default")));
		material.set_culling(false);
		material.set_transparent(true);

		materials.clear();
		materials.push_back(material);

		transformation.set_scale(0.5f);
srand( (unsigned)time( NULL ) );


		for (i = 0; i < 24; ++i)
		{
			for (j = 0; j < 24; ++j)
			{
				offset.x = x * get_tile_size() + (rand() % 18) / 6.0f;
				offset.y = y * get_tile_size() + (rand() % 18) / 6.0f;
				offset.z = 0.125f;

//				offset.x += SimplexNoise::noise(offset * 0.24123542f + glm::vec3(0.0f, 0.0f, 0.0f)) * 0.1f - 0.05f;
//				offset.y += SimplexNoise::noise(offset * 0.24123542f + glm::vec3(0.0f, 0.0f, 1.0f)) * 0.1f - 0.05f;

				transformation.set_translation(offset);

				glm::quat rotation;
				float tmp;

				tmp = static_cast<float>(rand()) / RAND_MAX;

				rotation = glm::quat();
				rotation = glm::rotate(rotation, 90.0f,
					glm::vec3(1.0f, 0.0f, 0.0f));
				rotation = glm::rotate(rotation, 0.0f,
					glm::vec3(0.0f, 1.0f, 0.0f));
				rotation = glm::rotate(rotation, 360.0f * tmp,
					glm::vec3(0.0f, 1.0f, 0.0f));

				transformation.set_rotation(rotation);

				id = get_free_ids().get_next_free_id(
					it_dynamic_object);

				m_instances_builder->add(ObjectData(transformation,
					String(UTF8("grass")), 0.0f, id,
					st_none, false, false, 0), materials);

				rotation = glm::quat();
				rotation = glm::rotate(rotation, 90.0f,
					glm::vec3(1.0f, 0.0f, 0.0f));
				rotation = glm::rotate(rotation, 0.0f,
					glm::vec3(0.0f, 1.0f, 0.0f));
				rotation = glm::rotate(rotation, 90.0f + 360.0f * tmp,
					glm::vec3(0.0f, 1.0f, 0.0f));

				transformation.set_rotation(rotation);

				id = get_free_ids().get_next_free_id(
					it_dynamic_object);

				m_instances_builder->add(ObjectData(transformation,
					String(UTF8("grass")), 0.0f, id,
					st_none, false, false, 0), materials);
			}
		}

/*
		for (i = 0; i < 12; ++i)
		{
			for (j = 0; j < 12; ++j)
			{
				offset.x = x * get_tile_size() + i * 0.25f;// + (j % 2) * 0.5f);// * 0.25f;
				offset.y = y * get_tile_size() + j * 0.25f;// + (i % 2) * 0.5f);// * 0.25f;
				offset.z = 0.125f;

//				offset.x += SimplexNoise::noise(offset * 0.24123542f + glm::vec3(0.0f, 0.0f, 0.0f)) * 0.1f - 0.05f;
//				offset.y += SimplexNoise::noise(offset * 0.24123542f + glm::vec3(0.0f, 0.0f, 1.0f)) * 0.1f - 0.05f;

				transformation.set_translation(offset);

				glm::quat rotation;
				float tmp;

				tmp = static_cast<float>(rand()) / RAND_MAX;

				rotation = glm::quat();
				rotation = glm::rotate(rotation, 90.0f,
					glm::vec3(1.0f, 0.0f, 0.0f));
				rotation = glm::rotate(rotation, 0.0f,
					glm::vec3(0.0f, 1.0f, 0.0f));
				rotation = glm::rotate(rotation, 90.0f * tmp,
					glm::vec3(0.0f, 1.0f, 0.0f));

				transformation.set_rotation(rotation);

				id = get_free_ids().get_next_free_id(
					it_dynamic_object);

				m_instances_builder->add(ObjectData(transformation,
					String(UTF8("grass")), 0.0f, id,
					st_none, false), materials);

				rotation = glm::quat();
				rotation = glm::rotate(rotation, 90.0f,
					glm::vec3(1.0f, 0.0f, 0.0f));
				rotation = glm::rotate(rotation, 0.0f,
					glm::vec3(0.0f, 1.0f, 0.0f));
				rotation = glm::rotate(rotation, 90.0f * (1.0f + tmp),
					glm::vec3(0.0f, 1.0f, 0.0f));

				transformation.set_rotation(rotation);

				id = get_free_ids().get_next_free_id(
					it_dynamic_object);

				m_instances_builder->add(ObjectData(transformation,
					String(UTF8("grass")), 0.0f, id,
					st_none, false), materials);
			}
		}
*/
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
