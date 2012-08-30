/****************************************************************************
 *            extrenderbuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "extrenderbuffer.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	ExtRenderBuffer::ExtRenderBuffer(const String &name,
		const Uint32 width, const Uint32 height,
		const TextureFormatType format): AbstractRenderTarget(name,
			width, height, 0, format), m_id(0)
	{
		assert(GLEW_EXT_framebuffer_object);

		glGenRenderbuffersEXT(1, &m_id);
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_id);

		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT,
			TextureFormatUtil::get_gl_format(get_format()),
			get_width(), get_height());
	}

	ExtRenderBuffer::~ExtRenderBuffer() noexcept
	{
		if (m_id != 0)
		{
			glDeleteRenderbuffersEXT(1, &m_id);
			m_id = 0;
		}
	}

	void ExtRenderBuffer::attach(const GLenum attachment,
		const CubeMapFaceType face, const Uint32 level,
		const Uint32 layer)
	{
		attach(attachment);
	}

	void ExtRenderBuffer::attach(const GLenum attachment)
	{
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachment,
			GL_RENDERBUFFER_EXT, m_id);
	}

}
