/****************************************************************************
 *            effect.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "shader/shaderbuildutil.hpp"

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
			boost::array<GlslProgramSharedPtr, 6> m_programs;
			ShaderBuildType m_debug_shader;

			void error_load();
			void do_load();

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
			void load();
			bool get_simple_shadow() const;

			inline void set_debug_shader(
				const ShaderBuildType debug_shader) noexcept
			{
				m_debug_shader = debug_shader;
			}

			inline ShaderBuildType get_debug_shader() const
				noexcept
			{
				return m_debug_shader;
			}

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

	};

}

#endif	/* UUID_b3b59620_0a7a_470f_b018_1b4f809a0c20 */
