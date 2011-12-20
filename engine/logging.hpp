/****************************************************************************
 *            logging.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_043a52e6_d0d2_4327_9877_06d299f16726
#define	UUID_043a52e6_d0d2_4327_9877_06d299f16726

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 */
namespace eternal_lands
{

	enum LogLevelType
	{
		llt_error = 0,
		llt_warning = 1,
		llt_info = 2,
		llt_debug = 3,
		llt_debug_verbose = 4
	};

	void init_logging(const std::string &log_file_name);
	void exit_logging();
	LogLevelType get_log_level();
	void set_log_level(const LogLevelType log_level);
	void log_message(const LogLevelType log_level,
		const std::string &message, const std::string &file,
		const Uint32 line);
	void log_exception(const std::exception &exception,
		const std::string &file, const Uint32 line);
	void log_exception(const boost::exception &exception,
		const std::string &file, const Uint32 line);
	void init_thread_log(const std::string &name);
	void enter_debug_mark(const std::string &name, const std::string &file,
		const Uint32 line);
	void leave_debug_mark(const std::string &name, const std::string &file,
		const Uint32 line);

}

#define LOG_EXCEPTION(exception)	\
	eternal_lands::log_exception(exception, __FILE__, __LINE__)

#define LOG_ERROR(description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_error,	\
			format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_WARNING(description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_warning,	\
			format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_INFO(description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_info,	\
			format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_DEBUG(description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_debug,	\
			format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_DEBUG_VERBOSE(description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_debug_verbose,	\
			format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#endif	/* UUID_043a52e6_d0d2_4327_9877_06d299f16726 */
