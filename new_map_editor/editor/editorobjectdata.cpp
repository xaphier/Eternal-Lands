/****************************************************************************
 *            editorobjectdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editorobjectdata.hpp"

namespace eternal_lands
{

	EditorObjectData::EditorObjectData()
	{
	}

	EditorObjectData::EditorObjectData(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const StringVector &materials,
		const String &name, const float transparency, const float scale,
		const Uint32 id, const SelectionType selection,
		const BlendType blend): ObjectData(Transformation(), name,
			transparency, id, selection, blend),
		m_materials(materials), m_rotation_angles(rotation_angles)
	{
	}

	EditorObjectData::~EditorObjectData() throw()
	{
	}

}
