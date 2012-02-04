/****************************************************************************
 *            cdlodterrainmanager.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cdlodterrainmanager.hpp"

namespace eternal_lands
{

	CdLodTerrainManager::CdLodTerrainManager(
		const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars, const String &name):
		BasicTerrainManager(name)
	{
	}

	CdLodTerrainManager::~CdLodTerrainManager() throw()
	{
	}

}
