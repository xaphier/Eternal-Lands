/****************************************************************************
 *            object.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
		m_object_data(object_data), m_lod_data(materials.size()),
		m_mesh(mesh)
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

	Object::Object(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialEffectDescriptionVector &materials,
		const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const LodData &lod_data): m_object_data(object_data),
		m_lod_data(lod_data), m_mesh(mesh)
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
		m_object_data(instance_data), m_lod_data(materials.size()),
		m_mesh(mesh), m_sub_objects(sub_objects)
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
		m_lod_data(materials.size()), m_mesh(mesh)
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

		m_bones.resize(m_model->getSkeleton()->getVectorBone().size());
	}

	Object::~Object() throw()
	{
	}

	void Object::update_bounding_box()
	{
		BoundingBox bounding_box;

		get_mesh()->get_bounding_box(get_world_transformation(),
			bounding_box);
		set_bounding_box(bounding_box);
	}

	void Object::update_bones()
	{
	}

}
