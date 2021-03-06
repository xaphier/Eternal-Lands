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

namespace eternal_lands
{

	class ImageValue
	{
		private:
			Uint16 m_x;
			Uint16 m_y;
			Uint8 m_value;

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

			inline Uint16 get_value() const
			{
				return m_value;
			}

			inline void set_value(const Uint16 value)
			{
				m_value = value;
			}

	};

	VECTOR(ImageValue);

}

#endif	/* UUID_98494b0c_83c4_450d_a2d4_dd06322d2a54 */
