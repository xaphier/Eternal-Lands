/****************************************************************************
 *            effectmainutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectmainutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String effect_main_names[] =
		{
			String(UTF8("source")),
			String(UTF8("nodes")),
			String(UTF8("terrain"))
		};

		const Uint32 effect_main_names_count =
			sizeof(effect_main_names) /
			sizeof(String);

	}

	const String &EffectMainUtil::get_str(
		const EffectMainType effect_main)
	{
		if (effect_main_names_count <= effect_main)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					get_effect_main_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_main))
				<< boost::errinfo_type_info_name(
					UTF8("EffectMainType")));
		}

		return effect_main_names[effect_main];
	}

	EffectMainType EffectMainUtil::get_effect_main(
		const String &str)
	{
		Uint32 i;
		EffectMainType effect_main;

		for (i = 0; i < effect_main_names_count; i++)
		{
			effect_main =
				static_cast<EffectMainType>(i);

			if (str == get_str(effect_main))
			{
				return effect_main;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectMainType")));
	}

	Uint32 EffectMainUtil::get_effect_main_count() noexcept
	{
		return effect_main_names_count;
	}

	OutStream &operator<<(OutStream &str, const EffectMainType value)
	{
		str << EffectMainUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, EffectMainType &value)
	{
		std::string string;

		str >> string;

		value = EffectMainUtil::get_effect_main(
			String(string));

		return str;
	}

}
