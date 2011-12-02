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

	ObjectData::ObjectData(const ObjectData &object_data):
		m_world_matrix(object_data.m_world_matrix),
		m_color(object_data.m_color), m_name(object_data.m_name),
		m_transparency(object_data.m_transparency),
		m_id(object_data.m_id), m_selection(object_data.m_selection),
		m_blend(object_data.m_blend)
	{
	}

	ObjectData::ObjectData(const glm::mat4x3 &world_matrix,
		const glm::vec4 &color, const String &name,
		const float transparency, const Uint32 id,
		const SelectionType selection, const bool blend):
		m_world_matrix(world_matrix), m_color(color), m_name(name),
		m_transparency(transparency), m_id(id), m_selection(selection),
		m_blend(blend)
	{
	}

	ObjectData::~ObjectData() throw()
	{
	}

}

