/****************************************************************************
 *            commonparameterutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "commonparameterutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class CommonParameterTypeData
		{
			private:
				const String m_name;
				const ParameterType m_type;
				const ParameterSizeType m_size;
				const Uint16 m_scale;

			public:
				inline CommonParameterTypeData(
					const String &name,
					const ParameterType type,
					const ParameterSizeType size = pst_one,
					const Uint16 scale = 1): m_name(name),
						m_type(type), m_size(size),
						m_scale(scale)
				{
				}

				inline ~CommonParameterTypeData() throw()
				{
				}

				inline const String &get_name() const
				{
					return m_name;
				}

				inline ParameterType get_type() const
				{
					return m_type;
				}

				inline ParameterSizeType get_size() const
				{
					return m_size;
				}

				inline Uint16 get_scale() const
				{
					return m_scale;
				}

		};

		const CommonParameterTypeData common_parameter_type_datas[] =
		{
			CommonParameterTypeData(String(L"diffuse_color"),
				pt_vec3),
			CommonParameterTypeData(String(L"specular_color"),
				pt_vec3),
			CommonParameterTypeData(String(L"shadow"), pt_float),
			CommonParameterTypeData(String(L"light_color"),
				pt_vec4),
			CommonParameterTypeData(String(L"light_position"),
				pt_vec4),
			CommonParameterTypeData(String(L"lighting_normal"),
				pt_vec3),
			CommonParameterTypeData(String(L"world_position"),
				pt_vec3),
			CommonParameterTypeData(String(L"world_normal"),
				pt_vec3),
			CommonParameterTypeData(String(L"world_tangent"),
				pt_vec3),
			CommonParameterTypeData(String(L"world_view_direction"),
				pt_vec4),
			CommonParameterTypeData(String(L"world_uv"), pt_vec2),
			CommonParameterTypeData(String(L"view_position"),
				pt_vec3),
			CommonParameterTypeData(String(L"diffuse"), pt_vec4),
			CommonParameterTypeData(String(L"specular"), pt_float),
			CommonParameterTypeData(String(L"fragment_normal"),
				pt_vec3),
			CommonParameterTypeData(String(L"fragment_uv"),
				pt_vec2),
			CommonParameterTypeData(String(L"fog"), pt_float),
			CommonParameterTypeData(String(L"shadow_uv"), pt_vec4,
				pst_shadow_map_count),
			CommonParameterTypeData(String(L"shadow_map_data"),
				pt_vec3)
		};

		const Uint32 common_parameter_type_datas_count =
			sizeof(common_parameter_type_datas) /
			sizeof(CommonParameterTypeData);

	}

	const String &CommonParameterUtil::get_str(
		const CommonParameterType common_parameter)
	{
		if (common_parameter_type_datas_count <= common_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					common_parameter_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					common_parameter))
				<< boost::errinfo_type_info_name(
					"CommonParameterType"));
		}

		return common_parameter_type_datas[common_parameter].get_name();
	}

	ParameterType CommonParameterUtil::get_type(
		const CommonParameterType common_parameter)
	{
		if (common_parameter_type_datas_count <= common_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					common_parameter_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					common_parameter))
				<< boost::errinfo_type_info_name(
					"CommonParameterType"));
		}

		return common_parameter_type_datas[common_parameter].get_type();
	}

	ParameterSizeType CommonParameterUtil::get_size(
		const CommonParameterType common_parameter)
	{
		if (common_parameter_type_datas_count <= common_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					common_parameter_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					common_parameter))
				<< boost::errinfo_type_info_name(
					"CommonParameterType"));
		}

		return common_parameter_type_datas[common_parameter].get_size();
	}

	Uint16 CommonParameterUtil::get_scale(
		const CommonParameterType common_parameter)
	{
		if (common_parameter_type_datas_count <= common_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					common_parameter_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					common_parameter))
				<< boost::errinfo_type_info_name(
					"CommonParameterType"));
		}

		return common_parameter_type_datas[
			common_parameter].get_scale();
	}

	CommonParameterType CommonParameterUtil::get_common_parameter(
		const String &str)
	{
		Uint32 i;
		CommonParameterType common_parameter;

		for (i = 0; i < common_parameter_type_datas_count; i++)
		{
			common_parameter = static_cast<CommonParameterType>(i);

			if (str == get_str(common_parameter))
			{
				return common_parameter;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name("CommonParameterType"));
	}

	bool CommonParameterUtil::get_common_parameter(const String &str,
		CommonParameterType &common_parameter)
	{
		Uint32 i;
		CommonParameterType tmp;

		for (i = 0; i < common_parameter_type_datas_count; i++)
		{
			tmp = static_cast<CommonParameterType>(i);

			if (str == get_str(tmp))
			{
				common_parameter = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 CommonParameterUtil::get_common_parameter_count()
	{
		return common_parameter_type_datas_count;
	}

	OutStream& operator<<(OutStream &str, const CommonParameterType value)
	{
		str << CommonParameterUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, CommonParameterType &value)
	{
		StringType string;

		str >> string;

		value = CommonParameterUtil::get_common_parameter(String(
			string));

		return str;
	}

}
