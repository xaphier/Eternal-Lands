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

		class EffectTextureTypeData
		{
			private:
				const String m_name;
				const String m_description;

			public:
				inline EffectTextureTypeData(
					const String &name,
					const String &description):
						m_name(name),
						m_description(description)
				{
				}

				inline ~EffectTextureTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline const String &get_description() const
					noexcept
				{
					return m_description;
				}

		};

		const EffectTextureTypeData effect_texture_datas[] =
		{
			EffectTextureTypeData(String(UTF8("default")),
				String(UTF8(""))),
			EffectTextureTypeData(String(UTF8("albedo")),
				String(UTF8(""))),
			EffectTextureTypeData(String(UTF8("normal")),
				String(UTF8(""))),
			EffectTextureTypeData(String(UTF8("parallax")),
				String(UTF8("")))
		};

		const Uint32 effect_texture_datas_count =
			sizeof(effect_texture_datas) /
			sizeof(EffectTextureTypeData);

	}

	const String &EffectTextureUtil::get_str(
		const EffectTextureType effect_texture)
	{
		if (effect_texture_datas_count <= effect_texture)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_texture_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_texture))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectTextureType")));
		}

		return effect_texture_datas[effect_texture].get_name();
	}

	const String &EffectTextureUtil::get_description(
		const EffectTextureType effect_texture)
	{
		if (effect_texture_datas_count <= effect_texture)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_texture_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_texture))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectTextureType")));
		}

		return effect_texture_datas[effect_texture].get_description();
	}

	EffectTextureType EffectTextureUtil::get_effect_texture(
		const String &str)
	{
		Uint32 i;
		EffectTextureType effect_texture;

		for (i = 0; i < effect_texture_datas_count; ++i)
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

		for (i = 0; i < effect_texture_datas_count; ++i)
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
		return effect_texture_datas_count;
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
