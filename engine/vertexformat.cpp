/****************************************************************************
 *            vertexformat.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "vertexformat.hpp"

namespace eternal_lands
{

	VertexFormat::VertexFormat(const String &name,
		const VertexElements &vertex_elements): m_name(name)
	{
		m_vertex_elements[0] = vertex_elements;
	}

	VertexFormat::VertexFormat(const String &name,
		const VertexElementsVector &vertex_elements): m_name(name)
	{
		Uint32 i, count;

		count = std::min(m_vertex_elements.size(),
			vertex_elements.size());

		for (i = 0; i < count; ++i)
		{
			m_vertex_elements[i] = vertex_elements[i];
		}
	}

	VertexFormat::VertexFormat(const String &name,
		const VertexElementsArray &vertex_elements):
		m_vertex_elements(vertex_elements), m_name(name)
	{
	}

	VertexFormat::~VertexFormat() throw()
	{
	}

}
