/****************************************************************************
 *            effect.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b3b59620_0a7a_470f_b018_1b4f809a0c20
#define	UUID_b3b59620_0a7a_470f_b018_1b4f809a0c20

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectdescription.hpp"

/**
 * @file
 * @brief The @c class Effect.
 * This file contains the @c class Effect.
 */
namespace eternal_lands
{

	class Effect: public boost::noncopyable
	{
		private:
			const ShaderSourceBuilderWeakPtr
				m_shader_source_builder;
			const EffectDescription m_description;
			GlslProgramSharedPtr m_default_program;
			GlslProgramSharedPtr m_depth_program;
			GlslProgramSharedPtr m_shadow_program;
			glm::ivec2 m_light_count;

			void error_load();
			void do_load();
			void build_default_shader(
				const EffectDescription &description,
				const Uint16 vertex_light_count,
				const Uint16 fragment_light_count);
			void build_deferred_shader(
				const EffectDescription &description);

			inline ShaderSourceBuilderSharedPtr
				get_shader_source_builder() const
			{
				ShaderSourceBuilderSharedPtr result;

				result = m_shader_source_builder.lock();

				assert(result.get() != 0);

				return result;
			}

		public:
			Effect();
			Effect(const ShaderSourceBuilderWeakPtr
					&shader_source_builder,
				const EffectDescription &description);
			~Effect() throw();
			void load();

			inline const GlslProgramSharedPtr &get_default_program(
				) const
			{
				return m_default_program;
			}

			inline glm::ivec3 get_dynamic_light_count(
				const Uint16 light_count) const
			{
				glm::ivec3 result;
				glm::ivec2 tmp;

				result = glm::vec3(m_light_count, 0);

				result.z = result.x + result.y;
				result.y = std::min(light_count,
					static_cast<Uint16>(result.y));
				result.z = std::min(light_count,
					static_cast<Uint16>(result.z));
				result.x = std::max(result.z - result.y, 0);

				return result;
			}

			inline const GlslProgramSharedPtr &get_default_program(
				const Uint16 light_count,
				glm::ivec3 &dynamic_light_count) const
			{
				dynamic_light_count =
					get_dynamic_light_count(light_count);

				return get_default_program();
			}

			inline const GlslProgramSharedPtr &get_depth_program()
				const
			{
				return m_depth_program;
			}

			inline const GlslProgramSharedPtr &get_shadow_program()
				const
			{
				return m_shadow_program;
			}

			const EffectDescription &get_description() const
			{
				return m_description;
			}

	};

}

#endif	/* UUID_b3b59620_0a7a_470f_b018_1b4f809a0c20 */
