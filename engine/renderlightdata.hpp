/****************************************************************************
 *            renderlightdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a7d4019e_d7b2_4d56_808b_573026ffbad4
#define	UUID_a7d4019e_d7b2_4d56_808b_573026ffbad4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class RenderLightData.
 * This file contains the @c class RenderLightData.
 */
namespace eternal_lands
{

	/**
	 * @brief visitor for lights of RStarTree.
	 *
	 */
	class RenderLightData
	{
		private:
			LightSharedPtr m_light;
			bool m_camera_inside;

		public:
			RenderLightData();
			RenderLightData(const LightSharedPtr &light);
			~RenderLightData() noexcept;

			inline void set_light(const LightSharedPtr &light)
				noexcept
			{
				m_light = light;
			}

			inline void set_camera_inside(const bool camera_inside)
				noexcept
			{
				m_camera_inside = camera_inside;
			}

			inline const LightSharedPtr &get_light() const noexcept
			{
				return m_light;
			}

			inline bool get_camera_inside() const noexcept
			{
				return m_camera_inside;
			}

	};
	/**
	 * @}
	 */

}
/**
 * @}
 */

#endif	/* UUID_a7d4019e_d7b2_4d56_808b_573026ffbad4 */
