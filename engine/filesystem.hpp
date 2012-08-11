/****************************************************************************
 *            filesystem.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			typedef std::map<String, Uint8Array20>
				StringUint8Array20Map;

			AbstractArchiveVector m_archives;
			StringUint8Array20Map m_sha1s;

			String get_tmp_file();

			static StringTypeVector get_stripped_path(
				const String &file_name);

		public:
			FileSystem();
			~FileSystem() noexcept;
			void add_dir(const String &dir_name);
			void add_zip(const String &zip_name);
			void add_zip(const String &zip_name,
				const Uint8Array20 &sha1);
			void replace_with_dir(const String &dir_name,
				const Uint32 index);
			void replace_with_zip(const String &zip_name,
				const Uint32 index);
			void replace_with_zip(const String &zip_name,
				const Uint8Array20 &sha1,
				const Uint32 index);
			bool remove_archive(const String &file_name);
			bool get_file_readable(const String &file_name);
			ReaderSharedPtr get_file(const String &file_name);
			bool get_file_if_readable(const String &file_name,
				ReaderSharedPtr &reader);
			String get_file_string(const String &file_name);
			StringSet get_files(const String &dir,
				const String &pattern) const;
			void clear();
			static String get_file_name(const String &file_name);
			static String get_dir_name(const String &file_name);
			static String get_strip_relative_path(
				const String &file_name);
			static String get_name_without_extension(
				const String &file_name);
			static String get_file_name_without_extension(
				const String &file_name);

	};

}

#endif	/* UUID_7699c24a_625f_4883_9b5a_46994a9adbde */
