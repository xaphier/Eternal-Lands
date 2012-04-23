/****************************************************************************
 *            effectprogramutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectprogramutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String effect_program_names[] =
		{
			String(UTF8("default")),
			String(UTF8("shadow")),
			String(UTF8("depth"))
		};

		const Uint32 effect_program_names_count =
			sizeof(effect_program_names) / sizeof(String);

	}

	const String &EffectProgramUtil::get_str(
		const EffectProgramType effect_program)
	{
		if (effect_program_names_count <= effect_program)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					get_effect_program_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_program))
				<< boost::errinfo_type_info_name(
					UTF8("EffectProgramType")));
		}

		return effect_program_names[effect_program];
	}

	EffectProgramType EffectProgramUtil::get_effect_program(
		const String &str)
	{
		Uint32 i;
		EffectProgramType effect_program;

		for (i = 0; i < effect_program_names_count; i++)
		{
			effect_program = static_cast<EffectProgramType>(i);

			if (str == get_str(effect_program))
			{
				return effect_program;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectProgramType")));
	}

	Uint32 EffectProgramUtil::get_effect_program_count()
	{
		return effect_program_names_count;
	}

	OutStream& operator<<(OutStream &str, const EffectProgramType value)
	{
		str << EffectProgramUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, EffectProgramType &value)
	{
		StringType string;

		str >> string;

		value = EffectProgramUtil::get_effect_program(String(string));

		return str;
	}

}
