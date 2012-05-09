/****************************************************************************
 *            ray.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			inline Ray(): m_direction(0.0f, 0.0f, 1.0f)
			{
			}

			inline Ray(const glm::vec3 origin,
				const glm::vec3 direction): m_origin(origin),
				m_direction(direction)
			{
			}

			inline ~Ray() noexcept
			{
			}

			inline const glm::vec3 &get_origin() const noexcept
			{
				return m_origin;
			}

			inline void set_origin(const glm::vec3 &origin) noexcept
			{
				m_origin = origin;
			}

			inline const glm::vec3 &get_direction() const noexcept
			{
				return m_direction;
			}

			inline void set_direction(const glm::vec3 &direction)
				noexcept
			{
				m_direction = direction;
			}

			inline glm::vec3 get_point(const float t) const 
				noexcept
			{
				return get_origin() + get_direction() * t;
			}

	};

}

#endif	/* UUID_e21c4dfe_4bf8_4bee_8144_378bb9bfb9f9 */
