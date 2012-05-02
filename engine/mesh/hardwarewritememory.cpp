/****************************************************************************
 *            hardwarewritememory.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "hardwarewritememory.hpp"

namespace eternal_lands
{

	HardwareWriteMemory::HardwareWriteMemory(
		const HardwareBufferSharedPtr &buffer,
		const HardwareBufferType type): m_buffer(buffer), m_type(type)
	{
		m_memory_buffer.resize(buffer->get_size());
	}

	HardwareWriteMemory::~HardwareWriteMemory() throw()
	{
		m_buffer->bind(get_type());
		m_buffer->update(get_type(), m_memory_buffer);
	}

	void* HardwareWriteMemory::get_ptr()
	{
		return m_memory_buffer.get_ptr();
	}

	Uint64 HardwareWriteMemory::get_size() const
	{
		return m_memory_buffer.get_size();
	}

}
