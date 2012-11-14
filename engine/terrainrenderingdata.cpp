/****************************************************************************
 *            terrainrenderingdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainrenderingdata.hpp"

namespace eternal_lands
{

	TerrainRenderingData::TerrainRenderingData(): m_offset(0),
		m_instances(0), m_max_instances(0)
	{
	}

	TerrainRenderingData::~TerrainRenderingData() noexcept
	{
	}

}
