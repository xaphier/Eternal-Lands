/****************************************************************************
 *            lightdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_65a1e530_211e_4a90_9b91_5ea71cd8ffc3
#define	UUID_65a1e530_211e_4a90_9b91_5ea71cd8ffc3

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class LightData.
 * This file contains the @c class LightData.
 */
namespace eternal_lands
{

	class LightData
	{
		private:
			glm::vec3 m_position;
			glm::vec3 m_color;
			float m_radius;
			Uint32 m_id;

		public:
			/**
			 * Default constructor.
			 */
			inline LightData(): m_radius(0.0f), m_id(0)
			{
			}

			/**
			 * Default constructor.
			 */
			inline LightData(const glm::vec3 &position,
				const glm::vec3 &color, const float radius,
				const Uint32 id): m_position(position),
				m_color(color), m_radius(radius), m_id(id)
			{
			}

			/**
			 * Default destructor.
			 */
			inline ~LightData() noexcept
			{
			}

			inline const glm::vec3 &get_position() const noexcept
			{
				return m_position;
			}

			inline void set_position(const glm::vec3 &position)
				noexcept
			{
				m_position = position;
			}

			/**
			 * The radius of the light.
			 */
			inline float get_radius() const noexcept
			{
				return m_radius;
			}

			inline void set_radius(const float radius) noexcept
			{
				m_radius = radius;
			}

			inline const glm::vec3 &get_color() const noexcept
			{
				return m_color;
			}

			inline void set_color(const glm::vec3 &color) noexcept
			{
				m_color = color;
			}

			inline Uint32 get_id() const noexcept
			{
				return m_id;
			}

			inline void set_id(const Uint32 id) noexcept
			{
				m_id = id;
			}

	};

}

#endif	/* UUID_65a1e530_211e_4a90_9b91_5ea71cd8ffc3 */
