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
			HardwareBuffer m_buffer;
			UniformBufferDescriptionSharedPtr
				m_uniform_buffer_description;

		public:
			UniformBuffer(
				const UniformBufferDescriptionCacheSharedPtr
					&uniform_buffer_description_cache,
				const UniformBufferType type);
			~UniformBuffer() noexcept;

	};

}

#endif	/* UUID_4fa2fc8a_8fec_4269_ac6f_6cd364a59eff */
