/****************************************************************************
 *            triangles.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "triangles.hpp"
#include "exceptions.hpp"
#include "submesh.hpp"

namespace eternal_lands
{

	bool Triangles::do_next_triangle()
	{
		if ((m_index + 3) > m_count)
		{
			return false;
		}

		m_current_indices[0] = m_indices[m_index + m_offset + 0] +
			m_base_vertex;
		m_current_indices[1] = m_indices[m_index + m_offset + 1] +
			m_base_vertex;
		m_current_indices[2] = m_indices[m_index + m_offset + 2] +
			m_base_vertex;
		m_index += 3;

		return true;
	}

	bool Triangles::get_degenerated_triangle() const
	{
		return (m_current_indices[0] == m_current_indices[1]) ||
			(m_current_indices[0] == m_current_indices[2]) ||
			(m_current_indices[1] == m_current_indices[2]);
	}

	bool Triangles::next_triangle()
	{
		Uint32 index;

		if ((m_primitive == pt_triangles) || (m_index == 0))
		{
			return do_next_triangle();
		}

		if ((m_index + 1) > m_count)
		{
			return false;
		}

		index = m_indices[m_index + m_offset];
		m_index++;

		if (m_use_restart_index && (index == m_restart_index))
		{
			m_flip_triangle = true;

			return do_next_triangle();
		}

		if (m_primitive == pt_triangle_fan)
		{
			m_current_indices[1] = m_current_indices[2];
			m_current_indices[2] = index + m_base_vertex;
		}
		else
		{
			if (m_flip_triangle)
			{
				m_current_indices[0] = m_current_indices[2];
				m_current_indices[2] = index + m_base_vertex;
			}
			else
			{
				m_current_indices[1] = m_current_indices[2];
				m_current_indices[2] = index + m_base_vertex;
			}

			m_flip_triangle = !m_flip_triangle;
		}

		if (get_degenerated_triangle())
		{
			return next_triangle();
		}

		return true;
	}

	Triangles::Triangles(const Uint32Vector &indices,
		const SubMeshVector &sub_meshs, const PrimitiveType primitive,
		const Uint32 restart_index, const bool use_restart_index):
		m_indices(indices), m_sub_meshs(sub_meshs), m_index(0),
		m_count(0), m_offset(0), m_base_vertex(0),
		m_restart_index(restart_index), m_primitive(primitive),
		m_use_restart_index(use_restart_index)
	{
	}

	Triangles::~Triangles() noexcept
	{
	}

	bool Triangles::start(const Uint32 sub_mesh_index,
		const bool use_base_vertex)
	{
		if (sub_mesh_index >= m_sub_meshs.size())
		{
			EL_THROW_EXCEPTION(RangeErrorException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(m_sub_meshs.size() - 1)
				<< errinfo_range_index(sub_mesh_index));
		}

		m_index = 0;
		m_count = m_sub_meshs[sub_mesh_index].get_count();
		m_offset = m_sub_meshs[sub_mesh_index].get_offset();
		m_flip_triangle = true;

		assert((m_count + m_offset) <= m_indices.size());

		if (use_base_vertex)
		{
			m_base_vertex = 0;
		}
		else
		{
			m_base_vertex =
				m_sub_meshs[sub_mesh_index].get_base_vertex();
		}

		return m_count >= 3;
	}

}
