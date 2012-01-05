/****************************************************************************
 *            hardwarebuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "hardwarebuffer.hpp"
#include "exceptions.hpp"
#include "memorybuffer.hpp"

namespace eternal_lands
{

	HardwareBuffer::HardwareBuffer(): m_size(0), m_id(0)
	{
		CHECK_GL_ERROR();

		glGenBuffers(1, &m_id);

		assert(m_id != 0);

		CHECK_GL_ERROR();
	}

	HardwareBuffer::~HardwareBuffer() throw()
	{
		glDeleteBuffers(1, &m_id);
	}

	void HardwareBuffer::set_size(const HardwareBufferType type,
		const Uint64 size, const HardwareBufferUsageType usage)
	{
		m_size = size;

		glBufferData(type, size, 0, usage);
	}

	void HardwareBuffer::set(const HardwareBufferType type,
		const AbstractReadMemoryBuffer &buffer,
		const HardwareBufferUsageType usage)
	{
		m_size = buffer.get_size();

		glBufferData(type, buffer.get_size(), buffer.get_ptr(), usage);
	}

	void HardwareBuffer::update(const HardwareBufferType type,
		const AbstractReadMemoryBuffer &buffer, const Uint64 offset)
	{
		assert(get_size() >= (offset + buffer.get_size()));

		glBufferSubData(type, offset, buffer.get_size(),
			buffer.get_ptr());
	}

	void HardwareBuffer::bind(const HardwareBufferType type)
	{
		glBindBuffer(type, m_id);
	}

	void HardwareBuffer::unbind(const HardwareBufferType type)
	{
		glBindBuffer(type, 0);
	}

	void* HardwareBuffer::map(const HardwareBufferType type,
		const HardwareBufferAccessType access)
	{
		return glMapBuffer(type, access);
	}

	bool HardwareBuffer::unmap(const HardwareBufferType type)
	{
		return glUnmapBuffer(type) == GL_TRUE;
	}

}
