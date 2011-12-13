/****************************************************************************
 *            actordatacache.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			const EffectCacheWeakPtr m_effect_cache;
			const TextureCacheWeakPtr m_texture_cache;
			const CodecManagerWeakPtr m_codec_manager;
			const FileSystemWeakPtr m_file_system;
			const GlobalVarsSharedPtr m_global_vars;

			inline MeshBuilderSharedPtr get_mesh_builder() const
			{
				MeshBuilderSharedPtr result;

				result = m_mesh_builder.lock();

				assert(result.get() != 0);

				return result;
			}

			inline const EffectCacheWeakPtr &get_effect_cache()
				const
			{
				return m_effect_cache;
			}

			inline const TextureCacheWeakPtr &get_texture_cache()
				const
			{
				return m_texture_cache;
			}

			inline const CodecManagerWeakPtr &get_codec_manager()
				const
			{
				return m_codec_manager;
			}

			inline const FileSystemWeakPtr &get_file_system() const
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
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const CodecManagerWeakPtr &codec_manager,
				const FileSystemWeakPtr &file_system,
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

