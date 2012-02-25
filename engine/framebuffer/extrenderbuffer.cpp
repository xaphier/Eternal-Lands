/****************************************************************************
 *            extrenderbuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "extrenderbuffer.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	ExtRenderBuffer::ExtRenderBuffer(const Uint32 width,
		const Uint32 height, const TextureFormatType format):
		m_id(0), m_width(width), m_height(height), m_format(format)
	{
		assert(GLEW_EXT_framebuffer_object);

		glGenRenderbuffersEXT(1, &m_id);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_id);

		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,
			TextureFormatUtil::get_gl_format(get_format()),
			get_width(), get_height());
	}

	ExtRenderBuffer::~ExtRenderBuffer() throw()
	{
		if (m_id != 0)
		{
			glDeleteRenderbuffersEXT(1, &m_id);
			m_id = 0;
		}
	}

	void ExtRenderBuffer::bind_to_framebuffer(const GLenum attachment)
	{
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment,
			GL_RENDERBUFFER_EXT, m_id);
	}

}
