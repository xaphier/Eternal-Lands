/****************************************************************************
 *            instancingdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6
#define	UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdescription.hpp"
#include "boundedobject.hpp"

/**
 * @file
 * @brief The @c class InstancingData.
 * This file contains the @c class InstancingData.
 */
namespace eternal_lands
{

	class InstancingData: public BoundedObject, public ObjectDescription
	{
		private:
			MeshDataToolSharedPtr m_mesh_data_tool;

		public:
			InstancingData(
				const MeshDataCacheSharedPtr &mesh_data_cache,
				const ObjectDescription &object_descrition);
			virtual ~InstancingData() noexcept;

			inline const MeshDataToolSharedPtr &get_mesh_data_tool()
				const noexcept
			{
				return m_mesh_data_tool;
			}

	};

}

#endif	/* UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6 */
