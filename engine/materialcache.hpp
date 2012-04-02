/****************************************************************************
 *            materialcache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_addbffbf_45bb_4b46_b871_21de1c3b627b
#define	UUID_addbffbf_45bb_4b46_b871_21de1c3b627b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MaterialCache.
 * This file contains the @c class MaterialCache.
 */
namespace eternal_lands
{

	class MaterialCache: public boost::noncopyable
	{
		private:
			typedef std::map<String, MaterialSharedPtr>
				StringMaterialSharedPtrMap;

			StringMaterialSharedPtrMap m_material_cache;
			const MaterialDescriptionCacheWeakPtr
				m_material_description_cache;
			const EffectCacheWeakPtr m_effect_cache;
			const TextureCacheWeakPtr m_texture_cache;

			inline MaterialDescriptionCacheSharedPtr
				get_material_description_cache() const
			{
				MaterialDescriptionCacheSharedPtr result;

				result = m_material_description_cache.lock();

				assert(result.get() != 0);

				return result;
			}

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

		public:
			MaterialCache(const MaterialDescriptionCacheWeakPtr
					&material_description_cache,
				const EffectCacheWeakPtr &effect_cache,
				const TextureCacheWeakPtr &texture_cache);
			~MaterialCache() throw();
			StringVector get_material_names() const;
			const MaterialSharedPtr &get_material(
				const String &name);
			bool get_has_material(const String &name) const;

	};

}

#endif	/* UUID_addbffbf_45bb_4b46_b871_21de1c3b627b */
