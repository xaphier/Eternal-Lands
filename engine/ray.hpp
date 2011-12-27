/****************************************************************************
 *            ray.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e21c4dfe_4bf8_4bee_8144_378bb9bfb9f9
#define	UUID_e21c4dfe_4bf8_4bee_8144_378bb9bfb9f9

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class Ray.
 * This file contains the @c class Ray.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for ray.
	 *
	 * @c class for ray.
	 */
	class Ray
	{
		private:
			glm::vec3 m_origin;
			glm::vec3 m_direction;

		public:
			Ray();
			Ray(const glm::vec3 origin, const glm::vec3 direction);
			~Ray() throw();

			inline const glm::vec3 &get_origin() const
			{
				return m_origin;
			}

			inline void set_origin(const glm::vec3 &origin)
			{
				m_origin = origin;
			}

			inline const glm::vec3 &get_direction() const
			{
				return m_direction;
			}

			inline void set_direction(const glm::vec3 &direction)
			{
				m_direction = direction;
			}

			inline glm::vec3 get_point(const float t)
			{
				return get_origin() + get_direction() * t;
			}

	};

}

#endif	/* UUID_e21c4dfe_4bf8_4bee_8144_378bb9bfb9f9 */
