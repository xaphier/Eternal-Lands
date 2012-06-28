/****************************************************************************
 *            imagevalue.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_98494b0c_83c4_450d_a2d4_dd06322d2a54
#define	UUID_98494b0c_83c4_450d_a2d4_dd06322d2a54

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	class ImageValue
	{
		private:
			struct BlendValue
			{
				Uint16 r : 4;
				Uint16 g : 4;
				Uint16 b : 4;
				Uint16 a : 4;
			};

			boost::array<BlendValue, 4> m_values;
			Uint16 m_x;
			Uint16 m_y;

		public:
			inline ImageValue(const Uint16 x, const Uint16 y):
				m_x(x), m_y(y)
			{
				m_values[0].r = 0;
				m_values[0].g = 0;
				m_values[0].b = 0;
				m_values[0].a = 0;

				m_values[1].r = 0;
				m_values[1].g = 0;
				m_values[1].b = 0;
				m_values[1].a = 0;

				m_values[2].r = 0;
				m_values[2].g = 0;
				m_values[2].b = 0;
				m_values[2].a = 0;

				m_values[3].r = 0;
				m_values[3].g = 0;
				m_values[3].b = 0;
				m_values[3].a = 0;
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
				return glm::uvec4(m_values[index].r,
					m_values[index].g, m_values[index].b,
					m_values[index].a);
			}

			inline Uvec4Array4 get_values() const
			{
				Uvec4Array4 result;

				result[0] = get_value(0);
				result[1] = get_value(1);
				result[2] = get_value(2);
				result[3] = get_value(3);

				return result;
			}

			inline void set_value(const glm::uvec4 &value,
				const Uint16 index)
			{
				m_values[index].r = value.r;
				m_values[index].g = value.g;
				m_values[index].b = value.b;
				m_values[index].a = value.a;
			}

			inline void set_values(const Uvec4Array4 &values)
			{
				set_value(values[0], 0);
				set_value(values[1], 1);
				set_value(values[2], 2);
				set_value(values[3], 3);
			}

	};

	BOOST_STATIC_ASSERT(sizeof(ImageValue) == 12);

	VECTOR(ImageValue);

}

#endif	/* UUID_98494b0c_83c4_450d_a2d4_dd06322d2a54 */
