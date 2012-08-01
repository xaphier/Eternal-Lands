/****************************************************************************
 *            xmlbuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
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

	class XmlBuffer
	{
		private:
			xmlBufferPtr m_buffer;

		public:
			inline XmlBuffer(): m_buffer(0)
			{
				m_buffer = xmlBufferCreate();
			}

			inline XmlBuffer(const char* str): m_buffer(0)
			{
				m_buffer = xmlBufferCreateStatic(
					const_cast<char*>(str), strlen(str));
			}

			inline ~XmlBuffer() throw()
			{
				xmlBufferFree(m_buffer);
				m_buffer = 0;
			}

			inline xmlBufferPtr &get_buffer()
			{
				return m_buffer;
			}

			inline const xmlBufferPtr &get_buffer() const
			{
				return m_buffer;
			}

	};

}

#endif	/* UUID_0a5fd92e_e647_4394_b804_7397ffda3dd4 */
