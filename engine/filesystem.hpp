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

		public:
			FileSystem();
			~FileSystem() throw();
			void add_dirs(const Uint16 major, const Uint16 minor,
				const Uint16 release);
			void add_dir(const String &dir_name);
			void add_zip(const String &file_name);
			bool remove_archive(const String &file_name);
			bool delete_file(const String &file_name);
//			bool replace_file(const String &file_name,
//				const IOStreamSharedPtr &file);
//			IOStreamSharedPtr get_temp_file();
			ReaderSharedPtr get_file(const String &file_name);
			String get_file_string(const String &file_name);
//			const ZipFile &get_zip_file(const String &file_name);

	};

}

#endif	/* UUID_7699c24a_625f_4883_9b5a_46994a9adbde */
