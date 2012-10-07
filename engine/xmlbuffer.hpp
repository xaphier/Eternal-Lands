/****************************************************************************
 *            xmlbuffer.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_0a5fd92e_e647_4394_b804_7397ffda3dd4
#define	UUID_0a5fd92e_e647_4394_b804_7397ffda3dd4

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class XmlBuffer.
 * This file contains the @c class XmlBuffer.
 */
namespace eternal_lands
{

	class XmlBuffer: public boost::noncopyable
	{
		private:
			xmlBufferPtr m_buffer;

		public:
			XmlBuffer();
			XmlBuffer(const char* str);
			~XmlBuffer() noexcept;
			String get_string() const;
			Uint32 get_size() const;

			inline xmlBufferPtr get_buffer()
			{
				return m_buffer;
			}

			inline const xmlBufferPtr get_buffer() const
			{
				return m_buffer;
			}

	};

}

#endif	/* UUID_0a5fd92e_e647_4394_b804_7397ffda3dd4 */
