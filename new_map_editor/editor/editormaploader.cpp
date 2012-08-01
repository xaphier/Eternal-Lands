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
		const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const MeshDataCacheSharedPtr &mesh_data_cache,
		const MaterialCacheSharedPtr &material_cache,
		const MaterialBuilderWeakPtr &material_builder,
		const TextureCacheWeakPtr &texture_cache,
		const FreeIdsManagerSharedPtr &free_ids,
		EditorMapData &data): AbstractMapLoader(file_system, free_ids),
		m_codec_manager(codec_manager), m_global_vars(global_vars),
		m_mesh_builder(mesh_builder), m_mesh_cache(mesh_cache),
		m_mesh_data_cache(mesh_data_cache),
		m_material_cache(material_cache),
		m_material_builder(material_builder),
		m_texture_cache(texture_cache), m_data(data)
	{
	}

	EditorMapLoader::~EditorMapLoader() throw()
	{
	}

	void EditorMapLoader::add_object(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const String &name,
		const float scale, const float transparency,
		const Uint32 id, const SelectionType selection,
		const BlendType blend, const StringVector &material_names)
	{
		SelectionType new_selection;

		new_selection = selection;

		if (new_selection == st_none)
		{
			new_selection = st_select;
		}

		m_data.add_object(EditorObjectDescription(translation,
			rotation_angles, material_names, name, scale,
			transparency, id, new_selection, blend));
	}

	void EditorMapLoader::add_light(const glm::vec3 &position,
		const glm::vec3 &color, const float radius, const Uint32 id)
	{
		m_data.add_light(LightData(position, color, radius, id));
	}

	void EditorMapLoader::add_particle(const glm::vec3 &position,
		const String &name, const Uint32 id)
	{
		m_data.add_particle(ParticleData(position, name, id));
	}

	void EditorMapLoader::add_terrain(const StringArray4 &albedo,
		const String &blend, const String &height, const String &dudv,
		const glm::vec3 &translation, const glm::vec2 &dudv_scale)
	{
/*
		AbstractTerrainManagerAutoPtr terrain;

		terrain.reset(new SimpleTerrainManager(get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_mesh_builder(), get_effect_cache(),
			get_texture_cache(), transformation, albedo, blend,
			height, dudv));

		m_map->add_terrain(terrain);
*/
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

	void EditorMapLoader::set_height_map_size(const Uint16 width,
		const Uint16 height)
	{
		m_data.set_height_map_size(width, height);
	}

	void EditorMapLoader::set_tile_map_size(const Uint16 width,
		const Uint16 height)
	{
		m_data.set_tile_map_size(width, height);
	}
		
	void EditorMapLoader::set_dungeon(const bool dungeon)
	{
		m_data.set_dungeon(dungeon);
	}

	void EditorMapLoader::instance()
	{
	}

	void EditorMapLoader::load(const String &name)
	{
		read(name);
	}
		
	MapSharedPtr EditorMapLoader::get_map(const String &name)
	{
		return boost::make_shared<Map>(get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_mesh_builder(), get_mesh_cache(),
			get_material_cache(), get_material_builder(),
			get_texture_cache(), name);
	}

}
