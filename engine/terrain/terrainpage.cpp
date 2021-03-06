/****************************************************************************
 *            terrainpage.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainpage.hpp"
#include "abstractmesh.hpp"

namespace eternal_lands
{

	TerrainPage::TerrainPage(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialSharedPtrVector &materials,
		const float min_z, const float max_z): Object(object_data, mesh,
			materials, 0), m_min_z(min_z), m_max_z(max_z)
	{
	}

	TerrainPage::~TerrainPage() noexcept
	{
	}

	void TerrainPage::update_bounding_box()
	{
		BoundingBox bounding_box;
		glm::vec3 min, max;

		get_mesh()->get_bounding_box(Transformation(), bounding_box);

		min = bounding_box.get_min();
		max = bounding_box.get_max();

		min.z = get_min_z();
		max.z = get_max_z();

		bounding_box.set_min_max(min, max);

		set_bounding_box(bounding_box.transform(
			get_world_transformation()));
	}

}
