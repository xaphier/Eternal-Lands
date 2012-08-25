/****************************************************************************
 *            terrainquadtree.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5b03e3c5_e474_4587_aa6c_9a38e9e11c95
#define	UUID_5b03e3c5_e474_4587_aa6c_9a38e9e11c95

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "alignedarrays.hpp"

/**
 * @file
 * @brief The @c class TerrainQuadTree.
 * This file contains the @c class TerrainQuadTree.
 */
namespace eternal_lands
{

	class TerrainQuadTree
	{
		private:
			AlignedVec4Array m_data;
			Uint32 m_node_count;

			/**
			 * node
			 * {
			 *	vec4 min_x, min_y, min_z; 
			 * 	vec4 max_x, max_y, max_z;
			 * 	uvec4 indices;
			 * 	Uint32 count;
			 * 	Uint32 leaf;
			 * 	Uint32 padd[2];
			 * }
			 */

		protected:
			void fill_node(const glm::uvec4 &indices,
				const Uint32 index, const Uint32 count);
			void fill_leaf(const glm::uvec4 &indices,
				const Uint32 index, const Uint32 count,
				const Uint32 line);
			void fill_leaf(const glm::uvec2 &indices,
				const glm::uvec2 &counts, const Uint32 index,
				const Uint32 line);

		public:
			TerrainQuadTree();
			~TerrainQuadTree() noexcept;
			void init(const ImageSharedPtr &terrain_image);

			static Uint32 calculate_data_size(const Uint32 width,
				const Uint32 height);

	};

}

#endif	/* UUID_5b03e3c5_e474_4587_aa6c_9a38e9e11c95 */
