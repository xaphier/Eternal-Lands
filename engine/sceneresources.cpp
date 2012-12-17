/****************************************************************************
 *            sceneresources.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "sceneresources.hpp"
#include "meshbuilder.hpp"
#include "meshcache.hpp"
#include "effect/effectcache.hpp"
#include "meshdatacache.hpp"
#include "texturecache.hpp"
#include "actordatacache.hpp"
#include "shader/shadersourcebuilder.hpp"
#include "filter.hpp"
#include "framebufferbuilder.hpp"
#include "materialdescriptioncache.hpp"
#include "materialbuilder.hpp"
#include "materialcache.hpp"
#include "shader/glslprogramcache.hpp"
#include "shader/uniformbufferdescriptioncache.hpp"
#include "script/materialscriptcache.hpp"
#include "script/materialscriptmanager.hpp"
#include "script/scriptengine.hpp"
#include "hardwarebuffer/hardwarebuffermapper.hpp"
#include "terrainbuilder.hpp"
#include "thread/threadpool.hpp"
#include "tilebuilder.hpp"

namespace eternal_lands
{

	SceneResources::SceneResources(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system)
	{
		m_thread_pool = boost::make_shared<ThreadPool>(4);
		m_script_engine = boost::make_shared<ScriptEngine>(file_system);
		m_material_script_cache =
			boost::make_shared<MaterialScriptCache>(
				get_script_engine());
		m_material_script_manager =
			boost::make_shared<MaterialScriptManager>(
				get_script_engine());
		m_hardware_buffer_mapper =
			boost::make_shared<HardwareBufferMapper>(global_vars);
		m_mesh_builder = boost::make_shared<MeshBuilder>(global_vars,
			get_hardware_buffer_mapper());
		m_uniform_buffer_description_cache =
			boost::make_shared<UniformBufferDescriptionCache>();
		m_shader_source_builder =
			boost::make_shared<ShaderSourceBuilder>(global_vars,
				file_system,
				get_uniform_buffer_description_cache());
		m_glsl_program_cache = boost::make_shared<GlslProgramCache>(
			get_uniform_buffer_description_cache());
		m_effect_cache = boost::make_shared<EffectCache>(
			get_glsl_program_cache(), get_shader_source_builder());
		m_texture_cache = boost::make_shared<TextureCache>(
			file_system, global_vars);
		m_material_description_cache =
			boost::make_shared<MaterialDescriptionCache>();
		m_material_builder = boost::make_shared<MaterialBuilder>(
			get_effect_cache(), get_texture_cache(),
			get_material_script_cache(),
			get_material_script_manager());
		m_material_cache = boost::make_shared<MaterialCache>(
			get_material_builder(),
			get_material_description_cache());
		m_mesh_data_cache = boost::make_shared<MeshDataCache>(
			file_system, global_vars);
		m_mesh_cache = boost::make_shared<MeshCache>(
			get_mesh_builder(), get_mesh_data_cache());
		m_actor_data_cache = boost::make_shared<ActorDataCache>(
			get_mesh_builder(), get_material_cache(),
			get_material_builder(),
			get_material_description_cache(), file_system,
			global_vars);
		m_framebuffer_builder = boost::make_shared<FrameBufferBuilder>(
			global_vars);
		m_terrain_builder = boost::make_shared<TerrainBuilder>(
			global_vars, get_effect_cache(),
			get_material_builder(), get_material_cache(),
			get_mesh_builder(), get_mesh_cache(),
			get_mesh_data_cache(), get_texture_cache());
		m_filter = boost::make_shared<Filter>(global_vars,
			get_glsl_program_cache(), get_mesh_cache());
		m_tile_builder = boost::make_shared<TileBuilder>(global_vars,
			get_mesh_builder(), get_material_cache());
	}

	SceneResources::~SceneResources() noexcept
	{
	}

	void SceneResources::clear()
	{
		m_mesh_builder.reset();
		m_mesh_cache.reset();
		m_effect_cache.reset();
		m_texture_cache.reset();
		m_mesh_data_cache.reset();
		m_actor_data_cache.reset();
		m_shader_source_builder.reset();
		m_framebuffer_builder.reset();
		m_material_builder.reset();
		m_material_cache.reset();
		m_material_description_cache.reset();
		m_material_script_cache.reset();
		m_material_script_manager.reset();
		m_script_engine.reset();
		m_terrain_builder.reset();
		m_glsl_program_cache.reset();
		m_uniform_buffer_description_cache.reset();
		m_hardware_buffer_mapper.reset();
		m_terrain_builder.reset();
		m_thread_pool.reset();
	}

	void SceneResources::init(const FileSystemSharedPtr &file_system)
	{
		m_shader_source_builder->load_xml(
			String(UTF8("shaders/shaders.xml")));

		m_shader_source_builder->load_shader_sources(
			String(UTF8("shaders/sources")));

		m_shader_source_builder->load_shader_sources(
			String(UTF8("shaders/terrains")));

		m_effect_cache->load_xml(file_system,
			String(UTF8("shaders/effects")));

		m_material_description_cache->load_xml(file_system,
			String(UTF8("materials.xml")));
	}

}
