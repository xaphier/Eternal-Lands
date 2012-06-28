/****************************************************************************
 *            parametersizeutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "parametersizeutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		class ParameterSizeTypeData
		{
			private:
				const String m_name;
				const Uint16 m_max_size;

			public:
				inline ParameterSizeTypeData(
					const String &name,
					const Uint16 max_size): m_name(name),
						m_max_size(max_size)
				{
				}

				inline ~ParameterSizeTypeData() noexcept
				{
				}

				inline const String &get_name() const noexcept
				{
					return m_name;
				}

				inline Uint16 get_max_size() const noexcept
				{
					return m_max_size;
				}

		};

		const ParameterSizeTypeData parameter_size_datas[] =
		{
			ParameterSizeTypeData(String(UTF8("one")), 1),
			ParameterSizeTypeData(String(UTF8("lights_count")), 8),
			ParameterSizeTypeData(String(UTF8("bones_count")), 80),
			ParameterSizeTypeData(String(UTF8("shadow_maps_count")),
				3),
			ParameterSizeTypeData(String(UTF8("clipmap_slices")),
				8),
			ParameterSizeTypeData(String(UTF8("render_targets")),
				4)
		};

		const Uint32 parameter_size_datas_count =
			sizeof(parameter_size_datas) /
			sizeof(ParameterSizeTypeData);

	}

	const String &ParameterSizeUtil::get_str(
		const ParameterSizeType parameter_size)
	{
		if (parameter_size_datas_count <= parameter_size)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_size_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter_size))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterSizeType")));
		}

		return parameter_size_datas[parameter_size].get_name();
	}

	ParameterSizeType ParameterSizeUtil::get_parameter_size(
		const String &str)
	{
		Uint32 i;
		ParameterSizeType parameter_size;

		for (i = 0; i < parameter_size_datas_count; ++i)
		{
			parameter_size = static_cast<ParameterSizeType>(i);

			if (str == get_str(parameter_size))
			{
				return parameter_size;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"ParameterSizeType")));
	}

	bool ParameterSizeUtil::get_parameter_size(const String &str,
		ParameterSizeType &parameter_size) noexcept
	{
		Uint32 i;
		ParameterSizeType tmp;

		for (i = 0; i < parameter_size_datas_count; ++i)
		{
			tmp = static_cast<ParameterSizeType>(i);

			if (str == get_str(tmp))
			{
				parameter_size = tmp;

				return true;
			}
		}

		return false;
	}

	Uint16 ParameterSizeUtil::get_max_size(
		const ParameterSizeType parameter_size)
	{
		if (parameter_size_datas_count <= parameter_size)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					parameter_size_datas_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					parameter_size))
				<< boost::errinfo_type_info_name(UTF8(
					"ParameterSizeType")));
		}

		return parameter_size_datas[parameter_size].get_max_size();
	}

	Uint32 ParameterSizeUtil::get_parameter_size_count() noexcept
	{
		return parameter_size_datas_count;
	}

	OutStream& operator<<(OutStream &str, const ParameterSizeType value)
	{
		str << ParameterSizeUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, ParameterSizeType &value)
	{
		StringType string;

		str >> string;

		value = ParameterSizeUtil::get_parameter_size(String(string));

		return str;
	}

}
