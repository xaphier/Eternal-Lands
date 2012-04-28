/****************************************************************************
 *            indexbuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c1bbdd0e_4ebd_45ac_9ba0_b1b9583a0719
#define	UUID_c1bbdd0e_4ebd_45ac_9ba0_b1b9583a0719

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class PlaneIndexBuilder.
 * This file contains the @c class PlaneIndexBuilder.
 */
namespace eternal_lands
{

	/**
	 * Types of directions.
	 * @{
	 */
	enum DirectionType
	{
		/**
		 * North
		 */
		dt_north = 0,
		/**
		 * East
		 */
		dt_east = 1,
		/**
		 * South
		 */
		dt_south = 2,
		/**
		 * West
		 */
		dt_west = 3
	};
	/**
	 * @}
	 */

	class IndexBuilder
	{
		public:
			static void build_plane_indices(
				Uint32Vector &indices, const Uint32 tile_size,
				const bool use_restart_index,
				const Uint32 skip, const bool split);

			static void build_plane_indices(
				Uint32Vector &indices, const Uint32 tile_size,
				const bool use_restart_index,
				const Uint32 skip,
				const Uint16Array4 &splits_outside,
				const bool split_inside);

	};

}

#endif	/* UUID_c1bbdd0e_4ebd_45ac_9ba0_b1b9583a0719 */

