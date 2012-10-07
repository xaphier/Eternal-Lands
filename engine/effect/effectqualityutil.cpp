/****************************************************************************
 *            effectqualityutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectqualityutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String effect_quality_names[] =
		{
			String(UTF8("low")),
			String(UTF8("medium")),
			String(UTF8("high"))
		};

		const Uint32 effect_quality_names_count =
			sizeof(effect_quality_names) / sizeof(String);

	}

	const String &EffectQualityUtil::get_str(
		const EffectQualityType effect_quality)
	{
		if (effect_quality_names_count <= effect_quality)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_quality_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_quality))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectQualityType")));
		}

		return effect_quality_names[effect_quality];
	}

	EffectQualityType EffectQualityUtil::get_effect_quality(
		const String &str)
	{
		Uint32 i;
		EffectQualityType effect_quality;

		for (i = 0; i < effect_quality_names_count; ++i)
		{
			effect_quality = static_cast<EffectQualityType>(i);

			if (str == get_str(effect_quality))
			{
				return effect_quality;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectQualityType")));
	}

	bool EffectQualityUtil::get_effect_quality(const String &str,
		EffectQualityType &effect_quality) noexcept
	{
		Uint32 i;
		EffectQualityType tmp;

		for (i = 0; i < effect_quality_names_count; ++i)
		{
			tmp = static_cast<EffectQualityType>(i);

			if (str == get_str(tmp))
			{
				effect_quality = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectQualityUtil::get_effect_quality_count() noexcept
	{
		return effect_quality_names_count;
	}

	OutStream &operator<<(OutStream &str, const EffectQualityType value)
	{
		str << EffectQualityUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, EffectQualityType &value)
	{
		std::string string;

		str >> string;

		value = EffectQualityUtil::get_effect_quality(String(string));

		return str;
	}

}
