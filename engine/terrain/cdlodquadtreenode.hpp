/****************************************************************************
 *            cdlodquadtreenode.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b45f1bba_ff12_49ea_947a_2f22db15522e
#define	UUID_b45f1bba_ff12_49ea_947a_2f22db15522e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class CdLodQuadTreeNode.
 * This file contains the @c class CdLodQuadTreeNode.
 */
namespace eternal_lands
{

	class CdLodQuadTreeNode
	{
		private:
			void* m_sub_nodes;
			Uint16 m_x, m_y;
			Uint16 m_size;
			Uint16 m_level;
			Uint16 m_min_z, m_max_z;

		public:
			inline Uint16 get_level() const
			{
				return m_level & 0x7FFF;
			}

			inline bool get_is_leaf() const
			{
				return (m_level & 0x8000) != 0;
			}

			inline Uint16 get_size() const
			{
				return m_size;
			}

			inline Uint16 get_x() const
			{
				return m_x;
			}

			inline Uint16 get_y() const
			{
				return m_y;
			}

			inline Uint16 get_min_z() const
			{
				return m_min_z;
			}

			inline Uint16 get_max_z() const
			{
				return m_max_z;
			}

			glm::vec3 get_world_min(const glm::vec3 &offset,
				const glm::vec3 &scale,
				const glm::vec2 &raster_size) const;
			glm::vec3 get_world_max(const glm::vec3 &offset,
				const glm::vec3 &scale,
				const glm::vec2 &raster_size) const;
			void get_world_bounding_box(const glm::vec3 &offset,
				const glm::vec3 &scale,
				const glm::vec2 &raster_size) const;

		public:
			CdLodQuadTreeNode(const ImageSharedPtr height_map,
				const glm::vec3 &scale,
				const glm::vec3 &offset, const Uint16 x,
				const Uint16 y, const Uint16 size,
				const Uint16 level);
			~CdLodQuadTreeNode() throw();

	};

}

#endif	/* UUID_b45f1bba_ff12_49ea_947a_2f22db15522e */
