/****************************************************************************
 *            shaderoutpututil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shaderoutpututil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String shader_output_names[] =
		{
			String(UTF8("float")),
			String(UTF8("int")),
			String(UTF8("uint"))
		};

		const Uint32 shader_output_names_count =
			sizeof(shader_output_names) / sizeof(String);

	}

	const String &ShaderOutputUtil::get_str(
		const ShaderOutputType shader_output)
	{
		if (shader_output_names_count <= shader_output)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					get_shader_output_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					shader_output))
				<< boost::errinfo_type_info_name(
					UTF8("ShaderOutputType")));
		}

		return shader_output_names[shader_output];
	}

	ShaderOutputType ShaderOutputUtil::get_shader_output(
		const String &str)
	{
		Uint32 i;
		ShaderOutputType shader_output;

		for (i = 0; i < shader_output_names_count; i++)
		{
			shader_output =
				static_cast<ShaderOutputType>(i);

			if (str == get_str(shader_output))
			{
				return shader_output;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("ShaderOutputType")));
	}

	Uint32 ShaderOutputUtil::get_shader_output_count() noexcept
	{
		return shader_output_names_count;
	}

	OutStream& operator<<(OutStream &str, const ShaderOutputType value)
	{
		str << ShaderOutputUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ShaderOutputType &value)
	{
		std::string string;

		str >> string;

		value = ShaderOutputUtil::get_shader_output(String(string));

		return str;
	}

}
