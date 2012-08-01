/****************************************************************************
 *            instancebuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			const EffectCacheWeakPtr m_effect_cache;
			const MeshDataCacheWeakPtr m_mesh_data_cache;
			const MaterialDescriptionCacheWeakPtr
				m_material_description_cache;
			const float m_max_size;
			const bool m_use_simd;
			const bool m_use_base_vertex;

			inline EffectCacheSharedPtr get_effect_cache() const
				noexcept
			{
				EffectCacheSharedPtr result;

				result = m_effect_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MeshDataCacheSharedPtr get_mesh_data_cache()
				const noexcept
			{
				MeshDataCacheSharedPtr result;

				result = m_mesh_data_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			inline MaterialDescriptionCacheSharedPtr
				get_material_description_cache() const noexcept
			{
				MaterialDescriptionCacheSharedPtr result;

				result = m_material_description_cache.lock();

				assert(result.get() != nullptr);

				return result;
			}

			void do_add(const InstancingData &instancing_data);
			Sint16Sint16Pair get_index(const glm::vec2 &pos) const;

		public:
			/**
			 * Default constructor.
			 */
			InstancesBuilder(const EffectCacheWeakPtr &effect_cache,
				const MeshDataCacheWeakPtr &mesh_data_cache,
				const MaterialDescriptionCacheWeakPtr
					&material_description_cache,
				const float max_size, const bool use_simd,
				const bool use_base_vertex);

			/**
			 * Default destructor.
			 */
			~InstancesBuilder() noexcept;

			void add(const ObjectDescription &object_description);
			void build(FreeIds &free_ids,
				InstanceDataVector &instances,
				ObjectDescriptionVector &uninstanced);
			void clear() noexcept;
			static InstanceData build(const glm::vec3 &center,
				const ObjectDescriptionVector &object_datas);

			inline bool get_use_simd() const noexcept
			{
				return m_use_simd;
			}

			inline bool get_use_base_vertex() const noexcept
			{
				return m_use_base_vertex;
			}

	};

}

#endif	/* UUID_c5ad910b_3077_488a_a922_6f4d71e27c6d */
