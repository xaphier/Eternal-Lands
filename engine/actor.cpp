/****************************************************************************
 *            actor.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "actor.hpp"
#include "abstractmesh.hpp"
#include "material.hpp"

namespace eternal_lands
{

	Actor::Actor(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialDescriptionVector &materials,
		const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const IndexUpdateSourceSharedPtr &index_source,
		CalCoreModel* core_model): Object(object_data, mesh,
			materials, effect_cache, texture_cache, core_model),
		m_index_source(index_source)
	{
	}

	Actor::~Actor() throw()
	{
	}

	void Actor::add_mesh(const Uint32 id)
	{
		m_meshs.insert(id);

		get_mesh()->update_indices(m_meshs, m_index_source);
	}

	void Actor::remove_mesh(const Uint32 id)
	{
		m_meshs.erase(id);

		get_mesh()->update_indices(m_meshs, m_index_source);
	}

	void Actor::init_enhanced_actor(
		const CodecManagerWeakPtr &codec_manager,
		const FileSystemWeakPtr &file_system)
	{
		m_actor_texture_builder =
			boost::make_shared<ActorTextureBuilder>(codec_manager,
				file_system, get_name());

		get_modifiable_materials()[0].set_texture(
			m_actor_texture_builder->get_texture(), stt_diffuse_0);
	}

	void Actor::set_parts(const ActorPartTextureTypeStringMap &parts)
	{
		assert(m_actor_texture_builder.get() != 0);

		m_actor_texture_builder->set_parts(parts);
		m_actor_texture_builder->build_actor_images();
		m_actor_texture_builder->build_actor_texture();

		if (m_actor_texture_builder->get_uses_alpha())
		{
			get_modifiable_materials()[0].set_effect(
				String(L"animated.transparent"));
		}
		else
		{
			get_modifiable_materials()[0].set_effect(
				String(L"animated.solid"));
		}
	}

}
