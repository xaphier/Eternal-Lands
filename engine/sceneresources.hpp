/****************************************************************************
 *            sceneresources.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef UUID_90784f4e_4047_496d_ba28_81d920a6e71e
#define UUID_90784f4e_4047_496d_ba28_81d920a6e71e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class SceneResources.
 * This file contains the @c class SceneResources.
 */
namespace eternal_lands
{

	class SceneResources
	{
		private:
			MeshBuilderSharedPtr m_mesh_builder;
			MeshCacheSharedPtr m_mesh_cache;
			EffectCacheSharedPtr m_effect_cache;
			TextureCacheSharedPtr m_texture_cache;
			CodecManagerSharedPtr m_codec_manager;
			MeshDataCacheSharedPtr m_mesh_data_cache;
			ActorDataCacheSharedPtr m_actor_data_cache;
			ShaderSourceBuilderSharedPtr m_shader_source_builder;
			boost::scoped_ptr<Filter> m_filter_channels_1;
			boost::scoped_ptr<Filter> m_filter_channels_2;
			boost::scoped_ptr<Filter> m_filter_channels_3;
			boost::scoped_ptr<Filter> m_filter_channels_4;

		public:
			SceneResources(const GlobalVarsSharedPtr &global_vars,
				const FileSystemWeakPtr &file_system);
			~SceneResources() throw();
			void clear();
			void init();

			inline MeshBuilder &get_mesh_builder()
			{
				return *get_mesh_builder_ptr();
			}

			inline MeshCache &get_mesh_cache()
			{
				return *get_mesh_cache_ptr();
			}

			inline EffectCache &get_effect_cache()
			{
				return *get_effect_cache_ptr();
			}

			inline TextureCache &get_texture_cache()
			{
				return *get_texture_cache_ptr();
			}

			inline CodecManager &get_codec_manager()
			{
				return *get_codec_manager_ptr();
			}

			inline MeshDataCache &get_mesh_data_cache()
			{
				return *get_mesh_data_cache_ptr();
			}

			inline ActorDataCache &get_actor_data_cache()
			{
				return *get_actor_data_cache_ptr();
			}

			inline ShaderSourceBuilder &get_shader_source_builder()
			{
				return *get_shader_source_builder_ptr();
			}

			inline MeshBuilderSharedPtr &get_mesh_builder_ptr()
			{
				return m_mesh_builder;
			}

			inline MeshCacheSharedPtr &get_mesh_cache_ptr()
			{
				return m_mesh_cache;
			}

			inline EffectCacheSharedPtr &get_effect_cache_ptr()
			{
				return m_effect_cache;
			}

			inline TextureCacheSharedPtr &get_texture_cache_ptr()
			{
				return m_texture_cache;
			}

			inline CodecManagerSharedPtr &get_codec_manager_ptr()
			{
				return m_codec_manager;
			}

			inline MeshDataCacheSharedPtr
				&get_mesh_data_cache_ptr()
			{
				return m_mesh_data_cache;
			}

			inline ActorDataCacheSharedPtr
				&get_actor_data_cache_ptr()
			{
				return m_actor_data_cache;
			}

			inline ShaderSourceBuilderSharedPtr
				&get_shader_source_builder_ptr()
			{
				return m_shader_source_builder;
			}

			inline Filter &get_filter(const Uint16 channels) const
			{
				switch (channels)
				{
					case 1:
						return *m_filter_channels_1;
					case 2:
						return *m_filter_channels_2;
					case 3:
						return *m_filter_channels_3;
					case 4:
					default:
						return *m_filter_channels_4;
				}
			}

	};

}

#endif	/* UUID_90784f4e_4047_496d_ba28_81d920a6e71e */

