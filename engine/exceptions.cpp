/****************************************************************************
 *            exceptions.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "exceptions.hpp"
#ifdef	WIN32
#include <windows.h>
#include <excpt.h>
#include <imagehlp.h>
#include <bfd.h>
#include <psapi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#else	/* WIN32 */
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

#ifdef	WIN32

		struct bfd_ctx
		{
			bfd * handle;
			asymbol** symbol;
		};

		struct bfd_set
		{
			char * name;
			struct bfd_ctx * bc;
			struct bfd_set *next;
		};

		struct find_info
		{
			asymbol **symbol;
			bfd_vma counter;
			const char *file;
			const char *func;
			unsigned line;
		};

		void  lookup_section(bfd *abfd, asection *sec,
			void *opaque_data)
		{
			find_info* data = static_cast<find_info*>(opaque_data);

			if (data->func)
			{
				return;
			}

			if (!(bfd_get_section_flags(abfd, sec) & SEC_ALLOC))
			{
				return;
			}

			bfd_vma vma = bfd_get_section_vma(abfd, sec);

			if ((data->counter < vma) || ((vma +
				bfd_get_section_size(sec)) <= data->counter))
			{
				return;
			}

			bfd_find_nearest_line(abfd, sec, data->symbol,
				data->counter - vma, &(data->file),
				&(data->func), &(data->line));
		}

		void find(bfd_ctx* b, DWORD offset, const char** file,
			const char** func, unsigned* line)
		{
			find_info data;

			data.func = nullptr;
			data.symbol = b->symbol;
			data.counter = offset;
			data.file = nullptr;
			data.func = nullptr;
			data.line = 0;

			bfd_map_over_sections(b->handle, &lookup_section,
				&data);

			if (file)
			{
				*file = data.file;
			}

			if (func)
			{
				*func = data.func;
			}

			if (line)
			{
				*line = data.line;
			}
		}

		int init_bfd_ctx(bfd_ctx* bc, const char* procname,
			StringStream &str)
		{
			bfd* b;
			void* symbol_table;
			unsigned dummy;
			int r1, r2, r3;

			bc->handle = nullptr;
			bc->symbol = nullptr;

			b = bfd_openr(procname, 0);

			if (!b)
			{
				str << UTF8("Failed to open bfd from '");
				str << procname << UTF8("'") << std::endl;

				return 1;
			}

			r1 = bfd_check_format(b, bfd_object);
			r2 = bfd_check_format_matches(b, bfd_object, nullptr);
			r3 = bfd_get_file_flags(b) & HAS_SYMS;

			if (!(r1 && r2 && r3))
			{
				bfd_close(b);

				str << UTF8("Failed to init bfd from '");
				str << procname << UTF8("'") << std::endl;

				return 1;
			}

			dummy = 0;

			if (bfd_read_minisymbols(b, FALSE, &symbol_table,
				&dummy) == 0)
			{
				if (bfd_read_minisymbols(b, TRUE,
					&symbol_table, &dummy) < 0)
				{
					free(symbol_table);
					bfd_close(b);

					str << UTF8("Failed to read symbols ");
					str << UTF8("from '") << procname;
					str << UTF8("'") << std::endl;

					return 1;
				}
			}

			bc->handle = b;
			bc->symbol = static_cast<asymbol**>(symbol_table);

			return 0;
		}

		void close_bfd_ctx(bfd_ctx* bc)
		{
			if (bc)
			{
				if (bc->symbol)
				{
					free(bc->symbol);
				}
		
				if (bc->handle)
				{
					bfd_close(bc->handle);
				}
			}
		}

		bfd_ctx* get_bc(bfd_set* set, const char* procname,
			StringStream &str)
		{
			bfd_ctx bc;

			while (set->name)
			{
				if (strcmp(set->name , procname) == 0)
				{
					return set->bc;
				}

				set = set->next;
			}

			if (init_bfd_ctx(&bc, procname , str))
			{
				return nullptr;
			}

			set->next = static_cast<bfd_set*>(
				calloc(1, sizeof(*set)));
			set->bc = static_cast<bfd_ctx*>(
				malloc(sizeof(bfd_ctx)));
			memcpy(set->bc, &bc, sizeof(bc));
			set->name = strdup(procname);

			return set->bc;
		}

		void release_set(bfd_set* set)
		{
			bfd_set* temp;

			while (set)
			{
				temp = set->next;

				free(set->name);

				close_bfd_ctx(set->bc);

				free(set);

				set = temp;
			}
		}

		void backtrace(bfd_set* set, int depth, LPCONTEXT context,
			StringStream &str)
		{
			char symbol_buffer[sizeof(IMAGEHLP_SYMBOL) + 255];
			char module_name_raw[MAX_PATH];
			char procname[MAX_PATH];
			bfd_ctx* bc;
			IMAGEHLP_SYMBOL *symbol;
			STACKFRAME frame;
			HANDLE process;
			HANDLE thread;
			DWORD module_base;
			DWORD dummy;
			const char* file;
			const char* func;
			const char* module_name;
			unsigned line;

			GetModuleFileNameA(nullptr, procname, sizeof(procname));

			bc = nullptr;

			memset(&frame, 0, sizeof(frame));

			frame.AddrPC.Offset = context->Eip;
			frame.AddrPC.Mode = AddrModeFlat;
			frame.AddrStack.Offset = context->Esp;
			frame.AddrStack.Mode = AddrModeFlat;
			frame.AddrFrame.Offset = context->Ebp;
			frame.AddrFrame.Mode = AddrModeFlat;

			process = GetCurrentProcess();
			thread = GetCurrentThread();

			while (StackWalk(IMAGE_FILE_MACHINE_I386, process, 
				thread, &frame, context, 0,
				SymFunctionTableAccess, SymGetModuleBase, 0))
			{
				--depth;

				if (depth < 0)
				{
					break;
				}

				symbol = (IMAGEHLP_SYMBOL*)symbol_buffer;
				symbol->SizeOfStruct = (sizeof *symbol) + 255;
				symbol->MaxNameLength = 254;

				module_base = SymGetModuleBase(process,
					frame.AddrPC.Offset);

				module_name = UTF8("[unknown module]");

				if (module_base && GetModuleFileNameA(
					(HINSTANCE)module_base, module_name_raw,
					MAX_PATH))
				{
					module_name = module_name_raw;
					bc = get_bc(set, module_name, str);
				}

				file = nullptr;
				func = nullptr;
				line = 0;

				if (bc)
				{
					find(bc, frame.AddrPC.Offset, &file,
						&func, &line);
				}

				if (file == nullptr)
				{
					dummy = 0;

					if (SymGetSymFromAddr(process,
						frame.AddrPC.Offset, &dummy,
						symbol))
					{
						file = symbol->Name;
					}
					else
					{
						file = UTF8("[unknown file]");
					}
				}

				if (func == nullptr)
				{
					BoostFormat format(UTF8("bt[%1%]: %2$x"
						" %3% %4%"));

					format % depth % frame.AddrPC.Offset %
						module_name % file;

					str << format << std::endl;
				}
				else
				{
					BoostFormat format(UTF8("bt[%1%]: %2$x"
						" %3% %4% %5% (%6%)"));

					format % depth % frame.AddrPC.Offset %
						module_name % file % func %
						line;

					str << format << std::endl;
				}
			}
		}

		// The following should be enough for walking the callstack...
		#define GET_CURRENT_CONTEXT(c, contextFlags) \
			do { \
				memset(&c, 0, sizeof(CONTEXT)); \
				c.ContextFlags = contextFlags; \
				__asm__("call x\n\t"	\
					"x: pop %%eax\n\t"	\
					"movl %%eax, %0\n\t"	\
					"movl %%ebp, %1\n\t"	\
					"movl %%esp, %2\n\t"	\
					: "=r"(c.Eip), "=r"(c.Ebp), "=r"(c.Esp)	\
					: /**/ : "%eax");	\
			} while (false);
#endif	/* WIN32 */

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
		StringStream str;
		bfd_set* set;
		CONTEXT context;

		if (!SymInitialize(GetCurrentProcess(), 0, TRUE))
		{
			str << UTF8("Failed to init symbol context");
		}
		else
		{
			bfd_init();

			set = static_cast<bfd_set*>(calloc(1, sizeof(bfd_set)));

			GET_CURRENT_CONTEXT(context, CONTEXT_FULL);

			backtrace(set, 150, &context, str);

			release_set(set);

			SymCleanup(GetCurrentProcess());
		}

		return String(str.str());
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
