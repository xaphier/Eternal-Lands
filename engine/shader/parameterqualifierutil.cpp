/****************************************************************************
 *            parameterqualifierutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "parameterqualifierutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String parameter_qualifier_names[] =
		{
			String(UTF8("in")),
			String(UTF8("out")),
			String(UTF8("inout"))
		};

		const Uint32 parameter_qualifier_names_count =
			sizeof(parameter_qualifier_names) /
			sizeof(String);

	}

	const String &ParameterQualifierUtil::get_str(
		const ParameterQualifierType parameter)
	{
		if (parameter_qualifier_names_count <= parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_qualifier_names_count
						- 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterQualifierType")));
		}

		return parameter_qualifier_names[parameter];
	}

	ParameterQualifierType ParameterQualifierUtil::get_parameter_qualifier(
		const String &str)
	{
		Uint32 i;
		ParameterQualifierType parameter_qualifier;

		for (i = 0; i < parameter_qualifier_names_count; ++i)
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
			<< boost::errinfo_type_info_name(UTF8(
				"ParameterQualifierType")));
	}

	bool ParameterQualifierUtil::get_parameter_qualifier(const String &str,
		ParameterQualifierType &parameter_qualifier) noexcept
	{
		Uint32 i;
		ParameterQualifierType tmp;

		for (i = 0; i < parameter_qualifier_names_count; ++i)
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

	Uint32 ParameterQualifierUtil::get_parameter_qualifier_count() noexcept
	{
		return parameter_qualifier_names_count;
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
