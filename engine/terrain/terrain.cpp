/****************************************************************************
 *			terrain.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrain.hpp"
#include "codec/codecmanager.hpp"

namespace eternal_lands
{

	TerrainManager::TerrainManager(
		const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars, const String &name):
		m_name(name)
	{
		m_height_image = codec_manager->load_image(name, file_system,
			ImageCompressionTypeSet(), true);
	}

	TerrainManager::~TerrainManager() throw()
	{
	}

}
