/****************************************************************************
 *            hardwarebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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

	void HardwareBuffer::set_size(const BufferTargetType target,
		const Uint64 size, const HardwareBufferUsageType usage)
	{
		m_size = size;

		glBufferData(target, size, 0, usage);
	}

	void HardwareBuffer::set(const BufferTargetType target,
		const AbstractReadMemory &buffer,
		const HardwareBufferUsageType usage)
	{
		m_size = buffer.get_size();

		glBufferData(target, buffer.get_size(), buffer.get_ptr(),
			usage);
	}

	void HardwareBuffer::update(const BufferTargetType target,
		const AbstractReadMemory &buffer, const Uint64 offset)
	{
		assert(get_size() >= (offset + buffer.get_size()));

		glBufferSubData(target, offset, buffer.get_size(),
			buffer.get_ptr());
	}

	void HardwareBuffer::bind(const BufferTargetType target)
	{
		glBindBuffer(target, m_id);
	}

	void HardwareBuffer::unbind(const BufferTargetType target)
	{
		glBindBuffer(target, 0);
	}

	void HardwareBuffer::bind(const BufferTargetType target,
		const Uint32 index)
	{
		glBindBufferBase(target, index, m_id);
	}

	void HardwareBuffer::unbind(const BufferTargetType target,
		const Uint32 index)
	{
		glBindBufferBase(target, index, 0);
	}

	void* HardwareBuffer::map(const BufferTargetType target,
		const HardwareBufferAccessType access)
	{
		return glMapBuffer(target, access);
	}

	void* HardwareBuffer::map_range(const BufferTargetType target,
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

		return glMapBufferRange(target, offset, size, access);
	}

	void* HardwareBuffer::map(const BufferTargetType target,
		const bool read, const bool write,
		const bool invalidate_buffer)
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

		return glMapBufferRange(target, 0, get_size(), access);
	}

	bool HardwareBuffer::unmap(const BufferTargetType target)
	{
		return glUnmapBuffer(target) == GL_TRUE;
	}

}
