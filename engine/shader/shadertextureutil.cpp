/****************************************************************************
 *            shadertextureutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadertextureutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class ShaderTextureTypeData
		{
			private:
				const String m_name;
				const Uint16 m_layer_index;
				const bool m_use_layer_index;

			public:
				inline ShaderTextureTypeData(const String &name,
					const Uint16 layer_index): m_name(name),
					m_layer_index(layer_index),
					m_use_layer_index(true)
				{
				}

				inline ShaderTextureTypeData(
					const String &name): m_name(name),
					m_layer_index(std::numeric_limits<
						Uint16>::max()),
					m_use_layer_index(false)
				{
				}

				inline ~ShaderTextureTypeData() throw()
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline Uint16 get_layer_index() const
				{
					return m_layer_index;
				}

				inline bool get_use_layer_index() const
				{
					return m_use_layer_index;
				}

		};

		const ShaderTextureTypeData shader_texture_type_datas[] =
		{
			ShaderTextureTypeData(
				String(UTF8("albedo_sampler_0")), 0),
			ShaderTextureTypeData(
				String(UTF8("albedo_sampler_1")), 1),
			ShaderTextureTypeData(
				String(UTF8("albedo_sampler_2"))),
			ShaderTextureTypeData(
				String(UTF8("albedo_sampler_3"))),
			ShaderTextureTypeData(
				String(UTF8("normal_sampler_0")), 2),
			ShaderTextureTypeData(
				String(UTF8("specular_sampler_0")), 3),
			ShaderTextureTypeData(
				String(UTF8("emission_sampler_0"))),
			ShaderTextureTypeData(String(UTF8("blend_sampler_0"))),
			ShaderTextureTypeData(String(UTF8("normal_sampler_1"))),
			ShaderTextureTypeData(
				String(UTF8("specular_sampler_1"))),
			ShaderTextureTypeData(
				String(UTF8("emission_sampler_1"))),
			ShaderTextureTypeData(String(UTF8("blend_sampler_1"))),
			ShaderTextureTypeData(String(UTF8("height_sampler"))),
			ShaderTextureTypeData(String(UTF8("dudv_sampler"))),
			ShaderTextureTypeData(
				String(UTF8("reflection_sampler"))),
			ShaderTextureTypeData(String(UTF8("shadow_sampler")))
		};

		const Uint32 shader_texture_type_datas_count =
			sizeof(shader_texture_type_datas) /
				sizeof(ShaderTextureTypeData);

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
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderTextureType")));
		}

		return shader_texture_type_datas[texture].get_name();
	}

	ShaderTextureType ShaderTextureUtil::get_shader_texture(
		const String &str)
	{
		Uint32 i;
		ShaderTextureType shader_texture_type;

		for (i = 0; i < shader_texture_type_datas_count; ++i)
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
			<< boost::errinfo_type_info_name(UTF8(
				"ShaderTextureType")));
	}

	bool ShaderTextureUtil::get_shader_texture(const String &str,
		ShaderTextureType &shader_texture)
	{
		Uint32 i;
		ShaderTextureType tmp;

		for (i = 0; i < shader_texture_type_datas_count; ++i)
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

	Uint16 ShaderTextureUtil::get_layer_index(
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
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderTextureType")));
		}

		return shader_texture_type_datas[texture].get_layer_index();
	}

	bool ShaderTextureUtil::get_use_layer_index(
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
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderTextureType")));
		}

		return shader_texture_type_datas[texture].get_use_layer_index();
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
