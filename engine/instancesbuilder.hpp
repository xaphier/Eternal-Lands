/****************************************************************************
 *            instancebuilder.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c5ad910b_3077_488a_a922_6f4d71e27c6d
#define	UUID_c5ad910b_3077_488a_a922_6f4d71e27c6d

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class InstancesBuilder.
 * This file contains the @c class InstancesBuilder.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for instance building.
	 *
	 * @c class for instance building.
	 */
	class InstancesBuilder
	{
		private:
			typedef std::map<Sint16Sint16Pair, InstancingDataVector>
				Sint16Sint16PairInstancingDataVectorMap;

			Sint16Sint16PairInstancingDataVectorMap
				m_instancing_datas;
			const MeshDataCacheWeakPtr m_mesh_data_cache;
			const float m_max_size;

			inline MeshDataCacheSharedPtr get_mesh_data_cache()
				const
			{
				MeshDataCacheSharedPtr result;

				result = m_mesh_data_cache.lock();

				assert(result.get() != 0);

				return result;
			}

			void do_add(const InstancingData &instancing_data);

		public:
			/**
			 * Default constructor.
			 */
			InstancesBuilder(
				const MeshDataCacheWeakPtr &mesh_data_cache,
				const float max_size = 16.0f);

			/**
			 * Default destructor.
			 */
			~InstancesBuilder() throw();

			void add(const ObjectData &object_data);
			void add(const ObjectData &object_data,
				const MaterialDescriptionVector &materials);
			void build(FreeIds &free_ids,
				InstanceDataVector &instances,
				ObjectDataVector &uninstanced);
			static InstanceData build(const glm::vec3 &center,
				const ObjectDataVector &object_datas);

			void clear();

	};

}

#endif	/* UUID_c5ad910b_3077_488a_a922_6f4d71e27c6d */