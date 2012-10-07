/****************************************************************************
 *            readwritememory.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "readwritememory.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	ReadWriteMemory::ReadWriteMemory(const Uint64 size): m_ptr(0),
		m_size(0)
	{
		resize(size);
	}

	ReadWriteMemory::~ReadWriteMemory() noexcept
	{
		if (m_ptr != nullptr)
		{
			free(m_ptr);
		}
	}

	void* ReadWriteMemory::get_ptr() noexcept
	{
		return m_ptr;
	}

	const void* ReadWriteMemory::get_ptr() const noexcept
	{
		return m_ptr;
	}

	Uint64 ReadWriteMemory::get_size() const noexcept
	{
		return m_size;
	}

	void ReadWriteMemory::resize(const Uint64 size)
	{
		Uint8ScopedArray ptr;

		if (size != get_size())
		{
			if (m_ptr == nullptr)
			{
				m_ptr = static_cast<Uint8*>(calloc(size, 1));
			}
			else
			{
				m_ptr = static_cast<Uint8*>(realloc(m_ptr,
					size));

				if (size > get_size())
				{
					memset(m_ptr + get_size(), 0,
						size - get_size());
				}
			}
		}

		m_size = size;
	}

	void ReadWriteMemory::copy(const AbstractReadMemory &memory_buffer,
		const Uint64 size, const Uint64 source_offset,
		const Uint64 dest_offset)
	{
		if (memory_buffer.get_size() < (source_offset + size))
		{
			EL_THROW_EXCEPTION(BufferTooSmallException()
				<< errinfo_size(memory_buffer.get_size()));
		}

		if (get_size() < (dest_offset + size))
		{
			EL_THROW_EXCEPTION(BufferTooSmallException()
				<< errinfo_size(get_size()));
		}

		memcpy(static_cast<Uint8*>(get_ptr()) + dest_offset,
			static_cast<const Uint8* const>(
			memory_buffer.get_ptr()) + source_offset, size);
	}

	void ReadWriteMemory::copy(const AbstractReadMemory &memory_buffer,
		const Uint64 source_offset, const Uint64 dest_offset)
	{
		copy(memory_buffer, memory_buffer.get_size(), source_offset,
			dest_offset);
	}

}
