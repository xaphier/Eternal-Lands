/****************************************************************************
 *            shadersourceutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadersourceutil.hpp"
#include "commonparameterutil.hpp"
#include "parameterqualifierutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String shader_source_type_datas[] =
		{
			String(UTF8("world_depth_transform")),
			String(UTF8("world_normal_transform")),
			String(UTF8("world_tangent_transform")),
			String(UTF8("view_direction")),
			String(UTF8("view_transform")),
			String(UTF8("fog")),
			String(UTF8("vertex_light")),
			String(UTF8("fragment_light")),
			String(UTF8("uv")),
			String(UTF8("shadow_uv")),
			String(UTF8("shadow_mapping")),
			String(UTF8("normal_mapping")),
			String(UTF8("normal_depth_mapping")),
			String(UTF8("diffuse_mapping")),
			String(UTF8("specular_mapping")),
			String(UTF8("emission_mapping")),
			String(UTF8("transparent")),
			String(UTF8("shadow_map")),
			String(UTF8("tbn_matrix"))
		};

		const Uint32 shader_source_type_datas_count =
			sizeof(shader_source_type_datas) / sizeof(String);

	}

	const String &ShaderSourceUtil::get_str(const ShaderSourceType source)
	{
		if (shader_source_type_datas_count <= source)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_source_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					source))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderSourceType")));
		}

		return shader_source_type_datas[source];
	}

	ShaderSourceType ShaderSourceUtil::get_shader_source(const String &str)
	{
		Uint32 i;
		ShaderSourceType shader_source_type;

		for (i = 0; i < shader_source_type_datas_count; ++i)
		{
			shader_source_type =
				static_cast<ShaderSourceType>(i);

			if (str == get_str(shader_source_type))
			{
				return shader_source_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"ShaderSourceType")));
	}

	Uint32 ShaderSourceUtil::get_shader_source_count()
	{
		return shader_source_type_datas_count;
	}

	OutStream& operator<<(OutStream &str, const ShaderSourceType value)
	{
		str << ShaderSourceUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ShaderSourceType &value)
	{
		StringType string;

		str >> string;

		value = ShaderSourceUtil::get_shader_source(String(string));

		return str;
	}

}
