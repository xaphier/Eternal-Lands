/****************************************************************************
 *            cdlodterrainmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cdlodterrainmanager.hpp"

namespace eternal_lands
{

	CdLodTerrainManager::CdLodTerrainManager(
		const ImageSharedPtr &vector_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MaterialSharedPtrVector &materials)
	{
	}

	CdLodTerrainManager::~CdLodTerrainManager() noexcept
	{
	}

}
