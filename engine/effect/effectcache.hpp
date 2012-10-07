/****************************************************************************
 *            effectcache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a5f5baae_126c_4cca_8fcd_e26eca3348d7
#define	UUID_a5f5baae_126c_4cca_8fcd_e26eca3348d7

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shader/shaderbuildutil.hpp"

/**
 * @file
 * @brief The @c class EffectCache.
 * This file contains the @c class EffectCache.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for effect cache.
	 *
	 * @c class for effect cache.
	 */
	class EffectCache
	{
		private:
			typedef std::map<String, EffectSharedPtr>
				EffectCacheMap;

			EffectCacheMap m_effect_cache;
			const GlslProgramCacheWeakPtr m_glsl_program_cache;
			const ShaderSourceBuilderWeakPtr
				m_shader_source_builder;
			EffectSharedPtr m_simple_effect;

			inline const GlslProgramCacheWeakPtr
				&get_glsl_program_cache() const noexcept
			{
				return m_glsl_program_cache;
			}

			inline const ShaderSourceBuilderWeakPtr
				&get_shader_source_builder() const noexcept
			{
				return m_shader_source_builder;
			}

			void load_effect(
				const FileSystemSharedPtr &file_system,
				const String &file_name);

		public:
			/**
			 * Default constructor.
			 */
			EffectCache(const GlslProgramCacheWeakPtr
					&glsl_program_cache,
				const ShaderSourceBuilderWeakPtr
					&shader_source_builder);

			/**
			 * Default destructor.
			 */
			~EffectCache() noexcept;
			const EffectSharedPtr &get_effect(const String &name);
			const EffectSharedPtr &get_simple_effect();
			void reload();
			void set_debug_shader(const ShaderBuildType debug);
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &dir);
			StringVector get_effect_names() const;
			void add_effect(
				const EffectDescription &effect_description);
			EffectSharedPtr build_effect(
				const EffectDescription &effect_description)
				const;

	};

}

#endif	/* UUID_a5f5baae_126c_4cca_8fcd_e26eca3348d7 */
