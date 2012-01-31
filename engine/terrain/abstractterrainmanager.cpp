/****************************************************************************
 *            abstractterrainmanager.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractterrainmanager.hpp"
#include "codec/codecmanager.hpp"

namespace eternal_lands
{

	AbstractTerrainManager::AbstractTerrainManager(const String &name):
		m_name(name)
	{
/*		m_height_image = codec_manager->load_image(name, file_system,
			ImageCompressionTypeSet(), true);
*/	}

	AbstractTerrainManager::~AbstractTerrainManager() throw()
	{
	}

}
