/****************************************************************************
 *            decaldescription.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2ce26792_31dc_4555_978c_8b3e82b5eaf0
#define	UUID_2ce26792_31dc_4555_978c_8b3e82b5eaf0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class DecalDescription.
 * This file contains the @c class DecalDescription.
 */
namespace eternal_lands
{

	class DecalDescription
	{
		private:
			String m_texture;
			glm::mat2x3 m_transform_matrix; 

		public:
			inline DecalDescription()
			{
			}

			inline DecalDescription(const String &texture,
				const glm::mat2x3 &transform_matrix):
				m_texture(texture),
				m_transform_matrix(transform_matrix)
			{
			}

			inline ~DecalDescription() noexcept
			{
			}

			inline const String &get_texture() const noexcept
			{
				return m_texture;
			}

			inline const glm::mat2x3 &get_transform_matrix() const
				noexcept
			{
				return m_transform_matrix;
			}

			inline void set_texture(const String &texture) noexcept
			{
				m_texture = texture;
			}

			inline void set_transform_matrix(
				const glm::mat2x3 &transform_matrix) noexcept
			{
				m_transform_matrix = transform_matrix;
			}

	};

}

#endif	/* UUID_2ce26792_31dc_4555_978c_8b3e82b5eaf0 */
