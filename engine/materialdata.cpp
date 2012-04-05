/****************************************************************************
 *            materialdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialdata.hpp"

namespace eternal_lands
{

	MaterialData::MaterialData(): m_cast_shadows(true), m_culling(true)
	{
		Uint16 i;

		for (i = 0; i < 4; ++i)
		{
			m_albedo_scale_offsets[i][0] = glm::vec4(1.0f);
			m_albedo_scale_offsets[i][1] = glm::vec4(0.0f);
		}

		m_emission_scale_offset[0] = glm::vec3(1.0f);
		m_emission_scale_offset[1] = glm::vec3(0.0f);
		m_specular_scale_offset = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
	}

	MaterialData::~MaterialData() throw()
	{
	}

}
