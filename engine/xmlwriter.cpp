/****************************************************************************
 *            xmlwriter.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "xmlwriter.hpp"
#include "exceptions.hpp"
#include "variantutil.hpp"

namespace eternal_lands
{

	XmlWriter::XmlWriter(xmlBufferPtr buffer)
	{
		m_writer = xmlNewTextWriterMemory(buffer, 0);

		/* Create a new XmlWriter for uri, with no compression. */
		if (m_writer == 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error creating the "
					"xml writer")));
		}

		if (xmlTextWriterSetIndent(m_writer, 8) < 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error at "
					"xmlTextWriterSetIndent")));
		}

		/* Start the document with the xml default for the version,
		 * encoding utf8 and the default for the standalone
		 * declaration. */
		if (xmlTextWriterStartDocument(m_writer, 0, "utf8", 0) < 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error at "
					"xmlTextWriterStartDocument")));
		}
	}

	XmlWriter::XmlWriter(const String &file_name)
	{
		m_writer = xmlNewTextWriterFilename(utf8_to_string(
			file_name.get()).c_str(), 0);

		/* Create a new XmlWriter for uri, with no compression. */
		if (m_writer == 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error creating the "
					"xml writer"))
				<< boost::errinfo_file_name(file_name));
		}

		if (xmlTextWriterSetIndent(m_writer, 8) < 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error at "
					"xmlTextWriterSetIndent"))
				<< boost::errinfo_file_name(file_name));
		}

		/* Start the document with the xml default for the version,
		 * encoding utf8 and the default for the standalone
		 * declaration. */
		if (xmlTextWriterStartDocument(m_writer, 0, "utf8", 0) < 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error at "
					"xmlTextWriterStartDocument"))
				<< boost::errinfo_file_name(file_name));
		}
	}

	XmlWriter::~XmlWriter() throw()
	{
		xmlTextWriterEndDocument(m_writer);

		xmlFreeTextWriter(m_writer);
	}

	void XmlWriter::start_element(const std::string &name)
	{
		if (xmlTextWriterStartElement(m_writer,
			BAD_CAST name.c_str()) < 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error at "
					"xmlTextWriterStartElement")));
		}
	}

	void XmlWriter::end_element()
	{
		if (xmlTextWriterEndElement(m_writer) < 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error at "
					"xmlTextWriterEndElement")));
		}
	}

	void XmlWriter::write_element(const std::string &name,
		const StringType &value)
	{
		if (xmlTextWriterWriteElement(m_writer, BAD_CAST name.c_str(),
			BAD_CAST value.c_str()) < 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error at "
					"xmlTextWriterWriteElement")));
		}

	}

	void XmlWriter::write_bool_element(const std::string &name,
		const bool value)
	{
		StringStream str;

		str << std::boolalpha << value;

		write_element(name, str.str());
	}

	void XmlWriter::write_float_element(const std::string &name,
		const float value)
	{
		StringStream str;

		str << value;

		write_element(name, str.str());
	}

	void XmlWriter::write_int_element(const std::string &name,
		const Sint64 value)
	{
		StringStream str;

		str << value;

		write_element(name, str.str());
	}

}