/****************************************************************************
 *            extframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "extframebuffer.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	namespace
	{

		String get_status_str(const GLuint status)
		{
			switch (status)
			{
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
					return String(UTF8("ATTACHMENT"));
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
					return String(UTF8(
						"MISSING_ATTACHMENT"));
				case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
					return String(UTF8("DIMENSIONS"));
				case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
					return String(UTF8("FORMATS"));
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
					return String(UTF8("DRAW_BUFFER"));
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
					return String(UTF8("READ_BUFFER"));
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT:
					return String(UTF8("MULTISAMPLE"));
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT:
					return String(UTF8("LAYER_TARGETS"));
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT:
					return String(UTF8("LAYER_COUNT"));
				case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
					return String(UTF8("UNSUPPORTED"));
				case GL_FRAMEBUFFER_COMPLETE_EXT:
					return String(UTF8("COMPLETE"));
				default:
					return String(UTF8("Unknown"));
			}
		}

	}

	ExtFrameBuffer::ExtFrameBuffer(): m_id(0)
	{
		assert(GLEW_EXT_framebuffer_object);

		glGenFramebuffersEXT(1, &m_id);
	}

	ExtFrameBuffer::~ExtFrameBuffer() throw()
	{
		if (m_id != 0)
		{
			glDeleteFramebuffersEXT(1, &m_id);
			m_id = 0;
		}
	}

	void ExtFrameBuffer::check_status() const
	{
		GLuint status;

		status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);

		if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
		{
			EL_THROW_EXCEPTION(OpenGlException()
				<< errinfo_message(get_status_str(status))
				<< errinfo_code(status));
		}
	}

	bool ExtFrameBuffer::get_status() const
	{
		return glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) ==
			GL_FRAMEBUFFER_COMPLETE_EXT;
	}

	void ExtFrameBuffer::unbind()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	}

	void ExtFrameBuffer::bind()
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_id);
	}

	void ExtFrameBuffer::bind(const bool draw)
	{
		if (draw)
		{
			glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, m_id);

			return;
		}

		glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, m_id);
	}

}
