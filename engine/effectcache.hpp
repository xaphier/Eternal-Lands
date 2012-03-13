/****************************************************************************
 *            effectcache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a5f5baae_126c_4cca_8fcd_e26eca3348d7
#define	UUID_a5f5baae_126c_4cca_8fcd_e26eca3348d7

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

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
			ShaderSourceBuilderWeakPtr m_shader_source_builder;
			FileSystemWeakPtr m_file_system;
			EffectSharedPtr m_simple_effect;

			inline const ShaderSourceBuilderWeakPtr
				&get_shader_source_builder() const
			{
				return m_shader_source_builder;
			}

			inline const FileSystemWeakPtr &get_file_system() const
			{
				return m_file_system;
			}

			void load_xml(const xmlNodePtr node);

		public:
			/**
			 * Default constructor.
			 */
			EffectCache(const ShaderSourceBuilderWeakPtr
				&shader_source_builder,
				const FileSystemWeakPtr &file_system);

			/**
			 * Default destructor.
			 */
			~EffectCache() throw();
			const EffectSharedPtr &get_effect(const String &name);
			const EffectSharedPtr &get_simple_effect();
			void reload();
			void load_xml(const FileSystemSharedPtr &file_system,
				const String &file_name);
			StringVector get_effect_names() const;
			void save_xml(const String &file_name) const;

	};

}

#endif	/* UUID_a5f5baae_126c_4cca_8fcd_e26eca3348d7 */
