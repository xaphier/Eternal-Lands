/****************************************************************************
 *            terrainvisitor.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainvisitor.hpp"

namespace eternal_lands
{

	TerrainVisitor::TerrainVisitor(
		const MappedUniformBufferSharedPtr &mapped_uniform_buffer):
		m_mapped_uniform_buffer(mapped_uniform_buffer), m_instances(0)
	{
	}

	TerrainVisitor::~TerrainVisitor() noexcept
	{
	}

}
