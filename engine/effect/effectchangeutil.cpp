/****************************************************************************
 *            effectchangeutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectchangeutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String effect_change_names[] =
		{
			String(UTF8("undefined")),
			String(UTF8("constant")),
			String(UTF8("vertex")),
			String(UTF8("fragment"))
		};

		const Uint32 effect_change_names_count =
			sizeof(effect_change_names) / sizeof(String);

	}

	const String &EffectChangeUtil::get_str(
		const EffectChangeType effect_change)
	{
		if (effect_change_names_count <= effect_change)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_change_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_change))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectChangeType")));
		}

		return effect_change_names[effect_change];
	}

	EffectChangeType EffectChangeUtil::get_effect_change(
		const String &str)
	{
		Uint32 i;
		EffectChangeType effect_change;

		for (i = 0; i < effect_change_names_count; ++i)
		{
			effect_change = static_cast<EffectChangeType>(i);

			if (str == get_str(effect_change))
			{
				return effect_change;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectChangeType")));
	}

	bool EffectChangeUtil::get_effect_change(const String &str,
		EffectChangeType &effect_change) noexcept
	{
		Uint32 i;
		EffectChangeType tmp;

		for (i = 0; i < effect_change_names_count; ++i)
		{
			tmp = static_cast<EffectChangeType>(i);

			if (str == get_str(tmp))
			{
				effect_change = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectChangeUtil::get_effect_change_count() noexcept
	{
		return effect_change_names_count;
	}

	OutStream& operator<<(OutStream &str, const EffectChangeType value)
	{
		str << EffectChangeUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, EffectChangeType &value)
	{
		std::string string;

		str >> string;

		value = EffectChangeUtil::get_effect_change(String(string));

		return str;
	}

}
