/****************************************************************************
 *            shaderblendutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shaderblendutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String shader_blend_type_names[] =
		{
			String(UTF8("blend")),
			String(UTF8("slope")),
			String(UTF8("height")),
			String(UTF8("texture")),
			String(UTF8("smoothstep_slope")),
			String(UTF8("smoothstep_height")),
			String(UTF8("smoothstep_texture"))
		};

		const Uint32 shader_blend_type_names_count =
			sizeof(shader_blend_type_names) / sizeof(String);

	}

	const String &ShaderBlendUtil::get_str(
		const ShaderBlendType shader_blend)
	{
		if (shader_blend_type_names_count <= shader_blend)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_blend_type_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					shader_blend))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderBlendType")));
		}

		return shader_blend_type_names[shader_blend];
	}

	ShaderBlendType ShaderBlendUtil::get_shader_blend(const String &str)
	{
		Uint32 i;
		ShaderBlendType shader_blend_type;

		for (i = 0; i < shader_blend_type_names_count; ++i)
		{
			shader_blend_type = static_cast<ShaderBlendType>(i);

			if (str == get_str(shader_blend_type))
			{
				return shader_blend_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"ShaderBlendType")));
	}

	Uint32 ShaderBlendUtil::get_shader_blend_count() noexcept
	{
		return shader_blend_type_names_count;
	}

	OutStream &operator<<(OutStream &str, const ShaderBlendType value)
	{
		str << ShaderBlendUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, ShaderBlendType &value)
	{
		std::string string;

		str >> string;

		value = ShaderBlendUtil::get_shader_blend(String(string));

		return str;
	}

}
