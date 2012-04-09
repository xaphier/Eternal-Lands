/****************************************************************************
 *            extsimpleframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "extsimpleframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	ExtSimpleFrameBuffer::ExtSimpleFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height, const Uint16 mipmaps,
		const TextureFormatType format, const bool depth):
		AbstractFrameBuffer(name, width, height)
	{
		DEBUG_CHECK_GL_ERROR();

		m_frame_buffer.bind();

		get_texture()->set_target(ttt_2d_texture);
		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(mipmaps);

		DEBUG_CHECK_GL_ERROR();

		get_texture()->init(get_width(), get_height(), 0, mipmaps);

		DEBUG_CHECK_GL_ERROR();

		m_depth = depth;
		m_stencil = false;

		if (TextureFormatUtil::get_depth(format))
		{
			m_depth = true;
			m_color = false;

			get_texture()->attach_ext(GL_DEPTH_ATTACHMENT_EXT, 0,
				0);

			DEBUG_CHECK_GL_ERROR();

			if (TextureFormatUtil::get_stencil(format))
			{
				m_stencil = true;

				get_texture()->attach_ext(
					GL_STENCIL_ATTACHMENT_EXT, 0, 0);
			}

			glDrawBuffer(GL_NONE);

			DEBUG_CHECK_GL_ERROR();
		}
		else
		{
			m_depth = depth;
			m_color = true;

			get_texture()->attach_ext(GL_COLOR_ATTACHMENT0_EXT, 0,
				0);

			DEBUG_CHECK_GL_ERROR();

			if (m_depth)
			{
				if (GLEW_EXT_packed_depth_stencil)
				{
					m_stencil = true;

					m_render_buffer.reset(
						new ExtRenderBuffer(
							get_width(),
							get_height(),
							tft_depth24_stencil8));

					DEBUG_CHECK_GL_ERROR();

					m_render_buffer->bind_to_framebuffer(
						GL_DEPTH_ATTACHMENT_EXT);

					DEBUG_CHECK_GL_ERROR();

					m_render_buffer->bind_to_framebuffer(
						GL_STENCIL_ATTACHMENT_EXT);
				}
				else
				{
					m_render_buffer.reset(
						new ExtRenderBuffer(
							get_width(),
							get_height(),
							tft_depth32));

					DEBUG_CHECK_GL_ERROR();

					m_render_buffer->bind_to_framebuffer(
						GL_DEPTH_ATTACHMENT_EXT);
				}
			}

			DEBUG_CHECK_GL_ERROR();

			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		}

		glReadBuffer(GL_NONE);

		DEBUG_CHECK_GL_ERROR();

		m_frame_buffer.check_status();
		m_frame_buffer.unbind();

		DEBUG_CHECK_GL_ERROR();
	}

	ExtSimpleFrameBuffer::~ExtSimpleFrameBuffer() throw()
	{
	}

	void ExtSimpleFrameBuffer::bind(const Uint32 layer)
	{
		assert(layer == 0);

		m_frame_buffer.bind();
	}

	void ExtSimpleFrameBuffer::bind_texture(const Uint32 layer)
	{
		assert(layer == 0);

		m_frame_buffer.bind();
	}

	void ExtSimpleFrameBuffer::blit()
	{
	}

	void ExtSimpleFrameBuffer::clear(const glm::vec4 &color)
	{
		GLenum mask;

		mask = 0;

		if (m_depth)
		{
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		if (m_stencil)
		{
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		if (m_color)
		{
			mask |= GL_COLOR_BUFFER_BIT;
		}

		glClear(mask);
	}

	void ExtSimpleFrameBuffer::clear(const glm::vec4 &color,
		const float depth)
	{
		GLenum mask;

		mask = 0;

		if (m_depth)
		{
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		if (m_stencil)
		{
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		if (m_color)
		{
			mask |= GL_COLOR_BUFFER_BIT;
		}

		glClear(mask);
	}

	void ExtSimpleFrameBuffer::unbind()
	{
		m_frame_buffer.unbind();
	}

}
