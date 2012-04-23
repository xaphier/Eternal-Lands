/****************************************************************************
 *            logging.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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

	enum LogType
	{
		lt_default = 0,
		lt_io,
		lt_texture,
		lt_glsl_program,
		lt_actor_texture,
		lt_image,
		lt_dds_image,
		lt_png_image,
		lt_jpeg_image,
		lt_e3d_loader,
		lt_e2d_loader,
		lt_map_loader,
		lt_mesh,
		lt_material,
		lt_shader_source,
		lt_angel_script,
		lt_framebuffer,
		lt_rendering
	};

	void init_logging(const std::string &log_file_name);
	void exit_logging();
	LogLevelType get_log_level();
	void set_log_level(const LogLevelType log_level);
	void log_message(const LogLevelType log_level, const LogType type,
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
	void log_exception_str(const std::string &message,
		const std::string &file, const Uint32 line);

}

#ifdef	LOG_EXCEPTION
#undef	LOG_EXCEPTION
#endif	/* LOG_EXCEPTION */

#ifdef	LOG_ERROR
#undef	LOG_ERROR
#endif	/* LOG_ERROR */

#ifdef	LOG_WARNING
#undef	LOG_WARNING
#endif	/* LOG_WARNING */

#ifdef	LOG_INFO
#undef	LOG_INFO
#endif	/* LOG_INFO */

#ifdef	LOG_DEBUG
#undef	LOG_DEBUG
#endif	/* LOG_DEBUG */

#ifdef	LOG_DEBUG_VERBOSE
#undef	LOG_DEBUG_VERBOSE
#endif	/* LOG_DEBUG_VERBOSE */

#define LOG_EXCEPTION(exception)	\
	eternal_lands::log_exception(exception, __FILE__, __LINE__)

#define LOG_EXCEPTION_STR(description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_exception_str(format_string.str(), __FILE__,	\
			__LINE__);	\
	}	\
	while (false)

#define LOG_ERROR(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_error, type,	\
			format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_WARNING(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_warning, type,	\
			format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_INFO(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_info, type,	\
			format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_DEBUG(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_debug, type,	\
			format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_DEBUG_VERBOSE(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat format_string(description);	\
	\
		format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_debug_verbose,	\
			type, format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#endif	/* UUID_043a52e6_d0d2_4327_9877_06d299f16726 */
