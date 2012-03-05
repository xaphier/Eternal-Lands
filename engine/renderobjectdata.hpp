/****************************************************************************
 *            renderobjectdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8488dc1c_d7e0_41c3_a3a4_b82272a97f76
#define	UUID_8488dc1c_d7e0_41c3_a3a4_b82272a97f76

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "blendutil.hpp"

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
			glm::ivec4 m_layers;
			float m_transparency;
			float m_distance;
			SubFrustumsMask m_sub_frustums_mask;
			Uint16 m_layer_count;
			BlendType m_blend;

			void update_layers();

		public:
			RenderObjectData();
			RenderObjectData(const ObjectSharedPtr &object,
				const SubFrustumsMask sub_frustums_mask = 0x1);
			RenderObjectData(const ObjectSharedPtr &object,
				const float transparency, const BlendType blend,
				const SubFrustumsMask sub_frustums_mask = 0x1);
			~RenderObjectData() throw();

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

			inline void set_blend(const BlendType blend)
			{
				m_blend = blend;
			}

			inline void set_sub_frustums_mask(
				const SubFrustumsMask &sub_frustums_mask)
			{
				m_sub_frustums_mask = sub_frustums_mask;
				update_layers();
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

			inline BlendType get_blend() const
			{
				return m_blend;
			}

			inline const SubFrustumsMask &get_sub_frustums_mask()
				const
			{
				return m_sub_frustums_mask;
			}

			inline bool get_sub_frustums_mask(const Uint16 index)
				const
			{
				return m_sub_frustums_mask[index];
			}

			inline const glm::ivec4 &get_layers() const
			{
				return m_layers;
			}

			inline Uint16 get_layer_count() const
			{
				return m_layer_count;
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
