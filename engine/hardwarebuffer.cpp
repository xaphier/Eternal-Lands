/****************************************************************************
 *            hardwarebuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "hardwarebuffer.hpp"
#include "exceptions.hpp"

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
		CHECK_GL_ERROR();

		m_size = size;
		glBindBuffer(type, m_id);
		glBufferData(type, size, 0, usage);

		CHECK_GL_ERROR();
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

