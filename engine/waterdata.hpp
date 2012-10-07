/****************************************************************************
 *            waterdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7cf09bf4_e0cd_4d83_bfba_ee8e1500f5c9
#define	UUID_7cf09bf4_e0cd_4d83_bfba_ee8e1500f5c9

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class WaterData.
 * This file contains the @c class WaterData.
 */
namespace eternal_lands
{

	class WaterData
	{
		private:
			String m_material;
			float m_height;

		public:
			WaterData();
			WaterData(const String &material, const float height);
			~WaterData() noexcept;

			inline void set_material(const String &material)
				noexcept
			{
				m_material = material;
			}

			inline void set_height(const float height) noexcept
			{
				m_height = height;
			}

			inline const String &get_material() const noexcept
			{
				return m_material;
			}

			inline float get_height() const noexcept
			{
				return m_height;
			}

	};

}

#endif	/* UUID_7cf09bf4_e0cd_4d83_bfba_ee8e1500f5c9 */
