/****************************************************************************
 *            logging.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "logging.hpp"
#include <bitset>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <vector>
#include <map>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <SDL/SDL_mutex.h>
#include <SDL/SDL_thread.h>
#include "../elc_private.h"
#include "../platform.h"
#include "buildinformations.hpp"
#include <boost/exception/all.hpp>
#include "utf.hpp"
#include "exceptions.hpp"
#include "abstractlogger.hpp"
#include "filesystem.hpp"

namespace eternal_lands
{

	namespace
	{

		std::string log_dir;
		SDL_mutex* log_mutex;
		std::string m_last_message;
		Uint32 m_last_message_count;
		std::fstream m_log_file;
		std::map<LogType, LogLevelType> log_levels;
		AbstractLoggerVector loggers;

		std::string get_str(const LogLevelType log_level)
		{
			switch (log_level)
			{
				case llt_error:
					return "Error";
				case llt_warning:
					return "Warning";
				case llt_info:
					return "Info";
				case llt_debug:
				case llt_debug_verbose:
					return "Debug";
				default:
					return "Unkown";
			}
		}

		void log_message(const LogLevelType log_level,
			const std::string &type, const std::string &message,
			const std::string &file, const Uint32 line)
		{
			char buffer[128];
			std::stringstream str, log_stream;
			std::time_t raw_time;
			String file_name;

			file_name = FileSystem::get_file_name(String(file));

			BOOST_FOREACH(AbstractLogger &logger, loggers)
			{
				logger.log_message(log_level, type, message,
					file_name, line);
			}

			std::time(&raw_time);
			memset(buffer, 0, sizeof(buffer));
			std::strftime(buffer, sizeof(buffer), "%X",
				std::localtime(&raw_time));

			str << ", " << file_name << ":" << line << "] " << type;
			str << ": " << message;

			if (str.str() == m_last_message)
			{
				m_last_message_count++;
				return;
			}

			if (m_last_message_count > 0)
			{
				log_stream << "[" << buffer;

				log_stream << "]";
				log_stream << "Last message repeated ";
				log_stream << m_last_message_count;
				log_stream << " time";

				if (m_last_message_count > 1)
				{
					log_stream << "s";
				}

				log_stream << "\n";
			}

			log_stream << "[" << buffer << str.str();

			if (message.rbegin() != message.rend())
			{
				if (*message.rbegin() != '\n')
				{
					log_stream << "\n";
				}
			}
			else
			{
				log_stream << "\n";
			}

			m_last_message = str.str();
			m_last_message_count = 0;

			m_log_file.write(log_stream.str().c_str(),
				log_stream.str().length());
			m_log_file.flush();
		}

		std::string get_local_time_string()
		{
			char buffer[512];
			std::time_t raw_time;

			std::time(&raw_time);

			memset(buffer, 0, sizeof(buffer));
			std::strftime(buffer, sizeof(buffer), "%c %Z",
				std::localtime(&raw_time));

			return buffer;
		}

		void init(const std::string &name, const bool debug)
		{
			std::stringstream file_name;
			std::stringstream str;
			LogLevelType log_level;

			if (debug)
			{
				log_level = llt_debug;
			}
			else
			{
				log_level = llt_info;
			}			

			log_levels[lt_default] = log_level;
			log_levels[lt_io] = log_level;
			log_levels[lt_texture] = log_level;
			log_levels[lt_glsl_program] = log_level;
			log_levels[lt_actor_texture] = log_level;
			log_levels[lt_image] = log_level;
			log_levels[lt_dds_image] = log_level;
			log_levels[lt_png_image] = log_level;
			log_levels[lt_jpeg_image] = log_level;
			log_levels[lt_j2k_image] = log_level;
			log_levels[lt_e3d_loader] = log_level;
			log_levels[lt_e2d_loader] = log_level;
			log_levels[lt_map_loader] = log_level;
			log_levels[lt_mesh] = log_level;
			log_levels[lt_material] = log_level;
			log_levels[lt_shader_source] = log_level;
			log_levels[lt_angel_script] = log_level;
			log_levels[lt_framebuffer] = log_level;
			log_levels[lt_rendering] = log_level;
			log_levels[lt_uniform_buffer] = log_level;
			log_levels[lt_effect] = log_level;
			log_levels[lt_terrain] = log_level;
			log_levels[lt_opengl] = log_level;

			file_name << log_dir << name << ".log";

			m_last_message_count = 0;

			m_log_file.exceptions(std::ios::failbit |
				std::ios::badbit);

			m_log_file.open(file_name.str().c_str(),
				std::ios::out);

			log_message(llt_info, "Git sha1", git_sha1_str,
				__FILE__, __LINE__);

			log_message(llt_info, "Git tag", git_tag_str,
				__FILE__, __LINE__);

			log_message(llt_info, "CMAKE_BUILD_TYPE",
				cmake_build_type_str, __FILE__, __LINE__);

			log_message(llt_info, "Log started at",
				get_local_time_string(), __FILE__, __LINE__);

			log_message(llt_info, "version", FILE_VERSION, __FILE__,
				__LINE__);
		}

		void clear_dir(const std::string &dir)
		{
			struct dirent *dp;
			DIR *dirp;
			std::string file_name;

			dirp = opendir(dir.c_str());

			if (dirp == 0)
			{
				return;
			}

			dp = readdir(dirp);

			while (dp != 0)
			{
				file_name = dir;
				file_name += "/";
				file_name += dp->d_name;
				std::remove(file_name.c_str());
				dp = readdir(dirp);
			}

			closedir(dirp);
		}

	}

	void init_logging(const std::string &dir, const bool debug)
	{
		std::string str;

		log_mutex = SDL_CreateMutex();

		log_dir = dir + "/";

		clear_dir(dir);
#ifdef	WINDOWS
		mkdir(dir.c_str());
#else	/* WINDOWS */
		mkdir(dir.c_str(), S_IRWXU | S_IRWXG);
