/****************************************************************************
 *            terrainvisitor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f8364206_a5a4_46d8_82d3_08e088ac5149
#define	UUID_f8364206_a5a4_46d8_82d3_08e088ac5149

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shader/mappeduniformbuffer.hpp"

/**
 * @file
 * @brief The @c class TerrainVisitor.
 * This file contains the @c class TerrainVisitor.
 */
namespace eternal_lands
{

	class TerrainVisitor
	{
		private:
			MappedUniformBufferSharedPtr m_mapped_uniform_buffer;
			AbstractMeshSharedPtr m_mesh;
			MaterialSharedPtr m_material;
			Uint32 m_instances;

		public:
			TerrainVisitor(const MappedUniformBufferSharedPtr
				&mapped_uniform_buffer);
			~TerrainVisitor() noexcept;

			inline void set_mapped_uniform_buffer(
				const MappedUniformBufferSharedPtr
					&mapped_uniform_buffer)
			{
				m_mapped_uniform_buffer = mapped_uniform_buffer;
			}

			inline void set_mesh(const AbstractMeshSharedPtr &mesh)
			{
				m_mesh = mesh;
			}

			inline void set_material(
				const MaterialSharedPtr &material)
			{
				m_material = material;
			}

			inline void set_instances(const Uint32 instances)
				noexcept
			{
				m_instances = instances;
			}

			inline const MappedUniformBufferSharedPtr
				&get_mapped_uniform_buffer() const noexcept
			{
				return m_mapped_uniform_buffer;
			}

			inline const AbstractMeshSharedPtr &get_mesh() const
				noexcept
			{
				return m_mesh;
			}

			inline const MaterialSharedPtr &get_material() const
				noexcept
			{
				return m_material;
			}

			inline Uint32 get_instances() const noexcept
			{
				return m_instances;
			}

	};

}

#endif	/* UUID_f8364206_a5a4_46d8_82d3_08e088ac5149 */
