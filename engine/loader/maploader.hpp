/****************************************************************************
 *            maploader.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c299b4bb_40d2_4781_a02d_4fd650012aca
#define	UUID_c299b4bb_40d2_4781_a02d_4fd650012aca

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractmaploader.hpp"

/**
 * @file
 * @brief The @c class MapLoader.
 * This file contains the @c class MapLoader.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for loading maps.
	 *
	 * @c class for loading maps.
	 */
	class MapLoader: public AbstractMapLoader
	{
		private:
			const CodecManagerSharedPtr m_codec_manager;
			const GlobalVarsSharedPtr m_global_vars;
			const MeshBuilderSharedPtr m_mesh_builder;
			const MeshCacheSharedPtr m_mesh_cache;
			const MeshDataCacheSharedPtr m_mesh_data_cache;
			const EffectCacheSharedPtr m_effect_cache;
			const MaterialCacheSharedPtr m_material_cache;
			const MaterialDescriptionCacheSharedPtr
				m_material_description_cache;
			const MaterialBuilderSharedPtr m_material_builder;
			const TextureCacheSharedPtr m_texture_cache;
			ReaderSharedPtr m_reader;
			MapSharedPtr m_map;
			boost::scoped_ptr<InstancesBuilder> m_instances_builder;
			StringVector m_material_names;

		protected:
			inline const CodecManagerSharedPtr &get_codec_manager()
				const noexcept
			{
				return m_codec_manager;
			}

			inline const GlobalVarsSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

			inline const MeshBuilderSharedPtr &get_mesh_builder()
				const noexcept
			{
				return m_mesh_builder;
			}

			inline const MeshCacheSharedPtr &get_mesh_cache() const
				noexcept
			{
				return m_mesh_cache;
			}

			inline const MeshDataCacheSharedPtr
				&get_mesh_data_cache() const noexcept
			{
				return m_mesh_data_cache;
			}

			inline const EffectCacheSharedPtr &get_effect_cache()
				const noexcept
			{
				return m_effect_cache;
			}

			inline const MaterialCacheSharedPtr
				&get_material_cache() const noexcept
			{
				return m_material_cache;
			}

			inline const MaterialDescriptionCacheSharedPtr
				&get_material_description_cache() const
				noexcept
			{
				return m_material_description_cache;
			}

			inline const MaterialBuilderSharedPtr
				&get_material_builder() const noexcept
			{
				return m_material_builder;
			}

			inline const TextureCacheSharedPtr
				&get_texture_cache() const noexcept
			{
				return m_texture_cache;
			}

			inline const ReaderSharedPtr &get_reader() const
				noexcept
			{
				return m_reader;
			}

			virtual void add_object(const glm::vec3 &position,
				const glm::vec3 &rotation_angles,
				const String &name, const float scale,
				const float transparency, const Uint32 id,
				const SelectionType selection,
				const BlendType blend, const bool walkable,
				const StringVector &material_names);
			virtual void add_light(const glm::vec3 &position,
				const glm::vec3 &color, const float radius,
				const Uint32 id);
			virtual void add_particle(const glm::vec3 &position,
				const String &name, const Uint32 id);
			virtual void add_terrain(const StringArray4 &albedo,
				const String &blend, const String &height,
				const String &dudv,
				const glm::vec3 &translation,
				const glm::vec2 &dudv_scale);
			virtual void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile);
			virtual void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height);
			virtual void set_ambient(const glm::vec3 &ambient);
			virtual void set_height_map_size(const Uint16 width,
				const Uint16 height);
			virtual void set_tile_map_size(const Uint16 width,
				const Uint16 height);
			virtual void set_dungeon(const bool dungeon);
			void instance();

		public:
			/**
			 * Default constructor.
			 */
			MapLoader(const CodecManagerSharedPtr &codec_manager,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const MeshCacheSharedPtr &mesh_cache,
				const MeshDataCacheSharedPtr &mesh_data_cache,
				const EffectCacheSharedPtr &effect_cache,
				const MaterialCacheSharedPtr &material_cache,
				const MaterialDescriptionCacheSharedPtr
					&material_description_cache,
				const MaterialBuilderSharedPtr
					&material_builder,
				const TextureCacheSharedPtr &texture_cache,
				const FreeIdsManagerSharedPtr &free_ids);

			/**
			 * Default destructor.
			 */
			virtual ~MapLoader() noexcept;

			MapSharedPtr load(const String &name);

	};

}

#endif	/* UUID_c299b4bb_40d2_4781_a02d_4fd650012aca */
