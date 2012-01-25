/****************************************************************************
 *            instancedata.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_0127e95e_4622_4fdd_80d8_6f3db96b1e66
#define	UUID_0127e95e_4622_4fdd_80d8_6f3db96b1e66

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "objectdescription.hpp"

/**
 * @file
 * @brief The @c class InstanceData.
 * This file contains the @c class InstanceData.
 */
namespace eternal_lands
{

	class InstanceData: public ObjectDescription
	{
		private:
			MeshDataToolSharedPtr m_mesh_data_tool;
			SubObjectVector m_instanced_objects;

		public:
			InstanceData(const ObjectData &object_data,
				const MeshDataToolSharedPtr &mesh_data_tool,
				const MaterialEffectDescriptionVector
					&materials,
				const SubObjectVector &instanced_objects);

			~InstanceData() throw();

			inline const MeshDataToolSharedPtr
				&get_mesh_data_tool() const
			{
				return m_mesh_data_tool;
			}

			inline const SubObjectVector &get_instanced_objects()
				const
			{
				return m_instanced_objects;
			}

	};

}

#endif	/* UUID_0127e95e_4622_4fdd_80d8_6f3db96b1e66 */
