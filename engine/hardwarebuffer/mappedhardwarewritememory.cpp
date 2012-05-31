/****************************************************************************
 *            mappedhardwarewritememory.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "mappedhardwarewritememory.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	MappedHardwareWriteMemory::MappedHardwareWriteMemory(
		const HardwareBufferSharedPtr &buffer,
		const BufferTargetType target): m_buffer(buffer),
		m_target(target)
	{
		m_buffer->bind(m_target);
		m_ptr = m_buffer->map(m_target, hbat_write_only);
		m_buffer->unbind(m_target);

		if (m_ptr == nullptr)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("buffer ptr is zero")));
		}
	}

	MappedHardwareWriteMemory::~MappedHardwareWriteMemory() noexcept
	{
		m_buffer->bind(m_target);
		m_buffer->unmap(m_target);
		m_buffer->unbind(m_target);
	}

	void* MappedHardwareWriteMemory::get_ptr() noexcept
	{
		return m_ptr;
	}

	Uint64 MappedHardwareWriteMemory::get_size() const noexcept
	{
		return m_buffer->get_size();
	}

}
