/****************************************************************************
 *            uniformbuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uniformbuffer.hpp"
#include "uniformbufferdescription.hpp"
#include "uniformbufferdescriptioncache.hpp"
#include "mappeduniformbuffer.hpp"
#include "hardwarebuffer/hardwarebuffermapper.hpp"
#include "exceptions.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	UniformBuffer::UniformBuffer(
		const HardwareBufferMapperWeakPtr &hardware_buffer_mapper,
		const UniformBufferDescriptionCacheSharedPtr
			&uniform_buffer_description_cache,
		const UniformBufferType type):
		m_hardware_buffer_mapper(hardware_buffer_mapper)
	{
		StringStream str;

		DEBUG_CHECK_GL_ERROR();

		m_uniform_buffer_description =
			uniform_buffer_description_cache->
				get_uniform_buffer_description(type);

		m_hardware_buffer = boost::make_shared<HardwareBuffer>();

		m_uniform_buffer_description->log(str);

		LOG_DEBUG(lt_uniform_buffer, UTF8("%1%: %2%"), type %
			str.str());

		if (m_uniform_buffer_description->get_size() == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_parameter_name(
					UniformBufferUtil::get_str(type)));
		}

		m_hardware_buffer->bind(btt_uniform);
		m_hardware_buffer->set_size(btt_uniform,
			m_uniform_buffer_description->get_size(),
			hbut_dynamic_draw);
		m_hardware_buffer->unbind(btt_uniform);

		DEBUG_CHECK_GL_ERROR();
	}

	UniformBuffer::~UniformBuffer() noexcept
	{
	}

	MappedUniformBufferSharedPtr UniformBuffer::get_mapped_uniform_buffer()
		const
	{
		MappedUniformBufferSharedPtr result;
		AbstractWriteMemorySharedPtr memory;

		DEBUG_CHECK_GL_ERROR();

		assert(get_uniform_buffer_description()->get_size() > 0);

		memory = get_hardware_buffer_mapper(
			)->write_map_hardware_buffer(get_hardware_buffer());

		result = boost::make_shared<MappedUniformBuffer>(memory,
			get_uniform_buffer_description());

		DEBUG_CHECK_GL_ERROR();

		return result;
	}

	void UniformBuffer::bind()
	{
		DEBUG_CHECK_GL_ERROR();

		get_hardware_buffer()->bind(btt_uniform,
			get_uniform_buffer_description()->get_type());

		DEBUG_CHECK_GL_ERROR();
	}

}
