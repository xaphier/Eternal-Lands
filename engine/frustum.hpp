/****************************************************************************
 *            frustum.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			boost::array<Plane, 6> m_planes;
			BitSet64 m_planes_mask;

			void build_frustum(const BoundingBox &box);
			void set_plane(const glm::mat4 &matrix,
				const PlaneType plane);

		public:
			Frustum();
			Frustum(const glm::mat4x4 &matrix);
			Frustum(const BoundingBox &box);
			~Frustum() noexcept;
			static Plane get_plane(const glm::mat4 &matrix,
				const PlaneType plane);
			static const String get_str(const PlaneType plane);
			static PlaneVector get_planes(const BoundingBox &box);
			PlaneVector get_planes() const;

			inline const BitSet64 &get_planes_mask() const
				noexcept
			{
				return m_planes_mask;
			}

			inline const Plane &get_plane(const Uint16 index) const
				noexcept
			{
				return m_planes[index];
			}

			inline IntersectionType intersect(
				const BoundingBox &box) const
			{
				BitSet64 out_mask;

				return intersect(box, get_planes_mask(),
					out_mask);
			}

			inline IntersectionType intersect(
				const BoundingBox &box,
				const BitSet64 in_mask) const
			{
				BitSet64 out_mask;

				return intersect(box, in_mask, out_mask);
			}

			IntersectionType intersect(const BoundingBox &box,
				const BitSet64 in_mask, BitSet64 &out_mask)
				const;

			inline IntersectionType intersect(
				const glm::vec3 &point) const
			{
				BitSet64 out_mask;

				return intersect(point, get_planes_mask(),
					out_mask);
			}

			inline IntersectionType intersect(
				const glm::vec3 &point,
				const BitSet64 in_mask) const
			{
				BitSet64 out_mask;

				return intersect(point, in_mask, out_mask);
			}

			IntersectionType intersect(const glm::vec3 &point,
				const BitSet64 in_mask, BitSet64 &out_mask)
				const;

			BoundingBox get_bounding_box() const;

			Vec3Array8 get_corner_points() const;

			void transform(const glm::mat4x3 &matrix);

			Frustum get_transformed(const glm::mat4x3 &matrix)
				const;

	};

	OutStream& operator<<(OutStream &str, const PlaneType value);

}

#endif	/* UUID_0fd3a14d_9ccd_4936_97eb_8dabed20ec21 */
