/****************************************************************************
 *            submesh.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			~SubMesh() throw();

			inline const BoundingBox &get_bounding_box() const
			{
				return m_bounding_box;
			}

			inline bool get_packed() const
			{
				return m_packed;
			}

			inline void set_bounding_box(
				const BoundingBox &bounding_box)
			{
				m_bounding_box = bounding_box;
			}

			inline void set_packed(const bool packed)
			{
				m_packed = packed;
			}

	};

}

#endif	/* UUID_1a3d2a09_d472_4e4c_9273_4fd010aae200 */
