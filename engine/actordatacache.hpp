/****************************************************************************
 *            actordatacache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			const MeshBuilderConstWeakPtr m_mesh_builder;
			const MaterialCacheWeakPtr m_material_cache;
			const MaterialBuilderConstWeakPtr m_material_builder;
			const MaterialDescriptionCacheWeakPtr
				m_material_description_cache;
			const GlobalVarsConstSharedPtr m_global_vars;
			const FileSystemConstSharedPtr m_file_system;

			inline MeshBuilderConstSharedPtr get_mesh_builder()
				const noexcept
			{
				MeshBuilderConstSharedPtr result;

				result = m_mesh_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialCacheSharedPtr get_material_cache() const
				noexcept
			{
				MaterialCacheSharedPtr result;

				result = m_material_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialBuilderConstSharedPtr get_material_builder()
				const noexcept
			{
				MaterialBuilderConstSharedPtr result;

				result = m_material_builder.lock();

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

			inline const GlobalVarsConstSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

			inline const FileSystemConstSharedPtr &get_file_system()
				const noexcept
			{
				return m_file_system;
			}

		public:
			/**
			 * Default constructor.
			 */
			ActorDataCache(
				const MeshBuilderConstWeakPtr &mesh_builder,
				const MaterialCacheWeakPtr &material_cache,
				const MaterialBuilderConstWeakPtr &material_builder,
				const MaterialDescriptionCacheWeakPtr
					&material_description_cache,
				const GlobalVarsConstSharedPtr &global_vars,
				const FileSystemConstSharedPtr &file_system);

			/**
			 * Default destructor.
			 */
			~ActorDataCache() noexcept;

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

