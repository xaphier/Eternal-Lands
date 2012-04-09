/****************************************************************************
 *            indexbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
					const Uint32 offset_x,
					const Uint32 offset_y) const;
				void build_indices(const Uint32 x,
					const Uint32 y, const Uint32 size,
					const Uint8Array4 splits);
				void build_triangles(const Uint32 x,
					const Uint32 y, const Uint32 size,
					const Uint8Array4 splits);
				void build_triangle_fan(const Uint32 x,
					const Uint32 y, const Uint32 size,
					const Uint8Array4 splits);
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
					const Uint8Array4 splits_outside,
					const bool split_inside);

		};

		Uint32 PlaneIndexBuilder::get_index(const Uint32 x,
			const Uint32 y,	const Uint32 offset_x,
			const Uint32 offset_y) const
		{
			return get_index(x + offset_x, y + offset_y);
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
			const Uint8Array4 splits)
		{
			if (get_use_restart_index())
			{
				build_triangle_fan(x, y, size, splits);
				m_indices.push_back(get_restart_index());
			}
			else
			{
				build_triangles(x, y, size, splits);
			}
		}

		void PlaneIndexBuilder::build_triangles(const Uint32 x,
			const Uint32 y, const Uint32 size,
			const Uint8Array4 splits)
		{
			Uint32 i, count, step;

			count = 1 << splits[dt_north];
			step = (size * 2) >> splits[dt_north];

			for (i = 0; i < count; ++i)
			{
				m_indices.push_back(get_index(x, y, size,
					size));
				m_indices.push_back(get_index(x, y, step * i,
					0));
				m_indices.push_back(get_index(x, y,
					step * (i + 1), 0));
			}

			count = 1 << splits[dt_east];
			step = (size * 2) >> splits[dt_east];

			for (i = 0; i < count; ++i)
			{
				m_indices.push_back(get_index(x, y, size,
					size));
				m_indices.push_back(get_index(x, y, size * 2,
					step * i));
				m_indices.push_back(get_index(x, y, size * 2,
					step * (i + 1)));
			}

			count = 1 << splits[dt_south];
			step = (size * 2) >> splits[dt_south];

			for (i = 0; i < count; ++i)
			{
				m_indices.push_back(get_index(x, y, size,
					size));
				m_indices.push_back(get_index(x, y,
					step * (count - i), size * 2));
				m_indices.push_back(get_index(x, y,
					step * (count - i - 1), size * 2));
			}

			count = 1 << splits[dt_west];
			step = (size * 2) >> splits[dt_west];

			for (i = 0; i < count; ++i)
			{
				m_indices.push_back(get_index(x, y, size,
					size));
				m_indices.push_back(get_index(x, y, 0,
					step * (count - i)));
				m_indices.push_back(get_index(x, y, 0,
					step * (count - i - 1)));
			}
		}

		void PlaneIndexBuilder::build_triangle_fan(const Uint32 x,
			const Uint32 y, const Uint32 size,
			const Uint8Array4 splits)
		{
			Uint32 i, count, step;

			m_indices.push_back(get_index(x, y, size, size));
			m_indices.push_back(get_index(x, y, 0, 0));

			count = 1 << splits[dt_north];
			step = (size * 2) >> splits[dt_north];

			for (i = 0; i < count; ++i)
			{
				m_indices.push_back(get_index(x, y,
					step * (i + 1), 0));
			}

			count = 1 << splits[dt_east];
			step = (size * 2) >> splits[dt_east];

			for (i = 0; i < count; ++i)
			{
				m_indices.push_back(get_index(x, y, size * 2,
					step * (i + 1)));
			}

			count = 1 << splits[dt_south];
			step = (size * 2) >> splits[dt_south];

			for (i = 0; i < count; ++i)
			{
				m_indices.push_back(get_index(x, y,
					step * (count - i - 1), size * 2));
			}

			count = 1 << splits[dt_west];
			step = (size * 2) >> splits[dt_west];

			for (i = 0; i < count; ++i)
			{
				m_indices.push_back(get_index(x, y, 0,
					step * (count - i - 1)));
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
			const Uint8Array4 splits_outside,
			const bool split_inside)
		{
			Uint32 i, j, count, step, split;
			Uint8Array4 splits;

			count = get_tile_size() >> (skip + 1);
			step = 1 << skip;

			assert((count * step * 2) == get_tile_size());

			split = 0;

			if (split_inside)
			{
				split = 1;
			}

			for (j = 0; j < count; ++j)
			{
				for (i = 0; i < count; ++i)
				{
					if (j == 0)
					{
						splits[dt_north] =
							splits_outside[dt_north];
					}
					else
					{
						splits[dt_north] = split;
					}

					if (i == (count - 1))
					{
						splits[dt_east] =
							splits_outside[dt_east];
					}
					else
					{
						splits[dt_east] = split;
					}

					if (j == (count - 1))
					{
						splits[dt_south] =
							splits_outside[dt_south];
					}
					else
					{
						splits[dt_south] = split;
					}

					if (i == 0)
					{
						splits[dt_west] =
							splits_outside[dt_west];
					}
					else
					{
						splits[dt_west] = split;
					}

					build_indices(i * step * 2,
						j * step * 2, step, splits);
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
		const Uint32 skip, const bool split)
	{
		PlaneIndexBuilder plane_index_builder(indices, tile_size,
			use_restart_index);
		Uint8Array4 splits_outside;

		if (split)
		{
			splits_outside[0] = 1;
			splits_outside[1] = 1;
			splits_outside[2] = 1;
			splits_outside[3] = 1;
		}
		else
		{
			splits_outside[0] = 0;
			splits_outside[1] = 0;
			splits_outside[2] = 0;
			splits_outside[3] = 0;
		}

		plane_index_builder.build_indices(skip, splits_outside, split);

		return plane_index_builder.get_restart_index();
	}

	Uint32 IndexBuilder::build_plane_indices(Uint32Vector &indices,
		const Uint32 tile_size, const bool use_restart_index,
		const Uint32 skip, const Uint8Array4 splits_outside,
		const bool split_inside)
	{
		PlaneIndexBuilder plane_index_builder(indices, tile_size,
			use_restart_index);

		plane_index_builder.build_indices(skip, splits_outside,
			split_inside);

		return plane_index_builder.get_restart_index();
	}

}
