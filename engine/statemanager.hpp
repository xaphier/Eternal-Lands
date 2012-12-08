/****************************************************************************
 *            statemanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_702b0a5e_885a_483d_8275_a520649e1f5c
#define	UUID_702b0a5e_885a_483d_8275_a520649e1f5c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "stateset.hpp"

/**
 * @file
 * @brief The @c class StateManager.
 * This file contains the @c class StateManager.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for objects.
	 *
	 * @c class for objects.
	 */
	class StateManager: public boost::noncopyable
	{
		private:
			const GlobalVarsSharedPtr m_global_vars;
			AbstractMeshSharedPtr m_mesh;
			GlslProgramSharedPtr m_program;
			TextureSharedPtrArray32 m_textures;
			glm::vec4 m_screen_size;
			BitSet32 m_program_used_texture_units;
			BitSet32 m_used_texture_units;
			BitSet32 m_used_attributes;
			StateSet m_state_set;

			inline const GlobalVarsSharedPtr &get_global_vars()
				const
			{
				return m_global_vars;
			}

			void log_texture_units();
			void gl_error_check();

			bool switch_texture_unit(const Uint16 texture_unit);
			void set_mesh(const AbstractMeshSharedPtr &mesh);
			void set_program(
				const GlslProgramSharedPtr &program);
			void set_texture(const Uint16 texture_unit,
				const TextureSharedPtr &texture);
			void set_multisample(const bool multisample) noexcept;
			void set_blend(const bool blend) noexcept;
			void set_culling(const bool culling) noexcept;
			void set_view_port(const glm::uvec4 &view_port)
				noexcept;
			void set_color_mask(const glm::bvec4 &color_mask)
				noexcept;
			void set_depth_mask(const bool depth_mask) noexcept;
			void set_depth_test(const bool depth_test) noexcept;
			void set_scissor_test(const bool scissor_test) noexcept;
			void set_polygon_offset_fill(
				const bool polygon_offset_fill) noexcept;
			void set_sample_alpha_to_coverage(
				const bool sample_alpha_to_coverage) noexcept;
			void set_stencil_test(const bool stencil) noexcept;
			void set_restart_index(const Uint32 restart_index)
				noexcept;
			void set_use_restart_index(const bool use_restart_index)
				noexcept;
			void set_flip_back_face_culling(
				const bool flip_back_face_culling) noexcept;
			void set_depth_clamp(const bool depth_clamp) noexcept;
			void set_framebuffer_sRGB(const bool framebuffer_sRGB)
				noexcept;

		public:
			/**
			 * Default constructor.
			 */
			StateManager(const GlobalVarsSharedPtr &global_vars);

			/**
			 * Default destructor.
			 */
			~StateManager() noexcept;
			void set_state_set(const StateSet &state_set);

			inline const StateSet &get_state_set() const noexcept
			{
				return m_state_set;
			}

			inline bool switch_mesh(
				const AbstractMeshSharedPtr &mesh)
			{
				if (m_mesh == mesh)
				{
					return false;
				}

				set_mesh(mesh);

				return true;
			}

			inline bool switch_program(
				const GlslProgramSharedPtr &program)
			{
				if (m_program == program)
				{
					return false;
				}

				set_program(program);

				return true;
			}

			inline bool switch_texture(const Uint16 texture_unit,
				const TextureSharedPtr &texture)
			{
				if (!m_program_used_texture_units[texture_unit])
				{
					return false;
				}

				if (m_textures[texture_unit] == texture)
				{
					return false;
				}

				set_texture(texture_unit, texture);

				return true;
			}

			inline bool switch_multisample(const bool multisample)
				noexcept
			{
				if (m_state_set.get_multisample() ==
					multisample)
				{
					return false;
				}

				set_multisample(multisample);

				return true;
			}

			inline bool switch_blend(const bool blend) noexcept
			{
				if (m_state_set.get_blend() == blend)
				{
					return false;
				}

				set_blend(blend);

				return true;
			}

			inline bool switch_stencil_test(const bool stencil_test)
				noexcept
			{
				if (m_state_set.get_stencil_test() ==
					stencil_test)
				{
					return false;
				}

				set_stencil_test(stencil_test);

				return true;
			}

			inline bool switch_culling(const bool culling) noexcept
			{
				if (m_state_set.get_culling() == culling)
				{
					return false;
				}

				set_culling(culling);

				return true;
			}

			inline bool switch_view_port(
				const glm::uvec4 &view_port)
			{
				if (glm::all(glm::equal(
					m_state_set.get_view_port(),
						view_port)))
				{
					return false;
				}

				set_view_port(view_port);

				return true;
			}

			inline bool switch_color_mask(
				const glm::bvec4 &color_mask) noexcept
			{
				if (glm::all(glm::equal(
					m_state_set.get_color_mask(),
						color_mask)))
				{
					return false;
				}

				set_color_mask(color_mask);

				return true;
			}

			inline bool switch_depth_mask(const bool depth_mask)
				noexcept
			{
				if (m_state_set.get_depth_mask() == depth_mask)
				{
					return false;
				}

				set_depth_mask(depth_mask);

				return true;
			}

			inline bool switch_depth_test(const bool depth_test)
				noexcept
			{
				if (m_state_set.get_depth_test() == depth_test)
				{
					return false;
				}

				set_depth_test(depth_test);

				return true;
			}

			inline bool switch_scissor_test(const bool scissor_test)
				noexcept
			{
				if (m_state_set.get_scissor_test() ==
					scissor_test)
				{
					return false;
				}

				set_scissor_test(scissor_test);

				return true;
			}

			inline bool switch_polygon_offset_fill(
				const bool polygon_offset_fill) noexcept
			{
				if (m_state_set.get_polygon_offset_fill() ==
					polygon_offset_fill)
				{
					return false;
				}

				set_polygon_offset_fill(polygon_offset_fill);

				return true;
			}

			inline bool switch_sample_alpha_to_coverage(
				const bool sample_alpha_to_coverage) noexcept
			{
				if (m_state_set.get_sample_alpha_to_coverage()
					== sample_alpha_to_coverage)
				{
					return false;
				}

				set_sample_alpha_to_coverage(
					sample_alpha_to_coverage);

				return true;
			}

			inline bool switch_restart_index(
				const Uint32 restart_index) noexcept
			{
				if (m_state_set.get_restart_index() ==
					restart_index)
				{
					return false;
				}

				set_restart_index(restart_index);

				return true;
			}

			inline bool switch_use_restart_index(
				const bool use_restart_index) noexcept
			{
				if (m_state_set.get_use_restart_index() ==
					use_restart_index)
				{
					return false;
				}

				set_use_restart_index(use_restart_index);

				return true;
			}

			inline bool switch_flip_back_face_culling(
				const bool flip_back_face_culling) noexcept
			{
				if (m_state_set.get_flip_back_face_culling() ==
					flip_back_face_culling)
				{
					return false;
				}

				set_flip_back_face_culling(
					flip_back_face_culling);

				return true;
			}


			inline bool switch_depth_clamp(const bool depth_clamp)
				noexcept
			{
				if (m_state_set.get_depth_clamp() ==
					depth_clamp)
				{
					return false;
				}

				set_depth_clamp(depth_clamp);

				return true;
			}

			inline bool switch_framebuffer_sRGB(
				const bool framebuffer_sRGB) noexcept
			{
				if (m_state_set.get_framebuffer_sRGB() ==
					framebuffer_sRGB)
				{
					return false;
				}

				set_framebuffer_sRGB(framebuffer_sRGB);

				return true;
			}

			inline const GlslProgramSharedPtr &get_program() const
				noexcept
			{
				return m_program;
			}

			inline const glm::vec4 &get_screen_size() const
				noexcept
			{
				return m_screen_size;
			}

			void init();
			bool unbind_all();
			bool unbind_mesh();
			bool unbind_program();
			bool unbind_texture(const Uint16 unit);
			bool unbind_textures();
			void draw(const Uint32 index, const Uint32 instances);
			void draw(const MeshDrawData &draw_data,
				const Uint32 instances);

	};

}

#endif	/* UUID_702b0a5e_885a_483d_8275_a520649e1f5c */
