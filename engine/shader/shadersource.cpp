/****************************************************************************
 *            shadersource.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersource.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "samplerparameterutil.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "shadersourcedata.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	namespace
	{

		inline bool sort_shader_source_data(
			const ShaderSourceData &shader_source_data_0,
			const ShaderSourceData &shader_source_data_1)
		{
			return  shader_source_data_0.get_version() >
				shader_source_data_1.get_version();
		}

	}

	ShaderSource::ShaderSource()
	{
	}

	ShaderSource::ShaderSource(const boost::uuids::uuid &uuid):
		m_uuid(uuid)
	{
	}

	ShaderSource::~ShaderSource() noexcept
	{
	}

	void ShaderSource::load_datas_xml(const String &source,
		const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("shader_source_datas"))
			!= 0)
		{
			return;
		}

		m_datas.clear();

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("shader_source_data")) == 0)
			{
				m_datas.push_back(ShaderSourceData(source, it));
			}
		}
		while (XmlUtil::next(it, true));

		std::sort(m_datas.begin(), m_datas.end(),
			sort_shader_source_data);
	}

	void ShaderSource::load_xml(const xmlNodePtr node)
	{
		xmlNodePtr it;
		String source;

		if (node == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("parameter is zero"))
				<< errinfo_parameter_name(UTF8("node")));
		}

		if (xmlStrcmp(node->name, BAD_CAST UTF8("shader_source")) != 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("name")) == 0)
			{
				set_name(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("type")) == 0)
			{
				set_type(ShaderSourceUtil::get_shader_source(
					XmlUtil::get_string_value(it)));
			}
		}
		while (XmlUtil::next(it, true));

		source = get_typed_name();

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name,
				BAD_CAST UTF8("shader_source_datas")) == 0)
			{
				load_datas_xml(source, it);
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSource::load_xml(const FileSystemSharedPtr &file_system,
		const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReaderSharedPtr(new XmlReader(file_system,
			file_name));

		load_xml(xml_reader->get_root_node());
	}

	void ShaderSource::load_xml(const String &file_name)
	{
		XmlReaderSharedPtr reader;

		reader = XmlReaderSharedPtr(new XmlReader(file_name));

		load_xml(reader->get_root_node());
	}

	void ShaderSource::save_xml(const XmlWriterSharedPtr &writer) const
	{
		writer->start_element(UTF8("shader_source"));

		writer->write_element(UTF8("name"), get_name());

		writer->write_element(UTF8("type"), ShaderSourceUtil::get_str(
			get_type()));

		writer->start_element(UTF8("shader_source_datas"));

		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			data.save_xml(writer);
		}

		writer->end_element();

		writer->end_element();
	}

	void ShaderSource::save_xml(const String &file_name) const
	{
		XmlWriterSharedPtr writer;

		writer = XmlWriterSharedPtr(new XmlWriter(file_name));

		save_xml(writer);
	}

	void ShaderSource::build_source(const ShaderVersionType &version,
		const ShaderSourceParameterVector &locals,
		const String &indent, OutStream &stream,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		stream << indent << UTF8("/* ") << get_typed_name();
		stream << UTF8(" */\n");
		get_data(version).build_source(locals, indent, stream, globals,
			uniform_buffers);
	}

	void ShaderSource::build_function(const ShaderVersionType &version,
		const ShaderSourceParameterVector &locals,
		const ParameterSizeTypeUint16Map &array_sizes,
		const String &indent, const String &parameter_prefix,
		const String &use_indent, OutStream &stream,
		OutStream &function, ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		function << indent << UTF8("/* ") << get_typed_name();
		function << UTF8(" */\n");

		stream << use_indent << UTF8("/* ") << get_typed_name();
		stream << UTF8(" */\n");

		get_data(version).build_function(locals, array_sizes, indent,
			get_typed_name(), parameter_prefix, use_indent, stream,
			function, globals, uniform_buffers);
	}

	bool ShaderSource::check_source_parameter(
		const ShaderVersionType &version, const String &name) const
	{
		return get_data(version).check_source_parameter(name);
	}

	bool ShaderSource::get_has_data(const ShaderVersionType version)
		const
	{
		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			if (data.get_version() <= version)
			{
				return true;
			}
		}

		return false;
	}

	const ShaderSourceData &ShaderSource::get_data(
		const ShaderVersionType version) const
	{
		BOOST_FOREACH(const ShaderSourceData &data, get_datas())
		{
			if (data.get_version() <= version)
			{
				return data;
			}
		}

		EL_THROW_MESSAGE_EXCEPTION(UTF8("No shader source data with "
			"shader version %1% in '%2%' found."),
			version % get_typed_name(), ItemNotFoundException());
	}

	void ShaderSource::set_datas(const ShaderSourceDataVector &datas)
	{
		m_datas = datas;

		std::sort(m_datas.begin(), m_datas.end(),
			sort_shader_source_data);
	}

	String ShaderSource::get_typed_name() const
	{
		StringStream str;

		str << get_name() << UTF8("_") << get_type();

		return String(str.str());
	}

}
