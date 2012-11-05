/****************************************************************************
 *            terrainbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainbuilder.hpp"
#include "terrain/cdlodterrain.hpp"
#include "terrain/simpleterrain.hpp"
#include "globalvars.hpp"
#include "clipmapterraindata.hpp"
#include "effect/effect.hpp"
#include "effect/effectcache.hpp"
#include "effect/effectdescription.hpp"
#include "shader/shadersourceterrain.hpp"

namespace eternal_lands
{

	TerrainBuilder::TerrainBuilder(const GlobalVarsSharedPtr &global_vars,
		const EffectCacheWeakPtr &effect_cache,
		const MaterialBuilderWeakPtr &material_builder,
		const MaterialCacheWeakPtr &material_cache,
		const MeshBuilderWeakPtr &mesh_builder,
		const MeshCacheWeakPtr &mesh_cache,
		const TextureCacheWeakPtr &texture_cache):
		m_global_vars(global_vars), m_effect_cache(effect_cache),
		m_material_builder(material_builder),
		m_material_cache(material_cache), m_mesh_builder(mesh_builder),
		m_mesh_cache(mesh_cache), m_texture_cache(texture_cache)
	{
		assert(global_vars.get() != nullptr);
		assert(!m_effect_cache.expired());
		assert(!m_material_builder.expired());
		assert(!m_material_cache.expired());
		assert(!m_mesh_builder.expired());
		assert(!m_mesh_cache.expired());
		assert(!m_texture_cache.expired());

		m_cdlod_terrain_material_name = String(UTF8("cdlod-terrain"));
		m_simple_terrain_material_name = String(UTF8("simple-terrain"));
		m_clipmap_terrain_effect_name = String(UTF8("terrain"));
	}

	TerrainBuilder::~TerrainBuilder() noexcept
	{
	}

	AbstractTerrainSharedPtr TerrainBuilder::get_terrain() const
	{
		if (get_global_vars()->get_opengl_3_1())
		{
			return boost::make_shared<CdLodTerrain>(
				get_global_vars(), get_effect_cache(),
				get_mesh_cache(), get_material_builder(),
				get_material_cache(),
				m_cdlod_terrain_material_name,
				m_clipmap_terrain_effect_name);
		}

		return boost::make_shared<SimpleTerrain>(get_global_vars(),
			get_effect_cache(), get_mesh_builder(),
			get_material_builder(), get_material_cache(),
			m_simple_terrain_material_name,
			m_clipmap_terrain_effect_name);
	}

	EffectSharedPtr TerrainBuilder::get_effect(const String &name,
		const TerrainMaterialData &material_data) const
	{
		ShaderSourceTerrainSharedPtr shader_source_terrain;
		EffectSharedPtr result;
		EffectDescription effect_description;

		result = get_effect_cache()->get_effect(
			m_clipmap_terrain_effect_name);

		effect_description = result->get_description();

		shader_source_terrain =
			boost::make_shared<ShaderSourceTerrain>();

		shader_source_terrain->set_name(name);
		shader_source_terrain->set_material_data(material_data);

		effect_description.set_main(
			shader_source_terrain->save_xml_string());

		result = get_effect_cache()->build_effect(effect_description);

		return result;
	}

}
