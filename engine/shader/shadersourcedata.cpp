/****************************************************************************
 *            shadersourcedata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourcedata.hpp"
#include "samplerparameterutil.hpp"
#include "shadersourceparameterbuilder.hpp"
#include "shadersourceparameter.hpp"
#include "xmlwriter.hpp"
#include "xmlutil.hpp"

namespace eternal_lands
{

	ShaderSourceData::ShaderSourceData(): m_version(svt_120)
	{
	}

	ShaderSourceData::ShaderSourceData(const String &source,
		const xmlNodePtr node): m_version(svt_120)
	{
		load_xml(source, node);
	}

	ShaderSourceData::~ShaderSourceData() noexcept
	{
	}

	void ShaderSourceData::build_source(
		const ShaderSourceParameterVector &locals,
		const String &indent, OutStream &stream,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		StringTypeVector lines;

		boost::split(lines, get_source().get(), boost::is_any_of(
			UTF8("\n")), boost::token_compress_on);

		stream << indent << UTF8("{") << std::endl;

		BOOST_FOREACH(const StringType &line, lines)
		{
			stream << indent << UTF8("\t") << line << std::endl;
		}

		stream << indent << UTF8("}") << std::endl;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			ShaderSourceParameterBuilder::add_parameter(parameter,
				locals, globals, uniform_buffers);
		}
	}

	void ShaderSourceData::build_function_use(const String &indent,
		const String &name, const String &parameter_prefix,
		OutStream &stream) const
	{
		bool first;

		stream << indent << name << UTF8("(");

		first = true;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			parameter.write_name(parameter_prefix, stream, first);
		}

		stream << UTF8(");") << std::endl;
	}

	void ShaderSourceData::build_function_source(
		const ShaderSourceParameterVector &locals,
		const String &indent, const String &name, OutStream &stream,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		StringTypeVector lines;
		bool first;

		stream << indent << UTF8("void ") << name << UTF8("(");

		first = true;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			parameter.write_parameter(String(), stream, first);
		}

		stream << UTF8(")") << std::endl;

		boost::split(lines, get_source().get(), boost::is_any_of(
			UTF8("\n")), boost::token_compress_on);

		stream << indent << UTF8("{") << std::endl;

		BOOST_FOREACH(const StringType &line, lines)
		{
			stream << indent << UTF8("\t") << line << std::endl;
		}

		stream << indent << UTF8("}") << std::endl;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			ShaderSourceParameterBuilder::add_parameter(parameter,
				locals, globals, uniform_buffers);
		}
	}

	bool ShaderSourceData::check_source_parameter(const String &name) const
	{
		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			if ((parameter.get_name() == name) &&
				(parameter.get_qualifier() != pqt_out))
			{
				return true;
			}
		}

		return false;
	}

	void ShaderSourceData::load_parameters_xml(const String &source,
		const xmlNodePtr node)
	{
		xmlNodePtr it;

		m_parameters.clear();

		if (!XmlUtil::has_children(node, true))
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("parameter"))
				== 0)
			{
				m_parameters.push_back(ShaderSourceParameter(
					source, it));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceData::load_xml(const String &source,
		const xmlNodePtr node)
	{
		xmlNodePtr it;

		if (xmlStrcmp(node->name, BAD_CAST UTF8("shader_source_data"))
			!= 0)
		{
			return;
		}

		it = XmlUtil::children(node, true);

		do
		{
			if (xmlStrcmp(it->name, BAD_CAST UTF8("parameters"))
				== 0)
			{
				load_parameters_xml(source, it);
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("source")) == 0)
			{
				set_source(XmlUtil::get_string_value(it));
			}

			if (xmlStrcmp(it->name, BAD_CAST UTF8("version")) == 0)
			{
				set_version(
					ShaderVersionUtil::get_shader_version(
						XmlUtil::get_string_value(it)));
			}
		}
		while (XmlUtil::next(it, true));
	}

	void ShaderSourceData::save_xml(const XmlWriterSharedPtr &writer) const
	{
		StringVariantMap::const_iterator it, end;

		writer->start_element(UTF8("shader_source_data"));

		writer->start_element(UTF8("parameters"));

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters())
		{
			parameter.save_xml(writer);
		}

		writer->end_element();

		writer->write_element(UTF8("source"), get_source());
		writer->write_element(UTF8("version"),
			ShaderVersionUtil::get_str(get_version()));

		writer->end_element();
	}

	void ShaderSourceData::set_parameters(
		const ShaderSourceParameterVector &parameters)
	{
		m_parameters = parameters;
	}

	void ShaderSourceData::build_function(
		const ShaderSourceParameterVector &locals,
		const String &indent, const String &name,
		const String &parameter_prefix, const String &use_indent,
		OutStream &stream, OutStream &function,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		build_function_source(locals, indent, name, function, globals,
			uniform_buffers);

		build_function_use(use_indent, name, parameter_prefix,
			stream);
	}

}
