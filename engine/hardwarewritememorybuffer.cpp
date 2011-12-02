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
		m_buffer->bind(get_type());
		m_ptr = m_buffer->map(get_type(), hbat_write_only);
	}

	HardwareWriteMemoryBuffer::~HardwareWriteMemoryBuffer() throw()
	{
		m_buffer->unmap(get_type());
	}

	void* HardwareWriteMemoryBuffer::get_ptr()
	{
		return m_ptr;
	}

	Uint64 HardwareWriteMemoryBuffer::get_size() const
	{
		return m_buffer->get_size();
	}

}

