/****************************************************************************
 *            abstractarchive.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_71a035a9_6296_4e43_9328_2a73e196e471
#define	UUID_71a035a9_6296_4e43_9328_2a73e196e471

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class AbstractArchive.
 * This file contains the @c class AbstractArchive.
 */
namespace eternal_lands
{

	class AbstractArchive: public boost::noncopyable
	{
		private:
			String m_name;

		protected:
			/**
			 * Default constructor.
			 */
			AbstractArchive(const String &name);

		public:
			/**
			 * Default destructor.
			 */
			virtual ~AbstractArchive() noexcept;

			/**
			 * Opens the file for reading.
			 * @return The Reader object of the file.
			 */
			virtual ReaderSharedPtr get_file(
				const String &file_name) const = 0;

			/**
			 * Checks if the file can be found in the archive.
			 * @return True if the file is in the archive, else
			 * false
			 */
			virtual bool get_has_file(const String &file_name)
				const = 0;

			/**
			 * Adds all files in the directory recursively
			 * that match the given pattern to the given set.
			 * @param dir The directory to start with.
			 * @param pattern The pattern the files must match.
			 * @param files The set of files where the found
			 * files are added.
			 */
			virtual void get_files(const String &dir,
				const String &pattern, StringSet &files)
				const = 0;

			/**
			 * Gets the name of the archive.
			 * @return The name of the archive.
			 */
			inline const String &get_name() const noexcept
			{
				return m_name;
			}

	};

}

#endif	/* UUID_71a035a9_6296_4e43_9328_2a73e196e471 */
