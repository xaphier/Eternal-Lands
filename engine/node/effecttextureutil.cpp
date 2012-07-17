/****************************************************************************
 *            effecttextureutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effecttextureutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String effect_texture_names[] =
		{
			String(UTF8("default")),
			String(UTF8("albedo")),
			String(UTF8("normal")),
			String(UTF8("parallax"))
		};

		const Uint32 effect_texture_names_count =
			sizeof(effect_texture_names) / sizeof(String);

	}

	const String &EffectTextureUtil::get_str(
		const EffectTextureType effect_texture)
	{
		if (effect_texture_names_count <= effect_texture)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_texture_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_texture))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectTextureType")));
		}

		return effect_texture_names[effect_texture];
	}

	EffectTextureType EffectTextureUtil::get_effect_texture(
		const String &str)
	{
		Uint32 i;
		EffectTextureType effect_texture;

		for (i = 0; i < effect_texture_names_count; ++i)
		{
			effect_texture = static_cast<EffectTextureType>(i);

			if (str == get_str(effect_texture))
			{
				return effect_texture;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectTextureType")));
	}

	bool EffectTextureUtil::get_effect_texture(const String &str,
		EffectTextureType &effect_texture) noexcept
	{
		Uint32 i;
		EffectTextureType tmp;

		for (i = 0; i < effect_texture_names_count; ++i)
		{
			tmp = static_cast<EffectTextureType>(i);

			if (str == get_str(tmp))
			{
				effect_texture = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectTextureUtil::get_effect_texture_count() noexcept
	{
		return effect_texture_names_count;
	}

	OutStream& operator<<(OutStream &str, const EffectTextureType value)
	{
		str << EffectTextureUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, EffectTextureType &value)
	{
		StringType string;

		str >> string;

		value = EffectTextureUtil::get_effect_texture(String(string));

		return str;
	}

}
