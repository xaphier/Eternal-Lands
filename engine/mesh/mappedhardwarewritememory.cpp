/****************************************************************************
 *            mappedhardwarewritememory.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "mappedhardwarewritememory.hpp"

namespace eternal_lands
{

	MappedHardwareWriteMemory::MappedHardwareWriteMemory(
		const HardwareBufferSharedPtr &buffer,
		const HardwareBufferType type): m_buffer(buffer), m_type(type)
	{
		m_ptr = m_buffer->map(get_type(), hbat_write_only);
	}

	MappedHardwareWriteMemory::~MappedHardwareWriteMemory()
		throw()
	{
		m_buffer->unmap(get_type());
	}

	void* MappedHardwareWriteMemory::get_ptr()
	{
		return m_ptr;
	}

	Uint64 MappedHardwareWriteMemory::get_size() const
	{
		return m_buffer->get_size();
	}

}
