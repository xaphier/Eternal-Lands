/****************************************************************************
 *            indexupdatesource.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "indexupdatesource.hpp"
#include "submesh.hpp"

namespace eternal_lands
{

	IndexUpdateSource::IndexUpdateSource(const Uint32Vector &indices,
		const SubMeshVector &sub_meshs,
		const bool use_16_bit_indices, const bool use_restart_index):
		m_sub_meshs(sub_meshs),
		m_use_16_bit_indices(use_16_bit_indices),
		m_use_restart_index(use_restart_index)
	{
		void* src;
		Uint32 i, count, size;

		if (use_16_bit_indices)
		{
			size = sizeof(Uint16);
		}
		else
		{
			size = sizeof(Uint32);
		}

		count = indices.size();

		m_indices.resize(size * count);

		src = m_indices.get_ptr();

		for (i = 0; i < count; ++i)
		{
			if (use_16_bit_indices)
			{
				static_cast<Uint16*>(src)[i] = indices[i];
			}
			else
			{
				static_cast<Uint32*>(src)[i] = indices[i];
			}
		}
	}

	IndexUpdateSource::~IndexUpdateSource() noexcept
	{
	}

	Uint32 IndexUpdateSource::get_count(const Uint32Set &blocks) const
	{
		Uint32 result;

		result = 0;

		BOOST_FOREACH(const Uint32 index, blocks)
		{
			assert(index < m_sub_meshs.size());
			result += m_sub_meshs[index].get_count();
		}

		return result;
	}

	void IndexUpdateSource::write_index_buffer(const Uint32Set &blocks,
		const AbstractWriteMemorySharedPtr &buffer) const
	{
		Uint64 offset, size, count;

		offset = 0;

		if (get_use_16_bit_indices())
		{
			size = sizeof(Uint16);
		}
		else
		{
			size = sizeof(Uint32);
		}

		BOOST_FOREACH(const Uint32 index, blocks)
		{
			count = m_sub_meshs[index].get_count() * size;
			memcpy(static_cast<Uint8*>(buffer->get_ptr()) + offset,
				static_cast<const Uint8*>(m_indices.get_ptr()) +
				m_sub_meshs[index].get_offset() * size, count);
			offset += count;
		}
	}

	SubMeshVector IndexUpdateSource::get_sub_meshs(const Uint32Set &blocks)
		const
	{
		SubMeshVector result;
		glm::vec3 min, max;
		Uint32 count, min_vertex, max_vertex;

		count = 0;
		min_vertex = std::numeric_limits<Uint32>::max();
		max_vertex = std::numeric_limits<Uint32>::min();
		min = glm::vec3(std::numeric_limits<float>::max());
		max = glm::vec3(-std::numeric_limits<float>::max());

		BOOST_FOREACH(const Uint32 index, blocks)
		{
			assert(index < m_sub_meshs.size());

			count += m_sub_meshs[index].get_count();
			min = glm::min(min, m_sub_meshs[index].get_bounding_box(
				).get_min());
			max = glm::max(max, m_sub_meshs[index].get_bounding_box(
				).get_max());
			min_vertex = std::min(min_vertex,
				m_sub_meshs[index].get_min_vertex());
			max_vertex = std::max(max_vertex,
				m_sub_meshs[index].get_max_vertex());
		}

		result.push_back(SubMesh(BoundingBox(min, max), 0, count,
			min_vertex, max_vertex, false));

		return result;
	}

}
