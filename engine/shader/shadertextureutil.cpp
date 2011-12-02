/****************************************************************************
 *            shadertextureutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadertextureutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String shader_texture_type_datas[] =
		{
			String(L"diffuse_sampler_0"),
			String(L"diffuse_sampler_1"),
			String(L"diffuse_sampler_2"),
			String(L"diffuse_sampler_3"),
			String(L"normal_sampler_0"),
			String(L"specular_sampler_0"),
			String(L"glow_sampler_0"),
			String(L"blend_sampler_0"),
			String(L"normal_sampler_1"),
			String(L"specular_sampler_1"),
			String(L"glow_sampler_1"),
			String(L"blend_sampler_1"),
			String(L"extra_sampler_0"),
			String(L"extra_sampler_1"),
			String(L"extra_sampler_2"),
			String(L"shadow_sampler")
		};

		const Uint32 shader_texture_type_datas_count =
			sizeof(shader_texture_type_datas) / sizeof(String);

	}

	const String &ShaderTextureUtil::get_str(
		const ShaderTextureType texture)
	{
		if (shader_texture_type_datas_count <= texture)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_texture_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					texture))
				<< boost::errinfo_type_info_name(
					"ShaderTextureType"));
		}

		return shader_texture_type_datas[texture];
	}

	ShaderTextureType ShaderTextureUtil::get_shader_texture(
		const String &str)
	{
		Uint32 i;
		ShaderTextureType shader_texture_type;

		for (i = 0; i < shader_texture_type_datas_count; i++)
		{
			shader_texture_type =
				static_cast<ShaderTextureType>(i);

			if (str == get_str(shader_texture_type))
			{
				return shader_texture_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name("ShaderTextureType"));
	}

	bool ShaderTextureUtil::get_shader_texture(const String &str,
		ShaderTextureType &shader_texture)
	{
		Uint32 i;
		ShaderTextureType tmp;

		for (i = 0; i < shader_texture_type_datas_count; i++)
		{
			tmp = static_cast<ShaderTextureType>(i);

			if (str == get_str(tmp))
			{
				shader_texture = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 ShaderTextureUtil::get_shader_texture_count()
	{
		return shader_texture_type_datas_count;
	}

	OutStream& operator<<(OutStream &str, const ShaderTextureType value)
	{
		str << ShaderTextureUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ShaderTextureType &value)
	{
		StringType string;

		str >> string;

		value = ShaderTextureUtil::get_shader_texture(String(string));

		return str;
	}

}
