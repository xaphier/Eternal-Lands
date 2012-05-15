/****************************************************************************
 *            uniformbufferutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uniformbufferutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String uniform_buffer_names[] =
		{
			String(UTF8("none")),
			String(UTF8("scene_data")),
			String(UTF8("material_data")),
			String(UTF8("terrain_data"))
		};

		const Uint32 uniform_buffer_names_count =
			sizeof(uniform_buffer_names) / sizeof(String);

	}

	const String &UniformBufferUtil::get_str(
		const UniformBufferType uniform_buffer)
	{
		if (uniform_buffer_names_count <= uniform_buffer)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					uniform_buffer_names_count - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					uniform_buffer))
				<< boost::errinfo_type_info_name(UTF8(
					"UniformBufferType")));
		}

		return uniform_buffer_names[uniform_buffer];
	}

	UniformBufferType UniformBufferUtil::get_uniform_buffer(
		const String &str)
	{
		Uint32 i;
		UniformBufferType uniform_buffer;

		for (i = 0; i < uniform_buffer_names_count; ++i)
		{
			uniform_buffer = static_cast<UniformBufferType>(i);

			if (str == get_str(uniform_buffer))
			{
				return uniform_buffer;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"UniformBufferType")));
	}

	bool UniformBufferUtil::get_uniform_buffer(const String &str,
		UniformBufferType &uniform_buffer) noexcept
	{
		Uint32 i;
		UniformBufferType tmp;

		for (i = 0; i < uniform_buffer_names_count; ++i)
		{
			tmp = static_cast<UniformBufferType>(i);

			if (str == get_str(tmp))
			{
				uniform_buffer = tmp;

				return true;
			}
		}

		return false;
	}

	Uint32 UniformBufferUtil::get_uniform_buffer_count() noexcept
	{
		return uniform_buffer_names_count;
	}

	OutStream& operator<<(OutStream &str, const UniformBufferType value)
	{
		str << UniformBufferUtil::get_str(value);

		return str;
	}

	InStream& operator>>(InStream &str, UniformBufferType &value)
	{
		StringType string;

		str >> string;

		value = UniformBufferUtil::get_uniform_buffer(String(string));

		return str;
	}

}
