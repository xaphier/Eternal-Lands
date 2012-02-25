/****************************************************************************
 *            editormaploader.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
 * @brief The @c class EditorMapLoader.
 * This file contains the @c class EditorMapLoader.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for loading maps.
	 *
	 * @c class for loading maps.
	 */
	class EditorMapLoader: public AbstractMapLoader
	{
		private:
			const CodecManagerSharedPtr m_codec_manager;
			const FileSystemSharedPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;
			const MeshBuilderSharedPtr m_mesh_builder;
			const MeshCacheSharedPtr m_mesh_cache;
			const MeshDataCacheSharedPtr m_mesh_data_cache;
			const EffectCacheSharedPtr m_effect_cache;
			const TextureCacheSharedPtr m_texture_cache;
			const MaterialDescriptionCacheSharedPtr
				m_material_description_cache;
			ReaderSharedPtr m_reader;
			MapSharedPtr m_map;
			StringVector m_material_names;
			StringSet m_harvestables, m_entrables;

		protected:
			inline const MaterialDescriptionCacheSharedPtr
				&get_material_description_cache() const
			{
				return m_material_description_cache;
			}

			virtual void add_object(const glm::vec3 &position,
				const glm::vec3 &rotation_angles,
				const String &name, const float scale,
				const float transparency, const Uint32 id,
				const bool blended,
				const SelectionType selection,
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
				const glm::vec3 &rotation_angles,
				const float scale);
			virtual void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile);
			virtual void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height);
			virtual void set_ambient(const glm::vec3 &ambient);
			virtual void set_height_map_size(const Uint16 width,
				const Uint16 height);

		public:
			/**
			 * Default constructor.
			 */
			EditorMapLoader(
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
				const FreeIdsManagerSharedPtr &free_ids);

			/**
			 * Default destructor.
			 */
			virtual ~EditorMapLoader() throw();

			MapSharedPtr load(const String &name);

	};

}

#endif	/* UUID_c299b4bb_40d2_4781_a02d_4fd650012aca */
