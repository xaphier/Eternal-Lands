/****************************************************************************
 *            effectnodeportutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "effectnodeportutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class EffectNodePortTypeData
		{
			private:
				const String m_name;
				const String m_description;
				const Uint16 m_count;
				const bool m_convertable;

			public:
				inline EffectNodePortTypeData(
					const String &name,
					const Uint16 count,
					const bool convertable): m_name(name),
						m_description(name),
						m_count(count),
						m_convertable(convertable)
				{
				}

				inline EffectNodePortTypeData(
					const String &name,
					const String &description,
					const Uint16 count,
					const bool convertable): m_name(name),
						m_description(description),
						m_count(count),
						m_convertable(convertable)
				{
				}

				inline ~EffectNodePortTypeData() noexcept
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

				inline Uint16 get_count() const noexcept
				{
					return m_count;
				}

				inline bool get_convertable() const noexcept
				{
					return m_convertable;
				}

		};

		const EffectNodePortTypeData effect_node_port_datas[] =
		{
			EffectNodePortTypeData(String(UTF8("r")), 1, true),
			EffectNodePortTypeData(String(UTF8("g")), 1, true),
			EffectNodePortTypeData(String(UTF8("b")), 1, true),
			EffectNodePortTypeData(String(UTF8("a")), 1, true),
			EffectNodePortTypeData(String(UTF8("rg")), 2, false),
			EffectNodePortTypeData(String(UTF8("rgb")), 3, false),
			EffectNodePortTypeData(String(UTF8("xyz")), 3, false),
			EffectNodePortTypeData(String(UTF8("rgba")), 4, false),
			EffectNodePortTypeData(String(UTF8("single")),
				String(), 1, true),
			EffectNodePortTypeData(String(UTF8("general")),
				String(), 0, true)
		};

		const Uint32 effect_node_port_datas_count =
			sizeof(effect_node_port_datas) /
			sizeof(EffectNodePortTypeData);

	}

	const String &EffectNodePortUtil::get_str(
		const EffectNodePortType effect_node_port)
	{
		if (effect_node_port_datas_count <= effect_node_port)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_node_port_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_node_port))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectNodePortType")));
		}

		return effect_node_port_datas[effect_node_port].get_name();
	}

	const String &EffectNodePortUtil::get_description(
		const EffectNodePortType effect_node_port)
	{
		if (effect_node_port_datas_count <= effect_node_port)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_node_port_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_node_port))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectNodePortType")));
		}

		return effect_node_port_datas[
			effect_node_port].get_description();
	}

	Uint16 EffectNodePortUtil::get_count(
		const EffectNodePortType effect_node_port)
	{
		if (effect_node_port_datas_count <= effect_node_port)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_node_port_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_node_port))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectNodePortType")));
		}

		return effect_node_port_datas[effect_node_port].get_count();
	}

	bool EffectNodePortUtil::get_convertable(
		const EffectNodePortType effect_node_port)
	{
		if (effect_node_port_datas_count <= effect_node_port)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					effect_node_port_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					effect_node_port))
				<< boost::errinfo_type_info_name(UTF8(
					"EffectNodePortType")));
		}

		return effect_node_port_datas[
			effect_node_port].get_convertable();
	}

	EffectNodePortType EffectNodePortUtil::get_effect_node_port(
		const String &str)
	{
		Uint32 i;
		EffectNodePortType effect_node_port;

		for (i = 0; i < effect_node_port_datas_count; ++i)
		{
			effect_node_port = static_cast<EffectNodePortType>(i);

			if (str == get_str(effect_node_port))
			{
				return effect_node_port;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(
				UTF8("EffectNodePortType")));
	}

	bool EffectNodePortUtil::get_effect_node_port(const String &str,
		EffectNodePortType &effect_node_port) noexcept
	{
		Uint32 i;
		EffectNodePortType tmp;

		for (i = 0; i < effect_node_port_datas_count; ++i)
		{
			tmp = static_cast<EffectNodePortType>(i);

			if (str == get_str(tmp))
			{
				effect_node_port = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 EffectNodePortUtil::get_effect_node_port_count() noexcept
	{
		return effect_node_port_datas_count;
	}

	OutStream& operator<<(OutStream &str, const EffectNodePortType value)
	{
		str << EffectNodePortUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, EffectNodePortType &value)
	{
		StringType string;

		str >> string;

		value = EffectNodePortUtil::get_effect_node_port(
			String(string));

		return str;
	}

}
