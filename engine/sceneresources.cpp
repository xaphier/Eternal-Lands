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

namespace eternal_lands
{

	SceneResources::SceneResources(const GlobalVarsSharedPtr &global_vars,
		const FileSystemWeakPtr &file_system)
	{
		m_mesh_builder = boost::make_shared<MeshBuilder>();
		m_shader_source_builder =
			boost::make_shared<ShaderSourceBuilder>(global_vars,
				file_system);
		m_effect_cache = boost::make_shared<EffectCache>(
			get_shader_source_builder_ptr(), file_system);
		m_codec_manager = boost::make_shared<CodecManager>();
		m_texture_cache = boost::make_shared<TextureCache>(
			get_codec_manager_ptr(), file_system);
		m_mesh_data_cache = boost::make_shared<MeshDataCache>(
			file_system);
		m_mesh_cache = boost::make_shared<MeshCache>(
			get_mesh_builder_ptr(), get_mesh_data_cache_ptr());
		m_actor_data_cache = boost::make_shared<ActorDataCache>(
			get_mesh_builder_ptr(), get_effect_cache_ptr(),
			get_texture_cache_ptr(), get_codec_manager_ptr(),
			file_system);
		m_filter.reset(new Filter(get_mesh_cache_ptr()));
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

	void SceneResources::init()
	{
		m_shader_source_builder->load(String(UTF8(
			"shaders/shaders.lua")));
		m_shader_source_builder->load_default(String(UTF8(
			"shaders/shaders.lua")));
	}

}
