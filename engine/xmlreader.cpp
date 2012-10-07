/****************************************************************************
 *            xmlreader.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "xmlreader.hpp"
#include "xmlbuffer.hpp"
#include "exceptions.hpp"
#include "filesystem.hpp"
#include "reader.hpp"
#include "abstractreadmemory.hpp"
#include "utf.hpp"

namespace eternal_lands
{

	XmlReader::XmlReader(const XmlBufferSharedPtr &buffer):
		m_buffer(buffer)
	{
		m_doc = xmlReadMemory((const char*)xmlBufferContent(
			buffer->get_buffer()), buffer->get_size(), "", 0,
			XML_PARSE_NOENT);

		if (m_doc == 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error reading the xml"
					" data")));
		}
	}

	XmlReader::XmlReader(const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		ReaderSharedPtr reader;

		reader = file_system->get_file(file_name);

		m_doc = xmlReadMemory(static_cast<const char*>(
			reader->get_buffer()->get_ptr()), reader->get_size(),
			"", 0, XML_PARSE_NOENT);

		if (m_doc == 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error reading the xml"
					" file"))
				<< boost::errinfo_file_name(file_name));
		}
	}

	XmlReader::XmlReader(const String &file_name)
	{
		std::string name;

		name = utf8_to_string(file_name);

		m_doc = xmlReadFile(name.c_str(), 0, XML_PARSE_NOENT);

		if (m_doc == 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error reading the xml"
					" file"))
				<< boost::errinfo_file_name(file_name));
		}
	}

	XmlReader::XmlReader(): m_doc(0)
	{
	}

	XmlReader::~XmlReader() noexcept
	{
		if (m_doc != 0)
		{
			xmlFreeDoc(m_doc);

			m_doc = 0;
		}
	}

	bool XmlReader::init_from_reader(const ReaderSharedPtr &reader)
	{
		m_doc = xmlReadMemory(static_cast<const char*>(
			reader->get_buffer()->get_ptr()),
			reader->get_size(), "", 0, XML_PARSE_NOENT);

		return m_doc != 0;
	}

	bool XmlReader::init_from_string(const std::string &str)
	{
		if (str.size() <= 5)
		{
			return false;
		}

		if ((str[0] != '<') || (str[1] != '?') || (str[2] != 'x') ||
			(str[3] != 'm') || (str[4] != 'l'))
		{
			return false;
		}

		m_doc = xmlReadMemory(str.c_str(), str.size(), "", 0,
			XML_PARSE_NOENT);

		return m_doc != 0;
	}

	xmlNodePtr XmlReader::get_root_node()
	{
		return xmlDocGetRootElement(m_doc);
	}

	bool XmlReader::get_xml_reader(const FileSystemSharedPtr &file_system,
		const String &file_name, XmlReaderSharedPtr &xml_reader)
	{
		ReaderSharedPtr reader;
		XmlReaderSharedPtr result;
		Uint8Array5 magic;

		result = boost::make_shared<XmlReader>();

		if (result->init_from_string(file_name))
		{
			xml_reader = result;

			return true;
		}

		if (!file_system->get_file_if_readable(file_name, reader))
		{
			return false;
		}

		if (reader->get_size() < 5)
		{
			return false;
		}

		reader->set_position(0);

		BOOST_FOREACH(Uint8 &value, magic)
		{
			value = reader->read_u8();
		}

		reader->set_position(0);

		if ((magic[0] != '<') || (magic[1] != '?') || (magic[2] != 'x')
			|| (magic[3] != 'm') || (magic[4] != 'l'))
		{
			return false;
		}

		if (!result->init_from_reader(reader))
		{
			return false;
		}

		xml_reader = result;

		return true;
	}

	XmlReaderSharedPtr XmlReader::get_xml_reader_from_string(
		const String &str)
	{
		XmlReaderSharedPtr result;

		result = boost::make_shared<XmlReader>();

		if (result->init_from_string(str))
		{
			return result;
		}

		EL_THROW_EXCEPTION(IoErrorException()
			<< errinfo_message(UTF8("Error reading the xml string"))
			<< errinfo_string_value(str));
	}

}
