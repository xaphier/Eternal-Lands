/****************************************************************************
 *            instancingdata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
		const ObjectData &object_data): ObjectData(object_data)
	{
		mesh_data_cache->get_mesh_data(get_name(), m_mesh_data_tool,
			m_materials);

		m_mesh_data_tool->get_bounding_box(get_world_matrix(),
			m_bounding_box);
	}

	InstancingData::InstancingData(
		const MeshDataCacheSharedPtr &mesh_data_cache,
		const ObjectData &object_data,
		const MaterialDescriptionVector &materials):
		ObjectData(object_data), m_materials(materials)
	{
		mesh_data_cache->get_mesh_data(get_name(), m_mesh_data_tool);

		m_mesh_data_tool->get_bounding_box(get_world_matrix(),
			m_bounding_box);
	}

	InstancingData::~InstancingData() throw()
	{
	}

	Sint16Sint16Pair InstancingData::get_index(const float max_size) const
	{
		Sint16Sint16Pair result;

		result.first = std::floor(get_bounding_box().get_center().x /
			max_size);
		result.second = std::floor(get_bounding_box().get_center().y /
			max_size);

		return result;
	}

}
