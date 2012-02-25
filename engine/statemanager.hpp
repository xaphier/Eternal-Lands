/****************************************************************************
 *            statemanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_702b0a5e_885a_483d_8275_a520649e1f5c
#define	UUID_702b0a5e_885a_483d_8275_a520649e1f5c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

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
			AbstractMeshSharedPtr m_mesh;
			GlslProgramSharedPtr m_program;
			TextureSharedPtrArray32 m_textures;
			BitSet32 m_program_used_texture_units;
			BitSet32 m_used_texture_units;
			BitSet32 m_used_attributes;
			glm::bvec4 m_color_mask;
			Uint16 m_texture_unit;
			bool m_multisample;
			bool m_blend;
			bool m_culling;
			bool m_depth_mask;
			bool m_depth_test;
			bool m_scissor_test;
			bool m_sample_alpha_to_coverage;
			bool m_polygon_offset_fill;
			bool m_stencil_test;

			void log_texture_units();
			void gl_error_check();

			bool switch_texture_unit(const Uint16 texture_unit);
			void set_mesh(const AbstractMeshSharedPtr &mesh);
			void set_program(
				const GlslProgramSharedPtr &program);
			void set_texture(const Uint16 texture_unit,
				const TextureSharedPtr &texture);
			void set_multisample(const bool multisample);
			void set_blend(const bool blend);
			void set_culling(const bool culling);
			void set_color_mask(const glm::bvec4 &color_mask);
			void set_depth_mask(const bool depth_mask);
			void set_depth_test(const bool depth_test);
			void set_scissor_test(const bool scissor_test);
			void set_polygon_offset_fill(
				const bool polygon_offset_fill);
			void set_sample_alpha_to_coverage(
				const bool sample_alpha_to_coverage);
			void set_stencil_test(const bool stencil);

		public:
			/**
			 * Default constructor.
			 */
			StateManager();

			/**
			 * Default destructor.
			 */
			~StateManager() throw();

			inline bool switch_mesh(
				const AbstractMeshSharedPtr &mesh)
			{
#ifndef	NO_STATE_TRACKING
				if (m_mesh == mesh)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_mesh(mesh);

				return true;
			}

			inline bool switch_program(
				const GlslProgramSharedPtr &program)
			{
#ifndef	NO_STATE_TRACKING
				if (m_program == program)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_program(program);

				return true;
			}

			inline bool switch_texture(const Uint16 texture_unit,
				const TextureSharedPtr &texture)
			{
#ifndef	NO_STATE_TRACKING
				if (!m_program_used_texture_units[texture_unit])
				{
					return false;
				}

				if (m_textures[texture_unit] == texture)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_texture(texture_unit, texture);

				return true;
			}

			inline bool switch_multisample(const bool multisample)
			{
#ifndef	NO_STATE_TRACKING
				if (m_multisample == multisample)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_multisample(multisample);

				return true;
			}

			inline bool switch_blend(const bool blend)
			{
#ifndef	NO_STATE_TRACKING
				if (m_blend == blend)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_blend(blend);

				return true;
			}

			inline bool switch_stencil_test(const bool stencil_test)
			{
#ifndef	NO_STATE_TRACKING
				if (m_stencil_test == stencil_test)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_stencil_test(stencil_test);

				return true;
			}

			inline bool switch_culling(const bool culling)
			{
#ifndef	NO_STATE_TRACKING
				if (m_culling == culling)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_culling(culling);

				return true;
			}

			inline bool switch_color_mask(
				const glm::bvec4 &color_mask)
			{
#ifndef	NO_STATE_TRACKING
				if (glm::all(glm::equal(m_color_mask,
					color_mask)))
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_color_mask(color_mask);

				return true;
			}

			inline bool switch_depth_mask(const bool depth_mask)
			{
#ifndef	NO_STATE_TRACKING
				if (m_depth_mask == depth_mask)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_depth_mask(depth_mask);

				return true;
			}

			inline bool switch_depth_test(const bool depth_test)
			{
#ifndef	NO_STATE_TRACKING
				if (m_depth_test == depth_test)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_depth_test(depth_test);

				return true;
			}

			inline bool switch_scissor_test(const bool scissor_test)
			{
#ifndef	NO_STATE_TRACKING
				if (m_scissor_test == scissor_test)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_scissor_test(scissor_test);

				return true;
			}

			inline bool switch_polygon_offset_fill(
				const bool polygon_offset_fill)
			{
#ifndef	NO_STATE_TRACKING
				if (m_polygon_offset_fill ==
					polygon_offset_fill)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_polygon_offset_fill(polygon_offset_fill);

				return true;
			}

			inline bool switch_sample_alpha_to_coverage(
				const bool sample_alpha_to_coverage)
			{
#ifndef	NO_STATE_TRACKING
				if (m_sample_alpha_to_coverage ==
					sample_alpha_to_coverage)
				{
					return false;
				}
#endif	/* NO_STATE_TRACKING */

				set_sample_alpha_to_coverage(
					sample_alpha_to_coverage);

				return true;
			}

			inline const GlslProgramSharedPtr &get_program() const
			{
				return m_program;
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
