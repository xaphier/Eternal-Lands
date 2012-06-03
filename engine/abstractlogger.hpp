/****************************************************************************
 *            abstractlogger.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c0873371_3226_401f_bd70_18bef6e29bf7
#define	UUID_c0873371_3226_401f_bd70_18bef6e29bf7

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "logging.hpp"

/**
 * @file
 * @brief The @c class AbstractLogger.
 * This file contains the @c class AbstractLogger.
 */
namespace eternal_lands
{

	class AbstractLogger: public boost::noncopyable
	{
		protected:
			/**
			 * Default constructor.
			 */
			AbstractLogger();

		public:
			/**
			 * Default destructor.
			 */
			virtual ~AbstractLogger() noexcept;

			/**
			 * Logs a message.
			 * @param log_level The log level, e.g. error or debug.
			 * @param type The log type, e.g. io or image.
			 * @param message The message.
			 * @param file The source file where the logging was
			 * called.
			 * @param line The line in the source file where the
			 * logging was called.
			 */
			virtual void log_message(const LogLevelType log_level,
				const std::string &type,
				const std::string &message,
				const std::string &file, const Uint32 line) = 0;

	};

}

#endif	/* UUID_c0873371_3226_401f_bd70_18bef6e29bf7 */
