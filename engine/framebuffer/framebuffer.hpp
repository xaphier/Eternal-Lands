/****************************************************************************
 *            framebuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5ee5718e_e602_4fb6_b9b7_ed82cf3bfac3
#define	UUID_5ee5718e_e602_4fb6_b9b7_ed82cf3bfac3

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class FrameBuffer.
 * This file contains the @c class FrameBuffer.
 */
namespace eternal_lands
{

	class FrameBuffer
	{
		private:
			GLuint m_id;

		public:
			FrameBuffer();
			~FrameBuffer() throw();
			void bind();
			void bind_read();
			void bind_draw();
			void unbind();
			void check_status() const;
			bool get_status() const;

	};

}

#endif	/* UUID_5ee5718e_e602_4fb6_b9b7_ed82cf3bfac3 */
