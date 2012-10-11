/****************************************************************************
 *            threadstateutil.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "threadstateutil.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		const String thread_state_names[] =
		{
			String(UTF8("waiting")),
			String(UTF8("running")),
			String(UTF8("exit"))
		};

		const Uint32 thread_state_names_count =
			sizeof(thread_state_names) / sizeof(String);

	}

	const String &ThreadStateUtil::get_str(
		const ThreadStateType thread_state)
	{
		if (get_thread_state_count() <= thread_state)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_range_min(0)
				<< errinfo_range_max(
					get_thread_state_count() - 1)
				<< errinfo_range_index(static_cast<Uint32>(
					thread_state))
				<< boost::errinfo_type_info_name(UTF8(
					"ThreadStateType")));
		}

		return thread_state_names[thread_state];
	}

	ThreadStateType ThreadStateUtil::get_thread_state(const String &str)
	{
		Uint32 i;
		ThreadStateType thread_state;

		for (i = 0; i < get_thread_state_count(); i++)
		{
			thread_state = static_cast<ThreadStateType>(i);

			if (str == get_str(thread_state))
			{
				return thread_state;
			}
		}

		EL_THROW_EXCEPTION(InvalidParameterException()
			<< errinfo_string_value(str)
			<< boost::errinfo_type_info_name(UTF8(
				"ThreadStateType")));
	}

	Uint32 ThreadStateUtil::get_thread_state_count() noexcept
	{
		return thread_state_names_count;
	}

	OutStream &operator<<(OutStream &str, const ThreadStateType value)
	{
		str << ThreadStateUtil::get_str(value);

		return str;
	}

}
