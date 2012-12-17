/****************************************************************************
 *            renderobjectdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			BitSet64 m_visibility_mask;
			BitSet64 m_blend_mask;
			float m_transparency;
			float m_distance;
			Uint32 m_occlusion_culling;
			Uint16 m_lod;
			BlendType m_blend;

		public:
			RenderObjectData();
			RenderObjectData(const ObjectSharedPtr &object,
				const BitSet64 visibility_mask);
			RenderObjectData(const ObjectSharedPtr &object,
				const BitSet64 visibility_mask,
				const BitSet64 blend_mask,
				const float transparency,
				const BlendType blend);
			~RenderObjectData() noexcept;

			inline void set_object(const ObjectSharedPtr &object)
				noexcept
			{
				m_object = object;
			}

			inline void set_visibility_mask(
				const BitSet64 visibility_mask) noexcept
			{
				m_visibility_mask = visibility_mask;
			}

			inline void set_blend_mask(const BitSet64 blend_mask)
				noexcept
			{
				m_blend_mask = blend_mask;
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

			inline void set_occlusion_culling(
				const Uint32 occlusion_culling) noexcept
			{
				m_occlusion_culling = occlusion_culling;
			}

			inline const ObjectSharedPtr &get_object() const
				noexcept
			{
				return m_object;
			}

			inline const BitSet64 get_visibility_mask() const
				noexcept
			{
				return m_visibility_mask;
			}

			inline const BitSet64 get_blend_mask() const noexcept
			{
				return m_blend_mask;
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

			inline Uint16 get_lod() const noexcept
			{
				return m_lod;
			}

			inline Uint32 get_occlusion_culling() const noexcept
			{
				return m_occlusion_culling;
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
