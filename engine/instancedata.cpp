/****************************************************************************
 *            instancedata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "instancedata.hpp"
#include "meshdatatool.hpp"
#include "materialdescription.hpp"
#include "subobject.hpp"

namespace eternal_lands
{

	InstanceData::InstanceData(const ObjectData &object_data,
		const MeshDataToolSharedPtr &mesh_data_tool,
		const StringVector &materials,
		const SubObjectVector &instanced_objects):
		ObjectDescription(object_data, materials),
		m_mesh_data_tool(mesh_data_tool),
		m_instanced_objects(instanced_objects)
	{
	}

	InstanceData::~InstanceData() throw()
	{
	}

}
