/****************************************************************************
 *            shaderbuildutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shaderbuildutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String shader_build_type_names[] =
		{
			String(UTF8("color")),
			String(UTF8("depth")),
			String(UTF8("shadow"))
		};

		const Uint32 shader_build_type_names_count =
			sizeof(shader_build_type_names) / sizeof(String);

	}

	const String &ShaderBuildUtil::get_str(
		const ShaderBuildType shader_build)
	{
		if (shader_build_type_names_count <= shader_build)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_build_type_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					shader_build))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderBuildType")));
		}

		return shader_build_type_names[shader_build];
	}

	ShaderBuildType ShaderBuildUtil::get_shader_build_type(
		const String &str)
	{
		Uint32 i;
		ShaderBuildType shader_build_type;

		for (i = 0; i < shader_build_type_names_count; ++i)
		{
			shader_build_type = static_cast<ShaderBuildType>(i);

			if (str == get_str(shader_build_type))
			{
				return shader_build_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"ShaderBuildType")));
	}

	OutStream& operator<<(OutStream &str, const ShaderBuildType value)
	{
		str << ShaderBuildUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ShaderBuildType &value)
	{
		StringType string;

		str >> string;

		value = ShaderBuildUtil::get_shader_build_type(String(
			string));

		return str;
	}

}