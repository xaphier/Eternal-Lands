/****************************************************************************
 *            elloggingwrapper.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "elloggingwrapper.h"
#undef LOG_ERROR
#undef LOG_WARNING
#undef LOG_INFO
#undef LOG_DEBUG
#undef LOG_DEBUG_VERBOSE
#include "engine/prerequisites.hpp"
#include "engine/logging.hpp"
#include "io/elpathwrapper.h"
#include "asc.h"
#include <cassert>

namespace el = eternal_lands;

extern "C" void init_logging(const char* log_file_name)
{
	char str[65536];

	snprintf(str, sizeof(str), "%s%s", get_path_config(), log_file_name);

	el::init_logging(str);
}

extern "C" void exit_logging()
{
	el::exit_logging();
}

extern "C" LogLevelType get_log_level()
{
	return static_cast<LogLevelType>(el::get_log_level(el::lt_default));
}

extern "C" void set_log_level(const LogLevelType log_level)
{
	el::set_log_level(el::lt_default, static_cast<el::LogLevelType>(log_level));
}

extern "C" void log_error(const char* file, const int line,
	const char* message, ...)
{
	va_list ap;
	char err_msg[65536];

	assert(message != 0);
	assert(strlen(message) > 0);

	if (el::get_log_level(el::lt_default) < el::llt_error)
	{
		return;
	}

	memset(err_msg, 0, sizeof(err_msg));

	if (message != 0)
	{
		va_start(ap, message);

        	vsnprintf(err_msg, sizeof(err_msg), message, ap);
        	err_msg[sizeof(err_msg) - 1] = '\0';

		va_end(ap);
	}

	el::log_message(el::llt_error, el::lt_default, err_msg, file, line);
}

extern "C" void log_warning(const char* file, const int line,
	const char* message, ...)
{
	va_list ap;
	char err_msg[65536];

	assert(message != 0);
	assert(strlen(message) > 0);

	if (el::get_log_level(el::lt_default) < el::llt_warning)
	{
		return;
	}

	memset(err_msg, 0, sizeof(err_msg));

	if (message != 0)
	{
		va_start(ap, message);

        	vsnprintf(err_msg, sizeof(err_msg), message, ap);
        	err_msg[sizeof(err_msg) - 1] = '\0';

		va_end(ap);
	}

	el::log_message(el::llt_warning, el::lt_default, err_msg, file, line);
}

extern "C" void log_info(const char* file, const int line,
	const char* message, ...)
{
	va_list ap;
	char err_msg[65536];

	assert(message != 0);
	assert(strlen(message) > 0);

	if (el::get_log_level(el::lt_default) < el::llt_info)
	{
		return;
	}

	memset(err_msg, 0, sizeof(err_msg));

	if (message != 0)
	{
		va_start(ap, message);

        	vsnprintf(err_msg, sizeof(err_msg), message, ap);
        	err_msg[sizeof(err_msg) - 1] = '\0';

		va_end(ap);
	}

	el::log_message(el::llt_info, el::lt_default, err_msg, file, line);
}

extern "C" void log_debug(const char* file, const int line,
	const char* message, ...)
{
	va_list ap;
	char err_msg[65536];

	assert(message != 0);
	assert(strlen(message) > 0);

	if (el::get_log_level(el::lt_default) < el::llt_debug)
	{
		return;
	}

	memset(err_msg, 0, sizeof(err_msg));

	if (message != 0)
	{
		va_start(ap, message);

        	vsnprintf(err_msg, sizeof(err_msg), message, ap);
        	err_msg[sizeof(err_msg) - 1] = '\0';

		va_end(ap);
	}

	el::log_message(el::llt_debug, el::lt_default, err_msg, file, line);
}

extern "C" void log_debug_verbose(const char* file, const int line,
	const char* message, ...)
{
	va_list ap;
	char err_msg[65536];

	assert(message != 0);
	assert(strlen(message) > 0);

	if (el::get_log_level(el::lt_default) < el::llt_debug_verbose)
	{
		return;
	}

	memset(err_msg, 0, sizeof(err_msg));

	if (message != 0)
	{
		va_start(ap, message);

        	vsnprintf(err_msg, sizeof(err_msg), message, ap);
        	err_msg[sizeof(err_msg) - 1] = '\0';

		va_end(ap);
	}

	el::log_message(el::llt_debug_verbose, el::lt_default, err_msg, file,
		line);
}
