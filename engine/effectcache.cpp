/****************************************************************************
 *            effectcache.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectcache.hpp"
#include "effect.hpp"

namespace eternal_lands
{

	EffectCache::EffectCache(const ShaderSourceBuilderWeakPtr
		&shader_source_builder, const FileSystemWeakPtr &file_system):
		m_shader_source_builder(shader_source_builder),
		m_file_system(file_system)
	{
		assert(!m_shader_source_builder.expired());
		assert(!m_file_system.expired());
	}

	EffectCache::~EffectCache() throw()
	{
	}

	const EffectSharedPtr &EffectCache::get_effect(const String &name)
	{
		EffectCacheMap::iterator found;

		found = m_effect_cache.find(name);

		if (found == m_effect_cache.end())
		{
			m_effect_cache[name] = boost::make_shared<Effect>(
				get_shader_source_builder(), get_file_system(),
				name);

			return m_effect_cache[name];
		}
		else
		{
			return found->second;
		}
	}

	const EffectSharedPtr &EffectCache::get_simple_effect()
	{
		if (m_simple_effect.get() == 0)
		{
			m_simple_effect = boost::make_shared<Effect>();
		}

		return m_simple_effect;
	}

	void EffectCache::reload()
	{
		EffectCacheMap::iterator it, end;

		end = m_effect_cache.end();

		for (it = m_effect_cache.begin(); it != end; ++it)
		{
			it->second->load();
		}
	}

}