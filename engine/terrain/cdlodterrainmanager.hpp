/****************************************************************************
 *            cdlodterrainmanager.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743
#define	UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractterrainmanager.hpp"

/**
 * @file
 * @brief The @c class CdLodTerrainManager.
 * This file contains the @c class CdLodTerrainManager.
 */
namespace eternal_lands
{

	class CdLodTerrainManager: public AbstractTerrainManager
	{
		private:
			ImageSharedPtr m_vector_map;
			ImageSharedPtr m_normal_image;
			ImageSharedPtr m_dudv_image;

		public:
			CdLodTerrainManager(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const GlobalVarsSharedPtr &global_vars,
				const MeshBuilderSharedPtr &mesh_builder,
				const MaterialSharedPtrVector &materials);
			virtual ~CdLodTerrainManager() noexcept;

//			get_visible_terrain_pages();
//			get_visible_terrain_pages();

	};

}

#endif	/* UUID_af0d2160_00a9_4a5b_b11e_2963cdb83743 */
