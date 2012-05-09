/****************************************************************************
 *            materialscriptcache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialscriptcache.hpp"
#include "scriptengine.hpp"
#include "materialscript.hpp"

namespace eternal_lands
{

	MaterialScriptCache::MaterialScriptCache(
		const ScriptEngineWeakPtr &script_engine):
		m_script_engine(script_engine)
	{
	}

	MaterialScriptCache::~MaterialScriptCache() noexcept
	{
	}

	const MaterialScriptSharedPtr &MaterialScriptCache::get_material_script(
		const String &name)
	{
		StringMaterialScriptSharedPtrMap::iterator found;
		MaterialScriptSharedPtr material;

		found = m_material_scripts.find(name);

		if (found != m_material_scripts.end())
		{
			return found->second;
		}

		material = boost::make_shared<MaterialScript>(
			get_script_engine(), name);

		m_material_scripts[name] = material;

		return m_material_scripts[name];
	}

}
