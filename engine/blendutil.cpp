/****************************************************************************
 *            blendutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "blendutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String blend_type_names[] =
		{
			String(UTF8("disabled")),
			String(UTF8("alpha_transparency_source_value")),
			String(UTF8("alpha_transparency_value")),
			String(UTF8("additive"))
		};

		const Uint32 blend_type_names_count = sizeof(blend_type_names)
			/ sizeof(String);

	}

	const String &BlendUtil::get_str(const BlendType blend_type)
	{
		if (get_blend_type_count() <= blend_type)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(get_blend_type_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					blend_type))
				<< boost::errinfo_type_info_name(UTF8(
					"BlendType")));
		}

		return blend_type_names[blend_type];
	}

	BlendType BlendUtil::get_blend(const String &str)
	{
		Uint32 i;
		BlendType blend_type;

		for (i = 0; i < get_blend_type_count(); i++)
		{
			blend_type = static_cast<BlendType>(i);

			if (str == get_str(blend_type))
			{
				return blend_type;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8("BlendType")));
	}

	Uint32 BlendUtil::get_blend_type_count()
	{
		return blend_type_names_count;
	}

	std::ostream& operator<< (std::ostream &str, const BlendType value)
	{
		str << BlendUtil::get_str(value);

		return str;
	}

}
