/****************************************************************************
 *            convexbody.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_672a4618_07e4_4f22_a944_4fa7c4540efe
#define	UUID_672a4618_07e4_4f22_a944_4fa7c4540efe

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "polygon.hpp"

/**
 * @file
 * @brief The @c class ConvexBody.
 * This file contains the @c class ConvexBody.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for convex body.
	 *
	 * @c class for convex body.
	 */
	class ConvexBody
	{
		private:
			class LessThanVec3
			{
				public:
					bool operator()(const glm::vec3 &v0,
						const glm::vec3 &v1) const
						noexcept
					{
						return glm::all(glm::lessThan(
							v0, v1));
					}

			};

			typedef std::vector<Polygon> PolygonVector;
			typedef std::multimap<glm::vec3, glm::vec3,
				LessThanVec3> EdgeMap;
			typedef std::pair<glm::vec3, glm::vec3> Edge;

			PolygonVector m_polygons;

			static bool find_and_erase_edge_pair(
				const glm::vec3 &vertex,
				EdgeMap& intersection_edges, glm::vec3 &next);

		public:
			ConvexBody();
			ConvexBody(const Frustum &frustum, const Uint16 index);
			ConvexBody(const BoundingBox &box);
			~ConvexBody() noexcept;
			void clip(const Frustum &frustum, const Uint16 index);
			void clip(const BoundingBox &box);
			void clip(const Plane &plane);
			void transform(const glm::mat4x3 &matrix);
			ConvexBody get_transformed(const glm::mat4x3 &matrix)
				const;
			BoundingBox get_bounding_box() const;

			inline void add_polygon(const Polygon &polygon) noexcept
			{
				m_polygons.push_back(polygon);
			}

			inline Uint32 get_polygon_count() const noexcept
			{
				return m_polygons.size();
			}

			inline const Polygon &get_polygon(const Uint32 index)
				const noexcept
			{
				return m_polygons[index];
			}

	};

}

#endif	/* UUID_672a4618_07e4_4f22_a944_4fa7c4540efe */
