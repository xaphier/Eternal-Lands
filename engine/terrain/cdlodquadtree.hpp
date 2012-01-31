/****************************************************************************
 *            cdlodquadtree.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_710c4558_b937_42c2_af6b_9e26370d4e50
#define	UUID_710c4558_b937_42c2_af6b_9e26370d4e50

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class CdLodQuadTree.
 * This file contains the @c class CdLodQuadTree.
 */
namespace eternal_lands
{

	class CdLodQuadTree
	{
		private:

		public:
			CdLodQuadTree(const ImageSharedPtr height_map,
				const glm::vec3 &scale,
				const glm::vec3 &offset);
			~CdLodQuadTree() throw();

	};

}

#endif	/* UUID_710c4558_b937_42c2_af6b_9e26370d4e50 */
