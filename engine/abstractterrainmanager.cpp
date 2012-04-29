/****************************************************************************
 *            abstractterrainmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractterrainmanager.hpp"

namespace eternal_lands
{

	namespace
	{

		const glm::vec3 terrain_offset_scale = glm::vec3(4.0f, 4.0f,
			32.0f);

	}

	AbstractTerrainManager::AbstractTerrainManager()
	{
	}

	AbstractTerrainManager::~AbstractTerrainManager() throw()
	{
	}

	const glm::vec3 &AbstractTerrainManager::get_terrain_offset_scale()
	{
		return terrain_offset_scale;
	}

}
