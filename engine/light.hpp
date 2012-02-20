/****************************************************************************
 *            light.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_44332907_0322_4e7c_8dac_20611701476a
#define	UUID_44332907_0322_4e7c_8dac_20611701476a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "boundedobject.hpp"
#include "lightdata.hpp"

/**
 * @file
 * @brief The @c class Light.
 * This file contains the @c class Light.
 */
namespace eternal_lands
{

	class Light: public BoundedObject
	{
		private:
			LightData m_light_data;
			float m_inv_sqr_radius;

			inline void set_inv_sqr_radius(const float radius)
			{
				m_inv_sqr_radius = 1.0f / (radius * radius);
			}

		public:
			/**
			 * Default constructor.
			 */
			Light(const LightData &light_data);

			/**
			 * Default destructor.
			 */
			virtual ~Light() throw();

			void update_bounding_box(const float scale = 1.0f);

			/**
			 * @brief Intersection test.
			 *
			 * Test the bouding box for intersection with this
			 * light.
			 * @param box Bouding box.
			 * @return True if the volumes intersect, else false.
			 */
			bool intersect(const BoundingBox &box) const;

			inline const glm::vec3 &get_position() const
			{
				return m_light_data.get_position();
			}

			inline void set_position(const glm::vec3 &position)
			{
				m_light_data.set_position(position);
			}

			/**
			 * The radius of the light.
			 */
			inline float get_radius() const
			{
				return m_light_data.get_radius();
			}

			/**
			 * The inverse squared radius of the light. Used to
			 * calculate the light attenuation.
			 * attenuation = max(0.0, 1.0 - (light_pos - pos)^2 *
			 * inv_sqr_radius)
			 */
			inline float get_inv_sqr_radius() const
			{
				return m_inv_sqr_radius;
			}

			inline void set_radius(const float radius)
			{
				m_light_data.set_radius(radius);
				set_inv_sqr_radius(radius);
			}

			inline const glm::vec3 &get_color() const
			{
				return m_light_data.get_color();
			}

			inline void set_color(const glm::vec3 &color)
			{
				m_light_data.set_color(color);
			}

			inline Uint32 get_id() const
			{
				return m_light_data.get_id();
			}

	};

}

#endif	/* UUID_44332907_0322_4e7c_8dac_20611701476a */
