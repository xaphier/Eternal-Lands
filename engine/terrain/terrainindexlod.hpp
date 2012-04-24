/****************************************************************************
 *            terrainindexlod.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_569e0d25_ec70_484d_a439_f10156f0efcd
#define	UUID_569e0d25_ec70_484d_a439_f10156f0efcd

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TerrainIndexLod.
 * This file contains the @c class TerrainIndexLod.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for terrain index lod.
	 *
	 * @c class for terrain index lod.
	 */
	class TerrainIndexLod
	{
		private:

		public:
			/**
			 * Default constructor.
			 */
			TerrainIndexLod(const Uint16 min_split_count,
				const Uint16 max_split_count,
				const Uint16 splits);

			/**
			 * Default destructor.
			 */
			~TerrainIndexLod() throw();
			Uint16 get_lod(const Uint8 splits_inside,
				const Uint16Array4 &splits_outside) const;

	};

}

#endif	/* UUID_569e0d25_ec70_484d_a439_f10156f0efcd */
