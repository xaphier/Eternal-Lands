/****************************************************************************
 *            xmlwriter.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a08cebb5_7815_4eae_a24f_7870891f66ec
#define	UUID_a08cebb5_7815_4eae_a24f_7870891f66ec

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

	class XmlWriter: public boost::noncopyable
	{
		private:
			xmlTextWriterPtr m_writer;

		public:
			XmlWriter(xmlBufferPtr buffer);
			XmlWriter(const String &file_name);
			~XmlWriter() throw();
			void start_element(const std::string &name);
			void end_element();
			void write_element(const std::string &name,
				const StringType &value);
			void write_bool_element(const std::string &name,
				const bool value);
			void write_float_element(const std::string &name,
				const float value);
			void write_int_element(const std::string &name,
				const Sint64 value);

	};

}

#endif	/* UUID_a08cebb5_7815_4eae_a24f_7870891f66ec */
