/****************************************************************************
 *            instancingdata.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6
#define	UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdata.hpp"
#include "boundingbox.hpp"

/**
 * @file
 * @brief The @c class InstancingData.
 * This file contains the @c class InstancingData.
 */
namespace eternal_lands
{

	class InstancingData: public ObjectData
	{
		private:
			BoundingBox m_bounding_box;
			MeshDataToolSharedPtr m_mesh_data_tool;
			MaterialDescriptionVector m_materials;

		public:
			InstancingData(
				const MeshDataCacheSharedPtr &mesh_data_cache,
				const ObjectData &object_data);
			InstancingData(
				const MeshDataCacheSharedPtr &mesh_data_cache,
				const ObjectData &object_data,
				const MaterialDescriptionVector &materials);

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

			inline const MaterialDescriptionVector &get_materials()
				const
			{
				return m_materials;
			}

	};

}

#endif	/* UUID_ab0cde40_6cc7_4d3a_bc9a_9cc2d7ec55c6 */