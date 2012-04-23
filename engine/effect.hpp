/****************************************************************************
 *            effect.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b3b59620_0a7a_470f_b018_1b4f809a0c20
#define	UUID_b3b59620_0a7a_470f_b018_1b4f809a0c20

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "effectdescription.hpp"
#include "effectprogramutil.hpp"

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
			boost::array<GlslProgramSharedPtr, 3> m_programs;
			Uint16 m_max_light_count;

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

			inline const GlslProgramSharedPtr &get_program(
				const EffectProgramType type) const
			{
				return m_programs[type];
			}

			inline const EffectDescription &get_description() const
			{
				return m_description;
			}

			inline Uint16 get_max_light_count() const
			{
				return m_max_light_count;
			}

	};

}

#endif	/* UUID_b3b59620_0a7a_470f_b018_1b4f809a0c20 */
