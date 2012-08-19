/****************************************************************************
 *            effectsamplerutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectsamplerutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String effect_sampler_names[] =
		{
			String(UTF8("sampler_1d")),
			String(UTF8("sampler_2d")),
			String(UTF8("sampler_3d")),
			String(UTF8("sampler_cube_map")),
			String(UTF8("sampler_1d_array")),
			String(UTF8("sampler_2d_array")),
			String(UTF8("sampler_rectangle")),
			String(UTF8("sampler_1d_project")),
			String(UTF8("sampler_2d_project")),
			String(UTF8("sampler_3d_project")),
			String(UTF8("sampler_rectangle_project"))
		};

		const Uint32 effect_sampler_names_count =
			sizeof(effect_sampler_names) / sizeof(String);

	}

	const String &EffectSamplerUtil::get_str(
		const EffectSamplerType effect_sampler)
	{
		if (effect_sampler_names_count <= effect_sampler)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_sampler_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_sampler))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectSamplerType")));
		}

		return effect_sampler_names[effect_sampler];
	}

	EffectSamplerType EffectSamplerUtil::get_effect_sampler(
		const String &str)
	{
		Uint32 i;
		EffectSamplerType effect_sampler;

		for (i = 0; i < effect_sampler_names_count; ++i)
		{
			effect_sampler = static_cast<EffectSamplerType>(i);

			if (str == get_str(effect_sampler))
			{
				return effect_sampler;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectSamplerType")));
	}

	bool EffectSamplerUtil::get_effect_sampler(const String &str,
		EffectSamplerType &effect_sampler) noexcept
	{
		Uint32 i;
		EffectSamplerType tmp;

		for (i = 0; i < effect_sampler_names_count; ++i)
		{
			tmp = static_cast<EffectSamplerType>(i);

			if (str == get_str(tmp))
			{
				effect_sampler = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectSamplerUtil::get_effect_sampler_count() noexcept
	{
		return effect_sampler_names_count;
	}

	OutStream& operator<<(OutStream &str, const EffectSamplerType value)
	{
		str << EffectSamplerUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, EffectSamplerType &value)
	{
		StringType string;

		str >> string;

		value = EffectSamplerUtil::get_effect_sampler(String(string));

		return str;
	}

}
