/****************************************************************************
 *            instancingdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "instancingdata.hpp"
#include "meshdatatool.hpp"
#include "materialdescription.hpp"
#include "meshdatacache.hpp"

namespace eternal_lands
{

	InstancingData::InstancingData(
		const MeshDataCacheSharedPtr &mesh_data_cache,
		const ObjectDescription &object_description):
		ObjectDescription(object_description)
	{
		StringVector material_names;
		Uint32 i, count;

		mesh_data_cache->get_mesh_data(get_name(), m_mesh_data_tool,
			material_names);

		count = std::min(material_names.size(),
			object_description.get_material_names().size());

		for (i = 0; i < count; ++i)
		{
			if (object_description.get_material_names()[i].get(
				).empty())
			{
				continue;
			}

			material_names[i] =
				object_description.get_material_names()[i];
		}

		set_material_names(material_names);

		m_mesh_data_tool->get_bounding_box(get_world_transformation(),
			m_bounding_box);
	}

	InstancingData::~InstancingData() throw()
	{
	}

}
