/****************************************************************************
 *            samplerparameterutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "samplerparameterutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String sampler_parameter_names[] =
		{
			String(UTF8("effect_sampler_0")),
			String(UTF8("effect_sampler_1")),
			String(UTF8("effect_sampler_2")),
			String(UTF8("effect_sampler_3")),
			String(UTF8("effect_sampler_4")),
			String(UTF8("effect_sampler_5")),
			String(UTF8("effect_sampler_6")),
			String(UTF8("effect_sampler_7")),
			String(UTF8("effect_sampler_8")),
			String(UTF8("effect_sampler_9")),
			String(UTF8("effect_sampler_10")),
			String(UTF8("effect_sampler_11")),
			String(UTF8("effect_sampler_12")),
			String(UTF8("effect_sampler_13")),
			String(UTF8("effect_sampler_14")),
			String(UTF8("effect_sampler_15"))
		};

		const Uint32 sampler_parameter_names_count =
			sizeof(sampler_parameter_names) / sizeof(String);

	}

	const String &SamplerParameterUtil::get_str(
		const SamplerParameterType sampler_parameter)
	{
		if (sampler_parameter_names_count <= sampler_parameter)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					sampler_parameter_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					sampler_parameter))
				<< boost::errinfo_type_info_name(UTF8(
					"SamplerParameterType")));
		}

		return sampler_parameter_names[sampler_parameter];
	}

	SamplerParameterType SamplerParameterUtil::get_sampler_parameter(
		const String &str)
	{
		Uint32 i;
		SamplerParameterType sampler_parameter;

		for (i = 0; i < sampler_parameter_names_count; ++i)
		{
			sampler_parameter =
				static_cast<SamplerParameterType>(i);

			if (str == get_str(sampler_parameter))
			{
				return sampler_parameter;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"SamplerParameterType")));
	}

	bool SamplerParameterUtil::get_sampler_parameter(const String &str,
		SamplerParameterType &sampler_parameter) noexcept
	{
		Uint32 i;
		SamplerParameterType tmp;

		for (i = 0; i < sampler_parameter_names_count; ++i)
		{
			tmp = static_cast<SamplerParameterType>(i);

			if (str == get_str(tmp))
			{
				sampler_parameter = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 SamplerParameterUtil::get_sampler_parameter_count() noexcept
	{
		return sampler_parameter_names_count;
	}

	OutStream &operator<<(OutStream &str, const SamplerParameterType value)
	{
		str << SamplerParameterUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, SamplerParameterType &value)
	{
		std::string string;

		str >> string;

		value = SamplerParameterUtil::get_sampler_parameter(
			String(string));

		return str;
	}

}
