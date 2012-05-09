/****************************************************************************
 *            sceneresources.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			MaterialBuilderSharedPtr m_material_builder;
			MaterialCacheSharedPtr m_material_cache;
			MaterialDescriptionCacheSharedPtr
				m_material_description_cache;
			MaterialScriptCacheSharedPtr m_material_script_cache;
			MaterialScriptManagerSharedPtr
				m_material_script_manager;
			ScriptEngineSharedPtr m_script_engine;

		public:
			SceneResources(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);
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

			inline const MaterialBuilderSharedPtr
				&get_material_builder() const
			{
				return m_material_builder;
			}

			inline const MaterialCacheSharedPtr
				&get_material_cache() const
			{
				return m_material_cache;
			}

			inline const MaterialDescriptionCacheSharedPtr
				&get_material_description_cache() const
			{
				return m_material_description_cache;
			}

			inline const MaterialScriptCacheSharedPtr
				&get_material_script_cache() const
			{
				return m_material_script_cache;
			}

			inline const MaterialScriptManagerSharedPtr
				&get_material_script_manager() const
			{
				return m_material_script_manager;
			}

			inline const ScriptEngineSharedPtr
				&get_script_engine() const
			{
				return m_script_engine;
			}

	};

}

#endif	/* UUID_90784f4e_4047_496d_ba28_81d920a6e71e */
