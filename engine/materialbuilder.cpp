/****************************************************************************
 *            materialbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialbuilder.hpp"
#include "material.hpp"

namespace eternal_lands
{

	MaterialBuilder::MaterialBuilder(const EffectCacheWeakPtr &effect_cache,
		const TextureCacheWeakPtr &texture_cache,
		const MaterialScriptCacheWeakPtr &material_script_cache,
		const MaterialScriptManagerWeakPtr &material_script_manager):
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_material_script_cache(material_script_cache),
		m_material_script_manager(material_script_manager)
	{
		assert(!m_effect_cache.expired());
		assert(!m_texture_cache.expired());
		assert(!m_material_script_cache.expired());
		assert(!m_material_script_manager.expired());
	}

	MaterialBuilder::~MaterialBuilder() noexcept
	{
	}

	MaterialSharedPtr MaterialBuilder::get_material(
		const MaterialDescription &material) const
	{
		MaterialSharedPtr result;

		result = boost::make_shared<Material>(get_effect_cache(),
			get_texture_cache(), get_material_script_cache(),
			get_material_script_manager(), material);

		return result;
	}

}
