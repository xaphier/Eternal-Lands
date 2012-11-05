/****************************************************************************
 *            indexbuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			/**
			 * Build plane indices using triangle fans if restart
			 * index is supported. The triangles are ordered like
			 * this:			unsplit patch:
			 * 0--0--0--0--0--0--0		0-----0-----0-----0
			 * |\ | /|\ | /|\ | /|		|\   /|\   /|\   /|
			 * | \|/ | \|/ | \|/ |		| \ / | \ / | \ / |
			 * 0--0--0--0--0--0--0		|  0  |  0  |  0  |
			 * | /|\ | /|\ | /|\ |		| / \ | / \ | / \ |
			 * |/ | \|/ | \|/ | \|		|/   \|/   \|/   \|
			 * 0--0--0--0--0--0--0		0-----0-----0-----0
			 * |\ | /|\ | /|\ | /|		|\   /|\   /|\   /|
			 * | \|/ | \|/ | \|/ |		| \ / | \ / | \ / |
			 * 0--0--0--0--0--0--0		|  0  |  0  |  0  |
			 * | /|\ | /|\ | /|\ |		| / \ | / \ | / \ |
			 * |/ | \|/ | \|/ | \|		|/   \|/   \|/   \|
			 * 0--0--0--0--0--0--0		0-----0-----0-----0
			 * This gives many lod levels and a uniform lighting
			 * for vertex lighting.
			 * @param tile_size The size of each tile in vertexes
			 * @param use_restart_index True if restart index should
			 * be used and triangle fans, else trianlges are used.
			 * @param skip Defines how many vertexes are skiped
			 * when building the indices. Used for different lod
			 * levels.
			 * @param split Defines if each patch should should be
			 * splitted
			 * @param indices The vector where all the indices are
			 * added.
			 */
			static void build_plane_indices(const Uint32 tile_size,
				const bool use_restart_index,
				const Uint32 skip, const bool split,
				Uint32Vector &indices);

			/**
			 * Build plane indices using triangle fans if restart
			 * index is supported. The triangles are ordered like
			 * this:			unsplit patch:
			 * 0--0--0--0--0--0--0		0-----0-----0-----0
			 * |\ | /|\ | /|\ | /|		|\   /|\   /|\   /|
			 * | \|/ | \|/ | \|/ |		| \ / | \ / | \ / |
			 * 0--0--0--0--0--0--0		|  0  |  0  |  0  |
			 * | /|\ | /|\ | /|\ |		| / \ | / \ | / \ |
			 * |/ | \|/ | \|/ | \|		|/   \|/   \|/   \|
			 * 0--0--0--0--0--0--0		0-----0-----0-----0
			 * |\ | /|\ | /|\ | /|		|\   /|\   /|\   /|
			 * | \|/ | \|/ | \|/ |		| \ / | \ / | \ / |
			 * 0--0--0--0--0--0--0		|  0  |  0  |  0  |
			 * | /|\ | /|\ | /|\ |		| / \ | / \ | / \ |
			 * |/ | \|/ | \|/ | \|		|/   \|/   \|/   \|
			 * 0--0--0--0--0--0--0		0-----0-----0-----0
			 * This gives many lod levels and a uniform lighting
			 * for vertex lighting.
			 * @param tile_size The size of each tile in vertexes
			 * @param use_restart_index True if restart index should
			 * be used and triangle fans, else trianlges are used.
			 * @param skip Defines how many vertexes are skiped
			 * when building the indices. Used for different lod
			 * levels.
			 * @param splits_outside Defines how often each patch
			 * is splitted on the borders so it can link to
			 * a a given lod level.
			 * @param split_inside Defines if the patch is splitted
			 * inside for greater resolution.
			 * @param indices The vector where all the indices are
			 * added.
			 */
			static void build_plane_indices(const Uint32 tile_size,
				const bool use_restart_index,
				const Uint32 skip,
				const Uint16Array4 &splits_outside,
				const bool split_inside,
				Uint32Vector &indices);

			/**
			 * Build plane indices using triangle strips if restart
			 * index is supported. The triangles are ordered like
			 * this:
			 * 0--0--0--0--0--0--0
			 * |\ |\ |\ |\ |\ |\ |
			 * | \| \| \| \| \| \|
			 * 0--0--0--0--0--0--0
			 * |\ |\ |\ |\ |\ |\ |
			 * | \| \| \| \| \| \|
			 * 0--0--0--0--0--0--0
			 * |\ |\ |\ |\ |\ |\ |
			 * | \| \| \| \| \| \|
			 * 0--0--0--0--0--0--0
			 * |\ |\ |\ |\ |\ |\ |
			 * | \| \| \| \| \| \|
			 * 0--0--0--0--0--0--0
			 * This gives many lod levels and a uniform lighting
			 * for vertex lighting.
			 * @param tile_size The size of each tile in vertexes
			 * @param use_restart_index True if restart index should
			 * be used and triangle fans, else trianlges are used.
			 * @param indices The vector where all the indices are
			 * added.
			 */
			static void build_terrain_indices(
				const Uint32 tile_size,
				const bool use_restart_index,
				Uint32Vector &indices);

	};

}

#endif	/* UUID_c1bbdd0e_4ebd_45ac_9ba0_b1b9583a0719 */
