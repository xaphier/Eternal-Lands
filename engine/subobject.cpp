/****************************************************************************
 *            subobject.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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

	OutStream& operator<<(OutStream &str, const SubObject &value)
	{
		str << "[offset]: " << value.get_offset() << std::endl;
		str << "[count]: " << value.get_count() << std::endl;
		str << "[min_vertex]: " << value.get_min_vertex() << std::endl;
		str << "[max_vertex]: " << value.get_max_vertex() << std::endl;
		str << "[base_vertex]: " << value.get_base_vertex() << std::endl;
		str << "[id]: " << value.get_id() << std::endl;
		str << "[material]: " << value.get_material() << std::endl;
		str << "[selection]: " << value.get_selection() << std::endl;

		return str;
	}

}
