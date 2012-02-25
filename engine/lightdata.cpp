/****************************************************************************
 *            lightdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "lightdata.hpp"

namespace eternal_lands
{

	LightData::LightData(const glm::vec3 &position, const glm::vec3 &color,
		const float radius, const Uint32 id): m_position(position),
		m_color(color), m_radius(radius), m_id(id)
	{
	}

	LightData::~LightData() throw()
	{
	}

}
