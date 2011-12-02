/****************************************************************************
 *            renderobjectdata.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8488dc1c_d7e0_41c3_a3a4_b82272a97f76
#define	UUID_8488dc1c_d7e0_41c3_a3a4_b82272a97f76

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractrstartreevisitor.hpp"

/**
 * @file
 * @brief The @c class RenderObjectData.
 * This file contains the @c class RenderObjectData.
 */
namespace eternal_lands
{

	/**
	 * @brief visitor for objects of RStarTree.
	 *
	 */
	class RenderObjectData
	{
		private:
			ObjectSharedPtr m_object;
			float m_transparency;
			float m_distance;
			bool m_blend;
			SubFrustumsMask m_sub_frustums_mask;

		public:
			RenderObjectData();
			RenderObjectData(const ObjectSharedPtr &object,
				const SubFrustumsMask sub_frustums_mask = 0x1);
			RenderObjectData(const ObjectSharedPtr &object,
				float transparency, bool blend,
				const SubFrustumsMask sub_frustums_mask = 0x1);
			virtual ~RenderObjectData() throw();

			inline void set_object(const ObjectSharedPtr &object)
			{
				m_object = object;
			}

			inline void set_transparency(const float transparency)
			{
				m_transparency = transparency;
			}

			inline void set_distance(const float distance)
			{
				m_distance = distance;
			}

			inline void set_blend(const bool blend)
			{
				m_blend = blend;
			}

			inline const ObjectSharedPtr &get_object() const
			{
				return m_object;
			}

			inline float get_transparency() const
			{
				return m_transparency;
			}

			inline float get_distance() const
			{
				return m_distance;
			}

			inline bool get_blend() const
			{
				return m_blend;
			}

			inline SubFrustumsMask get_sub_frustums_mask() const
			{
				return m_sub_frustums_mask;
			}

			inline bool get_sub_frustums_mask(const Uint16 index)
				const
			{
				return m_sub_frustums_mask[index];
			}

	};
	/**
	 * @}
	 */

}
/**
 * @}
 */

#endif	/* UUID_8488dc1c_d7e0_41c3_a3a4_b82272a97f76 */
