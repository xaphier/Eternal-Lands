/****************************************************************************
 *            materialcache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialcache.hpp"
#include "materialdescriptioncache.hpp"
#include "materialbuilder.hpp"

namespace eternal_lands
{

	MaterialCache::MaterialCache(
		const MaterialBuilderWeakPtr &material_builder,
		const MaterialDescriptionCacheWeakPtr
			&material_description_cache):
		m_material_builder(material_builder),
		m_material_description_cache(material_description_cache)
	{
		assert(!m_material_builder.expired());
		assert(!m_material_description_cache.expired());
	}

	MaterialCache::~MaterialCache() throw()
	{
	}

	const MaterialSharedPtr &MaterialCache::get_material(const String &name)
	{
		StringMaterialSharedPtrMap::iterator found;
		MaterialSharedPtr material;

		found = m_material_cache.find(name);

		if (found != m_material_cache.end())
		{
			return found->second;
		}

		material = get_material_builder()->get_material(
			get_material_description_cache(
				)->get_material_description(name));

		m_material_cache[name] = material;

		return m_material_cache[name];
	}

}
