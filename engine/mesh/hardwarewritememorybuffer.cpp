/****************************************************************************
 *            hardwarewritememorybuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "hardwarewritememorybuffer.hpp"

namespace eternal_lands
{

	HardwareWriteMemoryBuffer::HardwareWriteMemoryBuffer(
		const HardwareBufferSharedPtr &buffer,
		const HardwareBufferType type): m_buffer(buffer), m_type(type)
	{
		m_memory_buffer.resize(buffer->get_size());
	}

	HardwareWriteMemoryBuffer::~HardwareWriteMemoryBuffer() throw()
	{
		m_buffer->update(get_type(), m_memory_buffer);
	}

	void* HardwareWriteMemoryBuffer::get_ptr()
	{
		return m_memory_buffer.get_ptr();
	}

	Uint64 HardwareWriteMemoryBuffer::get_size() const
	{
		return m_memory_buffer.get_size();
	}

}
