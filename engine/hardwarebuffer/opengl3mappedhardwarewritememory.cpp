/****************************************************************************
 *            opengl3mappedhardwarewritememory.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl3mappedhardwarewritememory.hpp"

namespace eternal_lands
{

	Opengl3HardwareWriteMemory::Opengl3HardwareWriteMemory(
		const HardwareBufferSharedPtr &buffer,
		const BufferTargetType target): m_buffer(buffer),
		m_target(target)
	{
		m_buffer->bind(m_target);
		m_ptr = m_buffer->map(m_target, false, true, true);
		m_buffer->unbind(m_target);
	}

	Opengl3HardwareWriteMemory::~Opengl3HardwareWriteMemory()
		noexcept
	{
		m_buffer->bind(m_target);
		m_buffer->unmap(m_target);
		m_buffer->unbind(m_target);
	}

	void* Opengl3HardwareWriteMemory::get_ptr() noexcept
	{
		return m_ptr;
	}

	Uint64 Opengl3HardwareWriteMemory::get_size() const noexcept
	{
		return m_buffer->get_size();
	}

}
