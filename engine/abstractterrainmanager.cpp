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

		const glm::vec3 vector_scale = glm::vec3(4.0f, 4.0f, 32.0f);
		const glm::vec2 position_scales = glm::vec2(0.25f);

	}

	AbstractTerrainManager::AbstractTerrainManager()
	{
	}

	AbstractTerrainManager::~AbstractTerrainManager() throw()
	{
	}

	const glm::vec3 &AbstractTerrainManager::get_vector_scale()
	{
		return vector_scale;
	}

	const glm::vec2 &AbstractTerrainManager::get_position_scale()
	{
		return position_scales;
	}

}
