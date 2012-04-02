/****************************************************************************
 *            sceneresources.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "sceneresources.hpp"
#include "meshbuilder.hpp"
#include "meshcache.hpp"
#include "effectcache.hpp"
#include "meshdatacache.hpp"
#include "codec/codecmanager.hpp"
#include "texturecache.hpp"
#include "actordatacache.hpp"
#include "shader/shadersourcebuilder.hpp"
#include "filter.hpp"
#include "framebufferbuilder.hpp"
#include "materialdescriptioncache.hpp"
#include "materialcache.hpp"
#include "script/materialscriptcache.hpp"
#include "script/materialscriptmanager.hpp"
#include "script/scriptengine.hpp"

namespace eternal_lands
{

	SceneResources::SceneResources(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system)
	{
		m_mesh_builder = boost::make_shared<MeshBuilder>(global_vars);
		m_shader_source_builder =
			boost::make_shared<ShaderSourceBuilder>(global_vars);
		m_effect_cache = boost::make_shared<EffectCache>(
			get_shader_source_builder());
		m_codec_manager = boost::make_shared<CodecManager>();
		m_texture_cache = boost::make_shared<TextureCache>(
			get_codec_manager(), file_system, global_vars);
		m_material_description_cache =
			boost::make_shared<MaterialDescriptionCache>();
		m_material_cache = boost::make_shared<MaterialCache>(
			get_material_description_cache(),
			get_effect_cache(), get_texture_cache());
		m_mesh_data_cache = boost::make_shared<MeshDataCache>(
			file_system, global_vars);
		m_mesh_cache = boost::make_shared<MeshCache>(
			get_mesh_builder(), get_mesh_data_cache());
		m_actor_data_cache = boost::make_shared<ActorDataCache>(
			get_mesh_builder(), get_effect_cache(),
			get_texture_cache(), get_codec_manager(),
			get_material_cache(), get_material_description_cache(),
			file_system, global_vars);
		m_framebuffer_builder = boost::make_shared<FrameBufferBuilder>(
			global_vars);

		m_script_engine = boost::make_shared<ScriptEngine>(file_system);
		m_material_script_manager =
			boost::make_shared<MaterialScriptManager>(
				get_script_engine());
		m_material_script_cache =
			boost::make_shared<MaterialScriptCache>(
				get_script_engine());
	}

	SceneResources::~SceneResources() throw()
	{
	}

	void SceneResources::clear()
	{
		m_mesh_builder.reset();
		m_mesh_cache.reset();
		m_effect_cache.reset();
		m_texture_cache.reset();
		m_codec_manager.reset();
		m_mesh_data_cache.reset();
		m_actor_data_cache.reset();
		m_shader_source_builder.reset();
	}

	void SceneResources::init(const FileSystemSharedPtr &file_system)
	{
		m_shader_source_builder->load_xml(file_system, String(UTF8(
			"shaders/shaders.xml")));
		m_effect_cache->load_xml(file_system, String(UTF8(
			"shaders/effects.xml")));
		m_material_description_cache->load_xml(file_system, String(UTF8(
			"materials.xml")));
	}

}
