/****************************************************************************
 *            object.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "object.hpp"
#include "instancedata.hpp"
#include "abstractmesh.hpp"
#include "subobject.hpp"
#include "materialeffect.hpp"
#include "materialeffectdescription.hpp"

namespace eternal_lands
{

	Object::Object(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialEffectDescriptionVector &materials,
		const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache):
		m_object_data(object_data), m_mesh(mesh)
	{
		update_bounding_box();

		m_material_effects.clear();
		m_material_effects.reserve(materials.size());

		BOOST_FOREACH(const MaterialEffectDescription &material,
			materials)
		{
			m_material_effects.push_back(MaterialEffect(
				effect_cache, texture_cache, material));
		}
	}

	Object::Object(const InstanceData &instance_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialEffectDescriptionVector &materials,
		const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const SubObjectVector &sub_objects):
		m_object_data(instance_data), m_mesh(mesh),
		m_sub_objects(sub_objects)
	{
		assert(get_sub_objects().size() > 0);

		m_material_effects.clear();
		m_material_effects.reserve(materials.size());

		BOOST_FOREACH(const MaterialEffectDescription &material,
			materials)
		{
			m_material_effects.push_back(MaterialEffect(
				effect_cache, texture_cache, material));
		}

		update_bounding_box();
	}

	Object::Object(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialEffectDescriptionVector &materials,
		const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		CalCoreModel* core_model): m_object_data(object_data),
		m_mesh(mesh)
	{
		m_material_effects.clear();
		m_material_effects.reserve(materials.size());

		BOOST_FOREACH(const MaterialEffectDescription &material,
			materials)
		{
			m_material_effects.push_back(MaterialEffect(
				effect_cache, texture_cache, material));
		}

		m_model.reset(new CalModel(core_model));

		m_bones.resize(3 *
			m_model->getSkeleton()->getVectorBone().size());

		update_skeleton_bounding_box(0.5f);
	}

	Object::~Object() throw()
	{
	}

	void Object::update_bounding_box()
	{
		BoundingBox bounding_box;

		get_mesh()->get_bounding_box(get_world_matrix(),
			bounding_box);
		set_bounding_box(bounding_box);
	}

	void Object::update_skeleton_bounding_box(const float extend)
	{
		BoundingBox bounding_box;
		glm::vec3 min, max;

		m_model->getSkeleton()->getBoneBoundingBox(glm::value_ptr(min),
			glm::value_ptr(max));

		bounding_box = BoundingBox(min, max).transform(
			get_world_matrix());

		bounding_box.extend(extend);

		set_bounding_box(bounding_box);
	}

	void Object::update_bones()
	{
		Uint32 i, count;
		const std::vector<CalBone *>& bones =
			m_model->getSkeleton()->getVectorBone();

		count = bones.size();;

		for (i = 0; i < count; ++i)
		{
			const CalVector &translationBoneSpace =
				bones[i]->getTranslationBoneSpace();
			const CalMatrix &rotationMatrix =
				bones[i]->getTransformMatrix();

			m_bones[i * 3 + 0].x = rotationMatrix.dxdx;
			m_bones[i * 3 + 0].y = rotationMatrix.dxdy;
			m_bones[i * 3 + 0].z = rotationMatrix.dxdz;
			m_bones[i * 3 + 0].w = translationBoneSpace.x;
			m_bones[i * 3 + 1].x = rotationMatrix.dydx;
			m_bones[i * 3 + 1].y = rotationMatrix.dydy;
			m_bones[i * 3 + 1].z = rotationMatrix.dydz;
			m_bones[i * 3 + 1].w = translationBoneSpace.y;
			m_bones[i * 3 + 2].x = rotationMatrix.dzdx;
			m_bones[i * 3 + 2].y = rotationMatrix.dzdy;
			m_bones[i * 3 + 2].z = rotationMatrix.dzdz;
			m_bones[i * 3 + 2].w = translationBoneSpace.z;
		}
	}

}
