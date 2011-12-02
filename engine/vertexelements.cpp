/****************************************************************************
 *            vertexelements.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "vertexelements.hpp"

namespace eternal_lands
{

	namespace
	{

		Uint32 get_vertex_elements_stride(
			const VertexDescriptionMap &vertex_elements)
		{
			VertexDescriptionMap::const_iterator it, end;
			Uint32 result;

			result = 0;
			end = vertex_elements.end();

			for (it = vertex_elements.begin(); it != end; ++it)
			{
				result += VertexElement::get_size(it->second);
			}

			return result;
		}

		VertexElementVector build_vertex_elements(
			const VertexDescriptionMap &vertex_elements)
		{
			VertexElementVector result;
			VertexDescriptionMap::const_iterator it, end;
			Uint32 offset;

			offset = 0;
			end = vertex_elements.end();

			for (it = vertex_elements.begin(); it != end; ++it)
			{
				result.push_back(VertexElement(it->first,
					it->second, offset));

				offset += VertexElement::get_size(it->second);
			}

			return result;
		}

	}

	VertexElements::VertexElements(): m_stride(0), m_divisor(0)
	{
	}

	VertexElements::VertexElements(
		const VertexElementVector &vertex_elements,
		const Uint16 stride, const Uint16 divisor):
		m_vertex_elements(vertex_elements), m_stride(stride),
		m_divisor(divisor)
	{
	}

	VertexElements::VertexElements(
		const VertexDescriptionMap &vertex_elements,
		const Uint16 divisor):
		m_vertex_elements(build_vertex_elements(vertex_elements)),
		m_stride(get_vertex_elements_stride(vertex_elements)),
		m_divisor(divisor)
	{
	}

	VertexElements::~VertexElements() throw()
	{
	}

}

