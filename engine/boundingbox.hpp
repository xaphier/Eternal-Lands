/****************************************************************************
 *            boundingbox.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2ef1e43a_4a5f_44a9_a055_9c0c30f318df
#define	UUID_2ef1e43a_4a5f_44a9_a055_9c0c30f318df

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#ifdef	__SSE__
#include <xmmintrin.h>
#endif	/* __SSE__ */
#ifdef	__SSE4_1__
#include <smmintrin.h>
#endif	/* __SSE4_1__ */

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
#ifdef	__SSE__
			__m128 m_center;
#else	/* __SSE__ */
			glm::vec3 m_center;
#endif	/* __SSE__ */

			/**
			 * @brief Contains the three half size values.
			 *
			 * Contains the three half size values of the bounding
			 * box.
			 */
#ifdef	__SSE__
			__m128 m_half_size;
#else	/* __SSE__ */
			glm::vec3 m_half_size;
#endif	/* __SSE__ */

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
			~BoundingBox() noexcept;

			/**
			 * Sets the bounding box to an empty volume.
			 */
			inline void set_empty() noexcept
			{
#ifdef	__SSE__
				m_center = _mm_setzero_ps();
				m_half_size = _mm_set1_ps(-1e30f);
#else	/* __SSE__ */
				m_center = glm::vec3(0.0f);
				m_half_size = glm::vec3(-1e30f);
#endif	/* __SSE__ */
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
			inline void set_min_max(const glm::vec3 &min,
				const glm::vec3 &max) noexcept
			{
				if (!glm::all(glm::lessThanEqual(min, max)))
				{
					set_empty();

					return;
				}

				set_center((min + max) * 0.5f);
				set_half_size((max - min) * 0.5f);
			}

			/**
			 * @brief Half size set.
			 *
			 * Sets the half size of the bounding box.
			 * @throw RangeErrorException if half_size < 0.0f or
			 * half_size > MAX_BOUNDING_RADIUS
			 * @param half_size The minimal extend of the bounding
			 * box.
			 */
			inline void set_half_size(const glm::vec3 &half_size)
				noexcept
			{
				if (!glm::all(glm::greaterThanEqual(half_size,
					glm::vec3(0.0f))))
				{
					set_empty();

					return;
				}

#ifdef	__SSE__
				m_half_size = _mm_setr_ps(half_size.x,
					half_size.y, half_size.z, 0.0f);
#else	/* __SSE__ */
				m_half_size = half_size;
#endif	/* __SSE__ */
			}

			/**
			 * @brief Get minimum.
			*
			 * Gets the minimun of the box.
			 * @return The minimum of the box.
			 */
			inline glm::vec3 get_min() const noexcept
			{
				return get_center() - get_half_size();
			}

			/**
			 * @brief Get maximum.
			 *
			 * Gets the maximum of the box.
			 * @return The maximum of the box.
			 */
			inline glm::vec3 get_max() const noexcept
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
				const noexcept
			{
				glm::vec3 result;
				Uint16 i;

				for (i = 0; i < 3; ++i)
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

			Vec3Array8 get_corner_points() const noexcept
			{
				Vec3Array8 result;
				Uint16 i;

				for (i = 0; i < 8; ++i)
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
			inline void set_center(const glm::vec3 &center) noexcept
			{
				assert(std::abs(center[0]) < 1e7f);
				assert(std::abs(center[1]) < 1e7f);
				assert(std::abs(center[2]) < 1e7f);
#ifdef	__SSE__
				m_center = _mm_setr_ps(center.x, center.y,
					center.z, 0.0f);
#else	/* __SSE__ */
				m_center = center;
#endif	/* __SSE__ */
			}

			/**
			 * @brief Gets the center
			 *
			 * Returns the center of the bounding box.
			 * @return The center of the box.
			 */
			inline glm::vec3 get_center() const noexcept
			{
#ifdef	__SSE__
				glm::vec4 result;

				_mm_storeu_ps(glm::value_ptr(result), m_center);

				return glm::vec3(result);
#else	/* __SSE__ */
				return m_center;
#endif	/* __SSE__ */
			}

			/**
			 * @brief Gets the half sizes
			*
			 * Returns the half sizes of the bounding box.
			 * @return The half sizes of the box.
			 */
			inline glm::vec3 get_half_size() const noexcept
			{
#ifdef	__SSE__
				glm::vec4 result;

				_mm_storeu_ps(glm::value_ptr(result),
					m_half_size);

				return glm::vec3(result);
#else	/* __SSE__ */
				return m_half_size;
#endif	/* __SSE__ */
			}

			/**
			 * @brief Gets the half sizes
			*
			 * Returns the half sizes of the bounding box.
			 * @return The half sizes of the box.
			 */
			inline bool get_empty() const noexcept
			{
#ifdef	__SSE__
				return !((_mm_movemask_ps(_mm_cmpge_ps(
					m_half_size, _mm_setzero_ps())) & 0x7)
					== 0x7);
#else	/* __SSE__ */
				return !glm::all(glm::greaterThanEqual(
					m_half_size, glm::vec3(0.0f)));
#endif	/* __SSE__ */
			}

			/**
			 * @brief Scales the box.
			 *
			 * Scales the box using the given value.
			 * @param s The scale value.
			 */
			inline void scale(const float s) noexcept
			{
#ifdef	__SSE__
				m_half_size = _mm_mul_ps(m_half_size,
					_mm_set1_ps(s));
#else	/* __SSE__ */
				m_half_size *= s;
#endif	/* __SSE__ */
			}

			/**
			 * @brief Extends the box.
			 *
			 * Extends the box using the given value.
			 * @param s The extend value.
			 */
			inline void extend(const float e) noexcept
			{
#ifdef	__SSE__
				m_half_size = _mm_add_ps(m_half_size,
					_mm_setr_ps(e, e, e, 0.0f));
#else	/* __SSE__ */
				m_half_size += e;
#endif	/* __SSE__ */
			}

			/**
			 * @brief Volume calculation.
			 *
			 * Calculates the volume of this bounding box.
			 * @return The volume of this boundign box.
			 */
			inline float get_volume() const noexcept
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
			inline float get_margin() const noexcept
			{
#ifdef	__SSE4_1__
				return _mm_cvtss_f32(_mm_dp_ps(
					m_half_size, _mm_set1_ps(2.0f), 0x71));
#else	/* __SSE4_1__ */
				return (get_half_size()[0] + get_half_size()[1]
					+ get_half_size()[2]) * 2.0f;
#endif	/* __SSE4_1__ */
			}

			/**
			 * @brief Clamp.
			 *
			 * Clamps this bounding boxes with the given one.
			 * @param box Bounding box for merge.
			 */
			inline void clamp(const BoundingBox &box) noexcept
			{
#ifdef	__SSE__
				__m128 min0, max0, min1, max1, min, max;

				min0 = _mm_sub_ps(m_center, m_half_size);
				max0 = _mm_add_ps(m_center, m_half_size);

				min1 = _mm_sub_ps(box.m_center,
					box.m_half_size);
				max1 = _mm_add_ps(box.m_center,
					box.m_half_size);

				min = _mm_max_ps(min0, min1);
				max = _mm_min_ps(max0, max1);

				m_center = _mm_mul_ps(_mm_add_ps(min, max),
					_mm_set1_ps(0.5f));
				m_half_size = _mm_mul_ps(_mm_sub_ps(max, min),
					_mm_set1_ps(0.5f));
#else	/* __SSE__ */
				glm::vec3 min, max;

				min = glm::max(get_min(), box.get_min());
				max = glm::min(get_max(), box.get_max());

				set_min_max(min, max);
#endif	/* __SSE__ */
			}

			/**
			 * @brief Merge.
			 *
			 * Merges this bounding boxes with the given one.
			 * @param box Bounding box for merge.
			 */
			inline void merge(const BoundingBox &box) noexcept
			{
#ifdef	__SSE__
				__m128 min0, max0, min1, max1, min, max;

				min0 = _mm_sub_ps(m_center, m_half_size);
				max0 = _mm_add_ps(m_center, m_half_size);

				min1 = _mm_sub_ps(box.m_center,
					box.m_half_size);
				max1 = _mm_add_ps(box.m_center,
					box.m_half_size);

				min = _mm_min_ps(min0, min1);
				max = _mm_max_ps(max0, max1);

				m_center = _mm_mul_ps(_mm_add_ps(min, max),
					_mm_set1_ps(0.5f));
				m_half_size = _mm_mul_ps(_mm_sub_ps(max, min),
					_mm_set1_ps(0.5f));
#else	/* __SSE__ */
				glm::vec3 min, max;

				min = glm::min(get_min(), box.get_min());
				max = glm::max(get_max(), box.get_max());

				set_min_max(min, max);
#endif	/* __SSE__ */
			}

			/**
			 * @brief Overlap calculation.
			 *
			 * Calculates the overlap of this bounding boxes with
			 * the given one.
			 * @param box Bounding box for overlap.
			 * @return The overlap of this bounding boxes with the
			 * given one.
			 */
			inline float overlap(const BoundingBox &box) const
				noexcept
			{
#ifdef	__SSE__
				__m128 t, t0, t1;

				t0 = _mm_and_ps(_mm_sub_ps(m_center,
					box.m_center), _mm_castsi128_ps(
					_mm_set1_epi32(0x7FFFFFFF)));
				t1 = _mm_add_ps(m_half_size, box.m_half_size);

				t = _mm_max_ps(_mm_sub_ps(t1, t0),
					_mm_setzero_ps());

				return _mm_cvtss_f32(_mm_mul_ss(t,
					_mm_mul_ss(_mm_shuffle_ps(t, t,
						_MM_SHUFFLE(1, 1, 1, 1)),
					_mm_movehl_ps(t, t))));
#else	/* __SSE__ */
				glm::vec3 t, t0, t1;

				t0 = glm::abs(get_center() - box.get_center());
				t1 = get_half_size() + box.get_half_size();

				t = glm::max(t1 - t0, glm::vec3(0.0f));

				return t[0] * t[1] * t[2];
#endif	/* __SSE__ */
			}

			/**
			 * @brief Intersect calculation.
			 *
			 * Calculates if the boxes intersect.
			 * @param box Bounding box for intersect.
			 * @return True if the boxes intersect, false else.
			 */
			inline bool intersect(const BoundingBox &box) const
				noexcept
			{
#ifdef	__SSE__
				__m128 t0, t1;

				t0 = _mm_and_ps(_mm_sub_ps(m_center,
					box.m_center), _mm_castsi128_ps(
					_mm_set1_epi32(0x7FFFFFFF)));
				t1 = _mm_add_ps(m_half_size, box.m_half_size);

				return (_mm_movemask_ps(_mm_cmpgt_ps(
					_mm_sub_ps(t1, t0),
					_mm_setzero_ps())) & 0x7) == 0x7;
#else	/* __SSE__ */
				glm::vec3 t0, t1;

				t0 = glm::abs(get_center() - box.get_center());
				t1 = get_half_size() + box.get_half_size();

				return glm::all(glm::greaterThan(t1 - t0,
					glm::vec3(0.0f)));
#endif	/* __SSE__ */
			}

			/**
			 * @brief Contains test.
			 *
			 * Tests if it contains the boudning box.
			 * @param box Bounding box.
			 * @return Returns true if it contains the bounding box,
			 * else false.
			 */
			inline bool contains(const BoundingBox &box) const
				noexcept
			{
#ifdef	__SSE__
				__m128 dist;

				dist = _mm_and_ps(_mm_sub_ps(m_center,
					box.m_center), _mm_castsi128_ps(
					_mm_set1_epi32(0x7FFFFFFF)));
				dist = _mm_add_ps(dist, box.m_half_size);

				return (_mm_movemask_ps(_mm_cmple_ps(dist,
					_mm_mul_ps(m_half_size,
					_mm_set1_ps(1.0001f)))) & 0x7) == 0x7;
#else	/* __SSE__ */
				glm::vec3 dist;

				dist = glm::abs(get_center() -
					box.get_center()) + box.get_half_size();

				return glm::all(glm::lessThanEqual(dist,
					get_half_size() * 1.0001f));
#endif	/* __SSE__ */
			}

			/**
			 * @brief Gets a transformed box.
			 *
			 * Returns the transformed box using the given
			 * transformation.
			 * @param transformation The transformation to use.
			 * @return The transformed box.
			 */
			BoundingBox transform(
				const Transformation &transformation) const
				noexcept;

			/**
			 * @brief Gets a transformed box.
			 *
			 * Returns the transformed box using the given matrix.
			 * @param matrix The transformation matrix.
			 * @return The transformed box.
			 */
			BoundingBox transform(const glm::mat4x4 &matrix) const
				noexcept;

			/**
			 * @brief Gets the enlargment.
			 *
			 * Returns the enlargment of the box if it would be merged
			 * with the givien box.
			 * @param box The box that would be merged.
			 * @return The enlargement if the boxes would be merged.
			 */
			inline float enlargement(const BoundingBox &box) const
				noexcept
			{
#ifdef	__SSE__
				__m128 min0, max0, min1, max1, min, max, size;

				min0 = _mm_sub_ps(m_center, m_half_size);
				max0 = _mm_add_ps(m_center, m_half_size);

				min1 = _mm_sub_ps(box.m_center,
					box.m_half_size);
				max1 = _mm_add_ps(box.m_center,
					box.m_half_size);

				min = _mm_min_ps(min0, min1);
				max = _mm_max_ps(max0, max1);

				size = _mm_sub_ps(max, min);

				return _mm_cvtss_f32(_mm_sub_ps(_mm_mul_ss(size,
					_mm_mul_ss(_mm_shuffle_ps(size, size,
						_MM_SHUFFLE(1, 1, 1, 1)),
					_mm_movehl_ps(size, size))),
					_mm_mul_ss(_mm_mul_ss(m_half_size,
					_mm_mul_ss(_mm_shuffle_ps(m_half_size,
						m_half_size,
						_MM_SHUFFLE(1, 1, 1, 1)),
					_mm_movehl_ps(m_half_size,
						m_half_size))),
					_mm_set_ss(8.0f))));
#else	/* __SSE__ */
				glm::vec3 min, max, size;

				min = glm::min(get_min(), box.get_min());
				max = glm::max(get_max(), box.get_max());

				size = max - min;

				return size[0] * size[1] * size[2] -
					get_half_size()[0] * get_half_size()[1]
					* get_half_size()[2] * 8.0f;
#endif	/* __SSE__ */
			}

			/**
			 * @brief Gets the distance.
			 *
			 * Returns the distance of the box to the given point.
			 * @param point The point to use.
			 * @return The distance of the box to the given point.
			 */
			float distance(const glm::vec3 &point) const noexcept;

			bool intersect(const glm::mat4 &view_matrix,
				const glm::mat4 &projection_matrix,
				const glm::ivec4 &viewport,
				const glm::vec2 &center,
				const glm::vec2 &half_size) const noexcept;

	};

	OutStream &operator<<(OutStream &str, const BoundingBox &value);

}

#endif	/* UUID_2ef1e43a_4a5f_44a9_a055_9c0c30f318df */
