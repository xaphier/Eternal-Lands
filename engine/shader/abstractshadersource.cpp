/****************************************************************************
 *            abstractshadersource.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractshadersource.hpp"
#include "exceptions.hpp"
#include "logging.hpp"
#include "samplerparameterutil.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "shadersourcedata.hpp"
#include "xmlbuffer.hpp"
#include "xmlwriter.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	AbstractShaderSource::AbstractShaderSource()
	{
	}

	AbstractShaderSource::~AbstractShaderSource() noexcept
	{
	}

	void AbstractShaderSource::load_xml(
		const FileSystemSharedPtr &file_system, const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = boost::make_shared<XmlReader>(file_system,
			file_name);

		load_xml(xml_reader->get_root_node());
	}

	void AbstractShaderSource::load_xml(const String &file_name)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = boost::make_shared<XmlReader>(file_name);

		load_xml(xml_reader->get_root_node());
	}

	void AbstractShaderSource::load_xml_string(const String &str)
	{
		XmlReaderSharedPtr xml_reader;

		xml_reader = XmlReader::get_xml_reader_from_string(str);

		load_xml(xml_reader->get_root_node());
	}

	void AbstractShaderSource::load_xml(const xmlNodePtr node)
	{
		do_load_xml(node);
	}

	void AbstractShaderSource::save_xml(const String &file_name) const
	{
		XmlWriterSharedPtr xml_writer;

		xml_writer = boost::make_shared<XmlWriter>(file_name);

		do_save_xml(xml_writer);
	}

	String AbstractShaderSource::save_xml_string() const
	{
		XmlBufferSharedPtr xml_buffer;

		xml_buffer = boost::make_shared<XmlBuffer>();

		{
			XmlWriterSharedPtr xml_writer;

			xml_writer = boost::make_shared<XmlWriter>(xml_buffer);

			do_save_xml(xml_writer);
		}

		return xml_buffer->get_string();
	}

	void AbstractShaderSource::build_source(
		const ShaderSourceParameterVector &locals, 
		const String &indent, const ShaderType shader_type,
		const ShaderVersionType version, OutStream &stream,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		std::vector<std::string> lines;

		boost::split(lines, get_source(shader_type, version).get(),
			boost::is_any_of(UTF8("\n")),
			boost::token_compress_on);

		stream << indent << UTF8("{") << std::endl;

		BOOST_FOREACH(const std::string &line, lines)
		{
			stream << indent << UTF8("\t") << line << std::endl;
		}

		stream << indent << UTF8("}") << std::endl;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(shader_type, version))
		{
			ShaderSourceParameterBuilder::add_parameter(parameter,
				locals, globals, uniform_buffers);
		}
	}

	void AbstractShaderSource::build_function_use(const String &indent,
		const String &name, const String &parameter_prefix,
		const ShaderType shader_type, const ShaderVersionType version,
		OutStream &stream) const
	{
		bool first;

		stream << indent << name << UTF8("(");

		first = true;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(shader_type, version))
		{
			if (!parameter.get_auto_parameter())
			{
				parameter.write_name(parameter_prefix, stream,
					first);
			}
		}

		stream << UTF8(");") << std::endl;
	}

	void AbstractShaderSource::build_function_source(
		const ShaderSourceParameterVector &locals,
		const ParameterSizeTypeUint16Map &sizes, const String &indent,
		const String &name, const ShaderType shader_type,
		const ShaderVersionType version, OutStream &stream,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		std::vector<std::string> lines;
		bool first;

		stream << indent << UTF8("/* ") << get_typed_name();
		stream << UTF8(" */\n");
		stream << indent << UTF8("void ") << name << UTF8("(");

		first = true;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(shader_type, version))
		{
			if (!parameter.get_auto_parameter())
			{
				parameter.write_parameter(String(), sizes,
					stream, first);
			}
		}

		stream << UTF8(")") << std::endl;

		boost::split(lines, get_source(shader_type, version).get(),
			boost::is_any_of(UTF8("\n")),
			boost::token_compress_on);

		stream << indent << UTF8("{") << std::endl;

		BOOST_FOREACH(const std::string &line, lines)
		{
			stream << indent << UTF8("\t") << line << std::endl;
		}

		stream << indent << UTF8("}") << std::endl;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(shader_type, version))
		{
			ShaderSourceParameterBuilder::add_parameter(parameter,
				locals, globals, uniform_buffers);
		}
	}

	void AbstractShaderSource::build_function(
		const ShaderSourceParameterVector &locals,
		const ParameterSizeTypeUint16Map &array_sizes,
		const String &indent, const String &parameter_prefix,
		const String &use_indent, const ShaderType shader_type,
		const ShaderVersionType version, OutStream &stream,
		OutStream &function, ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		function << indent << UTF8("/* ") << get_typed_name();
		function << UTF8(" */\n");

		stream << use_indent << UTF8("/* ") << get_typed_name();
		stream << UTF8(" */\n");

		build_function_source(locals, array_sizes, indent,
			get_typed_name(), shader_type, version, function,
			globals, uniform_buffers);

		build_function_use(use_indent, get_typed_name(),
			parameter_prefix, shader_type, version, stream);
	}

	bool AbstractShaderSource::check_source_parameter(
		const ShaderType shader_type, const ShaderVersionType version,
		const String &name) const
	{
		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(shader_type, version))
		{
			if ((parameter.get_name() == name) &&
				(parameter.get_qualifier() != pqt_out))
			{
				return true;
			}
		}

		return false;
	}

	String AbstractShaderSource::get_typed_name() const
	{
		StringStream str;

		str << get_name() << UTF8("_") << get_type();

		return String(str.str());
	}

}
