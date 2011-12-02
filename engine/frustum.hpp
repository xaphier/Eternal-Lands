/****************************************************************************
 *            frustum.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_0fd3a14d_9ccd_4936_97eb_8dabed20ec21
#define	UUID_0fd3a14d_9ccd_4936_97eb_8dabed20ec21

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "plane.hpp"

/**
 * @file
 * @brief The @c class Frustum.
 * This file contains the @c class Frustum.
 */
namespace eternal_lands
{

	/**
	 * Different types of planes used for frustums.
	 * @{
	 */
	enum PlaneType
	{
		pt_right = 0,
		pt_left = 1,
		pt_top = 2,
		pt_bottom = 3,
		pt_near = 4,
		pt_far = 5
	};
	/**
	 * @}
	 */

	class Frustum
	{
		private:
			boost::array<Plane, 28> m_planes;
			PlanesMask m_planes_mask;

			void build_frustum(const BoundingBox &box);
			void set_plane(const glm::mat4 &matrix,
				const PlaneType plane,
				const Uint16 sub_frustum_index);
			void set_sub_frustum(const glm::mat4 &matrix,
				const Uint16 sub_frustum_index);
			void intersect(const BoundingBox &box,
				const PlanesMask in_mask,
				const Uint16 sub_frustum_index,
				PlanesMask &out_mask) const;
			Vec3Array8 get_corner_points(
				const Uint16 sub_frustum_index) const;

		public:
			Frustum();
			Frustum(const glm::mat4x4 &matrix);
			Frustum(const Mat4x4Vector &matrices);
			Frustum(const BoundingBox &box);
			~Frustum() throw();
			static Plane get_plane(const glm::mat4 &matrix,
				const PlaneType plane);
			static const String get_str(const PlaneType plane);

			inline const PlanesMask &get_planes_mask() const
			{
				return m_planes_mask;
			}

			inline const Plane &get_plane(const Uint32 index) const
			{
				return m_planes[index];
			}

			inline IntersectionType intersect(
				const BoundingBox &box) const
			{
				PlanesMask out_mask;

				return intersect(box, get_planes_mask(),
					out_mask);
			}

			inline IntersectionType intersect(
				const BoundingBox &box,
				const PlanesMask in_mask) const
			{
				PlanesMask out_mask;

				return intersect(box, in_mask, out_mask);
			}

			IntersectionType intersect(const BoundingBox &box,
				const PlanesMask in_mask, PlanesMask &out_mask)
				const;

			inline SubFrustumsMask intersect_sub_frustums(
				const BoundingBox &box) const
			{
				return intersect_sub_frustums(box,
					get_planes_mask());
			}

			SubFrustumsMask intersect_sub_frustums(
				const BoundingBox &box,
				const PlanesMask in_mask) const;

			SubFrustumsMask get_sub_frustums_mask(
				const PlanesMask mask) const;

			SubFrustumsBoundingBoxes get_bounding_boxes() const;

			Frustum transform(const glm::mat4x3 &world_matrix)
				const;

	};

	OutStream& operator<<(OutStream &str, const PlaneType value);

}

#endif	/* UUID_0fd3a14d_9ccd_4936_97eb_8dabed20ec21 */
