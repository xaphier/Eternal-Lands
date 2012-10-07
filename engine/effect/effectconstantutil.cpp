/****************************************************************************
 *            effectconstantutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectconstantutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class EffectConstantTypeData
		{
			private:
				const String m_name;
				const Uint16 m_count;

			public:
				inline EffectConstantTypeData(
					const String &name,
					const Uint16 count): m_name(name),
						m_count(count)
				{
				}

				inline ~EffectConstantTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline Uint16 get_count() const noexcept
				{
					return m_count;
				}

		};

		const EffectConstantTypeData effect_constant_datas[] =
		{
			EffectConstantTypeData(String(UTF8("color_rgb")), 3),
			EffectConstantTypeData(String(UTF8("direction_xy")),
				2),
			EffectConstantTypeData(String(UTF8("float")), 1),
			EffectConstantTypeData(String(UTF8("vec2")), 2),
			EffectConstantTypeData(String(UTF8("vec3")), 3),
			EffectConstantTypeData(String(UTF8("vec4")), 4)
		};

		const Uint32 effect_constant_datas_count =
			sizeof(effect_constant_datas) /
			sizeof(EffectConstantTypeData);

	}

	const String &EffectConstantUtil::get_str(
		const EffectConstantType effect_constant)
	{
		if (effect_constant_datas_count <= effect_constant)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_constant_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_constant))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectConstantType")));
		}

		return effect_constant_datas[effect_constant].get_name();
	}

	Uint16 EffectConstantUtil::get_count(
		const EffectConstantType effect_constant)
	{
		if (effect_constant_datas_count <= effect_constant)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_constant_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_constant))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectConstantType")));
		}

		return effect_constant_datas[effect_constant].get_count();
	}

	EffectConstantType EffectConstantUtil::get_effect_constant(const String &str)
	{
		Uint32 i;
		EffectConstantType effect_constant;

		for (i = 0; i < effect_constant_datas_count; ++i)
		{
			effect_constant = static_cast<EffectConstantType>(i);

			if (str == get_str(effect_constant))
			{
				return effect_constant;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectConstantType")));
	}

	bool EffectConstantUtil::get_effect_constant(const String &str,
		EffectConstantType &effect_constant) noexcept
	{
		Uint32 i;
		EffectConstantType tmp;

		for (i = 0; i < effect_constant_datas_count; ++i)
		{
			tmp = static_cast<EffectConstantType>(i);

			if (str == get_str(tmp))
			{
				effect_constant = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectConstantUtil::get_effect_constant_count() noexcept
	{
		return effect_constant_datas_count;
	}

	OutStream &operator<<(OutStream &str, const EffectConstantType value)
	{
		str << EffectConstantUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, EffectConstantType &value)
	{
		std::string string;

		str >> string;

		value = EffectConstantUtil::get_effect_constant(String(string));

		return str;
	}

}
