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
			float m_transparency;
			float m_distance;
			SubFrustumsMask m_sub_frustums_mask;
			Uint16 m_lod;
			BlendType m_blend;

		public:
			RenderObjectData();
			RenderObjectData(const ObjectSharedPtr &object,
				const SubFrustumsMask sub_frustums_mask = 0x1);
			RenderObjectData(const ObjectSharedPtr &object,
				const float transparency, const BlendType blend,
				const SubFrustumsMask sub_frustums_mask = 0x1);
			~RenderObjectData() noexcept;

			inline void set_object(const ObjectSharedPtr &object)
				noexcept
			{
				m_object = object;
			}

			inline void set_transparency(const float transparency)
				noexcept
			{
				m_transparency = transparency;
			}

			inline void set_distance(const float distance) noexcept
			{
				m_distance = distance;
			}

			inline void set_lod(const Uint16 lod) noexcept
			{
				m_lod = lod;
			}

			inline void set_blend(const BlendType blend) noexcept
			{
				m_blend = blend;
			}

			inline void set_sub_frustums_mask(
				const SubFrustumsMask &sub_frustums_mask)
				noexcept
			{
				m_sub_frustums_mask = sub_frustums_mask;
			}

			inline const ObjectSharedPtr &get_object() const
				noexcept
			{
				return m_object;
			}

			inline float get_transparency() const noexcept
			{
				return m_transparency;
			}

			inline float get_distance() const noexcept
			{
				return m_distance;
			}

			inline BlendType get_blend() const noexcept
			{
				return m_blend;
			}

			inline const SubFrustumsMask &get_sub_frustums_mask()
				const noexcept
			{
				return m_sub_frustums_mask;
			}

			inline bool get_sub_frustums_mask(const Uint16 index)
				const
			{
				return m_sub_frustums_mask[index];
			}

			inline Uint16 get_lod() const noexcept
			{
				return m_lod;
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
