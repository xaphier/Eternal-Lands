/****************************************************************************
 *            xmlwriter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "xmlwriter.hpp"
#include "xmlbuffer.hpp"
#include "exceptions.hpp"
#include "variantutil.hpp"
#include "glmutil.hpp"
#include "utf.hpp"

namespace eternal_lands
{

	XmlWriter::XmlWriter(const XmlBufferSharedPtr &buffer):
		m_buffer(buffer)
	{
		m_writer = xmlNewTextWriterMemory(buffer->get_buffer(), 0);

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

	XmlWriter::~XmlWriter() noexcept
	{
		xmlTextWriterEndDocument(m_writer);

		xmlFreeTextWriter(m_writer);
	}

	void XmlWriter::start_element(const String &name)
	{
		if (xmlTextWriterStartElement(m_writer,
			BAD_CAST name.get().c_str()) < 0)
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

	void XmlWriter::write_element(const String &name,
		const String &value)
	{
		if (xmlTextWriterWriteElement(m_writer,
			BAD_CAST name.get().c_str(),
			BAD_CAST value.get().c_str()) < 0)
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< errinfo_message(UTF8("Error at "
					"xmlTextWriterWriteElement")));
		}
	}

	void XmlWriter::write_bitset64_element(const String &name,
		const BitSet64 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_uuid_element(const String &name,
		const boost::uuids::uuid &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_bool_element(const String &name,
		const bool value)
	{
		StringStream str;

		str << std::boolalpha << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_float_element(const String &name,
		const float value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_int_element(const String &name,
		const Sint64 value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_variant_element(const String &element,
		const Variant &variant)
	{
		start_element(element);
		write_element(String(UTF8("type")),
			VariantUtil::get_type_string(variant));
		write_element(String(UTF8("value")),
			VariantUtil::get_value_string(variant));
		end_element();
	}

	void XmlWriter::write_string_variant_element(const String &element,
		const String &name, const Variant &variant)
	{
		start_element(element);
		write_element(String(UTF8("name")), name);
		write_element(String(UTF8("type")),
			VariantUtil::get_type_string(variant));
		write_element(String(UTF8("value")),
			VariantUtil::get_value_string(variant));
		end_element();
	}

	void XmlWriter::write_string_variant_map(const String &element,
		const String &elements, const StringVariantMap &variants)
	{
		start_element(element);

		BOOST_FOREACH(const StringVariantPair &variant, variants)
		{
			write_string_variant_element(elements, variant.first,
				variant.second);
		}

		end_element();
	}

	void XmlWriter::write_bvec2_element(const String &name,
		const glm::bvec2 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_bvec3_element(const String &name,
		const glm::bvec3 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_bvec4_element(const String &name,
		const glm::bvec4 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_uvec2_element(const String &name,
		const glm::uvec2 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_uvec3_element(const String &name,
		const glm::uvec3 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_uvec4_element(const String &name,
		const glm::uvec4 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_ivec2_element(const String &name,
		const glm::ivec2 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_ivec3_element(const String &name,
		const glm::ivec3 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_ivec4_element(const String &name,
		const glm::ivec4 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_vec2_element(const String &name,
		const glm::vec2 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_vec3_element(const String &name,
		const glm::vec3 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_vec4_element(const String &name,
		const glm::vec4 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_mat2_element(const String &name,
		const glm::mat2 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_mat2x3_element(const String &name,
		const glm::mat2x3 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_mat2x4_element(const String &name,
		const glm::mat2x4 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_mat3x2_element(const String &name,
		const glm::mat3x2 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_mat3_element(const String &name,
		const glm::mat3 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_mat3x4_element(const String &name,
		const glm::mat3x4 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_mat4x2_element(const String &name,
		const glm::mat4x2 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_mat4x3_element(const String &name,
		const glm::mat4x3 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_mat4_element(const String &name,
		const glm::mat4 &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

	void XmlWriter::write_quaternion_element(const String &name,
		const glm::quat &value)
	{
		StringStream str;

		str << value;

		write_element(name, String(str.str()));
	}

}
