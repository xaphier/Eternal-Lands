/****************************************************************************
 *            filesystem.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_7699c24a_625f_4883_9b5a_46994a9adbde
#define	UUID_7699c24a_625f_4883_9b5a_46994a9adbde

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 */
namespace eternal_lands
{

	class FileSystem
	{
		private:
			AbstractArchiveVector m_archives;
			String get_tmp_file();

			static StringTypeVector get_stripped_path(
				const String &file_name);

		public:
			FileSystem();
			~FileSystem() throw();
			void add_dirs(const Uint16 major, const Uint16 minor,
				const Uint16 release);
			void add_dir(const String &dir_name);
			void add_zip(const String &file_name);
			bool remove_archive(const String &file_name);
			bool get_file_readable(const String &file_name);
			ReaderSharedPtr get_file(const String &file_name);
			bool get_file_if_readable(const String &file_name,
				ReaderSharedPtr &reader);
			String get_file_string(const String &file_name);
			static String get_file_name(const String &file_name);
			static String get_dir_name(const String &file_name);
			static String get_strip_relative_path(
				const String &file_name);
			static String get_file_name_without_extension(
				const String &file_name);

	};

}

#endif	/* UUID_7699c24a_625f_4883_9b5a_46994a9adbde */
