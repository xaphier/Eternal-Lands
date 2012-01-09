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
			FrameBufferBuilderSharedPtr m_framebuffer_builder;
			boost::scoped_ptr<Filter> m_filter;

		public:
			SceneResources(const GlobalVarsSharedPtr &global_vars,
				const FileSystemWeakPtr &file_system);
			~SceneResources() throw();
			void clear();
			void init(const FileSystemSharedPtr &file_system);

			inline const MeshBuilderSharedPtr &get_mesh_builder()
				const
			{
				return m_mesh_builder;
			}

			inline const MeshCacheSharedPtr &get_mesh_cache() const
			{
				return m_mesh_cache;
			}

			inline const EffectCacheSharedPtr &get_effect_cache()
				const
			{
				return m_effect_cache;
			}

			inline const TextureCacheSharedPtr &get_texture_cache()
				const
			{
				return m_texture_cache;
			}

			inline const CodecManagerSharedPtr &get_codec_manager()
				const
			{
				return m_codec_manager;
			}

			inline const MeshDataCacheSharedPtr
				&get_mesh_data_cache() const
			{
				return m_mesh_data_cache;
			}

			inline const ActorDataCacheSharedPtr
				&get_actor_data_cache() const
			{
				return m_actor_data_cache;
			}

			inline const ShaderSourceBuilderSharedPtr
				&get_shader_source_builder() const
			{
				return m_shader_source_builder;
			}

			inline const FrameBufferBuilderSharedPtr
				&get_framebuffer_builder() const
			{
				return m_framebuffer_builder;
			}

			inline Filter &get_filter() const
			{
				return *m_filter;
			}

	};

}

#endif	/* UUID_90784f4e_4047_496d_ba28_81d920a6e71e */
