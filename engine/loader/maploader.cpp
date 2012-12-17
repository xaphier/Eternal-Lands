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
#include "tilebuilder.hpp"

namespace eternal_lands
{

	MapLoader::MapLoader(const FileSystemSharedPtr &file_system,
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
		const TileBuilderWeakPtr &tile_builder,
		const FreeIdsManagerSharedPtr &free_ids):
		AbstractMapLoader(file_system, free_ids, global_vars),
		m_effect_cache(effect_cache),m_mesh_builder(mesh_builder),
		m_mesh_cache(mesh_cache), m_mesh_data_cache(mesh_data_cache),
		m_material_cache(material_cache),
		m_material_description_cache(material_description_cache),
		m_terrain_builder(terrain_builder),
		m_texture_cache(texture_cache), m_tile_builder(tile_builder)
	{
		assert(!m_effect_cache.expired());
		assert(!m_mesh_builder.expired());
		assert(!m_mesh_cache.expired());
		assert(!m_mesh_data_cache.expired());
		assert(!m_material_cache.expired());
		assert(!m_material_description_cache.expired());
		assert(!m_terrain_builder.expired());
		assert(!m_texture_cache.expired());
		assert(!m_tile_builder.expired());
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

		m_map = boost::make_shared<Map>(get_global_vars(),
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
		const String &name, const BitSet64 blend_mask,
		const float transparency, const float glow, const Uint32 id,
		const SelectionType selection, const BlendType blend,
		const StringVector &material_names)
	{
		if (blend_mask.any() || (glow != 0.0f))
		{
			m_map->add_object(get_object_description(translation,
				rotation_angles, scale, material_names,
				name, blend_mask, transparency, glow, id,
				selection, blend));
		}
		else
		{
			m_instances_builder->add(get_object_description(
				translation, rotation_angles, scale,
				material_names, name, blend_mask, transparency,
				glow, id, selection, blend));
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

	void MapLoader::set_tile_page(const Uint16MultiArray2 &tile_page,
		const glm::uvec2 &offset, const float z_position,
		const Uint16 layer)
	{
		Transformation transformation;
		AbstractMeshSharedPtr mesh;
		MaterialSharedPtrVector materials;
		StringStream str;
		BitSet64 blend_mask;
		Uint32 x, y, id, tile, tile_size;
		bool use_tiles;

		id = get_free_ids().get_tile_id(offset.x, offset.y, layer);
		tile_size = TileBuilder::get_tile_size();
		use_tiles = false;

		for (y = 0; y < tile_size; y++)
		{
			for (x = 0; x < tile_size; x++)
			{
				tile = tile_page[x][y];
				use_tiles |= tile !=
					std::numeric_limits<Uint16>::max();
			}
		}

		if (!use_tiles)
		{
			return;
		}

		get_free_ids().use_object_id(id);

		str << UTF8("tile_") << offset.x << UTF8("x") << offset.y;
		str << UTF8("x") << layer;

		get_tile_builder()->get_tile(tile_page, mesh, materials,
			blend_mask);

		transformation.set_translation(glm::vec3(glm::vec2(offset *
			tile_size) * TileBuilder::get_tile_world_scale(),
			z_position));

		m_map->add_object(ObjectData(transformation, String(str.str()),
			blend_mask, 0.0f, 0.0f, id, st_none,
			bt_alpha_transparency_source_value), mesh, materials);
	}

	void MapLoader::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		m_map->set_height(x, y, height);
	}

	void MapLoader::set_ground_hemisphere(
		const glm::vec4 &ground_hemisphere)
	{
		m_map->set_ground_hemisphere(ground_hemisphere);
	}

	void MapLoader::set_map_size(const glm::uvec2 &size)
	{
		m_map->set_size(size);
	}

	void MapLoader::set_height_map_size(const glm::uvec2 &size)
	{
		m_map->set_height_map_size(size);
	}

	void MapLoader::set_tile_map_size(const glm::uvec2 &size)
	{
		m_map->set_tile_map_size(size);
	}

	void MapLoader::set_tile_layer_heights(const glm::vec4 &heights)
	{
	}

	void MapLoader::set_tile_layer(const Uint8MultiArray2 &tile_map,
		const float z_position, const Uint16 layer)
	{
		Uint16MultiArray2 tile_page;
		glm::uvec2 offset;
		Uint32 x, y, height, width, tile_size;

		width = tile_map.shape()[0];
		height = tile_map.shape()[1];

		tile_size = TileBuilder::get_tile_size();

		width = (width + tile_size - 1) / tile_size;
		height = (height + tile_size - 1) / tile_size;

		tile_page.resize(boost::extents[tile_size][tile_size]);

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				offset.x = x;
				offset.y = y;

				get_tile_layer_page(tile_map, offset,
					tile_page);

				LOG_DEBUG(lt_map_loader, UTF8("Set tile page "
					"<%1%, %2%, %3%>, %4%."), x % y % layer
					% z_position);

				set_tile_page(tile_page, offset, z_position,
					layer);
			}
		}
	}

	void MapLoader::get_tile_layer_page(const Uint8MultiArray2 &tile_map,
		const glm::uvec2 &offset, Uint16MultiArray2 &tile_page)
	{
		Uint32 x, y, xx, yy, height, width, tile_size, tile;

		width = tile_map.shape()[0];
		height = tile_map.shape()[1];

		tile_size = TileBuilder::get_tile_size();

		for (y = 0; y < tile_size; y++)
		{
			for (x = 0; x < tile_size; x++)
			{
				xx = x + offset.x * tile_size;
				yy = y + offset.y * tile_size;

				if ((xx < width) && (yy < height))
				{
					tile = tile_map[xx][yy];

					if (tile == 0xFF)
					{
						tile = std::numeric_limits<
							Uint16>::max();
					}
				}
				else
				{
					tile = std::numeric_limits<
						Uint16>::max();
				}

				tile_page[x][y] = tile;
			}
		}
	}

	void MapLoader::set_dungeon(const bool dungeon)
	{
		m_map->set_dungeon(dungeon);
	}

	void MapLoader::set_terrain(const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_tangent_map,
		const ImageSharedPtr &dudv_map,
		const ImageSharedPtr &blend_map,
		const StringVector &albedo_maps,
		const StringVector &specular_maps,
		const StringVector &gloss_maps,
		const StringVector &height_maps,
		const TerrainMaterialData &material_data,
		const glm::vec4 &dudv_scale_offset,
		const glm::vec3 &translation, const glm::uvec2 &sizes)
	{
		m_map->set_terrain_geometry_maps(displacement_map,
			normal_tangent_map, dudv_map, translation);
		m_map->set_terrain_blend_map(blend_map);
		m_map->set_terrain_material(albedo_maps, specular_maps,
			gloss_maps, height_maps, material_data);
		m_map->set_terrain_dudv_scale_offset(dudv_scale_offset);
	}

}
