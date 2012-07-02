/****************************************************************************
 *            effectnodeutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectnodeutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String effect_node_names[] =
		{
			String(UTF8("add")),
			String(UTF8("sub")),
			String(UTF8("mul")),
			String(UTF8("div")),
			String(UTF8("mad")),
			String(UTF8("swizzle")),
			String(UTF8("merge")),
			String(UTF8("split")),
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
			String(UTF8("color")),
			String(UTF8("constant")),
			String(UTF8("paramter")),
			String(UTF8("texture")),
			String(UTF8("output")),
			String(UTF8("inout"))
		};

		const Uint32 effect_node_names_count =
			sizeof(effect_node_names) / sizeof(String);

	}

	const String &EffectNodeUtil::get_str(const EffectNodeType effect_node)
	{
		if (effect_node_names_count <= effect_node)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_node_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_node))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectNodeType")));
		}

		return effect_node_names[effect_node];
	}

	EffectNodeType EffectNodeUtil::get_effect_node(const String &str)
	{
		Uint32 i;
		EffectNodeType effect_node;

		for (i = 0; i < effect_node_names_count; ++i)
		{
			effect_node = static_cast<EffectNodeType>(i);

			if (str == get_str(effect_node))
			{
				return effect_node;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectNodeType")));
	}

	bool EffectNodeUtil::get_effect_node(const String &str,
		EffectNodeType &effect_node) noexcept
	{
		Uint32 i;
		EffectNodeType tmp;

		for (i = 0; i < effect_node_names_count; ++i)
		{
			tmp = static_cast<EffectNodeType>(i);

			if (str == get_str(tmp))
			{
				effect_node = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectNodeUtil::get_effect_node_count() noexcept
	{
		return effect_node_names_count;
	}

	OutStream& operator<<(OutStream &str, const EffectNodeType value)
	{
		str << EffectNodeUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, EffectNodeType &value)
	{
		StringType string;

		str >> string;

		value = EffectNodeUtil::get_effect_node(String(string));

		return str;
	}

}
