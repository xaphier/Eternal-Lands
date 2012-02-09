/****************************************************************************
 *            objectdata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "objectdata.hpp"

namespace eternal_lands
{

	ObjectData::ObjectData(): m_transparency(1.0f), m_id(0),
		m_selection(st_none), m_blend(false)
	{
	}

	ObjectData::ObjectData(const Transformation &world_transformation,
		const String &name, const float transparency, const Uint32 id,
		const SelectionType selection, const bool blend):
		m_world_transformation(world_transformation), m_name(name),
		m_transparency(transparency), m_id(id), m_selection(selection),
		m_blend(blend)
	{
	}

	ObjectData::~ObjectData() throw()
	{
	}

}
