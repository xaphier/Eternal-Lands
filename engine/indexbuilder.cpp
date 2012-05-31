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
				bool m_use_restart_index;

				Uint32 get_index(const Uint32 x, const Uint32 y,
					const Uint32 offset_x,
					const Uint32 offset_y) const noexcept;
				void build_indices(const Uint32 x,
					const Uint32 y, const Uint32 size,
					const Uint16Array4 &splits) noexcept;
				void build_triangles(const Uint32 x,
					const Uint32 y, const Uint32 size,
					const Uint16Array4 &splits) noexcept;
				void build_triangle_fan(const Uint32 x,
					const Uint32 y, const Uint32 size,
					const Uint16Array4 &splits) noexcept;
				Uint32 get_index(const Uint32 x,
					const Uint32 y) const noexcept;

			public:
				PlaneIndexBuilder(const Uint32 tile_size,
					const bool use_restart_index,
					Uint32Vector &indices);
				~PlaneIndexBuilder() noexcept;
				bool get_use_restart_index() const noexcept;
				Uint32 get_restart_index() const noexcept;
				Uint32 get_tile_size() const noexcept;
				Uint32 get_tile_index_size() const noexcept;
				void build_indices(const Uint32 skip,
					const Uint16Array4 &splits_outside,
					const bool split_inside) noexcept;

		};

		Uint32 PlaneIndexBuilder::get_index(const Uint32 x,
			const Uint32 y,	const Uint32 offset_x,
			const Uint32 offset_y) const noexcept
		{
			return get_index(x + offset_x, y + offset_y);
		}

		Uint32 PlaneIndexBuilder::get_index(const Uint32 x,
			const Uint32 y) const noexcept
		{
			assert(x < get_tile_index_size());
			assert(y < get_tile_index_size());

			return x + y * get_tile_index_size();
		}

		void PlaneIndexBuilder::build_indices(const Uint32 x,
			const Uint32 y, const Uint32 size,
			const Uint16Array4 &splits) noexcept
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
			const Uint16Array4 &splits) noexcept
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
			const Uint16Array4 &splits) noexcept
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

		bool PlaneIndexBuilder::get_use_restart_index() const noexcept
		{
			return m_use_restart_index;
		}

		Uint32 PlaneIndexBuilder::get_restart_index() const noexcept
		{
			return std::numeric_limits<Uint32>::max();
		}

		Uint32 PlaneIndexBuilder::get_tile_size() const noexcept
		{
			return m_tile_size;
		}

		Uint32 PlaneIndexBuilder::get_tile_index_size() const noexcept
		{
			return get_tile_size() + 1;
		}

		void PlaneIndexBuilder::build_indices(const Uint32 skip,
			const Uint16Array4 &splits_outside,
			const bool split_inside) noexcept
		{
			Uint32 i, j, count, step, split;
			Uint16Array4 splits;

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

		PlaneIndexBuilder::PlaneIndexBuilder(const Uint32 tile_size,
			const bool use_restart_index, Uint32Vector &indices):
			m_indices(indices), m_tile_size(tile_size),
			m_use_restart_index(use_restart_index)
		{
		}

		PlaneIndexBuilder::~PlaneIndexBuilder() noexcept
		{
		}

		class TerrainIndexBuilder
		{
			private:
				Uint32Vector &m_indices;
				Uint32 m_tile_size;
				bool m_use_restart_index;

				void build_indices(const Uint32 y) noexcept;
				void build_triangles(const Uint32 y) noexcept;
				void build_triangle_strip(const Uint32 y)
					noexcept;
				Uint32 get_index(const Uint32 x,
					const Uint32 y) const noexcept;

			public:
				TerrainIndexBuilder(const Uint32 tile_size,
					const bool use_restart_index,
					Uint32Vector &indices);
				~TerrainIndexBuilder() noexcept;
				bool get_use_restart_index() const noexcept;
				Uint32 get_restart_index() const noexcept;
				Uint32 get_tile_size() const noexcept;
				Uint32 get_tile_index_size() const noexcept;
				void build_indices() noexcept;

		};

		Uint32 TerrainIndexBuilder::get_index(const Uint32 x,
			const Uint32 y) const noexcept
		{
			assert(x < get_tile_index_size());
			assert(y < get_tile_index_size());

			return x + y * get_tile_index_size();
		}

		void TerrainIndexBuilder::build_indices(const Uint32 y) noexcept
		{
			if (get_use_restart_index())
			{
				build_triangle_strip(y);
				m_indices.push_back(get_restart_index());
			}
			else
			{
				build_triangles(y);
			}
		}

		void TerrainIndexBuilder::build_triangles(const Uint32 y)
			noexcept
		{
			Uint32 x, count;

			count = get_tile_size();

			for (x = 0; x < count; ++x)
			{
				m_indices.push_back(get_index(x    , y + 1));
				m_indices.push_back(get_index(x    , y    ));
				m_indices.push_back(get_index(x + 1, y + 1));

				m_indices.push_back(get_index(x + 1, y + 1));
				m_indices.push_back(get_index(x    , y    ));
				m_indices.push_back(get_index(x + 1, y    ));
			}
		}

		void TerrainIndexBuilder::build_triangle_strip(const Uint32 y)
			noexcept
		{
			Uint32 x, count;

			count = get_tile_size();

			m_indices.push_back(get_index(0, y + 1));
			m_indices.push_back(get_index(0, y));

			for (x = 0; x < count; ++x)
			{
				m_indices.push_back(get_index(x + 1, y + 1));
				m_indices.push_back(get_index(x + 1, y    ));
			}
		}

		bool TerrainIndexBuilder::get_use_restart_index() const noexcept
		{
			return m_use_restart_index;
		}

		Uint32 TerrainIndexBuilder::get_restart_index() const noexcept
		{
			return std::numeric_limits<Uint32>::max();
		}

		Uint32 TerrainIndexBuilder::get_tile_size() const noexcept
		{
			return m_tile_size;
		}

		Uint32 TerrainIndexBuilder::get_tile_index_size() const noexcept
		{
			return get_tile_size() + 1;
		}

		void TerrainIndexBuilder::build_indices() noexcept
		{
			Uint32 i, count;

			count = get_tile_size();

			for (i = 0; i < count; ++i)
			{
				build_indices(i);
			}
		}

		TerrainIndexBuilder::TerrainIndexBuilder(const Uint32 tile_size,
			const bool use_restart_index, Uint32Vector &indices):
			m_indices(indices), m_tile_size(tile_size),
			m_use_restart_index(use_restart_index)
		{
		}

		TerrainIndexBuilder::~TerrainIndexBuilder() noexcept
		{
		}
	}

	void IndexBuilder::build_plane_indices(const Uint32 tile_size,
		const bool use_restart_index, const Uint32 skip,
		const bool split, Uint32Vector &indices)
	{
		PlaneIndexBuilder plane_index_builder(tile_size,
			use_restart_index, indices);
		Uint16Array4 splits_outside;

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
	}

	void IndexBuilder::build_plane_indices(const Uint32 tile_size,
		const bool use_restart_index, const Uint32 skip,
		const Uint16Array4 &splits_outside, const bool split_inside,
		Uint32Vector &indices)
	{
		PlaneIndexBuilder plane_index_builder(tile_size,
			use_restart_index, indices);

		plane_index_builder.build_indices(skip, splits_outside,
			split_inside);
	}

	void IndexBuilder::build_terrain_indices(const Uint32 tile_size,
		const bool use_restart_index, Uint32Vector &indices)
	{
		TerrainIndexBuilder terrain_index_builder(tile_size,
			use_restart_index, indices);

		terrain_index_builder.build_indices();
	}

}
