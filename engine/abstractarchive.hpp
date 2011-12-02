/****************************************************************************
 *            abstractarchive.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
 * @brief The @c class AbstractThreadTask.
 * This file contains the @c class AbstractThreadTask.
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
			virtual ~AbstractArchive() throw();

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
			 * Gets the name of the archive.
			 * @return The name of the archive.
			 */
			inline const String &get_name() const
			{
				return m_name;
			}

	};

}

#endif	/* UUID_71a035a9_6296_4e43_9328_2a73e196e471 */
