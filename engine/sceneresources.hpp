/****************************************************************************
 *            sceneresources.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			TerrainBuilderSharedPtr m_terrain_builder;
			GlslProgramCacheSharedPtr m_glsl_program_cache;
			UniformBufferDescriptionCacheSharedPtr
				m_uniform_buffer_description_cache;
			HardwareBufferMapperSharedPtr m_hardware_buffer_mapper;
			ThreadPoolSharedPtr m_thread_pool;
			FilterSharedPtr m_filter;
			TileBuilderSharedPtr m_tile_builder;
			WalkHeightBuilderSharedPtr m_walk_height_builder;

		public:
			SceneResources(
				const GlobalVarsConstSharedPtr &global_vars,
				const FileSystemConstSharedPtr &file_system);
			~SceneResources() noexcept;
			void clear();
			void init(const FileSystemConstSharedPtr &file_system);

			inline const MeshBuilderSharedPtr &get_mesh_builder()
				noexcept
			{
				return m_mesh_builder;
			}

			inline const MeshCacheSharedPtr &get_mesh_cache()
				noexcept
			{
				return m_mesh_cache;
			}

			inline const EffectCacheSharedPtr &get_effect_cache()
				noexcept
			{
				return m_effect_cache;
			}

			inline const TextureCacheSharedPtr &get_texture_cache()
				noexcept
			{
				return m_texture_cache;
			}

			inline const MeshDataCacheSharedPtr
				&get_mesh_data_cache() noexcept
			{
				return m_mesh_data_cache;
			}

			inline const ActorDataCacheSharedPtr
				&get_actor_data_cache() noexcept
			{
				return m_actor_data_cache;
			}

			inline const ShaderSourceBuilderSharedPtr
				&get_shader_source_builder() noexcept
			{
				return m_shader_source_builder;
			}

			inline const FrameBufferBuilderSharedPtr
				&get_framebuffer_builder() noexcept
			{
				return m_framebuffer_builder;
			}

			inline const MaterialBuilderSharedPtr
				&get_material_builder() noexcept
			{
				return m_material_builder;
			}

			inline const MaterialCacheSharedPtr
				&get_material_cache() noexcept
			{
				return m_material_cache;
			}

			inline const MaterialDescriptionCacheSharedPtr
				&get_material_description_cache() noexcept
			{
				return m_material_description_cache;
			}

			inline const MaterialScriptCacheSharedPtr
				&get_material_script_cache() noexcept
			{
				return m_material_script_cache;
			}

			inline const MaterialScriptManagerSharedPtr
				&get_material_script_manager() noexcept
			{
				return m_material_script_manager;
			}

			inline const ScriptEngineSharedPtr
				&get_script_engine() noexcept
			{
				return m_script_engine;
			}

			inline const GlslProgramCacheSharedPtr
				&get_glsl_program_cache() noexcept
			{
				return m_glsl_program_cache;
			}

			inline const UniformBufferDescriptionCacheSharedPtr
				&get_uniform_buffer_description_cache() noexcept
			{
				return m_uniform_buffer_description_cache;
			}

			inline const HardwareBufferMapperSharedPtr
				&get_hardware_buffer_mapper() noexcept
			{
				return m_hardware_buffer_mapper;
			}

			inline const TerrainBuilderSharedPtr
				&get_terrain_builder() noexcept
			{
				return m_terrain_builder;;
			}

			inline const ThreadPoolSharedPtr &get_thread_pool()
				noexcept
			{
				return m_thread_pool;
			}

			inline const FilterSharedPtr &get_filter() noexcept
			{
				return m_filter;
			}

			inline const TileBuilderSharedPtr &get_tile_builder()
				noexcept
			{
				return m_tile_builder;
			}

			inline const WalkHeightBuilderSharedPtr
				&get_walk_height_builder() noexcept
			{
				return m_walk_height_builder;
			}

	};

}

#endif	/* UUID_90784f4e_4047_496d_ba28_81d920a6e71e */
