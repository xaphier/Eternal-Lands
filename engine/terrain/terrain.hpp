/****************************************************************************
 *            terrain.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_45c1c534_56a4_4295_a35c_b836fe76721d
#define	UUID_45c1c534_56a4_4295_a35c_b836fe76721d

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TerrainManager.
 * This file contains the @c class TerrainManager.
 */

namespace eternal_lands
{

	class TerrainManager
	{
		private:
			ImageSharedPtr m_height_image;
			ImageSharedPtr m_normal_image;
			ImageSharedPtr m_dudv_image;
			String m_name;

		public:
			TerrainManager(
				const CodecManagerSharedPtr &codec_manager,
				const FileSystemSharedPtr &file_system,
				const GlobalVarsSharedPtr &global_vars,
				const String &name);
			~TerrainManager() throw();

//			get_visible_terrain_pages();
//			get_visible_terrain_pages();

	};

}

#endif	/* UUID_45c1c534_56a4_4295_a35c_b836fe76721d */
