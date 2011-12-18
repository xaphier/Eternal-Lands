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
			const FileSystemWeakPtr m_file_system;
			const String m_name;
			GlslProgramSharedPtrVector m_default_programs;
			GlslProgramSharedPtr m_depth_program;
			GlslProgramSharedPtr m_shadow_program;
			Ivec2Vector m_light_counts;
			Uint16 m_max_index;

			void error_load();
			void do_load();
			void build_default_shader(
				ShaderSourceDescription description,
				const Uint16 vertex_light_count,
				const Uint16 fragment_light_count);

			inline ShaderSourceBuilderSharedPtr
				get_shader_source_builder() const
			{
				ShaderSourceBuilderSharedPtr result;

				result = m_shader_source_builder.lock();

				assert(result.get() != 0);

				return result;
			}

			inline FileSystemSharedPtr get_file_system() const
			{
				FileSystemSharedPtr result;

				result = m_file_system.lock();

				assert(result.get() != 0);

				return result;
			}

			inline Uint16 get_max_index() const
			{
				return m_max_index;
			}

			inline Uint16 get_shadow_receiver_index(
				const bool shadow_receiver) const
			{
				if (shadow_receiver)
				{
					return 0;
				}

				return 1;
			}

			inline Uint16 get_index(const Uint16 light_count) const
			{
				return std::min(light_count, get_max_index());
			}

			inline Uint16 get_index(const Uint16 light_count,
				const bool shadow_receiver) const
			{
				return get_index(light_count) * 2 +
					get_shadow_receiver_index(
						shadow_receiver);
			}

		public:
			Effect();
			Effect(const ShaderSourceBuilderWeakPtr
				&shader_source_builder,
				const FileSystemWeakPtr &file_system,
				const String &name);
			~Effect() throw();
			void load();

			inline const String &get_name() const
			{
				return m_name;
			}

			inline const GlslProgramSharedPtr &get_default_program(
				const Uint16 light_count,
				const bool shadow_receiver) const
			{
				return m_default_programs[get_index(
					light_count, shadow_receiver)];
			}

			inline glm::ivec3 get_dynamic_light_count(
				const Uint16 light_count) const
			{
				glm::ivec3 result;
				glm::ivec2 tmp;

				result = glm::vec3(m_light_counts[
					get_index(light_count)], 0);

				result.z = result.x + result.y;
				result.y = std::min(light_count,
					static_cast<Uint16>(result.y));
				result.z = std::min(light_count,
					static_cast<Uint16>(result.z));
				result.x = std::max(result.z - result.y, 0);

				return result;
			}

			inline const GlslProgramSharedPtr &get_default_program(
				const Uint16 light_count, const bool shadowed,
				glm::ivec3 &dynamic_light_count) const
			{
				dynamic_light_count =
					get_dynamic_light_count(light_count);

				return get_default_program(light_count,
					shadowed);
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

	};

}

#endif	/* UUID_b3b59620_0a7a_470f_b018_1b4f809a0c20 */
