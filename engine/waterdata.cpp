/****************************************************************************
 *            waterdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "waterdata.hpp"

namespace eternal_lands
{

	WaterData::WaterData(): m_height(0.0f)
	{
	}

	WaterData::WaterData(const String &name, const String &material,
		const float height): m_name(name), m_material(material),
		m_height(height)
	{
	}

	WaterData::~WaterData() noexcept
	{
	}

}
