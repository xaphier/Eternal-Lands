/****************************************************************************
 *            terrainpage.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainpage.hpp"
#include "abstractmesh.hpp"

namespace eternal_lands
{

	TerrainPage::TerrainPage(const ObjectData &object_data,
		const AbstractMeshSharedPtr &mesh,
		const MaterialEffectDescriptionVector &materials,
		const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const float min_z, const float max_z): Object(object_data, mesh,
			materials, effect_cache, texture_cache, 0),
		m_min_z(min_z), m_max_z(max_z)
	{
	}

	TerrainPage::~TerrainPage() throw()
	{
	}

	void TerrainPage::update_bounding_box()
	{
		BoundingBox bounding_box;
		glm::vec3 min, max;

		get_mesh()->get_bounding_box(get_world_matrix(),
			bounding_box);

		min = bounding_box.get_min();
		max = bounding_box.get_max();

		min.z = get_min_z();
		max.z = get_max_z();

		bounding_box.set_min_max(min, max);

		set_bounding_box(bounding_box);
	}

}
