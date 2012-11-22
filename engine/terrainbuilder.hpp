/****************************************************************************
 *            terrainbuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_faa1dbdd_8ccb_4b12_ba53_d0a2dea1aea9
#define	UUID_faa1dbdd_8ccb_4b12_ba53_d0a2dea1aea9

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TerrainBuilder.
 * This file contains the @c class TerrainBuilder.
 */
namespace eternal_lands
{

	class TerrainBuilder: public boost::noncopyable
	{
		private:
			const GlobalVarsSharedPtr m_global_vars;
			const EffectCacheWeakPtr m_effect_cache;
			const MaterialBuilderWeakPtr m_material_builder;
			const MaterialCacheWeakPtr m_material_cache;
			const MeshBuilderWeakPtr m_mesh_builder;
			const MeshCacheWeakPtr m_mesh_cache;
			const MeshDataCacheWeakPtr m_mesh_data_cache;
			const TextureCacheWeakPtr m_texture_cache;
			StringArray3 m_cdlod_terrain_material_name;
			StringArray3 m_simple_terrain_material_name;
			String m_clipmap_terrain_effect_name;

			inline const GlobalVarsSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

			inline EffectCacheSharedPtr get_effect_cache() const
				noexcept
			{
				EffectCacheSharedPtr result;

				result = m_effect_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialBuilderSharedPtr get_material_builder()
				const noexcept
			{
				MaterialBuilderSharedPtr result;

				result = m_material_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialCacheSharedPtr get_material_cache()
				const noexcept
			{
				MaterialCacheSharedPtr result;

				result = m_material_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MeshBuilderSharedPtr get_mesh_builder() const
				noexcept
			{
				MeshBuilderSharedPtr result;

				result = m_mesh_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MeshCacheSharedPtr get_mesh_cache() const
				noexcept
			{
				MeshCacheSharedPtr result;

				result = m_mesh_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MeshDataCacheSharedPtr get_mesh_data_cache()
				const noexcept
			{
				MeshDataCacheSharedPtr result;

				result = m_mesh_data_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline TextureCacheSharedPtr get_texture_cache() const
				noexcept
			{
				TextureCacheSharedPtr result;

				result = m_texture_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

		public:
			TerrainBuilder(const GlobalVarsSharedPtr &global_vars,
				const EffectCacheWeakPtr &effect_cache,
				const MaterialBuilderWeakPtr &material_builder,
				const MaterialCacheWeakPtr &material_cache,
				const MeshBuilderWeakPtr &mesh_builder,
				const MeshCacheWeakPtr &mesh_cache,
				const MeshDataCacheWeakPtr &mesh_data_cache,
				const TextureCacheWeakPtr &texture_cache);
			~TerrainBuilder() noexcept;
			AbstractTerrainSharedPtr get_terrain() const;
			EffectSharedPtr get_effect(const String &name,
				const TerrainMaterialData &material_data)
				const;

	};

}

#endif	/* UUID_faa1dbdd_8ccb_4b12_ba53_d0a2dea1aea9 */
