/****************************************************************************
 *            instancingdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6
#define	UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdescription.hpp"
#include "boundingbox.hpp"

/**
 * @file
 * @brief The @c class InstancingData.
 * This file contains the @c class InstancingData.
 */
namespace eternal_lands
{

	class InstancingData: public ObjectDescription
	{
		private:
			BoundingBox m_bounding_box;
			MeshDataToolSharedPtr m_mesh_data_tool;

		public:
			InstancingData(
				const MeshDataCacheSharedPtr &mesh_data_cache,
				const ObjectData &object_data);
			InstancingData(
				const MeshDataCacheSharedPtr &mesh_data_cache,
				const ObjectData &object_data,
				const StringVector &materials);

			~InstancingData() throw();

			Sint16Sint16Pair get_index(const float max_size) const;

			inline const BoundingBox &get_bounding_box() const
			{
				return m_bounding_box;
			}

			inline const MeshDataToolSharedPtr &get_mesh_data_tool()
				const
			{
				return m_mesh_data_tool;
			}

	};

}

#endif	/* UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6 */
