/****************************************************************************
 *            polygon.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "polygon.hpp"
#include "exceptions.hpp"
#include "submesh.hpp"

namespace eternal_lands
{

	Polygon::Polygon()
	{
		m_vertices.reserve(4);
	}

	Polygon::~Polygon() noexcept
	{
	}

	void Polygon::remove_duplicates()
	{
		Uint32 i;

		for (i = 0; i < get_vertex_count(); ++i)
		{
			if (glm::all(glm::equalEpsilon(get_vertex(i),
				get_vertex((i + 1) % get_vertex_count()),
				epsilon)))
			{
				remove_vertex(i);
				--i;
			}
		}
	}

	void Polygon::transform(const glm::mat4x3 &matrix)
	{
		BOOST_FOREACH(glm::vec3 &vertex, m_vertices)
		{
			vertex = matrix * glm::vec4(vertex, 1.0f);
		}
	}

	Polygon Polygon::get_transformed(const glm::mat4x3 &matrix) const
	{
		Polygon result;

		result = *this;

		result.transform(matrix);

		return result;
	}

}
