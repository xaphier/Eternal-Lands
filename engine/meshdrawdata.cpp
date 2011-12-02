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
		const Uint32 base_vertex): m_offset(offset), m_count(count),
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

}
