/****************************************************************************
 *            subobject.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "subobject.hpp"

namespace eternal_lands
{

	SubObject::SubObject(): MeshDrawData(), m_id(0), m_material(0),
		m_selection(st_none)
	{
	}

	SubObject::SubObject(const MeshDrawData &mesh_draw, const Uint32 id,
		const Uint32 material, SelectionType selection):
		MeshDrawData(mesh_draw), m_id(id), m_material(material),
		m_selection(selection)
	{
	}

	SubObject::~SubObject() throw()
	{
	}

}
