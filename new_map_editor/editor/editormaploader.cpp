/****************************************************************************
 *            editormaploader.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editormaploader.hpp"
#include "map.hpp"
#include "editormapdata.hpp"
#include "lightdata.hpp"
#include "particledata.hpp"
#include "materialdescription.hpp"
#include "materialdescriptioncache.hpp"
#include "editorobjectdescription.hpp"

namespace eternal_lands
{

	EditorMapLoader::EditorMapLoader(
		const CodecManagerWeakPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const EffectCacheWeakPtr &effect_cache,
		const MeshBuilderWeakPtr &mesh_builder,
		const MeshCacheWeakPtr &mesh_cache,
		const MeshDataCacheWeakPtr &mesh_data_cache,
		const MaterialCacheWeakPtr &material_cache,
		const TerrainBuilderWeakPtr &terrain_builder,
		const TextureCacheWeakPtr &texture_cache,
		const FreeIdsManagerSharedPtr &free_ids,
		EditorMapData &data): AbstractMapLoader(codec_manager,
			file_system, free_ids, global_vars),
		m_effect_cache(effect_cache), m_mesh_builder(mesh_builder),
		m_mesh_cache(mesh_cache), m_mesh_data_cache(mesh_data_cache),
		m_material_cache(material_cache),
		m_terrain_builder(terrain_builder),
		m_texture_cache(texture_cache), m_data(data)
	{
		assert(!m_effect_cache.expired());;
		assert(!m_mesh_builder.expired());;
		assert(!m_mesh_cache.expired());;
		assert(!m_mesh_data_cache.expired());;
		assert(!m_material_cache.expired());;
		assert(!m_terrain_builder.expired());
		assert(!m_texture_cache.expired());
	}

	EditorMapLoader::~EditorMapLoader() throw()
	{
	}

	void EditorMapLoader::add_object(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const glm::vec3 &scale,
		const String &name, const float transparency, const Uint32 id,
		const SelectionType selection, const BlendType blend,
		const bool walkable, const StringVector &material_names)
	{
		SelectionType new_selection;

		new_selection = selection;

		if (new_selection == st_none)
		{
			new_selection = st_select;
		}

		m_data.add_object(EditorObjectDescription(translation,
			rotation_angles, scale, material_names, name,
			transparency, id, new_selection, blend, walkable),
			sct_no);
	}

	void EditorMapLoader::add_light(const glm::vec3 &position,
		const glm::vec3 &color, const float radius, const Uint32 id)
	{
		m_data.add_light(LightData(position, color, radius, id),
			sct_no);
	}

	void EditorMapLoader::add_particle(const glm::vec3 &position,
		const String &name, const Uint32 id)
	{
		m_data.add_particle(ParticleData(position, name, id),
			sct_no);
	}

	void EditorMapLoader::add_decal(const glm::vec2 &position,
		const glm::vec2 &scale, const float rotation_angel,
		const String &texture, const Uint32 id)
	{
//		m_data.add_decal(EditorDecalDescription(position, scale,
//			rotation_angel, scale, texture, id));
	}

	void EditorMapLoader::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
		m_data.set_tile(x, y, tile);
	}

	void EditorMapLoader::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		m_data.set_height(x, y, height);
	}

	void EditorMapLoader::set_ambient(const glm::vec3 &ambient)
	{
		m_data.set_ambient(ambient);
	}

	void EditorMapLoader::set_map_size(const glm::uvec2 &size)
	{
		m_data.set_map_size(size);
	}

	void EditorMapLoader::set_height_map_size(const glm::uvec2 &size)
	{
		m_data.set_height_map_size(size);
	}

	void EditorMapLoader::set_tile_map_size(const glm::uvec2 &size)
	{
		m_data.set_tile_map_size(size);
	}
		
	void EditorMapLoader::set_dungeon(const bool dungeon)
	{
		m_data.set_dungeon(dungeon);
	}

	void EditorMapLoader::instance()
	{
	}

	void EditorMapLoader::load(const String &name,
		const MapItemsTypeSet &skip_items)
	{
		m_data.clear_terrain();
		read(name, skip_items);
	}

	MapSharedPtr EditorMapLoader::get_map(const String &name)
	{
		MapSharedPtr result;

		result = boost::make_shared<Map>(get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_mesh_builder(), get_mesh_cache(),
			get_material_cache(), get_terrain_builder(),
			get_texture_cache());

		result->set_name(name);

		return result;
	}

	void EditorMapLoader::set_terrain(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const ImageSharedPtr &blend_map,
		const StringVector &albedo_maps,
		const StringVector &extra_maps,
		const TerrainMaterialData &material_data,
		const glm::vec4 &dudv_scale_offset,
		const glm::uvec2 &sizes)
	{
		m_data.set_terrain(displacement_map, normal_map,
			dudv_map, blend_map, albedo_maps, extra_maps,
			material_data, dudv_scale_offset, sizes);
	}

}