#endif	/* WINDOWS */

		init("main", debug);
	}

	void exit_logging()
	{
		m_log_file.close();

		loggers.clear();

		SDL_DestroyMutex(log_mutex);
	}

	LogLevelType get_log_level(const LogType log)
	{
		LogLevelType result;

		SDL_LockMutex(log_mutex);

		result = log_levels[log];

		SDL_UnlockMutex(log_mutex);

		return result;
	}

	void set_log_level(const LogType log, const LogLevelType log_level)
	{
		SDL_LockMutex(log_mutex);

		log_levels[log] = log_level;

		SDL_UnlockMutex(log_mutex);
	}

	void log_message(const LogLevelType log_level, const LogType log,
		const std::string &message, const std::string &file,
		const Uint32 line)
	{
		SDL_LockMutex(log_mutex);

		if (log_levels[log] < log_level)
		{
			SDL_UnlockMutex(log_mutex);

			return;
		}

		log_message(log_level, get_str(log_level), message, file, line);

		SDL_UnlockMutex(log_mutex);
	}

	void log_exception_message(const std::string &message,
		const std::string &file, const Uint32 line)
	{
		SDL_LockMutex(log_mutex);

		log_message(llt_error, UTF8("exception"), message, file, line);

		SDL_UnlockMutex(log_mutex);
	}

	void log_exception(const boost::exception &exception,
		const std::string &file, const Uint32 line)
	{
		String str, throw_file;
		Uint32 throw_line;

		throw_file = UTF8("Unkown file");

		if (boost::get_error_info<boost::throw_file>(exception) != 0)
		{
			throw_file = *boost::get_error_info<boost::throw_file>(
				exception);
		}

		throw_line = 0;

		if (boost::get_error_info<boost::throw_line>(exception) != 0)
		{
			throw_line = *boost::get_error_info<boost::throw_line>(
				exception);
		}

		if (typeid(exception) == typeid(FileNotFoundException))
		{
			BoostFormat format_string(UTF8("File '%1%' not found"));

			str = UTF8("Unkown file");

			if (boost::get_error_info<
				boost::errinfo_file_name>(exception) != 0)
			{
				str = *boost::get_error_info<
					boost::errinfo_file_name>(exception);
			}

			format_string % str;

			log_exception_message(String(format_string.str()),
				throw_file, throw_line);

			return;
		}

		if (typeid(exception) == typeid(IoErrorException))
		{
			BoostFormat format_string(UTF8("File '%1%' is invalid"));

			str = UTF8("Unkown file");

			if (boost::get_error_info<
				boost::errinfo_file_name>(exception) != 0)
			{
				str = *boost::get_error_info<
					boost::errinfo_file_name>(exception);
			}

			format_string % str;

			log_exception_message(String(format_string.str()),
				throw_file, throw_line);

			return;
		}

		if (typeid(exception) == typeid(FileNotFoundException))
		{
			BoostFormat format_string(UTF8("File '%1%' not found"));

			str = UTF8("Unkown file");

			if (boost::get_error_info<
				boost::errinfo_file_name>(exception) != 0)
			{
				str = *boost::get_error_info<
					boost::errinfo_file_name>(exception);
			}

			format_string % str;

			log_exception_message(String(format_string.str()),
				throw_file, throw_line);

			return;
		}

		log_exception_message(boost::diagnostic_information(
			exception), file, line);
	}

	void log_exception(const std::exception &exception,
		const std::string &file, const Uint32 line)
	{
		if (exception.what() == 0)
		{
			log_exception_message("invalid exception", file, line);
		}
		else
		{		
			log_exception_message(exception.what(), file, line);
		}
	}

	void log_exception_str(const std::string &message,
		const std::string &file, const Uint32 line)
	{
		log_exception_message(message, file, line);
	}

	void register_logger(std::auto_ptr<AbstractLogger> &logger)
	{
		loggers.push_back(logger);
	}

}
