/****************************************************************************
 *            framebufferbuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "framebufferbuilder.hpp"
#include "framebuffer/extsimpleframebuffer.hpp"
#include "framebuffer/simpleframebuffer.hpp"
#include "framebuffer/multisampleframebuffer.hpp"
#include "framebuffer/filterframebuffer.hpp"

namespace eternal_lands
{
#if	0
	FrameBuffer::FrameBuffer(const String &name,
		const TextureFormatType format, const Uint32 width,
		const Uint32 height, const Uint32 depth, const Uint16 mipmaps,
		const Uint16 samples): m_name(name), m_id(0),
		m_multisample_id(0), m_width(width), m_height(height),
		m_depth(depth), m_mipmaps(mipmaps), m_samples(samples),
		m_format(format)
	{
		if (get_depth() > 0)
		{
			init(ttt_2d_texture_array);
		}
		else
		{
			init(ttt_2d_texture);
		}
	}

	FrameBuffer::FrameBuffer(const String &name,
		const TextureFormatType format, const Uint32 width,
		const Uint32 height): m_name(name), m_id(0),
		m_multisample_id(0), m_width(width), m_height(height),
		m_depth(0), m_mipmaps(0), m_samples(0), m_format(format)
	{
		init(ttt_texture_rectangle);
	}

	FrameBuffer::~FrameBuffer() throw()
	{
		if (m_id != 0)
		{
			glDeleteFramebuffers(1, &m_id);
			m_id = 0;
		}

		if (m_multisample_id != 0)
		{
			glDeleteFramebuffers(1, &m_multisample_id);
			m_multisample_id = 0;
		}
	}

	void FrameBuffer::init(const TextureTargetType target)
	{
		String texture_name;
		GLuint status;

		m_depth_only = TextureFormatUtil::get_depth(get_format());

		glGenFramebuffers(1, &m_id);

		if (get_samples() > 0)
		{
			glGenFramebuffers(1, &m_multisample_id);

			glBindFramebuffer(GL_FRAMEBUFFER, m_multisample_id);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_id);
		}

		CHECK_GL_ERROR();

		if (get_depth_only())
		{
			texture_name = String(get_name().get() + L":depth");
		}
		else
		{
			texture_name = String(get_name().get() + L":color");
		}

		m_texture = boost::make_shared<Texture>(texture_name);

		m_texture->set_target(target);
		m_texture->set_format(get_format());
		m_texture->set_wrap_s(twt_clamp);
		m_texture->set_wrap_t(twt_clamp);
		m_texture->set_wrap_r(twt_clamp);

		m_texture->set_mipmap_count(0);
		m_texture->init(get_width(), get_height(), get_depth(),
			get_mipmaps());

		CHECK_GL_ERROR();

		if (get_depth_only())
		{
			m_texture->attach(GL_DEPTH_ATTACHMENT, 0, 0);

			CHECK_GL_ERROR();

			if (TextureFormatUtil::get_stencil(get_format()))
			{
				m_texture->attach(GL_STENCIL_ATTACHMENT, 0, 0);
			}

			CHECK_GL_ERROR();

			glDrawBuffer(GL_NONE);
		}
		else
		{
			m_texture->attach(GL_COLOR_ATTACHMENT0, 0, 0);

			if (get_samples() > 0)
			{
				glDrawBuffer(GL_COLOR_ATTACHMENT0);

				glBindFramebuffer(GL_FRAMEBUFFER, m_id);

				m_multisample_render_buffer.reset(new
					RenderBuffer(get_width(), get_height(),
						get_samples(),
						m_texture->get_gl_format()));
				m_multisample_render_buffer->bind_to_framebuffer(
					GL_COLOR_ATTACHMENT0);
			}

			m_render_buffer.reset(new RenderBuffer(get_width(),
				get_height(), get_samples(),
				GL_DEPTH24_STENCIL8));
			m_render_buffer->bind_to_framebuffer(
				GL_DEPTH_ATTACHMENT);
			m_render_buffer->bind_to_framebuffer(
				GL_STENCIL_ATTACHMENT);

			glDrawBuffer(GL_COLOR_ATTACHMENT0);
		}

		CHECK_GL_ERROR();

		if (get_samples() > 0)
		{
			glReadBuffer(GL_COLOR_ATTACHMENT0);
		}
		else
		{
			glReadBuffer(GL_NONE);
		}

		CHECK_GL_ERROR();

		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			EL_THROW_EXCEPTION(OpenGlException()
				<< errinfo_message(get_status_str(status))
				<< errinfo_gl_error(status));
		}

		unbind();

		CHECK_GL_ERROR();
	}

	void FrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void FrameBuffer::swap_buffers()
	{
		assert(get_samples() > 0);
		assert(m_multisample_id != 0);

		std::swap(m_id, m_multisample_id);
	}

	void FrameBuffer::blit_buffers(const Uint32 layer,
		const Uint16 mipmap)
	{
		assert(get_samples() > 0);
		assert(m_id != 0);
		assert(m_multisample_id != 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_multisample_id);

		assert(!TextureFormatUtil::get_depth(get_format()));

		m_texture->attach(GL_COLOR_ATTACHMENT0, mipmap, layer);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// blit from multisample buffer to final buffer, triggers resolve
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_multisample_id);

		glBlitFramebuffer(0, 0, get_width(), get_height(), 0, 0,
			get_width(), get_height(), GL_COLOR_BUFFER_BIT,
			GL_NEAREST);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		glViewport(0, 0, get_width(), get_height());
	}

	void FrameBuffer::switch_layer(const Uint32 layer,
		const Uint16 mipmap)
	{
		assert((layer < get_depth()) || (layer == 0));
		assert(mipmap <= get_mipmaps());

		glViewport(0, 0, (get_width() >> mipmap),
			(get_height() >> mipmap));

		if (TextureFormatUtil::get_depth(get_format()))
		{
			m_texture->attach(GL_DEPTH_ATTACHMENT, mipmap, layer);

			if (TextureFormatUtil::get_stencil(get_format()))
			{
				m_texture->attach(GL_STENCIL_ATTACHMENT,
					mipmap, layer);
			}
		}
		else
		{
			m_texture->attach(GL_COLOR_ATTACHMENT0, mipmap, layer);
		}
	}

	void FrameBuffer::switch_mipmap(const Uint16 mipmap)
	{
		assert(mipmap <= get_mipmaps());

		glViewport(0, 0, (get_width() >> mipmap),
			(get_height() >> mipmap));

		if (TextureFormatUtil::get_depth(get_format()))
		{
			m_texture->attach(GL_DEPTH_ATTACHMENT, mipmap);

			if (TextureFormatUtil::get_stencil(get_format()))
			{
				m_texture->attach(GL_STENCIL_ATTACHMENT,
					mipmap);
			}
		}
		else
		{
			m_texture->attach(GL_COLOR_ATTACHMENT0, mipmap);
		}
	}
#endif

	AbstractFrameBufferSharedPtr FrameBufferBuilder::build(
		const String &name, const Uint32 width, const Uint32 height,
		const Uint16 mipmaps, const TextureFormatType format)
	{
		if (GLEW_ARB_framebuffer_object || GLEW_VERSION_3_0)
		{
			return AbstractFrameBufferSharedPtr(
				new SimpleFrameBuffer(name, width, height, 0,
					mipmaps, format));
		}
		else
		{
			return AbstractFrameBufferSharedPtr(
				new ExtSimpleFrameBuffer(name, width, height,
					mipmaps, format));
		}
	}

	AbstractFrameBufferSharedPtr FrameBufferBuilder::build_filter(
		const String &name, const Uint32 width, const Uint32 height,
		const TextureFormatType format)
	{
		return AbstractFrameBufferSharedPtr(new FilterFrameBuffer(name,
			width, height, format));
	}

	AbstractFrameBufferSharedPtr FrameBufferBuilder::build(
		const String &name, const Uint32 width, const Uint32 height,
		const Uint32 layers, const Uint16 mipmaps, const Uint16 samples,
		const TextureFormatType format)
	{
		if (samples == 0)
		{
			return AbstractFrameBufferSharedPtr(
				new SimpleFrameBuffer(name, width, height,
					layers, mipmaps, format));
		}
		else
		{
			return AbstractFrameBufferSharedPtr(
				new MultiSampleFrameBuffer(name, width, height,
					layers, mipmaps, samples, format));
		}
	}

}
