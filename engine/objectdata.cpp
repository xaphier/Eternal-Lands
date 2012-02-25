/****************************************************************************
 *            objectdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "objectdata.hpp"

namespace eternal_lands
{

	ObjectData::ObjectData(): m_transparency(1.0f), m_id(0),
		m_selection(st_none), m_stencil_value(0)
	{
	}

	ObjectData::ObjectData(const Transformation &world_transformation,
		const String &name, const float transparency, const Uint32 id,
		const SelectionType selection, const bool blend,
		const bool stencil, const Uint8 stencil_value):
		m_world_transformation(world_transformation), m_name(name),
		m_transparency(transparency), m_id(id), m_selection(selection),
		m_stencil_value(stencil_value)
	{
		set_state_blend(blend);
		set_state_stencil(stencil);
	}

	ObjectData::~ObjectData() throw()
	{
	}

}
