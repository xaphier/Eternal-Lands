/****************************************************************************
 *            terraininstancesbuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_4fa2fc8a_8fec_4269_ac6f_6cd364a59eff
#define	UUID_4fa2fc8a_8fec_4269_ac6f_6cd364a59eff

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "autoparameterutil.hpp"

/**
 * @file
 * @brief The @c class TerrainInstancesBuffer.
 * This file contains the @c class TerrainInstancesBuffer.
 */
namespace eternal_lands
{

	class TerrainInstancesBuffer
	{
		private:
			AbstractWriteMemorySharedPtr m_memory;

		public:
			void set_terrain_instance(const glm::mat2x4 &data,
				const Uint16 index);

			static inline bool has_auto_parameter(
				const AutoParameterType auto_parameter) noexcept
			{
				return auto_parameter == apt_terrain_instances;
			}
/*
			static inline UniformBufferType get_type() noexcept
			{
				return ubt_terrain_instances;
			}
*/
	};

}

#endif	/* UUID_4fa2fc8a_8fec_4269_ac6f_6cd364a59eff */
