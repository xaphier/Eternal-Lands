/****************************************************************************
 *            xmlreader.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "xmlreader.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	XmlReader::XmlReader(xmlBufferPtr buffer)
	{
		m_doc = xmlReadMemory((const char*)xmlBufferContent(buffer),
			xmlBufferLength(buffer), "", 0, XML_PARSE_NOENT);

		if (m_doc == 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error reading the xml "
					"data")));
		}
	}

	XmlReader::XmlReader(const String &file_name)
	{
		m_doc = xmlReadFile(utf8_to_string(file_name.get()).c_str(), 0,
			XML_PARSE_NOENT);

		if (m_doc == 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error reading the xml "
					"file"))
				<< boost::errinfo_file_name(file_name));
		}
	}

	XmlReader::XmlReader(const String &file_name,
		const FileSystemSharedPtr file_system)
	{
		m_doc = xmlReadFile(utf8_to_string(file_name.get()).c_str(), 0,
			XML_PARSE_NOENT);

		if (m_doc == 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error reading the xml "
					"file"))
				<< boost::errinfo_file_name(file_name));
		}
	}

	XmlReader::~XmlReader() throw()
	{
		xmlFreeDoc(m_doc);
	}

	xmlNodePtr XmlReader::get_root_node()
	{
		return xmlDocGetRootElement(m_doc);
	}

}