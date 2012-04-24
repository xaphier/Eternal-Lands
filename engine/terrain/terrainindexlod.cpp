/****************************************************************************
 *            terrainindexlod.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainindexlod.hpp"
#include "indexbuilder.hpp"

namespace eternal_lands
{

	TerrainIndexLod::TerrainIndexLod(const Uint16 min_split_count,
		const Uint16 max_split_count, const Uint16 splits)
	{
/*		Uint32Vector indices;
		Uint32 restart_index, tile_size;
		Uint16Array4 splits_outside;
		bool use_restart_index;

		tile_size = 1 << max_split_count;

		for (i = min_split_count; i <= max_split_count; ++i)
		{
			for (
			build_plane_indices(indices, tile_size,
				use_restart_index, skip, 
				const Uint16Array4 &splits_outside,
				const bool split_inside);
		}
*/
		Uint32Vector indices;
		Uint32 size, tile_size, count;
		Uint16 i, j0, j1, j2, j3;
		Uint16Array4 splits_outside;
		bool use_restart_index;

		tile_size = 1 << max_split_count;

		use_restart_index = false;

		for (i = min_split_count; i <= max_split_count; ++i)
		{
			size = indices.size();
			count = std::min((int)splits, max_split_count - i);
			for (j0 = 0; j0 <= count; ++j0)
			{
				for (j1 = 0; j1 <= count; ++j1)
				{
					for (j2 = 0; j2 <= count; ++j2)
					{
						for (j3 = 0; j3 <= count; ++j3)
						{
							splits_outside[0] = j0;
							splits_outside[1] = j1;
							splits_outside[2] = j2;
							splits_outside[3] = j3;

							if ((j0 != 0) && (j1 != 0) &&
								(j2 != 0) && (j3 != 0))
							IndexBuilder::build_plane_indices(
								indices, tile_size,
								use_restart_index,
								max_split_count - i, 
								splits_outside,
								true);

							IndexBuilder::build_plane_indices(
								indices, tile_size,
								use_restart_index,
								max_split_count - i, 
								splits_outside,
								false);
						}
					}
				}
			}

			size = indices.size() - size;
			std::cout << "[" << i << "] indices.size(): " << size << std::endl;
		}

		std::cout << "indices.size(): " << indices.size() << std::endl;

		indices.clear();

		use_restart_index = true;

		for (i = min_split_count; i <= max_split_count; ++i)
		{
			size = indices.size();
			count = std::min((int)splits, max_split_count - i) + 1;

			for (j0 = 0; j0 <= count; ++j0)
			{
				for (j1 = 0; j1 <= count; ++j1)
				{
					for (j2 = 0; j2 <= count; ++j2)
					{
						for (j3 = 0; j3 <= count; ++j3)
						{
							splits_outside[0] = j0;
							splits_outside[1] = j1;
							splits_outside[2] = j2;
							splits_outside[3] = j3;

							if ((j0 != 0) && (j1 != 0) &&
								(j2 != 0) && (j3 != 0))
							IndexBuilder::build_plane_indices(
								indices, tile_size,
								use_restart_index,
								max_split_count - i, 
								splits_outside,
								true);

							IndexBuilder::build_plane_indices(
								indices, tile_size,
								use_restart_index,
								max_split_count - i, 
								splits_outside,
								false);
						}
					}
				}
			}

			size = indices.size() - size;
			std::cout << "[" << i << "] indices.size(): " << size << std::endl;
		}

		std::cout << "indices.size(): " << indices.size() << std::endl;
	}

	TerrainIndexLod::~TerrainIndexLod() throw()
	{
	}

	Uint16 TerrainIndexLod::get_lod(const Uint8 splits_inside,
		const Uint16Array4 &splits_outside) const
	{
		return 0;
	}

}
