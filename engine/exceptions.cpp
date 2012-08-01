/****************************************************************************
 *            exceptions.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "exceptions.hpp"
#ifndef	WIN32
#include <execinfo.h>
#include <cxxabi.h>
#endif	/* WIN32 */

namespace eternal_lands
{

	namespace
	{

		String get_gl_error_str(const GLenum error)
		{
			switch (error)
			{
				case GL_INVALID_ENUM:
					return String(UTF8("invalid enum"));
				case GL_INVALID_VALUE:
					return String(UTF8("invalid value"));
				case GL_INVALID_OPERATION:
					return String(
						UTF8("invalid operation"));
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					return String(UTF8("invalid "
						"framebuffer operation"));
				case GL_OUT_OF_MEMORY:
					return String(UTF8("out of memory"));
				default:
					return String(UTF8("unknown error"));
			}
		}

	}

	String get_gl_error_string(const GLenum error)
	{
		BoostFormat format_string(UTF8("%1%:%2$x"));

		format_string % get_gl_error_str(error) % error;

		return String(format_string.str());
	}

	String get_stack_string()
	{
#ifdef	WIN32
		return String();
#else	/* WIN32 */
		/**
		 * Using GNU backtrace to get the stacktrace.
		 **/
		void* array[150];
		char** messages;
		char* demangled_name;
		StringStream str;
		StringType message, name, address, offset;
		std::size_t mangled_name, offset_begin, offset_end;
		int i, size, status;

		size = backtrace(array, 150);

		messages = backtrace_symbols(array, size);    

		assert(messages != 0);

		// skip first stack frame (points here)
		for (i = 1; i < size; ++i)
		{
			BoostFormat format(UTF8("bt[%1%]: %2% {%3%}"));

			message = messages[i];

			mangled_name = message.find('(');
			offset_begin = message.find('+', mangled_name);
			offset_end = message.find(')', offset_begin);

			name = message.substr(mangled_name + 1,
				offset_begin - mangled_name - 1);
			offset = message.substr(offset_begin + 1,
				offset_end - offset_begin - 1);
			address = message.substr(offset_end + 1);

			demangled_name = abi::__cxa_demangle(name.c_str(), 0, 0,
				&status);

			if ((status == 0) && (demangled_name != 0))
			{
				name = demangled_name;
			}

			free(demangled_name);

			format % i % name % message;

			str << format.str() << std::endl;
		}

		free(messages);

		return String(str.str());
#endif	/* WIN32 */
	}

}
