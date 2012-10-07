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
			Uint16Array4 m_values;
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
				return glm::uvec4(PackTool::unpack_uint_4_4_4_4(
					false, m_values[index]));
			}

			inline glm::vec4 get_normalized_value(
				const Uint16 index) const
			{
				return PackTool::unpack_uint_4_4_4_4(true,
					m_values[index]);
			}

			inline Uint16 get_packed_value(const Uint16 index) const
			{
				return m_values[index];
			}

			inline void set_value(const glm::uvec4 &value,
				const Uint16 index)
			{
				m_values[index] = PackTool::pack_uint_4_4_4_4(
					false, glm::vec4(value));
			}

			inline void set_normalized_value(
				const glm::vec4 &value, const Uint16 index)
			{
				m_values[index] = PackTool::pack_uint_4_4_4_4(
					true, glm::vec4(value));
			}

			inline void set_packed_value(const Uint16 value,
				const Uint16 index)
			{
				m_values[index] = value;
			}

			static inline Uint16 get_image_count()
			{
				return Uint16Array4::size() * 4;
			}

	};

	VECTOR(ImageValue);

}

#endif	/* UUID_98494b0c_83c4_450d_a2d4_dd06322d2a54 */
