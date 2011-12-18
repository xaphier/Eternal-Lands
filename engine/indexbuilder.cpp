/****************************************************************************
 *            indexbuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "indexbuilder.hpp"

namespace eternal_lands
{

	namespace
	{

		class PlaneIndexBuilder
		{
			private:
				Uint32Vector &m_indices;
				Uint32 m_tile_size;
				Uint32 m_restart_index;
				bool m_use_restart_index;

				Uint32 get_index(const Uint32 x, const Uint32 y,
					const Uint32 scale_x,
					const Uint32 scale_y,
					const Uint32 size) const;
				void build_indices(const Uint32 x,
					const Uint32 y, const Uint32 size,
					const BitSet4 split);
				void build_triangles(const Uint32 x,
					const Uint32 y, const Uint32 size,
					const BitSet4 split);
				void build_triangle_fan(const Uint32 x,
					const Uint32 y, const Uint32 size,
					const BitSet4 split);
				Uint32 get_index(const Uint32 x,
					const Uint32 y) const;

			public:
				PlaneIndexBuilder(Uint32Vector &indices,
					const Uint32 tile_size,
					const bool use_restart_index);
				~PlaneIndexBuilder() throw();
				bool get_use_restart_index() const;
				Uint32 get_restart_index() const;
				Uint32 get_tile_size() const;
				Uint32 get_tile_index_size() const;
				void build_indices(const Uint32 skip,
					const bool invert,
					const BitSet4 connected);

		};

		Uint32 PlaneIndexBuilder::get_index(const Uint32 x,
			const Uint32 y,	const Uint32 scale_x,
			const Uint32 scale_y, const Uint32 size) const
		{
			return get_index(x + scale_x * size, y + scale_y * size);
		}

		Uint32 PlaneIndexBuilder::get_index(const Uint32 x,
			const Uint32 y) const
		{
			assert(x < get_tile_index_size());
			assert(y < get_tile_index_size());

			return x + y * get_tile_index_size();
		}

		void PlaneIndexBuilder::build_indices(const Uint32 x,
			const Uint32 y, const Uint32 size,
			const BitSet4 split)
		{
			if (get_use_restart_index())
			{
				build_triangle_fan(x, y, size, split);
				m_indices.push_back(get_restart_index());
			}
			else
			{
				build_triangles(x, y, size, split);
			}
		}

		void PlaneIndexBuilder::build_triangles(const Uint32 x,
			const Uint32 y, const Uint32 size,
			const BitSet4 split)
		{
			if (split[dt_north])
			{
				m_indices.push_back(get_index(x, y, 0, 0, size));
				m_indices.push_back(get_index(x, y, 1, 0, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));

				m_indices.push_back(get_index(x, y, 1, 0, size));
				m_indices.push_back(get_index(x, y, 2, 0, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));
			}
			else
			{
				m_indices.push_back(get_index(x, y, 0, 0, size));
				m_indices.push_back(get_index(x, y, 2, 0, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));
			}

			if (split[dt_east])
			{
				m_indices.push_back(get_index(x, y, 2, 0, size));
				m_indices.push_back(get_index(x, y, 2, 1, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));

				m_indices.push_back(get_index(x, y, 2, 1, size));
				m_indices.push_back(get_index(x, y, 2, 2, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));
			}
			else
			{
				m_indices.push_back(get_index(x, y, 2, 0, size));
				m_indices.push_back(get_index(x, y, 2, 2, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));
			}

			if (split[dt_south])
			{
				m_indices.push_back(get_index(x, y, 2, 2, size));
				m_indices.push_back(get_index(x, y, 1, 2, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));

				m_indices.push_back(get_index(x, y, 1, 2, size));
				m_indices.push_back(get_index(x, y, 0, 2, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));
			}
			else
			{
				m_indices.push_back(get_index(x, y, 2, 2, size));
				m_indices.push_back(get_index(x, y, 0, 2, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));
			}

			if (split[dt_west])
			{
				m_indices.push_back(get_index(x, y, 0, 2, size));
				m_indices.push_back(get_index(x, y, 0, 1, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));

				m_indices.push_back(get_index(x, y, 0, 1, size));
				m_indices.push_back(get_index(x, y, 0, 0, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));
			}
			else
			{
				m_indices.push_back(get_index(x, y, 0, 2, size));
				m_indices.push_back(get_index(x, y, 0, 0, size));
				m_indices.push_back(get_index(x, y, 1, 1, size));
			}
		}

		void PlaneIndexBuilder::build_triangle_fan(const Uint32 x,
			const Uint32 y, const Uint32 size,
			const BitSet4 split)
		{
			m_indices.push_back(get_index(x, y, 1, 1, size));
			m_indices.push_back(get_index(x, y, 0, 0, size));

			if (split[dt_north])
			{
				m_indices.push_back(get_index(x, y, 1, 0, size));
				m_indices.push_back(get_index(x, y, 2, 0, size));
			}
			else
			{
				m_indices.push_back(get_index(x, y, 2, 0, size));
			}

			if (split[dt_east])
			{
				m_indices.push_back(get_index(x, y, 2, 1, size));
				m_indices.push_back(get_index(x, y, 2, 2, size));
			}
			else
			{
				m_indices.push_back(get_index(x, y, 2, 2, size));
			}

			if (split[dt_south])
			{
				m_indices.push_back(get_index(x, y, 1, 2, size));
				m_indices.push_back(get_index(x, y, 0, 2, size));
			}
			else
			{
				m_indices.push_back(get_index(x, y, 0, 2, size));
			}

			if (split[dt_west])
			{
				m_indices.push_back(get_index(x, y, 0, 1, size));
				m_indices.push_back(get_index(x, y, 0, 0, size));
			}
			else
			{
				m_indices.push_back(get_index(x, y, 0, 0, size));
			}
		}

		bool PlaneIndexBuilder::get_use_restart_index() const
		{
			return m_use_restart_index;
		}

		Uint32 PlaneIndexBuilder::get_restart_index() const
		{
			return m_restart_index;
		}

		Uint32 PlaneIndexBuilder::get_tile_size() const
		{
			return m_tile_size;
		}

		Uint32 PlaneIndexBuilder::get_tile_index_size() const
		{
			return get_tile_size() + 1;
		}

		void PlaneIndexBuilder::build_indices(const Uint32 skip,
			const bool invert, const BitSet4 connected)
		{
			Uint32 i, j, count, step;
			BitSet4 mask, split;

			count = get_tile_size() >> (skip + 1);
			step = 1 << skip;

			assert((count * step * 2) == get_tile_size());

			for (j = 0; j < count; ++j)
			{
				for (i = 0; i < count; ++i)
				{
					mask[dt_north] = (j == 0);
					mask[dt_east] = (i == (count - 1));
					mask[dt_south] = (j == (count - 1));
					mask[dt_west] = (i == 0);

					split = connected & mask;

					if (invert)
					{
						split = split.flip();
					}

					build_indices(i * step * 2,
						j * step * 2, step, split);
				}
			}
		}

		PlaneIndexBuilder::PlaneIndexBuilder(Uint32Vector &indices,
			const Uint32 tile_size, const bool use_restart_index):
			m_indices(indices), m_tile_size(tile_size),
			m_use_restart_index(use_restart_index)
		{
			Uint32 max_index;

			max_index = get_tile_index_size();
			max_index *= get_tile_index_size();
			max_index -= 1;

			if (max_index < std::numeric_limits<Uint16>::max())
			{
				m_restart_index =
					std::numeric_limits<Uint16>::max();
			}
			else
			{
				m_restart_index =
					std::numeric_limits<Uint32>::max();
			}
		}

		PlaneIndexBuilder::~PlaneIndexBuilder() throw()
		{
		}

	}

	Uint32 IndexBuilder::build_plane_indices(Uint32Vector &indices,
		const Uint32 tile_size, const bool use_restart_index,
		const Uint32 skip, const bool invert,
		const BitSet4 connected)
	{
		PlaneIndexBuilder plane_index_builder(indices, tile_size,
			use_restart_index);

		plane_index_builder.build_indices(skip, invert, connected);

		return plane_index_builder.get_restart_index();
	}

}
