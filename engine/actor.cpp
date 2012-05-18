/****************************************************************************
 *            actor.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "actor.hpp"
#include "abstractmesh.hpp"
#include "material.hpp"
#include "indexupdatesource.hpp"
#include "submesh.hpp"

namespace eternal_lands
{

	Actor::Actor(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialSharedPtrVector &materials,
		const IndexUpdateSourceSharedPtr &index_source,
		CalCoreModel* core_model): Object(object_data, mesh,
			materials, core_model), m_index_source(index_source)
	{
	}

	Actor::Actor(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialSharedPtrVector &materials,
		CalCoreModel* core_model): Object(object_data, mesh,
			materials, core_model)
	{
	}

	Actor::~Actor() noexcept
	{
	}

	void Actor::add_mesh(const Uint32 id)
	{
		if (m_index_source.get() != nullptr)
		{
			m_meshs.insert(id);

			m_index_source->write_index_buffer(m_meshs,
				get_mesh()->get_index_buffer());

			get_mesh()->set_sub_meshs(
				m_index_source->get_sub_meshs(m_meshs));
		}
	}

	void Actor::remove_mesh(const Uint32 id)
	{
		if (m_index_source.get() != nullptr)
		{
			m_meshs.erase(id);

			m_index_source->write_index_buffer(m_meshs,
				get_mesh()->get_index_buffer());

			get_mesh()->set_sub_meshs(
				m_index_source->get_sub_meshs(m_meshs));
		}
	}

	void Actor::init_enhanced_actor(
		const CodecManagerWeakPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars)
	{
		assert(m_index_source.get() != nullptr);

		m_actor_texture_builder =
			boost::make_shared<ActorTextureBuilder>(codec_manager,
				file_system, global_vars, get_name());

		get_materials()[0]->set_texture(
			m_actor_texture_builder->get_texture(), spt_albedo_0);
	}

	void Actor::set_parts(const ActorPartTextureTypeStringMap &parts)
	{
		assert(m_actor_texture_builder.get() != nullptr);

		m_actor_texture_builder->set_parts(parts);
		m_actor_texture_builder->build_actor_images();
		m_actor_texture_builder->build_actor_texture();

		get_materials()[0]->set_effect(
			m_actor_texture_builder->get_effect());
	}

	void Actor::update_bounding_box()
	{
		BoundingBox bounding_box;
		glm::vec3 min, max;

		get_model()->getSkeleton()->getBoneBoundingBox(
			glm::value_ptr(min), glm::value_ptr(max));

		bounding_box = BoundingBox(min, max).transform(
			get_world_transformation());

		bounding_box.extend(0.5f);

		set_bounding_box(bounding_box);
	}

	void Actor::update_bones()
	{
		glm::mat2x4 data;
		glm::quat nondual, dual;
		Uint32 i, count;
		const std::vector<CalBone *>& bones =
			get_model()->getSkeleton()->getVectorBone();

		count = bones.size();;

		for (i = 0; i < count; ++i)
		{
			const CalVector &translation =
				bones[i]->getTranslationBoneSpace();
			const CalQuaternion &rotation =
				bones[i]->getRotationBoneSpace();

			data[0].x = -rotation.x;
			data[0].y = -rotation.y;
			data[0].z = -rotation.z;
			data[0].w = rotation.w;

			data[1].w = -0.5f * ( translation.x * -rotation.x + translation.y * -rotation.y + translation.z * -rotation.z);
			data[1].x =  0.5f * ( translation.x * rotation.w + translation.y * -rotation.z - translation.z * -rotation.y);
			data[1].y =  0.5f * (-translation.x * -rotation.z + translation.y * rotation.w + translation.z * -rotation.x);
			data[1].z =  0.5f * ( translation.x * -rotation.y - translation.y * -rotation.x + translation.z * rotation.w);

			set_bone(i, data);
		}
	}

}
