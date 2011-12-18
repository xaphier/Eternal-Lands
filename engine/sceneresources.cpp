/****************************************************************************
 *            sceneresources.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
#include "texturearraycache.hpp"

namespace eternal_lands
{

	SceneResources::SceneResources(const GlobalVarsSharedPtr &global_vars,
		const FileSystemWeakPtr &file_system)
	{
		m_mesh_builder = boost::make_shared<MeshBuilder>(global_vars);
		m_shader_source_builder =
			boost::make_shared<ShaderSourceBuilder>(global_vars,
				file_system);
		m_effect_cache = boost::make_shared<EffectCache>(
			get_shader_source_builder(), file_system);
		m_texture_array_cache = boost::make_shared<TextureArrayCache>();
		m_codec_manager = boost::make_shared<CodecManager>();
		m_texture_cache = boost::make_shared<TextureCache>(
			get_codec_manager(), get_texture_array_cache(),
			file_system, global_vars);
		m_mesh_data_cache = boost::make_shared<MeshDataCache>(
			get_texture_array_cache(), file_system, global_vars);
		m_mesh_cache = boost::make_shared<MeshCache>(
			get_mesh_builder(), get_mesh_data_cache());
		m_actor_data_cache = boost::make_shared<ActorDataCache>(
			get_mesh_builder(), get_effect_cache(),
			get_texture_cache(), get_codec_manager(),
			file_system, global_vars);
		m_framebuffer_builder = boost::make_shared<FrameBufferBuilder>(
			global_vars);
		m_filter.reset(new Filter(get_mesh_cache(), global_vars));
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
		m_shader_source_builder->load(String(UTF8(
			"shaders/shaders.lua")));
		m_shader_source_builder->load_default(String(UTF8(
			"shaders/shaders.lua")));
		m_texture_array_cache->load_xml(file_system,
			String(UTF8("textures/arrays.xml")));
		m_texture_cache->add_texture_arrays();
	}

}
