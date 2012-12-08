/****************************************************************************
 *            stateset.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_171734bc_bcce_4957_a923_1d3dcf9ce2bf
#define	UUID_171734bc_bcce_4957_a923_1d3dcf9ce2bf

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class StateSet.
 * This file contains the @c class StateSet.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for set of OpenGL states.
	 *
	 * @c class for set of OpenGL states.
	 */
	class StateSet
	{
		private:
			glm::uvec4 m_view_port;
			glm::bvec4 m_color_mask;
			Uint32 m_restart_index;
			Uint16 m_texture_unit;
			bool m_multisample;
			bool m_blend;
			bool m_culling;
			bool m_depth_mask;
			bool m_depth_test;
			bool m_scissor_test;
			bool m_sample_alpha_to_coverage;
			bool m_use_restart_index;
			bool m_polygon_offset_fill;
			bool m_stencil_test;
			bool m_flip_back_face_culling;
			bool m_depth_clamp;
			bool m_framebuffer_sRGB;

		public:
			/**
			 * Default constructor.
			 */
			StateSet();

			/**
			 * Default destructor.
			 */
			~StateSet() noexcept;

			inline const glm::uvec4 &get_view_port() const
				noexcept
			{
				return m_view_port;
			}

			inline const glm::bvec4 &get_color_mask() const
				noexcept
			{
				return m_color_mask;
			}

			inline Uint32 get_restart_index() const noexcept
			{
				return m_restart_index;
			}

			inline Uint16 get_texture_unit() const noexcept
			{
				return m_texture_unit;
			}

			inline bool get_multisample() const noexcept
			{
				return m_multisample;
			}

			inline bool get_blend() const noexcept
			{
				return m_blend;
			}

			inline bool get_culling() const noexcept
			{
				return m_culling;
			}

			inline bool get_depth_mask() const noexcept
			{
				return m_depth_mask;
			}

			inline bool get_depth_test() const noexcept
			{
				return m_depth_test;
			}

			inline bool get_scissor_test() const noexcept
			{
				return m_scissor_test;
			}

			inline bool get_sample_alpha_to_coverage() const
				noexcept
			{
				return m_sample_alpha_to_coverage;
			}

			inline bool get_use_restart_index() const noexcept
			{
				return m_use_restart_index;
			}

			inline bool get_polygon_offset_fill() const noexcept
			{
				return m_polygon_offset_fill;
			}

			inline bool get_stencil_test() const noexcept
			{
				return m_stencil_test;
			}

			inline bool get_flip_back_face_culling() const noexcept
			{
				return m_flip_back_face_culling;
			}

			inline bool get_depth_clamp() const noexcept
			{
				return m_depth_clamp;
			}

			inline bool get_framebuffer_sRGB() const noexcept
			{
				return m_framebuffer_sRGB;
			}

			inline void set_view_port(
				const glm::uvec4 &view_port) noexcept
			{
				m_view_port = view_port;
			}

			inline void set_color_mask(
				const glm::bvec4 &color_mask) noexcept
			{
				m_color_mask = color_mask;
			}

			inline void set_restart_index(
				const Uint32 restart_index) noexcept
			{
				m_restart_index = restart_index;
			}

			inline void set_texture_unit(const Uint16 texture_unit)
				noexcept
			{
				m_texture_unit = texture_unit;
			}

			inline void set_multisample(const bool multisample)
				noexcept
			{
				m_multisample = multisample;
			}

			inline void set_blend(const bool blend) noexcept
			{
				m_blend = blend;
			}

			inline void set_culling(const bool culling) noexcept
			{
				m_culling = culling;
			}

			inline void set_depth_mask(const bool depth_mask)
				noexcept
			{
				m_depth_mask = depth_mask;
			}

			inline void set_depth_test(const bool depth_test)
				noexcept
			{
				m_depth_test = depth_test;
			}

			inline void set_scissor_test(const bool scissor_test)
				noexcept
			{
				m_scissor_test = scissor_test;
			}

			inline void set_sample_alpha_to_coverage(
				const bool sample_alpha_to_coverage) noexcept
			{
				m_sample_alpha_to_coverage =
					sample_alpha_to_coverage;
			}

			inline void set_use_restart_index(
				const bool use_restart_index) noexcept
			{
				m_use_restart_index = use_restart_index;
			}

			inline void set_polygon_offset_fill(
				const bool polygon_offset_fill) noexcept
			{
				m_polygon_offset_fill = polygon_offset_fill;
			}

			inline void set_stencil_test(const bool stencil_test)
				noexcept
			{
				m_stencil_test = stencil_test;
			}

			inline void set_flip_back_face_culling(
				const bool flip_back_face_culling) noexcept
			{
				m_flip_back_face_culling =
					flip_back_face_culling;
			}

			inline void set_depth_clamp(const bool depth_clamp)
				noexcept
			{
				m_depth_clamp = depth_clamp;
			}

			inline void set_framebuffer_sRGB(
				const bool framebuffer_sRGB) noexcept
			{
				m_framebuffer_sRGB = framebuffer_sRGB;
			}

	};

}

#endif	/* UUID_171734bc_bcce_4957_a923_1d3dcf9ce2bf */
