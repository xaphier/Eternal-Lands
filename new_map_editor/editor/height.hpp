/****************************************************************************
 *            height.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c6561ea6_a6f9_43f7_a9b0_0fddabf3903e
#define	UUID_c6561ea6_a6f9_43f7_a9b0_0fddabf3903e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	class Height
	{
		private:
			Uint16 m_value;
			Uint16 m_x;
			Uint16 m_y;

		public:
			inline Height(const Uint16 x, const Uint16 y,
				const Uint16 value = 0): m_value(value),
				m_x(x), m_y(y)
			{
			}

			inline float get_value() const
			{
				return m_value;
			}

			inline Uint16 get_x() const
			{
				return m_x;
			}

			inline Uint16 get_y() const
			{
				return m_y;
			}

			inline void set_value(const float value)
			{
				m_value = value;
			}

			inline Uint32 get_index() const
			{
				return static_cast<Uint32>(get_x()) +
					(static_cast<Uint32>(get_y()) << 16);
			}

	};

	VECTOR(Height);

}

#endif	/* UUID_c6561ea6_a6f9_43f7_a9b0_0fddabf3903e */
