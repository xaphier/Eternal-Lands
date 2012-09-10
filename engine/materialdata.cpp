/****************************************************************************
 *            materialdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialdata.hpp"

namespace eternal_lands
{

	MaterialData::MaterialData(): m_cast_shadows(true), m_culling(true)
	{
		m_color = glm::vec4(0.0f);
		m_dudv_scale = glm::vec2(1.0f);
	}

	MaterialData::~MaterialData() noexcept
	{
	}

}
