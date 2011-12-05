/****************************************************************************
 *            memorybuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "memorybuffer.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	MemoryBuffer::MemoryBuffer(const Uint64 size): m_size(0)
	{
		resize(size);
	}

	MemoryBuffer::~MemoryBuffer() throw()
	{
	}

	void* MemoryBuffer::get_ptr()
	{
		return m_ptr.get();
	}

	const void* MemoryBuffer::get_ptr() const
	{
		return m_ptr.get();
	}

	Uint64 MemoryBuffer::get_size() const
	{
		return m_size;
	}

	void MemoryBuffer::resize(const Uint64 size)
	{
		Uint8ScopedArray ptr;

		if (size != get_size())
		{
			if ((size > 0) && (get_size() > 0))
			{
				ptr.reset(new Uint8[size]);

				memcpy(ptr.get(), m_ptr.get(),
					std::min(size, get_size()));

				ptr.swap(m_ptr);
			}
			else
			{
				if (size > 0)
				{
					m_ptr.reset(new Uint8[size]);
				}
				else
				{
					m_ptr.reset();
				}
			}
		}

		m_size = size;
	}

	void MemoryBuffer::copy(const AbstractReadMemoryBuffer &memory_buffer,
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

	void MemoryBuffer::copy(const AbstractReadMemoryBuffer &memory_buffer,
		const Uint64 source_offset, const Uint64 dest_offset)
	{
		copy(memory_buffer, memory_buffer.get_size(), source_offset,
			dest_offset);
	}

}
