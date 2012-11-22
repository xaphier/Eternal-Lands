/****************************************************************************
 *            qualityutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "qualityutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String quality_names[] =
		{
			String(UTF8("low")),
			String(UTF8("medium")),
			String(UTF8("high"))
		};

		const Uint32 quality_names_count =
			sizeof(quality_names) / sizeof(String);

	}

	const String &QualityUtil::get_str(const QualityType quality)
	{
		if (quality_names_count <= quality)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					quality_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					quality))
				<< boost::errinfo_type_info_name(UTF8(
					"QualityType")));
		}

		return quality_names[quality];
	}

	QualityType QualityUtil::get_quality(const String &str)
	{
		Uint32 i;
		QualityType quality;

		for (i = 0; i < quality_names_count; ++i)
		{
			quality = static_cast<QualityType>(i);

			if (str == get_str(quality))
			{
				return quality;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"QualityType")));
	}

	Uint32 QualityUtil::get_quality_count() noexcept
	{
		return quality_names_count;
	}

	OutStream &operator<<(OutStream &str, const QualityType value)
	{
		str << QualityUtil::get_str(value);

		return str;
	}

	InStream &operator>>(InStream &str, QualityType &value)
	{
		std::string string;

		str >> string;

		value = QualityUtil::get_quality(String(string));

		return str;
	}

}
