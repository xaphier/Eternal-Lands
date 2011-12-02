/****************************************************************************
 *            cal3dloader.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ca9c1644_1ccc_4496_9445_dee651b8a370
#define	UUID_ca9c1644_1ccc_4496_9445_dee651b8a370

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include <cal3d/cal3d.h>

/**
 * @file
 * @brief The @c class Cal3dLoader.
 * This file contains the @c class Cal3dLoader.
 */
namespace eternal_lands
{

	/**
	 * @brief @c Class for loading cal3d meshs.
	 *
	 * @c Class Cal3dLoader is used for loading cal3d meshs.
	 * @see MeshCache
	 */
	class Cal3dLoader
	{
		private:
			CalCoreModel* m_core_model;
			Uint32 m_index_count;
			Uint32 m_vertex_count;

		public:
			Cal3dLoader(CalCoreModel* core_model);
			~Cal3dLoader() throw();
			void load(MeshDataToolSharedPtr &mesh_data_tool,
				const bool use_extra_weight);

	};

}

#endif	/* UUID_ca9c1644_1ccc_4496_9445_dee651b8a370 */

