/****************************************************************************
 *            materialbuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_63b396f8_bab6_4d59_8a3d_ad67b59b257a
#define	UUID_63b396f8_bab6_4d59_8a3d_ad67b59b257a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MaterialBuilder.
 * This file contains the @c class MaterialBuilder.
 */
namespace eternal_lands
{

	class MaterialBuilder: public boost::noncopyable
	{
		private:
			const EffectCacheWeakPtr m_effect_cache;
			const TextureCacheWeakPtr m_texture_cache;
			const MaterialScriptCacheWeakPtr
				m_material_script_cache;
			const MaterialScriptManagerWeakPtr
				m_material_script_manager;

			inline const EffectCacheWeakPtr &get_effect_cache()
				const
			{
				return m_effect_cache;
			}

			inline const TextureCacheWeakPtr &get_texture_cache()
				const
			{
				return m_texture_cache;
			}

			inline const MaterialScriptCacheWeakPtr
				&get_material_script_cache() const
			{
				return m_material_script_cache;
			}

			inline const MaterialScriptManagerWeakPtr
				&get_material_script_manager() const
			{
				return m_material_script_manager;
			}

		public:
			MaterialBuilder(const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache,
				const MaterialScriptCacheWeakPtr
					&material_script_cache,
				const MaterialScriptManagerWeakPtr
					&material_script_manager);
			~MaterialBuilder() throw();
			MaterialSharedPtr get_material(
				const MaterialDescription &material) const;

	};

}

#endif	/* UUID_63b396f8_bab6_4d59_8a3d_ad67b59b257a */
