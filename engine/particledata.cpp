/****************************************************************************
 *            particledata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "particledata.hpp"

namespace eternal_lands
{

	ParticleData::ParticleData(const glm::vec3 &position,
		const String &name): m_position(position), m_name(name)
	{
	}

	ParticleData::~ParticleData() throw()
	{
	}

}
