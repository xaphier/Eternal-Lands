/****************************************************************************
 *            instancebuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_3fffd9fc_c64c_41e4_8508_052f25ba7207
#define	UUID_3fffd9fc_c64c_41e4_8508_052f25ba7207

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class InstanceBuilder.
 * This file contains the @c class InstanceBuilder.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for instance building.
	 *
	 * @c class for instance building.
	 */
	class InstanceBuilder
	{
		private:
			const InstancingDataVector &m_instancing_datas;
			const Uint32 m_id;
			const bool m_use_simd;
			const bool m_use_base_vertex;
			std::auto_ptr<InstanceData> m_instance_data;

			inline Uint32 get_id() const
			{
				return m_id;
			}

			inline const InstancingDataVector
				&get_instancing_datas() const
			{
				return m_instancing_datas;
			}

			inline const InstancingData &get_instancing_data(
				const Uint32 index) const
			{
				return m_instancing_datas[index];
			}

			glm::vec3 get_center() const;
			void build_instance_sub_mesh(const glm::vec3 &center,
				const MeshDataToolSharedPtr &mesh_data_tool,
				const String &material,
				const Uint32 sub_mesh_index,
				const Uint32 base_vertex,
				Uint32 &vertex_offset, Uint32 &index_offset,
				SubObjectVector &sub_objects);

		public:
			/**
			 * Default constructor.
			 */
			InstanceBuilder(
				const InstancingDataVector &instancing_datas,
				const Uint32 id, const bool use_simd,
				const bool use_base_vertex);

			/**
			 * Default destructor.
			 */
			~InstanceBuilder() throw();
			void build_instance();
			void set_instance(InstanceDataVector &instances);
			static void remove_singles(
				InstancingDataVector &instancing_datas,
				ObjectDescriptionVector &uninstanced);

			inline void operator()()
			{
				build_instance();
			}

			inline bool get_use_simd() const
			{
				return m_use_simd;
			}

			inline bool get_use_base_vertex() const
			{
				return m_use_base_vertex;
			}

	};

}

#endif	/* UUID_3fffd9fc_c64c_41e4_8508_052f25ba7207 */
