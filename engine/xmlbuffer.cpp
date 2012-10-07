/****************************************************************************
 *            xmlbuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "xmlbuffer.hpp"

namespace eternal_lands
{

	XmlBuffer::XmlBuffer(): m_buffer(0)
	{
		m_buffer = xmlBufferCreate();

		assert(m_buffer != 0);
	}

	XmlBuffer::XmlBuffer(const char* str): m_buffer(0)
	{
		m_buffer = xmlBufferCreateStatic(const_cast<char*>(str),
			strlen(str));

		assert(m_buffer != 0);
	}

	XmlBuffer::~XmlBuffer() noexcept
	{
		assert(m_buffer != 0);

		xmlBufferFree(m_buffer);

		m_buffer = 0;
	}

	String XmlBuffer::get_string() const
	{
		return String(std::string(reinterpret_cast<const char*>(
			xmlBufferContent(m_buffer)), get_size()));
	}

	Uint32 XmlBuffer::get_size() const
	{
		return xmlBufferLength(m_buffer);
	}

}
