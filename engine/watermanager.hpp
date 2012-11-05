/****************************************************************************
 *            watermanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_83d73ea6_026b_4989_976d_4d8c337ebbb2
#define	UUID_83d73ea6_026b_4989_976d_4d8c337ebbb2

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class WaterManager.
 * This file contains the @c class WaterManager.
 */
namespace eternal_lands
{

	class WaterManager
	{
		private:
			Uint8MultiArray2 m_water_map;

		protected:

		public:
			WaterManager();
			WaterManager(const String &name, const String &material,
				const float height);
			~WaterManager() noexcept;

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline void set_material(const String &material)
				noexcept
			{
				m_material = material;
			}

			inline void set_height(const float height) noexcept
			{
				m_height = height;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
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

#endif	/* UUID_83d73ea6_026b_4989_976d_4d8c337ebbb2 */
