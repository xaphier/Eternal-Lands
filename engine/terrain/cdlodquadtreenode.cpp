/****************************************************************************
 *			cdlodquadtreenode.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cdlodquadtreenode.hpp"

namespace eternal_lands
{

	CdLodQuadTreeNode::CdLodQuadTreeNode(const ImageSharedPtr height_map,
		const glm::vec3 &scale, const glm::vec3 &offset, const Uint16 x,
		const Uint16 y, const Uint16 size, const Uint16 level):
		m_x(x), m_y(y), m_size(size), m_level(level)
	{
	}

	CdLodQuadTreeNode::~CdLodQuadTreeNode() throw()
	{
	}

	glm::vec3 CdLodQuadTreeNode::get_world_min(const glm::vec3 &offset,
		const glm::vec3 &scale, const glm::vec2 &raster_size) const
	{
		glm::vec3 result;

		result = offset;

		result.x += get_x() * scale.x / raster_size.x;
		result.y += get_y() * scale.y / raster_size.y;
		result.z += (get_min_z() * scale.z) / 65535.0f;

		return result;
	}

	glm::vec3 CdLodQuadTreeNode::get_world_max(const glm::vec3 &offset,
		const glm::vec3 &scale, const glm::vec2 &raster_size) const
	{
		glm::vec3 result;

		result = offset;

		result.x += (get_x() + get_size()) * scale.x / raster_size.x;
		result.y += (get_y() + get_size()) * scale.y / raster_size.y;
		result.z += (get_max_z() * scale.z) / 65535.0f;

		return result;
	}

	void CdLodQuadTreeNode::get_world_bounding_box(const glm::vec3 &offset,
		const glm::vec3 &scale, const glm::vec2 &raster_size) const
	{
	}

}
