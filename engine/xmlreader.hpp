/****************************************************************************
 *            xmlreader.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			const XmlBufferSharedPtr m_buffer;
			xmlDocPtr m_doc;

			bool init_from_reader(const ReaderSharedPtr &reader);
			bool init_from_string(const std::string &str);

		public:
			XmlReader();
			XmlReader(const XmlBufferSharedPtr &buffer);
			XmlReader(const FileSystemSharedPtr &file_system,
				const String &file_name);
			XmlReader(const String &file_name);
			~XmlReader() noexcept;
			xmlNodePtr get_root_node();

			static bool get_xml_reader(
				const FileSystemSharedPtr &file_system,
				const String &file_name,
				XmlReaderSharedPtr &xml_reader);

			static XmlReaderSharedPtr get_xml_reader_from_string(
				const String &str);

	};

}

#endif	/* UUID_8b00810f_292a_4a95_9bfc_6f2886d11422 */
