/****************************************************************************
 *            framebuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "framebuffer.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		String get_status_str(const GLuint status)
		{
			switch (status)
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
					return String(L"ATTACHMENT");
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					return String(L"MISSING_ATTACHMENT");
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
					return String(L"DRAW_BUFFER");
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
					return String(L"READ_BUFFER");
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
					return String(L"MULTISAMPLE");
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
					return String(L"LAYER_TARGETS");
				case GL_FRAMEBUFFER_UNSUPPORTED:
					return String(L"UNSUPPORTED");
				case GL_FRAMEBUFFER_COMPLETE:
					return String(L"COMPLETE");
				default:
					return String(L"Unknown");
			}
		}

	}

	FrameBuffer::FrameBuffer(): m_id(0)
	{
		assert(GLEW_ARB_framebuffer_object || GLEW_VERSION_3_0);

		glGenFramebuffers(1, &m_id);
	}

	FrameBuffer::~FrameBuffer() throw()
	{
		if (m_id != 0)
		{
			glDeleteFramebuffers(1, &m_id);
			m_id = 0;
		}
	}

	void FrameBuffer::check_status() const
	{
		GLuint status;

		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			EL_THROW_EXCEPTION(OpenGlException()
				<< errinfo_message(get_status_str(status))
				<< errinfo_gl_error(status));
		}

		CHECK_GL_ERROR();
	}

	bool FrameBuffer::get_status() const
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
			GL_FRAMEBUFFER_COMPLETE;
	}

	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	}

	void FrameBuffer::bind_read()
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
	}

	void FrameBuffer::bind_draw()
	{
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
	}

	void FrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}

