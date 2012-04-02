/****************************************************************************
 *            materialcache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialcache.hpp"
#include "materialdescriptioncache.hpp"
#include "material.hpp"

namespace eternal_lands
{

	MaterialCache::MaterialCache(const MaterialDescriptionCacheWeakPtr
			&material_description_cache,
		const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache):
		m_material_description_cache(material_description_cache),
		m_effect_cache(effect_cache), m_texture_cache(texture_cache)
	{
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

		material = boost::make_shared<Material>(get_effect_cache(),
			get_texture_cache(), get_material_description_cache(
				)->get_material_description(name));

		m_material_cache[name] = material;

		return m_material_cache[name];
	}

}
