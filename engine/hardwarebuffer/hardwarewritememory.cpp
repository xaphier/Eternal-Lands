/****************************************************************************
 *            hardwarewritememory.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "hardwarewritememory.hpp"

namespace eternal_lands
{

	HardwareWriteMemory::HardwareWriteMemory(
		const HardwareBufferSharedPtr &buffer,
		const BufferTargetType target): m_buffer(buffer),
		m_target(target)
	{
		m_memory_buffer.resize(buffer->get_size());
	}

	HardwareWriteMemory::~HardwareWriteMemory() noexcept
	{
		m_buffer->bind(m_target);
		m_buffer->update(m_target, m_memory_buffer);
		m_buffer->unbind(m_target);
	}

	void* HardwareWriteMemory::get_ptr() noexcept
	{
		return m_memory_buffer.get_ptr();
	}

	Uint64 HardwareWriteMemory::get_size() const noexcept
	{
		return m_memory_buffer.get_size();
	}

}
