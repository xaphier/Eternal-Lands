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
			LightData();

			/**
			 * Default constructor.
			 */
			LightData(const glm::vec3 &position,
				const glm::vec3 &color, const float radius,
				const Uint32 id);

			/**
			 * Default destructor.
			 */
			virtual ~LightData() throw();

			inline const glm::vec3 &get_position() const
			{
				return m_position;
			}

			inline void set_position(const glm::vec3 &position)
			{
				m_position = position;
			}

			/**
			 * The radius of the light.
			 */
			inline float get_radius() const
			{
				return m_radius;
			}

			inline void set_radius(const float radius)
			{
				m_radius = radius;
			}

			inline const glm::vec3 &get_color() const
			{
				return m_color;
			}

			inline void set_color(const glm::vec3 &color)
			{
				m_color = color;
			}

			inline Uint32 get_id() const
			{
				return m_id;
			}

			inline void set_id(const Uint32 id)
			{
				m_id = id;
			}

	};

}

#endif	/* UUID_65a1e530_211e_4a90_9b91_5ea71cd8ffc3 */
