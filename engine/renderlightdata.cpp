/****************************************************************************
 *            renderlightdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "renderlightdata.hpp"
#include "light.hpp"

namespace eternal_lands
{

	RenderLightData::RenderLightData(): m_camera_inside(false)
	{
	}

	RenderLightData::RenderLightData(const LightSharedPtr &light,
		const bool camera_inside): m_light(light),
		m_camera_inside(camera_inside)
	{
	}

	RenderLightData::~RenderLightData() noexcept
	{
	}

}
