/****************************************************************************
 *            zipfile.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2287080d_cfe4_4d94_a1ce_88477ece143b
#define	UUID_2287080d_cfe4_4d94_a1ce_88477ece143b

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractarchive.hpp"

/**
 * @file
 * @brief The @c class ZipFile.
 * This file contains the @c class ZipFile.
 */
namespace eternal_lands
{

	class ZipFile: public AbstractArchive
	{
		private:
			class ZipFileEntry;
			typedef std::map<String, ZipFileEntry> ZipFileEntries;

			ZipFileEntries m_files;
			void* m_file;

			void init();

		public:
			/**
			 * Default constructor.
			 */
			ZipFile(const String &name);

			/**
			 * Default destructor.
			 */
			virtual ~ZipFile() noexcept;

			/**
			 * Opens the file for reading.
			 * @return The Reader object of the file.
			 */
			virtual ReaderSharedPtr get_file(
				const String &file_name) const;

			/**
			 * Checks if the file can be found in the archive.
			 * @return True if the file is in the archive, else
			 * false
			 */
			virtual bool get_has_file(const String &file_name)
				const;

			virtual void get_files(const String &dir,
				const String &pattern, StringSet &files)
				const;

	};

}

#endif	/* UUID_2287080d_cfe4_4d94_a1ce_88477ece143b */
