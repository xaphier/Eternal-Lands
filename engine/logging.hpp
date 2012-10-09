/****************************************************************************
 *            logging.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
		lt_j2k_image,
		lt_e3d_loader,
		lt_e2d_loader,
		lt_map_loader,
		lt_mesh,
		lt_material,
		lt_shader_source,
		lt_angel_script,
		lt_framebuffer,
		lt_rendering,
		lt_uniform_buffer,
		lt_effect,
		lt_terrain,
		lt_opengl
	};

	void init_logging(const std::string &log_file_name,
		const bool debug);
	void exit_logging();
	LogLevelType get_log_level(const LogType log);
	void set_log_level(const LogType log, const LogLevelType log_level);
	void log_message(const LogLevelType log_level, const LogType log,
		const std::string &message, const std::string &file,
		const Uint32 line);
	void log_exception(const std::exception &exception,
		const std::string &file, const Uint32 line);
	void log_exception(const boost::exception &exception,
		const std::string &file, const Uint32 line);
	void log_exception_str(const std::string &message,
		const std::string &file, const Uint32 line);
	void register_logger(std::auto_ptr<AbstractLogger> &logger);

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
		::eternal_lands::BoostFormat log_format_string(description);	\
	\
		log_format_string % arguments;	\
	\
		eternal_lands::log_exception_str(log_format_string.str(),	\
			__FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_ERROR(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat log_format_string(description);	\
	\
		log_format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_error, type,	\
			log_format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_WARNING(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat log_format_string(description);	\
	\
		log_format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_warning, type,	\
			log_format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_INFO(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat log_format_string(description);	\
	\
		log_format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_info, type,	\
			log_format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_DEBUG(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat log_format_string(description);	\
	\
		log_format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_debug, type,	\
			log_format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#define LOG_DEBUG_VERBOSE(type, description, arguments)	\
	do	\
	{	\
		::eternal_lands::BoostFormat log_format_string(description);	\
	\
		log_format_string % arguments;	\
	\
		eternal_lands::log_message(eternal_lands::llt_debug_verbose,	\
			type, log_format_string.str(), __FILE__, __LINE__);	\
	}	\
	while (false)

#endif	/* UUID_043a52e6_d0d2_4327_9877_06d299f16726 */
