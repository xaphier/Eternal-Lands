/****************************************************************************
 *            boundingbox.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2ef1e43a_4a5f_44a9_a055_9c0c30f318df
#define	UUID_2ef1e43a_4a5f_44a9_a055_9c0c30f318df

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	/**
	 * @brief Axis aligned bounding box.
	 *
	 * One axis aligned bounding box.
	 */
	class BoundingBox
	{
		private:
			/**
			 * @brief Contains the three center values.
			 *
			 * Contains the three center values of the bounding box.
			 */
			glm::vec3 m_center;

			/**
			 * @brief Contains the three half size values.
			 *
			 * Contains the three half size values of the bounding
			 * box.
			 */
			glm::vec3 m_half_size;

		public:
			/**
			 * @brief Default constructor.
			 *
			 * Default constructor. Sets all values to zero
			 */
			BoundingBox();

			/**
			 * @brief Min-Max constructor.
			 *
			 * Constructs the bounding box from the given min and
			 * max values.
			 * @throw InvalidParameter if min > max.
			 * @param min The minimal extend of the bounding box.
			 * @param max The maximal extend of the bounding box.
			 */
			BoundingBox(const glm::vec3 &min, const glm::vec3 &max);

			/**
			 * @brief Points constructor.
			 *
			 * Constructs the bounding box from the eight points.
			 * @param points The eight points.
			 */
			BoundingBox(const Vec3Array8 &points);

			/**
			 * Default destructor.
			 */
			~BoundingBox() throw();

			/**
			 * Sets the bounding box to an empty volume.
			 */
			inline void set_empty()
			{
				m_center = glm::vec3(0.0f);
				m_half_size = glm::vec3(-1e30f);
			}

			/**
			 * @brief Min-Max set.
			 *
			 * Sets the bounding box from the given min and max
			 * values.
			 * @throw InvalidParameter if min > max.
			 * @param min The minimal extend of the bounding box.
			 * @param max The maximal extend of the bounding box.
			 */
			void set_min_max(const glm::vec3 &min,
				const glm::vec3 &max);

			/**
			 * @brief Half size set.
			 *
			 * Sets the half size of the bounding box.
			 * @throw RangeErrorException if half_size < 0.0f or
			 * half_size > MAX_BOUNDING_RADIUS
			 * @param half_size The minimal extend of the bounding
			 * box.
			 */
			void set_half_size(const glm::vec3 &half_size);

			/**
			 * @brief Get minimum.
			*
			 * Gets the minimun of the box.
			 * @return The minimum of the box.
			 */
			inline glm::vec3 get_min() const
			{
				return get_center() - get_half_size();
			}

			/**
			 * @brief Get maximum.
			 *
			 * Gets the maximum of the box.
			 * @return The maximum of the box.
			 */
			inline glm::vec3 get_max() const
			{
				return get_center() + get_half_size();
			}

			/**
			 * @brief Gets one of the coner points.
			 *
			 * Returns the conver point indexed by the given index.
			 * @param index The index of the corner point.
			 * @return The corner point indexed by index.
			 */
			inline glm::vec3 get_point(const std::bitset<3> index)
				const
			{
				glm::vec3 result;
				Uint16 i;

				for (i = 0; i < 3; i++)
				{
					if (index[i])
					{
						result[i] = get_min()[i];
					}
					else
					{
						result[i] = get_max()[i];
					}
				}

				return result;
			}

			Vec3Array8 get_corner_points() const
			{
				Vec3Array8 result;
				Uint16 i;

				for (i = 0; i < 8; i++)
				{
					result[i] = get_point(
						std::bitset<3>(i));
				}

				return result;
			}

			/**
			 * @brief Sets the center
			 *
			 * Sets the center of the bounding box.
			 * @param center The new center of the box.
			 */
			inline void set_center(const glm::vec3 &center)
			{
				assert(std::abs(center[0]) < 1e7f);
				assert(std::abs(center[1]) < 1e7f);
				assert(std::abs(center[2]) < 1e7f);
				m_center = center;
			}

			/**
			 * @brief Gets the center
			 *
			 * Returns the center of the bounding box.
			 * @return The center of the box.
			 */
			inline const glm::vec3 &get_center() const
			{
				return m_center;
			}

			/**
			 * @brief Gets the half sizes
			*
			 * Returns the half sizes of the bounding box.
			 * @return The half sizes of the box.
			 */
			inline const glm::vec3 &get_half_size() const
			{
				return m_half_size;
			}

			/**
			 * @brief Gets the half sizes
			*
			 * Returns the half sizes of the bounding box.
			 * @return The half sizes of the box.
			 */
			inline bool get_empty() const
			{
				return !glm::all(glm::greaterThanEqual(
					m_half_size, glm::vec3(0.0f)));
			}

			/**
			 * @brief Scales the box.
			 *
			 * Scales the box using the given value.
			 * @param s The scale value.
			 */
			inline void scale(const float s)
			{
				m_half_size *= s;
			}

			/**
			 * @brief Extends the box.
			 *
			 * Extends the box using the given value.
			 * @param s The extend value.
			 */
			inline void extend(const float e)
			{
				m_half_size += e;
			}

			/**
			 * @brief Volume calculation.
			 *
			 * Calculates the volume of this bounding box.
			 * @return The volume of this boundign box.
			 */
			inline float get_volume() const
			{
				return get_half_size()[0] * get_half_size()[1]
					* get_half_size()[2] * 8.0f;
			}

			/**
			 * @brief Margin calculation.
			 *
			 * Calculates the margin of this bounding box.
			 * @return The margin of this boundign box.
			 */
			inline float get_margin() const
			{
				return (get_half_size()[0] + get_half_size()[1]
					+ get_half_size()[2]) * 2.0f;
			}

			/**
			 * @brief Clamp.
			 *
			 * Clamps this bounding boxes with the given one.
			 * @param box Bounding box for merge.
			 */
			void clamp(const BoundingBox &box);

			/**
			 * @brief Merge.
			 *
			 * Merges this bounding boxes with the given one.
			 * @param box Bounding box for merge.
			 */
			void merge(const BoundingBox &box);

			/**
			 * @brief Overlap calculation.
			 *
			 * Calculates the overlap of this bounding boxes with
			 * the given one.
			 * @param box Bounding box for overlap.
			 * @return The overlap of this bounding boxes with the
			 * given one.
			 */
			float overlap(const BoundingBox &box) const;

			/**
			 * @brief Contains test.
			 *
			 * Tests if it contains the boudning box.
			 * @param box Bounding box.
			 * @return Returns true if it contains the bounding box,
			 * else false.
			 */
			bool contains(const BoundingBox &box) const;

			/**
			 * @brief Gets a transformed box.
			 *
			 * Returns the transformed box using the given matrix.
			 * @param matrix The transform matrix.
			 * @return The transformed box.
			 */
			BoundingBox transform(const glm::mat4x3 &matrix) const;

			/**
			 * @brief Gets a transformed box.
			 *
			 * Returns the transformed box using the given matrix.
			 * @param matrix The transform matrix.
			 * @return The transformed box.
			 */
			BoundingBox transform(const glm::mat4x4 &matrix) const;

			/**
			 * @brief Gets the enlargment.
			 *
			 * Returns the enlargment of the box if it would be merged
			 * with the givien box.
			 * @param box The box that would be merged.
			 * @return The enlargement if the boxes would be merged.
			 */
			float enlargement(const BoundingBox &box) const;

			/**
			 * @brief Gets the square distance.
			 *
			 * Returns the square distance of the box to the given
			 * point.
			 * @param point The point to use.
			 * @return The square distance of the box to the given
			 * point.
			 */
			float sqr_distance(const glm::vec3 &point) const;

			/**
			 * @brief Gets the distance.
			 *
			 * Returns the distance of the box to the given point.
			 * @param point The point to use.
			 * @return The distance of the box to the given point.
			 */
			float distance(const glm::vec3 &point) const;

			bool intersect(const glm::mat4 &view_matrix,
				const glm::mat4 &projection_matrix,
				const glm::ivec4 &viewport,
				const glm::vec2 &center,
				const glm::vec2 &half_size) const;

	};

	OutStream& operator<<(OutStream &str, const BoundingBox &value);

}

#endif	/* UUID_2ef1e43a_4a5f_44a9_a055_9c0c30f318df */
