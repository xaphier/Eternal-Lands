/****************************************************************************
 *            submesh.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_1a3d2a09_d472_4e4c_9273_4fd010aae200
#define	UUID_1a3d2a09_d472_4e4c_9273_4fd010aae200

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "boundingbox.hpp"
#include "meshdrawdata.hpp"

/**
 * @file
 * @brief The @c class SubMesh.
 * This file contains the @c class SubMesh.
 */
namespace eternal_lands
{

	class SubMesh: public MeshDrawData
	{
		private:
			BoundingBox m_bounding_box;
			Uint32 m_min_max_boxes_index;
			Uint32 m_min_max_boxes_count;
			bool m_packed;

		public:
			SubMesh(const BoundingBox &bounding_box,
				const Uint32 offset, const Uint32 count,
				const Uint32 min_vertex,
				const Uint32 max_vertex,
				const bool packed = true);
			SubMesh(const BoundingBox &bounding_box,
				const Uint32 offset, const Uint32 count,
				const Uint32 min_vertex,
				const Uint32 max_vertex,
				const Uint32 base_vertex,
				const bool packed = true);
			SubMesh();
			~SubMesh() noexcept;

			inline void set_bounding_box(
				const BoundingBox &bounding_box) noexcept
			{
				m_bounding_box = bounding_box;
			}

			inline void set_min_max_boxes_index(
				const Uint32 min_max_boxes_index) noexcept
			{
				m_min_max_boxes_index = min_max_boxes_index;
			}

			inline void set_min_max_boxes_count(
				const Uint32 min_max_boxes_count) noexcept
			{
				m_min_max_boxes_count = min_max_boxes_count;
			}

			inline void set_packed(const bool packed) noexcept
			{
				m_packed = packed;
			}

			inline const BoundingBox &get_bounding_box() const
				noexcept
			{
				return m_bounding_box;
			}

			inline Uint32 get_min_max_boxes_index() const noexcept
			{
				return m_min_max_boxes_index;
			}

			inline Uint32 get_min_max_boxes_count() const noexcept
			{
				return m_min_max_boxes_count;
			}

			inline bool get_packed() const noexcept
			{
				return m_packed;
			}

	};

}

#endif	/* UUID_1a3d2a09_d472_4e4c_9273_4fd010aae200 */
