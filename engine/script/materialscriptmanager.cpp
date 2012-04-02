/****************************************************************************
 *            materialscriptmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "materialscriptmanager.hpp"

namespace eternal_lands
{

	MaterialScriptManager::MaterialScriptManager(
		const ScriptEngineWeakPtr &script_engine):
		m_script_engine(script_engine)
	{
	}

	MaterialScriptManager::~MaterialScriptManager() throw()
	{
	}

}
