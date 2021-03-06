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
#include "effect/effect.hpp"
#include "effect/effectcache.hpp"
#include "effect/effectdescription.hpp"
#include "shader/shadersourceterrain.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	TerrainBuilder::TerrainBuilder(
		const GlobalVarsConstSharedPtr &global_vars,
		const EffectCacheWeakPtr &effect_cache,
		const MaterialBuilderConstWeakPtr &material_builder,
		const MaterialCacheWeakPtr &material_cache,
		const MeshBuilderConstWeakPtr &mesh_builder,
		const MeshCacheWeakPtr &mesh_cache,
		const MeshDataCacheWeakPtr &mesh_data_cache,
		const TextureCacheWeakPtr &texture_cache):
		m_global_vars(global_vars), m_effect_cache(effect_cache),
		m_material_builder(material_builder),
		m_material_cache(material_cache), m_mesh_builder(mesh_builder),
		m_mesh_cache(mesh_cache), m_mesh_data_cache(mesh_data_cache),
		m_texture_cache(texture_cache)
	{
		assert(global_vars.get() != nullptr);
		assert(!m_effect_cache.expired());
		assert(!m_material_builder.expired());
		assert(!m_material_cache.expired());
		assert(!m_mesh_builder.expired());
		assert(!m_mesh_cache.expired());
		assert(!m_mesh_data_cache.expired());
		assert(!m_texture_cache.expired());

		m_cdlod_terrain_material_name[qt_low] =
			String(UTF8("cdlod-terrain-low-quality"));
		m_cdlod_terrain_material_name[qt_medium] =
			String(UTF8("cdlod-terrain-medium-quality"));
		m_cdlod_terrain_material_name[qt_high] =
			String(UTF8("cdlod-terrain-high-quality"));
		m_simple_terrain_material_name[qt_low] =
			String(UTF8("simple-terrain-all-quality"));
		m_simple_terrain_material_name[qt_medium] =
			String(UTF8("simple-terrain-all-quality"));
		m_simple_terrain_material_name[qt_high] =
			String(UTF8("simple-terrain-all-quality"));
		m_clipmap_terrain_effect_name = String(UTF8("terrain"));
	}

	TerrainBuilder::~TerrainBuilder() noexcept
	{
	}

	AbstractTerrainSharedPtr TerrainBuilder::get_terrain() const
	{
		if (get_global_vars()->get_opengl_3_3())
		{
			return boost::make_shared<CdLodTerrain>(
				get_global_vars(), get_effect_cache(),
				get_mesh_builder(), get_mesh_cache(),
				get_mesh_data_cache(), get_material_builder(),
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

		LOG_DEBUG(lt_effect, UTF8("terrain effect '%1%'."),
			shader_source_terrain->save_xml_string());

		effect_description.set_main(
			shader_source_terrain->save_xml_string());

		if (material_data.get_write_specular_gloss())
		{
			effect_description.set_description(edt_screen_quad_2);
			effect_description.set_output_channel_count(4, 1);
		}
		else
		{
			effect_description.set_description(edt_screen_quad_1);
			effect_description.set_output_channel_count(0, 1);
		}

		if (material_data.get_write_height())
		{
			effect_description.set_output_channel_count(4, 0);
		}
		else
		{
			effect_description.set_output_channel_count(3, 0);
		}

		result = get_effect_cache()->build_effect(effect_description);

		return result;
	}

}
