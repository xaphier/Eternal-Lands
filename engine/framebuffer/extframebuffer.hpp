/****************************************************************************
 *            extframebuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2a3e55b6_f548_4033_afa4_1af1ad6ea481
#define	UUID_2a3e55b6_f548_4033_afa4_1af1ad6ea481

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class ExtFrameBuffer.
 * This file contains the @c class ExtFrameBuffer.
 */
namespace eternal_lands
{

	class ExtFrameBuffer
	{
		private:
			GLuint m_id;

		public:
			ExtFrameBuffer();
			~ExtFrameBuffer() throw();
			void bind();
			void unbind();
			void check_status() const;
			bool get_status() const;

	};

}

#endif	/* UUID_2a3e55b6_f548_4033_afa4_1af1ad6ea481 */
