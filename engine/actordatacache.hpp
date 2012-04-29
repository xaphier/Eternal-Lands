/****************************************************************************
 *            actordatacache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_12f66ad3_a433_4413_888d_642751314caa
#define	UUID_12f66ad3_a433_4413_888d_642751314caa

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdata.hpp"

/**
 * @file
 * @brief The @c class ActorDataCache.
 * This file contains the @c class Actor.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for actor data cache.
	 *
	 * @c class for actor data cache.
	 */
	class ActorDataCache
	{
		private:
			class ActorDataCacheItem;

			typedef std::map<Uint32, ActorDataCacheItem>
				ActorDataCacheMap;

			ActorDataCacheMap m_actor_data_cache;
			const MeshBuilderWeakPtr m_mesh_builder;
			const CodecManagerWeakPtr m_codec_manager;
			const MaterialCacheWeakPtr m_material_cache;
			const MaterialBuilderWeakPtr m_material_builder;
			const MaterialDescriptionCacheWeakPtr
				m_material_description_cache;
			const FileSystemSharedPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;

			inline MeshBuilderSharedPtr get_mesh_builder() const
			{
				MeshBuilderSharedPtr result;

				result = m_mesh_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline const CodecManagerWeakPtr &get_codec_manager()
				const
			{
				return m_codec_manager;
			}

			inline MaterialCacheSharedPtr get_material_cache() const
			{
				MaterialCacheSharedPtr result;

				result = m_material_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialBuilderSharedPtr get_material_builder()
				const
			{
				MaterialBuilderSharedPtr result;

				result = m_material_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialDescriptionCacheSharedPtr
				get_material_description_cache() const
			{
				MaterialDescriptionCacheSharedPtr result;

				result = m_material_description_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline const FileSystemSharedPtr &get_file_system()
				const
			{
				return m_file_system;
			}

			inline const GlobalVarsSharedPtr &get_global_vars()
				const
			{
				return m_global_vars;
			}

		public:
			/**
			 * Default constructor.
			 */
			ActorDataCache(const MeshBuilderWeakPtr &mesh_builder,
				const CodecManagerWeakPtr &codec_manager,
				const MaterialCacheWeakPtr &material_cache,
				const MaterialBuilderWeakPtr &material_builder,
				const MaterialDescriptionCacheWeakPtr
					&material_description_cache,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars);

			/**
			 * Default destructor.
			 */
			~ActorDataCache() throw();

			void add_actor(const Uint32 id,
				CalCoreModel *core_model, const String &name,
				const String &skin_name,
				const String &file_name,
				const float actor_scale, const float scale,
				const float mesh_scale, const float skel_scale,
				const bool ghost);

			std::auto_ptr<Actor> get_actor(const Uint32 type_id,
				const Uint32 id, const String &name,
				const SelectionType selection,
				const bool enhanced_actor);

	};

}

#endif	/* UUID_12f66ad3_a433_4413_888d_642751314caa */

