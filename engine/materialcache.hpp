/****************************************************************************
 *            materialcache.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			const MaterialBuilderWeakPtr m_material_builder;
			const MaterialDescriptionCacheWeakPtr
				m_material_description_cache;

			inline MaterialBuilderSharedPtr get_material_builder()
				const noexcept
			{
				MaterialBuilderSharedPtr result;

				result = m_material_builder.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialDescriptionCacheSharedPtr
				get_material_description_cache() const noexcept
			{
				MaterialDescriptionCacheSharedPtr result;

				result = m_material_description_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

		public:
			MaterialCache(
				const MaterialBuilderWeakPtr &material_builder,
				const MaterialDescriptionCacheWeakPtr
					&material_description_cache);
			~MaterialCache() noexcept;
			StringVector get_material_names() const;
			const MaterialSharedPtr &get_material(
				const String &name);

	};

}

#endif	/* UUID_addbffbf_45bb_4b46_b871_21de1c3b627b */
