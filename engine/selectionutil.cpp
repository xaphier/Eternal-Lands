/****************************************************************************
 *            selectionutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "selectionutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String selection_names[] =
		{
			String(UTF8("none")),
			String(UTF8("select")),
			String(UTF8("harvest")),
			String(UTF8("pick")),
			String(UTF8("enter")),
			String(UTF8("pick")),
			String(UTF8("npc")),
			String(UTF8("player")),
			String(UTF8("animal"))
		};

		const Uint32 selection_names_count =
			sizeof(selection_names) / sizeof(String);

	}

	const String &SelectionUtil::get_str(const SelectionType selection)
	{
		if (selection_names_count <= selection)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					selection_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					selection))
				<< boost::errinfo_type_info_name(UTF8(
					"SelectionType")));
		}

		return selection_names[selection];
	}

	SelectionType SelectionUtil::get_selection_type(const String &str)
	{
		Uint32 i;
		SelectionType selection_type;

		for (i = 0; i < selection_names_count; ++i)
		{
			selection_type = static_cast<SelectionType>(i);

			if (str == get_str(selection_type))
			{
				return selection_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"SelectionType")));
	}

	OutStream& operator<<(OutStream &str, const SelectionType value)
	{
		str << SelectionUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, SelectionType &value)
	{
		StringType string;

		str >> string;

		value = SelectionUtil::get_selection_type(String(string));

		return str;
	}

}
