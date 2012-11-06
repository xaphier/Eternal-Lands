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
			const MeshBuilderWeakPtr m_mesh_builder;
			const MeshCacheWeakPtr m_mesh_cache;
			const MeshDataCacheWeakPtr m_mesh_data_cache;
			const MaterialCacheWeakPtr m_material_cache;
			const MaterialDescriptionCacheWeakPtr
				m_material_description_cache;
			const TerrainBuilderWeakPtr m_terrain_builder;
			const TextureCacheWeakPtr m_texture_cache;
			ReaderSharedPtr m_reader;
			EditorMapData &m_data;
			StringVector m_material_names;
			StringSet m_harvestables, m_entrables;

		protected:
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

			inline TerrainBuilderSharedPtr get_terrain_builder()
				const noexcept
			{
				TerrainBuilderSharedPtr result;

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
				const float transparency, const Uint32 id,
				const SelectionType selection,
				const BlendType blend, const bool walkable,
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
			virtual void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile) override;
			virtual void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height) override;
			virtual void set_ambient(const glm::vec3 &ambient)
				override;
			virtual void set_height_map_size(const Uint16 width,
				const Uint16 height) override;
			virtual void set_tile_map_size(const Uint16 width,
				const Uint16 height) override;
			virtual void set_dungeon(const bool dungeon) override;
			virtual void set_terrain(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const ImageSharedPtr &blend_map,
				const StringVector &albedo_maps,
				const StringVector &extra_maps,
				const TerrainMaterialData &material_data,
				const glm::vec4 &dudv_scale_offset,
				const glm::uvec2 &sizes) override;
			virtual void instance() override;

		public:
			/**
			 * Default constructor.
			 */
			EditorMapLoader(
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
