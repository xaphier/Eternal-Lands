/****************************************************************************
 *            statemanager.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			glm::vec4 m_layer_index;
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
			void set_layer_index(const glm::vec4 &layer_index);

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
			{
				if (m_multisample == multisample)
				{
					return false;
				}

				set_multisample(multisample);

				return true;
			}

			inline bool switch_blend(const bool blend)
			{
				if (m_blend == blend)
				{
					return false;
				}

				set_blend(blend);

				return true;
			}

			inline bool switch_culling(const bool culling)
			{
				if (m_culling == culling)
				{
					return false;
				}

				set_culling(culling);

				return true;
			}

			inline bool switch_color_mask(
				const glm::bvec4 &color_mask)
			{
				if (glm::all(glm::equal(m_color_mask,
					color_mask)))
				{
					return false;
				}

				set_color_mask(color_mask);

				return true;
			}

			inline bool switch_depth_mask(const bool depth_mask)
			{
				if (m_depth_mask == depth_mask)
				{
					return false;
				}

				set_depth_mask(depth_mask);

				return true;
			}

			inline bool switch_depth_test(const bool depth_test)
			{
				if (m_depth_test == depth_test)
				{
					return false;
				}

				set_depth_test(depth_test);

				return true;
			}

			inline bool switch_scissor_test(const bool scissor_test)
			{
				if (m_scissor_test == scissor_test)
				{
					return false;
				}

				set_scissor_test(scissor_test);

				return true;
			}

			inline bool switch_polygon_offset_fill(
				const bool polygon_offset_fill)
			{
				if (m_polygon_offset_fill ==
					polygon_offset_fill)
				{
					return false;
				}

				set_polygon_offset_fill(polygon_offset_fill);

				return true;
			}

			inline bool switch_sample_alpha_to_coverage(
				const bool sample_alpha_to_coverage)
			{
				if (m_sample_alpha_to_coverage ==
					sample_alpha_to_coverage)
				{
					return false;
				}

				set_sample_alpha_to_coverage(
					sample_alpha_to_coverage);

				return true;
			}

			inline bool switch_layer_index(
				const glm::vec4 &layer_index)
			{
				if (glm::all(glm::equal(m_layer_index,
					layer_index)))
				{
					return false;
				}

				set_layer_index(layer_index);

				return true;
			}

			void init();
			bool unbind_all();
			bool unbind_mesh();
			bool unbind_program();
			bool unbind_texture(const Uint16 unit);
			bool unbind_textures();

			inline const AbstractMeshSharedPtr &get_mesh() const
			{
				return m_mesh;
			}

			inline const GlslProgramSharedPtr &get_program() const
			{
				return m_program;
			}

	};

}

#endif	/* UUID_702b0a5e_885a_483d_8275_a520649e1f5c */
