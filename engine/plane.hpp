/****************************************************************************
 *            plane.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2ffdda7b_fa7c_49d2_85d8_d5641f070cc4
#define	UUID_2ffdda7b_fa7c_49d2_85d8_d5641f070cc4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "boundingbox.hpp"
#include "ray.hpp"

/**
 * @file
 * @brief The @c class Plane.
 * This file contains the @c class Plane.
 */
namespace eternal_lands
{

	/**
	 * Different types of intersection.
	 * @{
	 */
	enum IntersectionType
	{
		it_inside = 0,
		it_outside = 1,
		it_intersect = 2
	};
	/**
	 * @}
	 */

	/**
	 * @brief Simple plane.
	 *
	 * Simple plane.
	 */
	class Plane
	{
		private:
			glm::vec4 m_normal_and_distance;

			void set_distance(const float distance)
			{
				m_normal_and_distance.w = distance;
			}

			void recalculate_distance(const glm::vec3 &point)
			{
				set_distance(-glm::dot(point, get_normal()));
			}

			void set_normal(const glm::vec3 &normal)
			{
				glm::vec3 tmp;

				tmp = glm::normalize(normal);

				m_normal_and_distance = glm::vec4(tmp,
					get_distance());
			}

			void set_data(const glm::vec4 &data)
			{
				glm::vec3 normal;

				normal = glm::vec3(data);

				set_normal(normal);

				set_distance(data.w / glm::length(normal));
			}

		public:
			/**
			 * @brief Default constructor.
			 *
			 * Default constructor. Sets all values to zero
			 */
			Plane();
			Plane(const glm::vec3 &point, const glm::vec3 &dir);
			Plane(const glm::vec3 &dir, const float distance);
			Plane(const glm::vec4 &data);

			/**
			 * Default destructor.
			 */
			~Plane() throw();

			inline const glm::vec4 &get_data() const
			{
				return m_normal_and_distance;
			}

			inline glm::vec3 get_normal() const
			{
				return glm::vec3(m_normal_and_distance);
			}

			inline glm::vec3 get_abs_normal() const
			{
				return glm::abs(get_normal());
			}

			inline float get_distance() const
			{
				return m_normal_and_distance.w;
			}

			/**
			 * Gets a member point of the plane.
			 */
			inline glm::vec3 get_member_point() const
			{
				return get_normal() * -get_distance();
			}

			inline float get_distance(const glm::vec3 &point) const
			{
				return glm::dot(get_normal(), point) +
					get_distance();
			}

			inline IntersectionType intersect(
				const glm::vec3 &point) const
			{
				float dist;

				dist = get_distance(point);

				if (dist < epsilon)
				{
					return it_outside;
				}
				else
				{
					if (dist > epsilon)
					{
						return it_inside;
					}
					else
					{
						return it_intersect;
					}
				}
			}

			inline bool intersect(const Ray &ray, float &t) const
			{
				float angle, distance;

				angle = glm::dot(get_normal(),
					ray.get_direction());

				if (std::abs(angle) < epsilon)
				{
					t = 0.0f;

					return false;
				}

				distance = get_distance(ray.get_origin());

				t = -distance / angle;

				return t >= 0.0f;
			}

			inline float get_distance(const BoundingBox &box) const
			{
				float dist, size;

				dist = get_distance(box.get_center());

				size = glm::dot(get_abs_normal(),
					box.get_half_size());

				return std::max(dist - size, 0.0f);
			}

			inline IntersectionType intersect(
				const BoundingBox &box) const
			{
				float dist, size;

				dist = get_distance(box.get_center());

				size = glm::dot(get_abs_normal(),
					box.get_half_size());

				if (dist < -size)
				{
					return it_outside;
				}
				else
				{
					if (dist > size)
					{
						return it_inside;
					}
					else
					{
						return it_intersect;
					}
				}
			}
#ifdef	USE_SSE2_
			IntersectionType intersect_sse2(const BoundingBox &box)
				const;
#endif	/* USE_SSE2 */
			/**
			 * @brief Gets a transformed plane.
			 *
			 * Returns the transformed plane using the given matrix.
			 * @param matrix The transform matrix.
			 * @return The transformed plane.
			 */
			void transform(const glm::mat4x3 &matrix);
			Plane get_transformed(const glm::mat4x3 &matrix) const;

			inline bool intersection_point(const Plane p0,
				const Plane p1, glm::vec3 &point) const
			{
				return intersection_point(p0, p1, *this,
					point);
			}

			static bool intersection_point(const Plane p0,
				const Plane p1, const Plane p2,
				glm::vec3 &point);

			static const String &get_str(
				const IntersectionType intersection);

			static IntersectionType get_intersection_type(
				const String &str);

	};

	OutStream& operator<<(OutStream &str, const IntersectionType value);
	InStream& operator>>(InStream &str, IntersectionType &value);

}

#endif	/* UUID_2ffdda7b_fa7c_49d2_85d8_d5641f070cc4 */
