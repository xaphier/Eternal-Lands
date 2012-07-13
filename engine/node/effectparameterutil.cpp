/****************************************************************************
 *            effectparameterutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectparameterutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String effect_parameter_names[] =
		{
			String(UTF8("position")),
			String(UTF8("normal")),
			String(UTF8("tangent")),
			String(UTF8("view_direction")),
			String(UTF8("uv")),
			String(UTF8("fragment_coordinate")),
			String(UTF8("time")),
			String(UTF8("camera"))
		};

		const Uint32 effect_parameter_names_count =
			sizeof(effect_parameter_names) / sizeof(String);

	}

	const String &EffectParameterUtil::get_str(
		const EffectParameterType effect_parameter)
	{
		if (effect_parameter_names_count <= effect_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_parameter_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectParameterType")));
		}

		return effect_parameter_names[effect_parameter];
	}

	EffectParameterType EffectParameterUtil::get_effect_parameter(
		const String &str)
	{
		Uint32 i;
		EffectParameterType effect_parameter;

		for (i = 0; i < effect_parameter_names_count; ++i)
		{
			effect_parameter = static_cast<EffectParameterType>(i);

			if (str == get_str(effect_parameter))
			{
				return effect_parameter;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectParameterType")));
	}

	bool EffectParameterUtil::get_effect_parameter(const String &str,
		EffectParameterType &effect_parameter) noexcept
	{
		Uint32 i;
		EffectParameterType tmp;

		for (i = 0; i < effect_parameter_names_count; ++i)
		{
			tmp = static_cast<EffectParameterType>(i);

			if (str == get_str(tmp))
			{
				effect_parameter = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectParameterUtil::get_effect_parameter_count() noexcept
	{
		return effect_parameter_names_count;
	}

	OutStream& operator<<(OutStream &str, const EffectParameterType value)
	{
		str << EffectParameterUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, EffectParameterType &value)
	{
		StringType string;

		str >> string;

		value = EffectParameterUtil::get_effect_parameter(String(string));

		return str;
	}

}
