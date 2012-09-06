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

	void AbstractShaderSource::build_source(const ShaderVersionType version,
		const ShaderSourceParameterVector &locals, 
		const String &indent, OutStream &stream,
		ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		std::vector<std::string> lines;

		boost::split(lines, get_source(version).get(),
			boost::is_any_of(UTF8("\n")),
			boost::token_compress_on);

		stream << indent << UTF8("{") << std::endl;

		BOOST_FOREACH(const std::string &line, lines)
		{
			stream << indent << UTF8("\t") << line << std::endl;
		}

		stream << indent << UTF8("}") << std::endl;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(version))
		{
			ShaderSourceParameterBuilder::add_parameter(parameter,
				locals, globals, uniform_buffers);
		}
	}

	void AbstractShaderSource::build_function_use(const String &indent,
		const String &name, const String &parameter_prefix,
		const ShaderVersionType version, OutStream &stream) const
	{
		bool first;

		stream << indent << name << UTF8("(");

		first = true;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(version))
		{
			parameter.write_name(parameter_prefix, stream, first);
		}

		stream << UTF8(");") << std::endl;
	}

	void AbstractShaderSource::build_function_source(
		const ShaderSourceParameterVector &locals,
		const ParameterSizeTypeUint16Map &sizes, const String &indent,
		const String &name, const ShaderVersionType version,
		OutStream &stream, ShaderSourceParameterVector &globals,
		UniformBufferUsage &uniform_buffers) const
	{
		std::vector<std::string> lines;
		bool first;

		stream << indent << UTF8("/* ") << get_typed_name();
		stream << UTF8(" */\n");
		stream << indent << UTF8("void ") << name << UTF8("(");

		first = true;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(version))
		{
			parameter.write_parameter(String(), sizes, stream,
				first);
		}

		stream << UTF8(")") << std::endl;

		boost::split(lines, get_source(version).get(),
			boost::is_any_of(UTF8("\n")),
			boost::token_compress_on);

		stream << indent << UTF8("{") << std::endl;

		BOOST_FOREACH(const std::string &line, lines)
		{
			stream << indent << UTF8("\t") << line << std::endl;
		}

		stream << indent << UTF8("}") << std::endl;

		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(version))
		{
			ShaderSourceParameterBuilder::add_parameter(parameter,
				locals, globals, uniform_buffers);
		}
	}

	void AbstractShaderSource::build_function(
		const ShaderVersionType version,
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

		build_function_source(locals, array_sizes, indent,
			get_typed_name(), version, function, globals,
			uniform_buffers);

		build_function_use(use_indent, get_typed_name(),
			parameter_prefix, version, stream);
	}

	bool AbstractShaderSource::check_source_parameter(
		const ShaderVersionType version, const String &name) const
	{
		BOOST_FOREACH(const ShaderSourceParameter &parameter,
			get_parameters(version))
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
