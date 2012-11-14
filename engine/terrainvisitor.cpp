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
		const AbstractWriteMemorySharedPtr &buffer,
		const Uint64 offset, const Uint16 max_instances):
		m_buffer(buffer), m_offset(offset), m_instances(0),
		m_max_instances(max_instances)
	{
	}

	TerrainVisitor::~TerrainVisitor() noexcept
	{
	}

}
