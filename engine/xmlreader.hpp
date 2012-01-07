/****************************************************************************
 *            xmlreader.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8b00810f_292a_4a95_9bfc_6f2886d11422
#define	UUID_8b00810f_292a_4a95_9bfc_6f2886d11422

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class XmlWriter.
 * This file contains the @c class XmlWriter.
 */
namespace eternal_lands
{

	class XmlReader: public boost::noncopyable
	{
		private:
			xmlDocPtr m_doc;

		public:
			XmlReader(xmlBufferPtr buffer);
			XmlReader(const String &file_name);
			XmlReader(const FileSystemSharedPtr &file_system,
				const String &file_name);
			XmlReader(const ReaderSharedPtr &reader);
			~XmlReader() throw();
			xmlNodePtr get_root_node();

	};

}

#endif	/* UUID_8b00810f_292a_4a95_9bfc_6f2886d11422 */
