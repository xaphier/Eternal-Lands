/****************************************************************************
 *            effectfunctionutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectfunctionutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String effect_function_names[] =
		{
			String(UTF8("add")),
			String(UTF8("sub")),
			String(UTF8("mul")),
			String(UTF8("div")),
			String(UTF8("mad")),
			String(UTF8("radians")),
			String(UTF8("degrees")),
			String(UTF8("sin")),
			String(UTF8("cos")),
			String(UTF8("tan")),
			String(UTF8("asin")),
			String(UTF8("acos")),
			String(UTF8("atan")),
			String(UTF8("atan2")),
			String(UTF8("sinh")),
			String(UTF8("cosh")),
			String(UTF8("tanh")),
			String(UTF8("asinh")),
			String(UTF8("acosh")),
			String(UTF8("atanh")),
			String(UTF8("pow")),
			String(UTF8("exp")),
			String(UTF8("log")),
			String(UTF8("exp2")),
			String(UTF8("log2")),
			String(UTF8("sqrt")),
			String(UTF8("inversesqrt")),
			String(UTF8("abs")),
			String(UTF8("sign")),
			String(UTF8("round")),
			String(UTF8("trunc")),
			String(UTF8("floor")),
			String(UTF8("ceil")),
			String(UTF8("fract")),
			String(UTF8("mod")),
			String(UTF8("min")),
			String(UTF8("max")),
			String(UTF8("clamp")),
			String(UTF8("mix")),
			String(UTF8("step")),
			String(UTF8("smoothstep")),
			String(UTF8("reflect")),
			String(UTF8("length")),
			String(UTF8("distance")),
			String(UTF8("dot")),
			String(UTF8("normalize")),
			String(UTF8("saturate")),
			String(UTF8("refract")),
			String(UTF8("dFdx")),
			String(UTF8("dFdy"))
		};

		const Uint32 effect_function_names_count =
			sizeof(effect_function_names) / sizeof(String);

	}

	const String &EffectFunctionUtil::get_str(
		const EffectFunctionType effect_function)
	{
		if (effect_function_names_count <= effect_function)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_function_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_function))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectFunctionType")));
		}

		return effect_function_names[effect_function];
	}

	EffectFunctionType EffectFunctionUtil::get_effect_function(
		const String &str)
	{
		Uint32 i;
		EffectFunctionType effect_function;

		for (i = 0; i < effect_function_names_count; ++i)
		{
			effect_function = static_cast<EffectFunctionType>(i);

			if (str == get_str(effect_function))
			{
				return effect_function;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectFunctionType")));
	}

	bool EffectFunctionUtil::get_effect_function(const String &str,
		EffectFunctionType &effect_function) noexcept
	{
		Uint32 i;
		EffectFunctionType tmp;

		for (i = 0; i < effect_function_names_count; ++i)
		{
			tmp = static_cast<EffectFunctionType>(i);

			if (str == get_str(tmp))
			{
				effect_function = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectFunctionUtil::get_effect_function_count() noexcept
	{
		return effect_function_names_count;
	}

	OutStream& operator<<(OutStream &str, const EffectFunctionType value)
	{
		str << EffectFunctionUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, EffectFunctionType &value)
	{
		StringType string;

		str >> string;

		value = EffectFunctionUtil::get_effect_function(String(string));

		return str;
	}

}
