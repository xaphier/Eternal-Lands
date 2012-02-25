/****************************************************************************
 *            particledata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "particledata.hpp"

namespace eternal_lands
{

	ParticleData::ParticleData(const glm::vec3 &position,
		const String &name, const Uint32 id): m_position(position),
		m_name(name), m_id(id)
	{
	}

	ParticleData::~ParticleData() throw()
	{
	}

}
