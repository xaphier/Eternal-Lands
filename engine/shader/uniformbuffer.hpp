/****************************************************************************
 *            uniformbuffer.hpp
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
#include "hardwarebuffer/hardwarebuffer.hpp"
#include "uniformbufferutil.hpp"

/**
 * @file
 * @brief The @c class UniformBuffer.
 * This file contains the @c class UniformBuffer.
 */
namespace eternal_lands
{

	class UniformBuffer
	{
		private:
			const HardwareBufferMapperWeakPtr
				m_hardware_buffer_mapper;
			HardwareBuffer m_buffer;
			UniformBufferDescriptionSharedPtr
				m_uniform_buffer_description;

			inline HardwareBufferMapperSharedPtr
				get_hardware_buffer_mapper() const noexcept
			{
				HardwareBufferMapperSharedPtr result;

				result = m_hardware_buffer_mapper.lock();

				assert(result.get() != nullptr);

				return result;
			}

		public:
			UniformBuffer(const HardwareBufferMapperWeakPtr
					&hardware_buffer_mapper,
				const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache,
				const UniformBufferType type);
			~UniformBuffer() noexcept;

	};

}

#endif	/* UUID_4fa2fc8a_8fec_4269_ac6f_6cd364a59eff */
