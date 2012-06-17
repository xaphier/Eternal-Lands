/****************************************************************************
 *            terrainvalue.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_64090f12_e559_4c72_a2b7_1ec0fc1d5ce3
#define	UUID_64090f12_e559_4c72_a2b7_1ec0fc1d5ce3

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	class TerrainValue
	{
		private:
			struct
			{
				Uint32 r : 10;
				Uint32 g : 10;
				Uint32 b : 10;
				Uint32 a : 2;
			} m_value;
			Uint16 m_x;
			Uint16 m_y;

		public:
			inline TerrainValue(const Uint16 x, const Uint16 y):
				m_x(x), m_y(y)
			{
				m_value.r = 0;
				m_value.g = 0;
				m_value.b = 0;
				m_value.a = 0;
			}

			inline glm::uvec4 get_value() const
			{
				return glm::uvec4(m_value.r, m_value.g,
					m_value.b, m_value.a);
			}

			inline Uint16 get_x() const
			{
				return m_x;
			}

			inline Uint16 get_y() const
			{
				return m_y;
			}

			inline void set_value(const glm::uvec4 &value)
			{
				m_value.r = value.r;
				m_value.g = value.g;
				m_value.b = value.b;
				m_value.a = value.a;
			}

			inline Uint32 get_index() const
			{
				return static_cast<Uint32>(get_x()) +
					(static_cast<Uint32>(get_y()) << 16);
			}

	};

	BOOST_STATIC_ASSERT(sizeof(TerrainValue) == 8);

	VECTOR(TerrainValue);

}

#endif	/* UUID_64090f12_e559_4c72_a2b7_1ec0fc1d5ce3 */
