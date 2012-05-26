/****************************************************************************
 *            uniformbuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uniformbuffer.hpp"
#include "uniformbufferdescription.hpp"
#include "uniformbufferdescriptioncache.hpp"

namespace eternal_lands
{

	UniformBuffer::UniformBuffer(
		const HardwareBufferMapperWeakPtr &hardware_buffer_mapper,
		const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const UniformBufferType type):
		m_hardware_buffer_mapper(hardware_buffer_mapper)
	{
		m_uniform_buffer_description =
			uniform_buffer_description_cache->
				get_uniform_buffer_description(type);
	}

	UniformBuffer::~UniformBuffer() noexcept
	{
	}

}
