/****************************************************************************
 *            mappedhardwarewritememorybuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "mappedhardwarewritememorybuffer.hpp"

namespace eternal_lands
{

	MappedHardwareWriteMemoryBuffer::MappedHardwareWriteMemoryBuffer(
		const HardwareBufferSharedPtr &buffer,
		const HardwareBufferType type): m_buffer(buffer), m_type(type)
	{
		m_buffer->bind(get_type());
		m_ptr = m_buffer->map(get_type(), hbat_write_only);
	}

	MappedHardwareWriteMemoryBuffer::~MappedHardwareWriteMemoryBuffer()
		throw()
	{
		m_buffer->unmap(get_type());
		m_buffer->unbind(get_type());
	}

	void* MappedHardwareWriteMemoryBuffer::get_ptr()
	{
		return m_ptr;
	}

	Uint64 MappedHardwareWriteMemoryBuffer::get_size() const
	{
		return m_buffer->get_size();
	}

}
