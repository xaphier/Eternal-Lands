/****************************************************************************
 *            editorobjectdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editorobjectdata.hpp"

namespace eternal_lands
{

	EditorObjectData::EditorObjectData(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const String &name,
		const float transparency, const float scale, const Uint32 id,
		const SelectionType selection, const bool blend):
		m_translation(translation), m_rotation_angles(rotation_angles),
		m_name(name), m_transparency(transparency), m_scale(scale),
		m_id(id), m_selection(selection), m_blend(blend)
	{
	}

	EditorObjectData::~EditorObjectData() throw()
	{
	}

}
