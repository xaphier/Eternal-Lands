/****************************************************************************
 *            particledata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_05bced99_02f0_467f_aa2e_f4d58be040f6
#define	UUID_05bced99_02f0_467f_aa2e_f4d58be040f6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ParticleData.
 * This file contains the @c class ParticleData.
 */
namespace eternal_lands
{

	class ParticleData
	{
		private:
			glm::vec3 m_position;
			String m_name;
			Uint32 m_id;

		public:
			inline ParticleData(const glm::vec3 &position,
				const String &name, const Uint32 id):
				m_position(position), m_name(name), m_id(id)
			{
			}

			inline ~ParticleData() noexcept
			{
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
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

#endif	/* UUID_05bced99_02f0_467f_aa2e_f4d58be040f6 */
