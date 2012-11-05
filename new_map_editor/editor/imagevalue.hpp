/****************************************************************************
 *            imagevalue.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_98494b0c_83c4_450d_a2d4_dd06322d2a54
#define	UUID_98494b0c_83c4_450d_a2d4_dd06322d2a54

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "packtool.hpp"

namespace eternal_lands
{

	class ImageValue
	{
		private:
			Uint8Array16 m_values;
			Uint16 m_x;
			Uint16 m_y;

		public:
			inline ImageValue(const Uint16 x, const Uint16 y):
				m_x(x), m_y(y)
			{
			}

			inline Uint16 get_x() const
			{
				return m_x;
			}

			inline Uint16 get_y() const
			{
				return m_y;
			}

			inline Uint32 get_index() const
			{
				return static_cast<Uint32>(get_x()) +
					(static_cast<Uint32>(get_y()) << 16);
			}

			inline glm::uvec4 get_value(const Uint16 index) const
			{
				glm::uvec4 result;

				result.x = m_values[index * 4 + 0];
				result.y = m_values[index * 4 + 1];
				result.z = m_values[index * 4 + 2];
				result.w = m_values[index * 4 + 3];

				return result;
			}

			inline glm::vec4 get_normalized_value(
				const Uint16 index) const
			{
				glm::vec4 result;

				result.x = m_values[index * 4 + 0];
				result.y = m_values[index * 4 + 1];
				result.z = m_values[index * 4 + 2];
				result.w = m_values[index * 4 + 3];

				return result / 255.0f;
			}

			inline void set_value(const glm::uvec4 &value,
				const Uint16 index)
			{
				m_values[index * 4 + 0] = value.x;
				m_values[index * 4 + 1] = value.y;
				m_values[index * 4 + 2] = value.z;
				m_values[index * 4 + 3] = value.w;
			}

			inline void set_normalized_value(
				const glm::vec4 &value, const Uint16 index)
			{
				m_values[index * 4 + 0] = value.x * 255.0f +
					0.5f;
				m_values[index * 4 + 1] = value.y * 255.0f +
					0.5f;
				m_values[index * 4 + 2] = value.z * 255.0f +
					0.5f;
				m_values[index * 4 + 3] = value.w * 255.0f +
					0.5f;
			}

			static inline Uint16 get_image_count()
			{
				return Uint16Array4::size() * 4;
			}

	};

	VECTOR(ImageValue);

}

#endif	/* UUID_98494b0c_83c4_450d_a2d4_dd06322d2a54 */
