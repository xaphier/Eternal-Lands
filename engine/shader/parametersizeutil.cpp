/****************************************************************************
 *            parametersizeutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "parametersizeutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String parameter_size_type_datas[] =
		{
			String(UTF8("one")),
			String(UTF8("light_count")),
			String(UTF8("bone_count")),
			String(UTF8("shadow_map_count")),
			String(UTF8("layer_count"))
		};

		const Uint32 parameter_size_type_datas_count =
			sizeof(parameter_size_type_datas) / sizeof(String);

	}

	const String &ParameterSizeUtil::get_str(
		const ParameterSizeType parameter_size)
	{
		if (parameter_size_type_datas_count <= parameter_size)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_size_type_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter_size))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterSizeType")));
		}

		return parameter_size_type_datas[parameter_size];
	}

	ParameterSizeType ParameterSizeUtil::get_parameter_size(
		const String &str)
	{
		Uint32 i;
		ParameterSizeType parameter_size;

		for (i = 0; i < parameter_size_type_datas_count; ++i)
		{
			parameter_size = static_cast<ParameterSizeType>(i);

			if (str == get_str(parameter_size))
			{
				return parameter_size;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"ParameterSizeType")));
	}

	bool ParameterSizeUtil::get_parameter_size(const String &str,
		ParameterSizeType &parameter_size)
	{
		Uint32 i;
		ParameterSizeType tmp;

		for (i = 0; i < parameter_size_type_datas_count; ++i)
		{
			tmp = static_cast<ParameterSizeType>(i);

			if (str == get_str(tmp))
			{
				parameter_size = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 ParameterSizeUtil::get_parameter_size_count()
	{
		return parameter_size_type_datas_count;
	}

	OutStream& operator<<(OutStream &str, const ParameterSizeType value)
	{
		str << ParameterSizeUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ParameterSizeType &value)
	{
		StringType string;

		str >> string;

		value = ParameterSizeUtil::get_parameter_size(String(string));

		return str;
	}

}
