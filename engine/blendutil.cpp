/****************************************************************************
 *            blendutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "blendutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String blend_names[] =
		{
			String(UTF8("disabled")),
			String(UTF8("alpha_transparency_source_value")),
			String(UTF8("alpha_transparency_value")),
			String(UTF8("additive"))
		};

		const Uint32 blend_names_count = sizeof(blend_names)
			/ sizeof(String);

	}

	const String &BlendUtil::get_str(const BlendType blend)
	{
		if (blend_names_count <= blend)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(get_blend_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					blend))
				<< boost::errinfo_type_info_name(
					UTF8("BlendType")));
		}

		return blend_names[blend];
	}

	BlendType BlendUtil::get_blend(const String &str)
	{
		Uint32 i;
		BlendType blend;

		for (i = 0; i < blend_names_count; i++)
		{
			blend = static_cast<BlendType>(i);

			if (str == get_str(blend))
			{
				return blend;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8("BlendType")));
	}

	Uint32 BlendUtil::get_blend_count() noexcept
	{
		return blend_names_count;
	}

	OutStream& operator<<(OutStream &str, const BlendType value)
	{
		str << BlendUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, BlendType &value)
	{
		std::string string;

		str >> string;

		value = BlendUtil::get_blend(String(string));

		return str;
	}

}
