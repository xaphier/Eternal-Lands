/****************************************************************************
 *            atlas.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "atlas.hpp"

namespace eternal_lands
{

	Atlas::Atlas(const Uint32 width, const Uint32 height): m_width(width),
		m_height(height)
	{
		m_nodes.push_back(glm::uvec3(0, 0, get_width()));
	}

	Atlas::~Atlas() noexcept
	{
	}

	void Atlas::clear()
	{
		m_nodes.clear();
		m_nodes.push_back(glm::uvec3(0, 0, get_width()));
	}

	Sint32 Atlas::fit(const Uint32 index, const Uint32 width,
		const Uint32 height) const
	{
		Sint32 width_left;
		Uint32 x, y, i;

		assert(index < m_nodes.size());

		width_left = width;
		i = index;
		x = m_nodes[i].x;
		y = m_nodes[i].y;

		if ((x + width) > get_width())
		{
			return -1;
		}

		while (width_left > 0)
		{
			assert(i < m_nodes.size());

			y = std::max(y, m_nodes[i].y);

			if ((y + height) > get_height())
			{
				return -1;
			}

			width_left -= m_nodes[i].z;
			++i;
		}
		
		return y;
	}

	void Atlas::merge()
	{
		Uvec3Vector::iterator it, next;
		Uint32 i;

		i = 0;

		while ((i + 1) < m_nodes.size())
		{
			it = m_nodes.begin() + i;
			next = it + 1;

			if (it->y == next->y)
			{
				it->z += next->z;
				m_nodes.erase(next);
			}
			else
			{
				++i;
			}
		}
	}

	bool Atlas::get_region(const Uint32 width, const Uint32 height,
		glm::uvec2 &offset)
	{
		Uvec3Vector::iterator it, prev;
		Sint32 y, best_index;
		Uint32 best_height, best_width, shrink, i, count;

		best_index  = -1;
		best_height = std::numeric_limits<Uint32>::max();
		best_width = std::numeric_limits<Uint32>::max();

		count = m_nodes.size();

		for (i = 0; i < count; ++i)
		{
			y = fit(i, width, height);

			if ((y >= 0) && (((y + height) < best_height) ||
				(((y + height) == best_height) &&
					(m_nodes[i].z < best_width))))
			{
				best_height = y + height;
				best_index = i;
				best_width = m_nodes[i].z;
				offset.x = m_nodes[i].x;
				offset.y = y;
			}
		}

		if (best_index == -1)
		{
			return false;
		}

		m_nodes.insert(m_nodes.begin() + best_index,
			glm::uvec3(offset.x, offset.y + height, width));

		while ((best_index + 1) < m_nodes.size())
		{
			prev = m_nodes.begin() + best_index;
			it = prev + 1;

			if (it->x < (prev->x + prev->z))
			{
				shrink = (prev->x + prev->z) - it->x;

				if (it->z < shrink)
				{
					m_nodes.erase(it);
				}
				else
				{
					it->x += shrink;
					it->z -= shrink;

					break;
				}
			}
			else
			{
				break;
			}
		}

		merge();

		return true;
	}

}
