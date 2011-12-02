/****************************************************************************
 *            parameterqualifierutil.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "parameterqualifierutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String parameter_qualifier_type_datas[] =
		{
			String(L"in"),
			String(L"out"),
			String(L"inout")
		};

		const Uint32 parameter_qualifier_type_datas_count =
			sizeof(parameter_qualifier_type_datas) /
			sizeof(String);

	}

	const String &ParameterQualifierUtil::get_str(
		const ParameterQualifierType parameter)
	{
		if (parameter_qualifier_type_datas_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_qualifier_type_datas_count
						- 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(
					"ParameterQualifierType"));
		}

		return parameter_qualifier_type_datas[parameter];
	}

	ParameterQualifierType ParameterQualifierUtil::get_parameter_qualifier(
		const String &str)
	{
		Uint32 i;
		ParameterQualifierType parameter_qualifier;

		for (i = 0; i < parameter_qualifier_type_datas_count; i++)
		{
			parameter_qualifier =
				static_cast<ParameterQualifierType>(i);

			if (str == get_str(parameter_qualifier))
			{
				return parameter_qualifier;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				"ParameterQualifierType"));
	}

	bool ParameterQualifierUtil::get_parameter_qualifier(const String &str,
		ParameterQualifierType &parameter_qualifier)
	{
		Uint32 i;
		ParameterQualifierType tmp;

		for (i = 0; i < parameter_qualifier_type_datas_count; i++)
		{
			tmp = static_cast<ParameterQualifierType>(i);

			if (str == get_str(tmp))
			{
				parameter_qualifier = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 ParameterQualifierUtil::get_parameter_qualifier_count()
	{
		return parameter_qualifier_type_datas_count;
	}

	OutStream& operator<<(OutStream &str,
		const ParameterQualifierType value)
	{
		str << ParameterQualifierUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ParameterQualifierType &value)
	{
		StringType string;

		str >> string;

		value = ParameterQualifierUtil::get_parameter_qualifier(
			String(string));

		return str;
	}

}
