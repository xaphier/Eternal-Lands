/****************************************************************************
 *            ray.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "ray.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	Ray::Ray(): m_origin(0.0f), m_direction(0.0f, 0.0f, 1.0f)
	{
	}

	Ray::Ray(const glm::vec3 origin, const glm::vec3 direction):
		m_origin(origin), m_direction(direction)
	{
	}

	Ray::~Ray() throw()
	{
	}

}
