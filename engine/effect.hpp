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
			const GlslProgramCacheWeakPtr m_glsl_program_cache;
			const ShaderSourceBuilderWeakPtr
				m_shader_source_builder;
			const EffectDescription m_description;
			boost::array<GlslProgramSharedPtr, 3> m_programs;
			Uint16 m_max_light_count;

			void error_load();
			void do_load(const Uint16 debug);
			void build_default_shader(
				const EffectDescription &description,
				const Uint16 vertex_light_count,
				const Uint16 fragment_light_count,
				const Uint16 debug);
			void build_light_index_shader(
				const EffectDescription &description);

			inline GlslProgramCacheSharedPtr
				get_glsl_program_cache() const noexcept
			{
				GlslProgramCacheSharedPtr result;

				result = m_glsl_program_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline ShaderSourceBuilderSharedPtr
				get_shader_source_builder() const noexcept
			{
				ShaderSourceBuilderSharedPtr result;

				result = m_shader_source_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

		public:
			Effect(const GlslProgramCacheWeakPtr
				&glsl_program_cache);
			Effect(const GlslProgramCacheWeakPtr
					&glsl_program_cache,
				const ShaderSourceBuilderWeakPtr
					&shader_source_builder,
				const EffectDescription &description);
			~Effect() noexcept;
			void load(const Uint16 debug);
			bool get_simple_shadow() const;

			inline const GlslProgramSharedPtr &get_program(
				const EffectProgramType type) const
			{
				return m_programs[type];
			}

			inline const EffectDescription &get_description() const
				noexcept
			{
				return m_description;
			}

			inline Uint16 get_max_light_count() const noexcept
			{
				return m_max_light_count;
			}

	};

}

#endif	/* UUID_b3b59620_0a7a_470f_b018_1b4f809a0c20 */
