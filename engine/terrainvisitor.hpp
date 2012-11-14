/****************************************************************************
 *            terrainvisitor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f8364206_a5a4_46d8_82d3_08e088ac5149
#define	UUID_f8364206_a5a4_46d8_82d3_08e088ac5149

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "shader/mappeduniformbuffer.hpp"
#include "boundingbox.hpp"

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
			BoundingBox m_bounding_box;
			AbstractWriteMemorySharedPtr m_buffer;
			MaterialSharedPtr m_material;
			glm::vec4 m_terrain_lod_offset;
			Uint64 m_offset;
			Uint32 m_instances;
			Uint16 m_max_instances;

		public:
			TerrainVisitor(
				const AbstractWriteMemorySharedPtr &buffer,
				const Uint64 offset,
				const Uint16 max_instances);
			~TerrainVisitor() noexcept;

			inline void set_bounding_box(
				const BoundingBox &bounding_box)
			{
				m_bounding_box = bounding_box;
			}

			inline void set_buffer(
				const AbstractWriteMemorySharedPtr &buffer)
			{
				m_buffer = buffer;
			}

			inline void set_material(
				const MaterialSharedPtr &material)
			{
				m_material = material;
			}

			inline void set_terrain_lod_offset(
				const glm::vec4 &terrain_lod_offset)
			{
				m_terrain_lod_offset = terrain_lod_offset;
			}

			inline void set_offset(const Uint64 offset)
				noexcept
			{
				m_offset = offset;
			}

			inline void set_instances(const Uint32 instances)
				noexcept
			{
				m_instances = instances;
			}

			inline void set_max_instances(
				const Uint16 max_instances) noexcept
			{
				m_max_instances = max_instances;
			}

			inline const BoundingBox &get_bounding_box() const
				noexcept
			{
				return m_bounding_box;
			}

			inline const AbstractWriteMemorySharedPtr &get_buffer()
				const noexcept
			{
				return m_buffer;
			}

			inline const MaterialSharedPtr &get_material() const
				noexcept
			{
				return m_material;
			}

			inline const glm::vec4 &get_terrain_lod_offset() const
				noexcept
			{
				return m_terrain_lod_offset;
			}

			inline Uint64 get_offset() const noexcept
			{
				return m_offset;
			}

			inline Uint32 get_instances() const noexcept
			{
				return m_instances;
			}

			inline Uint16 get_max_instances() const noexcept
			{
				return m_max_instances;
			}

	};

}

#endif	/* UUID_f8364206_a5a4_46d8_82d3_08e088ac5149 */
