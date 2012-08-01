/****************************************************************************
 *            object.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "object.hpp"
#include "instancedata.hpp"
#include "abstractmesh.hpp"
#include "subobject.hpp"
#include "material.hpp"
#include "materialdescription.hpp"

namespace eternal_lands
{

	Object::Object(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialSharedPtrVector &materials):
		m_object_data(object_data), m_mesh(mesh), m_materials(materials)
	{
		assert(materials.size() > 0);

		update_bounding_box();
	}

	Object::Object(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialSharedPtrVector &materials,
		const LodData &lod_data): m_object_data(object_data),
		m_mesh(mesh), m_materials(materials)
	{
		assert(materials.size() > 0);

		update_bounding_box();
	}

	Object::Object(const InstanceData &instance_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialSharedPtrVector &materials,
		const SubObjectVector &sub_objects):
		m_object_data(instance_data), m_mesh(mesh),
		m_materials(materials), m_sub_objects(sub_objects)
	{
		assert(get_sub_objects().size() > 0);
		assert(materials.size() > 0);

		update_bounding_box();
	}

	Object::Object(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialSharedPtrVector &materials,
		CalCoreModel* core_model): m_object_data(object_data),
		m_mesh(mesh), m_materials(materials)
	{
		assert(materials.size() > 0);

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
