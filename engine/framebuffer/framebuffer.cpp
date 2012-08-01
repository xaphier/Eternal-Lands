/****************************************************************************
 *            framebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
					return String(UTF8("ATTACHMENT"));
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
					return String(UTF8(
						"MISSING_ATTACHMENT"));
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
					return String(UTF8("DRAW_BUFFER"));
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
					return String(UTF8("READ_BUFFER"));
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
					return String(UTF8("MULTISAMPLE"));
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
					return String(UTF8("LAYER_TARGETS"));
				case GL_FRAMEBUFFER_UNSUPPORTED:
					return String(UTF8("UNSUPPORTED"));
				case GL_FRAMEBUFFER_COMPLETE:
					return String(UTF8("COMPLETE"));
				default:
					return String(UTF8("Unknown"));
			}
		}

	}

	FrameBuffer::FrameBuffer(): m_id(0)
	{
		assert(GLEW_VERSION_3_0 || GL_ARB_framebuffer_object);

		glGenFramebuffers(1, &m_id);
	}

	FrameBuffer::~FrameBuffer() noexcept
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
				<< errinfo_opengl_error(status));
		}
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

	void FrameBuffer::bind(const bool draw)
	{
		if (draw)
		{
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);

			return;
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
	}

	void FrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
