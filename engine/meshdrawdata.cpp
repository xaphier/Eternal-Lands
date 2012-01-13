/****************************************************************************
 *            meshdrawdata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "meshdrawdata.hpp"

namespace eternal_lands
{

	MeshDrawData::MeshDrawData(const Uint32 offset, const Uint32 count,
		const Uint32 min_vertex, const Uint32 max_vertex,
		const Sint32 base_vertex): m_offset(offset), m_count(count),
		m_min_vertex(min_vertex), m_max_vertex(max_vertex),
		m_base_vertex(base_vertex)
	{
	}

	MeshDrawData::MeshDrawData(): m_offset(0), m_count(0), m_min_vertex(0),
		m_max_vertex(0), m_base_vertex(0)
	{
	}

	MeshDrawData::~MeshDrawData() throw()
	{
	}

	OutStream& operator<<(OutStream &str, const MeshDrawData &value)
	{
		str << "[offset]: " << value.get_offset() << std::endl;
		str << "[count]: " << value.get_count() << std::endl;
		str << "[min_vertex]: " << value.get_min_vertex() << std::endl;
		str << "[max_vertex]: " << value.get_max_vertex() << std::endl;
		str << "[base_vertex]: " << value.get_base_vertex() << std::endl;

		return str;
	}

}
