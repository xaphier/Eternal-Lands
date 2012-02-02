/****************************************************************************
 *            actor.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "actor.hpp"
#include "abstractmesh.hpp"
#include "materialeffect.hpp"

namespace eternal_lands
{

	Actor::Actor(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialEffectDescriptionVector &materials,
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

		get_mesh()->init_indices(m_meshs, m_index_source);
	}

	void Actor::remove_mesh(const Uint32 id)
	{
		m_meshs.erase(id);

		get_mesh()->init_indices(m_meshs, m_index_source);
	}

	void Actor::init_enhanced_actor(
		const CodecManagerWeakPtr &codec_manager,
		const FileSystemWeakPtr &file_system,
		const GlobalVarsSharedPtr &global_vars)
	{
		m_actor_texture_builder =
			boost::make_shared<ActorTextureBuilder>(codec_manager,
				file_system, global_vars, get_name());

		get_modifiable_material_effects()[0].set_texture(
			m_actor_texture_builder->get_texture(), stt_albedo_0);
	}

	void Actor::set_parts(const ActorPartTextureTypeStringMap &parts)
	{
		EffectDescription effect;
		assert(m_actor_texture_builder.get() != 0);

		m_actor_texture_builder->set_parts(parts);
		m_actor_texture_builder->build_actor_images();
		m_actor_texture_builder->build_actor_texture();

		effect = get_material_effects()[0].get_effect_description();

		effect.set_transparent(
			m_actor_texture_builder->get_uses_alpha());

		get_modifiable_material_effects()[0].set_effect(effect);
	}

	void Actor::update_bounding_box()
	{
		BoundingBox bounding_box;
		glm::vec3 min, max;

		get_model()->getSkeleton()->getBoneBoundingBox(
			glm::value_ptr(min), glm::value_ptr(max));

		bounding_box = BoundingBox(min, max).transform(
			get_world_transform());

		bounding_box.extend(0.5f);

		set_bounding_box(bounding_box);
	}

	void Actor::update_bones()
	{
		glm::vec4 data;
		Uint32 i, count;
		const std::vector<CalBone *>& bones =
			get_model()->getSkeleton()->getVectorBone();

		count = bones.size();;

		for (i = 0; i < count; ++i)
		{
			const CalVector &translationBoneSpace =
				bones[i]->getTranslationBoneSpace();
			const CalMatrix &rotationMatrix =
				bones[i]->getTransformMatrix();

			data.x = rotationMatrix.dxdx;
			data.y = rotationMatrix.dxdy;
			data.z = rotationMatrix.dxdz;
			data.w = translationBoneSpace.x;
			set_bone(i * 3 + 0, data);
			data.x = rotationMatrix.dydx;
			data.y = rotationMatrix.dydy;
			data.z = rotationMatrix.dydz;
			data.w = translationBoneSpace.y;
			set_bone(i * 3 + 1, data);
			data.x = rotationMatrix.dzdx;
			data.y = rotationMatrix.dzdy;
			data.z = rotationMatrix.dzdz;
			data.w = translationBoneSpace.z;
			set_bone(i * 3 + 2, data);
		}
	}

}
