/****************************************************************************
 *            effectdescriptionutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectdescriptionutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{


		class EffectDescriptionTypeData
		{
			private:
				const String m_name;
				const Uint16 m_render_targets_count;

			public:
				inline EffectDescriptionTypeData(
					const String &name,
					const Uint16 render_targets_count):
					m_name(name),
					m_render_targets_count(
						render_targets_count)
				{
				}

				inline ~EffectDescriptionTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline const Uint16 get_render_targets_count()
					const noexcept
				{
					return m_render_targets_count;
				}

		};

		const EffectDescriptionTypeData effect_description_datas[] =
		{
			EffectDescriptionTypeData(String(UTF8("default")), 1),
			EffectDescriptionTypeData(
				String(UTF8("screen_quad_1")), 1),
			EffectDescriptionTypeData(
				String(UTF8("screen_quad_2")), 2),
			EffectDescriptionTypeData(
				String(UTF8("screen_quad_3")), 3),
			EffectDescriptionTypeData(
				String(UTF8("screen_quad_4")), 4)
		};

		const Uint32 effect_description_datas_count =
			sizeof(effect_description_datas) /
			sizeof(EffectDescriptionTypeData);

	}

	const String &EffectDescriptionUtil::get_str(
		const EffectDescriptionType effect_description)
	{
		if (effect_description_datas_count <= effect_description)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					get_effect_description_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_description))
				<< boost::errinfo_type_info_name(
					UTF8("EffectDescriptionType")));
		}

		return effect_description_datas[effect_description].get_name();
	}

	Uint16 EffectDescriptionUtil::get_render_targets_count(
		const EffectDescriptionType effect_description)
	{
		if (effect_description_datas_count <= effect_description)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					get_effect_description_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_description))
				<< boost::errinfo_type_info_name(
					UTF8("EffectDescriptionType")));
		}

		return effect_description_datas[
			effect_description].get_render_targets_count();
	}

	EffectDescriptionType EffectDescriptionUtil::get_effect_description(
		const String &str)
	{
		Uint32 i;
		EffectDescriptionType effect_description;

		for (i = 0; i < effect_description_datas_count; i++)
		{
			effect_description =
				static_cast<EffectDescriptionType>(i);

			if (str == get_str(effect_description))
			{
				return effect_description;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectDescriptionType")));
	}

	Uint32 EffectDescriptionUtil::get_effect_description_count() noexcept
	{
		return effect_description_datas_count;
	}

	OutStream &operator<<(OutStream &str, const EffectDescriptionType value)
	{
		str << EffectDescriptionUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, EffectDescriptionType &value)
	{
		std::string string;

		str >> string;

		value = EffectDescriptionUtil::get_effect_description(
			String(string));

		return str;
	}

}
