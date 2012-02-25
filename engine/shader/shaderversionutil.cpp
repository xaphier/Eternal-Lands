/****************************************************************************
 *            shaderversionutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shaderversionutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class ShaderVersionTypeData
		{
			private:
				const String m_name;
				const Uint16 m_number;

			public:
				inline ShaderVersionTypeData(const String &name,
					const Uint16 number): m_name(name),
					m_number(number)
				{
				}

				inline ~ShaderVersionTypeData() throw()
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline Uint16 get_number() const
				{
					return m_number;
				}

		};

		const ShaderVersionTypeData shader_version_type_datas[] =
		{
			ShaderVersionTypeData(String(UTF8("glsl_120")), 120),
			ShaderVersionTypeData(String(UTF8("glsl_130")), 130),
			ShaderVersionTypeData(String(UTF8("glsl_140")), 140),
			ShaderVersionTypeData(String(UTF8("glsl_150")), 150),
			ShaderVersionTypeData(String(UTF8("glsl_330")), 330),
			ShaderVersionTypeData(String(UTF8("glsl_400")), 400),
			ShaderVersionTypeData(String(UTF8("glsl_410")), 410),
			ShaderVersionTypeData(String(UTF8("glsl_420")), 420)
		};

		const Uint32 shader_version_type_datas_count =
			sizeof(shader_version_type_datas) /
				sizeof(ShaderVersionTypeData);

	}

	const String &ShaderVersionUtil::get_str(
		const ShaderVersionType version)
	{
		if (shader_version_type_datas_count <= version)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_version_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					version))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderVersionType")));
		}

		return shader_version_type_datas[version].get_name();
	}

	Uint16 ShaderVersionUtil::get_shader_version_number(
		const ShaderVersionType version)
	{
		if (shader_version_type_datas_count <= version)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					shader_version_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					version))
				<< boost::errinfo_type_info_name(UTF8(
					"ShaderVersionType")));
		}

		return shader_version_type_datas[version].get_number();
	}

	ShaderVersionType ShaderVersionUtil::get_shader_version(
		const String &str)
	{
		Uint32 i;
		ShaderVersionType shader_version;

		for (i = 0; i < shader_version_type_datas_count; ++i)
		{
			shader_version = static_cast<ShaderVersionType>(i);

			if (str == get_str(shader_version))
			{
				return shader_version;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"ShaderVersionType")));
	}

	bool ShaderVersionUtil::get_shader_version(const String &str,
		ShaderVersionType &shader_version)
	{
		Uint32 i;
		ShaderVersionType tmp;

		for (i = 0; i < shader_version_type_datas_count; ++i)
		{
			tmp = static_cast<ShaderVersionType>(i);

			if (str == get_str(tmp))
			{
				shader_version = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 ShaderVersionUtil::get_shader_version_count()
	{
		return shader_version_type_datas_count;
	}

	OutStream& operator<<(OutStream &str, const ShaderVersionType value)
	{
		str << ShaderVersionUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ShaderVersionType &value)
	{
		StringType string;

		str >> string;

		value = ShaderVersionUtil::get_shader_version(String(string));

		return str;
	}

}
