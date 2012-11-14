/****************************************************************************
 *            terrainrenderingdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_973297b6_c644_4c26_8dae_9b5cf48f790f
#define	UUID_973297b6_c644_4c26_8dae_9b5cf48f790f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class TerrainRenderingData.
 * This file contains the @c class TerrainRenderingData.
 */
namespace eternal_lands
{

	class TerrainRenderingData
	{
		private:
			AbstractMeshSharedPtr m_mesh;
			MaterialSharedPtr m_material;
			glm::vec4 m_terrain_lod_offset;
			Uint64 m_offset;
			Uint32 m_instances;
			Uint16 m_max_instances;

		public:
			TerrainRenderingData();
			~TerrainRenderingData() noexcept;

			inline void set_mesh(const AbstractMeshSharedPtr &mesh)
			{
				m_mesh = mesh;
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

#endif	/* UUID_973297b6_c644_4c26_8dae_9b5cf48f790f */
