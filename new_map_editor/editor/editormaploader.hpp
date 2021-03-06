/****************************************************************************
 *            editormaploader.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a7e21ae9_d21e_423a_9bf8_e15b5cb3fc80
#define	UUID_a7e21ae9_d21e_423a_9bf8_e15b5cb3fc80

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractmaploader.hpp"

/**
 * @file
 * @brief The @c class EditorMapLoader.
 * This file contains the @c class EditorMapLoader.
 */
namespace eternal_lands
{

	class EditorMapData;

	/**
	 * @brief @c class for loading maps.
	 *
	 * @c class for loading maps.
	 */
	class EditorMapLoader: public AbstractMapLoader
	{
		private:
			const EffectCacheWeakPtr m_effect_cache;
			const MeshBuilderConstWeakPtr m_mesh_builder;
			const MeshCacheWeakPtr m_mesh_cache;
			const MeshDataCacheWeakPtr m_mesh_data_cache;
			const MaterialCacheWeakPtr m_material_cache;
			const MaterialDescriptionCacheWeakPtr
				m_material_description_cache;
			const TerrainBuilderConstWeakPtr m_terrain_builder;
			const TextureCacheWeakPtr m_texture_cache;
			ReaderSharedPtr m_reader;
			EditorMapData &m_data;
			StringVector m_material_names;
			StringSet m_harvestables, m_entrables;

		protected:
			inline MeshBuilderConstSharedPtr get_mesh_builder()
				const noexcept
			{
				MeshBuilderConstSharedPtr result;

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

			inline EffectCacheSharedPtr get_effect_cache() const
				noexcept
			{
				EffectCacheSharedPtr result;

				result = m_effect_cache.lock();

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

			inline MaterialDescriptionCacheSharedPtr
				get_material_description_cache() const noexcept
			{
				MaterialDescriptionCacheSharedPtr result;

				result = m_material_description_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline TerrainBuilderConstSharedPtr get_terrain_builder()
				const noexcept
			{
				TerrainBuilderConstSharedPtr result;

				result = m_terrain_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline TextureCacheSharedPtr get_texture_cache()
				const noexcept
			{
				TextureCacheSharedPtr result;

				result = m_texture_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline const ReaderSharedPtr &get_reader() const
			{
				return m_reader;
			}

			virtual void add_object(const glm::vec3 &position,
				const glm::vec3 &rotation_angles,
				const glm::vec3 &scale, const String &name,
				const BitSet64 blend_mask,
				const float transparency, const float glow,
				const Uint32 id, const SelectionType selection,
				const BlendType blend,
				const StringVector &material_names) override;
			virtual void add_light(const glm::vec3 &position,
				const glm::vec3 &color, const float radius,
				const Uint32 id) override;
			virtual void add_particle(const glm::vec3 &position,
				const String &name, const Uint32 id) override;
			virtual void add_decal(const glm::vec2 &position,
				const glm::vec2 &scale, const float rotation,
				const String &texture, const Uint32 id)
				override;
			virtual void set_ground_hemisphere(
				const glm::vec4 &ground_hemisphere) override;
			virtual void set_map_size(const glm::uvec2 &size)
				override;
			virtual void set_tile_layer_heights(
				const glm::vec4 &heights) override;
			virtual void set_tile_layer(
				const Uint8MultiArray2 &tile_map,
				const float z_position, const Uint16 layer)
				override;
			virtual void set_walk_height_map(
				const Uint8MultiArray2 &walk_height_map) override;
			virtual void set_dungeon(const bool dungeon) override;
			virtual void set_terrain(
				const ImageConstSharedPtr &displacement_map,
				const ImageConstSharedPtr &normal_tangent_map,
				const ImageConstSharedPtr &dudv_map,
				const ImageConstSharedPtr &blend_map,
				const StringVector &albedo_maps,
				const StringVector &specular_maps,
				const StringVector &gloss_maps,
				const StringVector &height_maps,
				const TerrainMaterialData &material_data,
				const glm::vec4 &dudv_scale_offset,
				const glm::vec3 &translation,
				const glm::uvec2 &sizes) override;
			virtual void instance() override;

		public:
			/**
			 * Default constructor.
			 */
			EditorMapLoader(
				const GlobalVarsConstSharedPtr &global_vars,
				const FileSystemConstSharedPtr &file_system,
				const EffectCacheWeakPtr &effect_cache,
				const MeshBuilderConstWeakPtr &mesh_builder,
				const MeshCacheWeakPtr &mesh_cache,
				const MeshDataCacheWeakPtr &mesh_data_cache,
				const MaterialCacheWeakPtr &material_cache,
				const TerrainBuilderConstWeakPtr &terrain_builder,
				const TextureCacheWeakPtr &texture_cache,
				const FreeIdsManagerSharedPtr &free_ids,
				EditorMapData &data);

			/**
			 * Default destructor.
			 */
			virtual ~EditorMapLoader() throw();
			void load(const String &name,
				const MapItemsTypeSet &skip_items);
			MapSharedPtr get_map(const String &name);

	};

}

#endif	/* UUID_a7e21ae9_d21e_423a_9bf8_e15b5cb3fc80 */
