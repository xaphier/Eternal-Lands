/****************************************************************************
 *            terrainrenderingdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			UniformBufferSharedPtr m_uniform_buffer;
			AbstractMeshSharedPtr m_mesh;
			MaterialSharedPtr m_material;
			Uint32 m_instances;

		public:
			TerrainRenderingData();
			~TerrainRenderingData() noexcept;

			inline void set_uniform_buffer(
				const UniformBufferSharedPtr &uniform_buffer)
			{
				m_uniform_buffer = uniform_buffer;
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

			inline const UniformBufferSharedPtr
				&get_uniform_buffer() const noexcept
			{
				return m_uniform_buffer;
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

#endif	/* UUID_973297b6_c644_4c26_8dae_9b5cf48f790f */
