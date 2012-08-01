/****************************************************************************
 *            renderbuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "renderbuffer.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	RenderBuffer::RenderBuffer(const Uint32 width, const Uint32 height,
		const Uint32 samples, const TextureFormatType format): m_id(0),
		m_width(width), m_height(height), m_samples(samples),
		m_format(format)
	{
		GLint max_samples;

		assert(GLEW_VERSION_3_0 || GL_ARB_framebuffer_object);

		glGenRenderbuffers(1, &m_id);
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);

		glGetIntegerv(GL_MAX_SAMPLES, &max_samples);

		if (static_cast<GLint>(samples) > max_samples)
		{
			LOG_WARNING(lt_framebuffer, UTF8("Can`t use '%1%' "
				"samples for frame buffer multisample, because"
				" extensions no more than '%2%' samples"
				" are supported."), samples % max_samples);
			m_samples = max_samples;
		}

		if (get_samples() > 0)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER,
				get_samples(),
				TextureFormatUtil::get_gl_format(get_format()),
				get_width(), get_height());
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER,
				TextureFormatUtil::get_gl_format(get_format()),
				get_width(), get_height());
		}
	}

	RenderBuffer::~RenderBuffer() noexcept
	{
		if (m_id != 0)
		{
			glDeleteRenderbuffers(1, &m_id);
			m_id = 0;
		}
	}

	void RenderBuffer::bind_to_framebuffer(const GLenum attachment)
	{
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment,
			GL_RENDERBUFFER, m_id);
	}

}
