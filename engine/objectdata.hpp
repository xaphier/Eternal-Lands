/****************************************************************************
 *            objectdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ab022a3e_b1f5_489b_8a77_2867286be10e
#define	UUID_ab022a3e_b1f5_489b_8a77_2867286be10e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "selectionutil.hpp"
#include "transformation.hpp"
#include "blendutil.hpp"

/**
 * @file
 * @brief The @c class ObjectData.
 * This file contains the @c class ObjectData.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for object data.
	 *
	 * @c class for object data.
	 */
	class ObjectData
	{
		private:
			Transformation m_world_transformation;
			String m_name;
			BitSet64 m_blend_mask;
			float m_transparency;
			float m_glow;
			Uint32 m_id;
			SelectionType m_selection;
			BlendType m_blend;

		public:
			/**
			 * Default constructor.
			 */
			inline ObjectData(): m_transparency(1.0f),
				m_glow(1.0f), m_id(0), m_selection(st_none),
				m_blend(bt_alpha_transparency_source_value)
			{
			}

			/**
			 * Default constructor.
			 */
			inline ObjectData(
				const Transformation &world_transformation,
				const String &name, const BitSet64 blend_mask,
				const float transparency, const float glow,
				const Uint32 id, const SelectionType selection,
				const BlendType blend):
				m_world_transformation(world_transformation),
				m_name(name), m_blend_mask(blend_mask),
				m_transparency(transparency), m_glow(glow),
				m_id(id), m_selection(selection),
				m_blend(blend)
			{
			}

			/**
			 * Default destructor.
			 */
			inline ~ObjectData() noexcept
			{
			}

			inline void set_world_transformation(
				const Transformation &world_transformation)
				noexcept
			{
				m_world_transformation = world_transformation;
			}

			inline void set_name(const String &name) noexcept
			{
				m_name = name;
			}

			inline void set_blend_mask(
				const BitSet64 blend_mask) noexcept
			{
				m_blend_mask = blend_mask;
			}

			inline void set_transparency(const float transparency)
				noexcept
			{
				m_transparency = transparency;
			}

			inline void set_glow(const float glow) noexcept
			{
				m_glow = glow;
			}

			inline void set_blend(const BlendType blend) noexcept
			{
				m_blend = blend;
			}

			inline void set_selection(const SelectionType selection)
				noexcept
			{
				m_selection = selection;
			}

			inline void set_id(const Uint32 id) noexcept
			{
				m_id = id;
			}

			inline const Transformation &get_world_transformation()
				const noexcept
			{
				return m_world_transformation;
			}

			inline const String &get_name() const noexcept
			{
				return m_name;
			}

			inline const BitSet64 get_blend_mask() const
				noexcept
			{
				return m_blend_mask;
			}

			inline float get_transparency() const noexcept
			{
				return m_transparency;
			}

			inline float get_glow() const noexcept
			{
				return m_glow;
			}

			inline Uint32 get_id() const noexcept
			{
				return m_id;
			}

			inline BlendType get_blend() const noexcept
			{
				return m_blend;
			}

			inline SelectionType get_selection() const noexcept
			{
				return m_selection;
			}

	};

}

#endif	/* UUID_ab022a3e_b1f5_489b_8a77_2867286be10e */
