/****************************************************************************
 *            editormaploader.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editormaploader.hpp"
#include "map.hpp"
#include "lightdata.hpp"
#include "particledata.hpp"
#include "materialeffectdescription.hpp"
#include "materialdescriptioncache.hpp"

namespace eternal_lands
{

	EditorMapLoader::EditorMapLoader(
		const CodecManagerSharedPtr &codec_manager,
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

	EditorMapLoader::~EditorMapLoader() throw()
	{
	}

	void EditorMapLoader::add_object(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const String &name,
		const float scale, const float transparency,
		const Uint32 id, const SelectionType selection,
		const BlendType blend, const StringVector &material_names)
	{
		m_map->add_object(get_object_data(translation, rotation_angles,
			name, scale, id, selection, blend));
	}

	void EditorMapLoader::add_light(const glm::vec3 &position,
		const glm::vec3 &color, const float radius, const Uint32 id)
	{
		m_map->add_light(LightData(position, color, radius, id));
	}

	void EditorMapLoader::add_particle(const glm::vec3 &position,
		const String &name, const Uint32 id)
	{
		m_map->add_particle(ParticleData(position, name, id));
	}

	void EditorMapLoader::add_terrain(const StringArray4 &albedo,
		const String &blend, const String &height, const String &dudv,
		const glm::vec3 &translation, const glm::vec3 &rotation_angles,
		const float scale)
	{
		AbstractTerrainManagerAutoPtr terrain;
		Transformation transformation;
/*
		terrain.reset(new SimpleTerrainManager(get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_mesh_builder(), get_effect_cache(),
			get_texture_cache(), transformation, albedo, blend,
			height, dudv));
*/
		m_map->add_terrain(terrain);
	}

	void EditorMapLoader::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
		MaterialEffectDescriptionVector materials;
		MaterialEffectDescription material;
		StringStream str;
		String file_name;
		Transformation transformation;
		glm::vec3 offset;
		Uint32 id;

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
		transformation.set_scale(3.0f);

		if ((tile != 0) && (tile != 240))
		{
			str << UTF8("tile") << static_cast<Uint16>(tile);
		}
		else
		{
			if (tile == 240)
			{
				str << UTF8("lava");
			}
			else
			{
				str << UTF8("water");
			}
		}

		material.set_material_descriptiont(
			get_material_description_cache(
				)->get_material_description(String(str.str())));

		material.set_world_transformation(String(UTF8("default")));

		materials.push_back(material);

		id = x + (y << 10) + 0xF0000000;

		m_map->add_object(ObjectData(transformation,
			String(UTF8("plane_4")), 0.0f, id, st_none,
			bt_disabled), materials);
	}

	void EditorMapLoader::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		m_map->set_height(x, y, height);
	}

	void EditorMapLoader::set_ambient(const glm::vec3 &ambient)
	{
		m_map->set_ambient(ambient);
	}

	void EditorMapLoader::set_height_map_size(const Uint16 width,
		const Uint16 height)
	{
		m_map->set_height_map_size(width, height);
	}

}
