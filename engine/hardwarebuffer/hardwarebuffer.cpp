/****************************************************************************
 *            hardwarebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "hardwarebuffer.hpp"
#include "exceptions.hpp"
#include "abstractreadmemory.hpp"

namespace eternal_lands
{

	HardwareBuffer::HardwareBuffer(): m_size(0), m_id(0)
	{
		CHECK_GL_ERROR();

		glGenBuffers(1, &m_id);

		assert(m_id != 0);

		CHECK_GL_ERROR();
	}

	HardwareBuffer::~HardwareBuffer() noexcept
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
		const AbstractReadMemory &buffer,
		const HardwareBufferUsageType usage)
	{
		m_size = buffer.get_size();

		glBufferData(type, buffer.get_size(), buffer.get_ptr(), usage);
	}

	void HardwareBuffer::update(const HardwareBufferType type,
		const AbstractReadMemory &buffer, const Uint64 offset)
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

	void HardwareBuffer::bind(const HardwareBufferType type,
		const Uint32 index)
	{
		glBindBufferBase(type, index, m_id);
	}

	void HardwareBuffer::unbind(const HardwareBufferType type,
		const Uint32 index)
	{
		glBindBufferBase(type, index, 0);
	}

	void* HardwareBuffer::map(const HardwareBufferType type,
		const HardwareBufferAccessType access)
	{
		return glMapBuffer(type, access);
	}

	void* HardwareBuffer::map_range(const HardwareBufferType type,
		const Uint32 offset, const Uint32 size, const bool read,
		const bool write, const bool invalidate_range)
	{
		GLbitfield access;

		assert(get_size() >= (offset + size));
		assert(read || write);
		assert(!(invalidate_range && read));

		access = 0;

		if (read)
		{
			access |= GL_MAP_READ_BIT;
		}

		if (write)
		{
			access |= GL_MAP_WRITE_BIT;
		}

		if (invalidate_range)
		{
			access |= GL_MAP_INVALIDATE_RANGE_BIT;
		}

		return glMapBufferRange(type, offset, size, access);
	}

	void* HardwareBuffer::map(const HardwareBufferType type,
		const bool read, const bool write, const bool invalidate_buffer)
	{
		GLbitfield access;

		assert(read || write);
		assert(!(invalidate_buffer && read));

		access = 0;

		if (read)
		{
			access |= GL_MAP_READ_BIT;
		}

		if (write)
		{
			access |= GL_MAP_WRITE_BIT;
		}

		if (invalidate_buffer)
		{
			access |= GL_MAP_INVALIDATE_BUFFER_BIT;
		}

		return glMapBufferRange(type, 0, get_size(), access);
	}

	bool HardwareBuffer::unmap(const HardwareBufferType type)
	{
		return glUnmapBuffer(type) == GL_TRUE;
	}

}
