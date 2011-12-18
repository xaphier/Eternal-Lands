/****************************************************************************
 *            vertexformat.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
		const VertexElementsArray16 &vertex_elements):
		m_vertex_elements(vertex_elements), m_name(name)
	{
	}

	VertexFormat::~VertexFormat() throw()
	{
	}

}

