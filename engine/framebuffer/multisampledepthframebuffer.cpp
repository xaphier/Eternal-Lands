/****************************************************************************
 *            multisampledepthframebuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "multisampledepthframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	MultiSampleDepthFrameBuffer::MultiSampleDepthFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height, const Uint32 layers,
		const Uint16 mipmaps, const Uint16 samples,
		const TextureFormatType format): AbstractFrameBuffer(name,
			width, height), m_layer(0)
	{
		if (layers > 0)
		{
			get_texture()->set_target(ttt_2d_texture_array);
		}
		else
		{
			get_texture()->set_target(ttt_2d_texture);
		}

		m_stencil = TextureFormatUtil::get_stencil(format);

		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(mipmaps);
		get_texture()->init(get_width(), get_height(), layers, mipmaps);

		m_multisample_frame_buffer.bind();

		m_render_buffer.reset(new RenderBuffer(width, height, samples,
			format));
		m_render_buffer->bind_to_framebuffer(GL_DEPTH_ATTACHMENT);

		if (m_stencil)
		{
			m_render_buffer->bind_to_framebuffer(
				GL_STENCIL_ATTACHMENT);
		}

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		m_multisample_frame_buffer.check_status();
		m_multisample_frame_buffer.unbind();

		m_frame_buffer.bind();
		get_texture()->attach(GL_DEPTH_ATTACHMENT, 0, 0);

		if (m_stencil)
		{
			get_texture()->attach(GL_STENCIL_ATTACHMENT, 0, 0);
		}

		m_layer = 0;

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		m_frame_buffer.check_status();
		m_frame_buffer.unbind();
	}

	MultiSampleDepthFrameBuffer::~MultiSampleDepthFrameBuffer() throw()
	{
	}

	void MultiSampleDepthFrameBuffer::bind(const Uint32 layer)
	{
		if (layer != m_layer)
		{
			m_frame_buffer.bind();

			get_texture()->attach(GL_DEPTH_ATTACHMENT, 0, layer);

			if (m_stencil)
			{
				get_texture()->attach(GL_STENCIL_ATTACHMENT, 0,
					layer);
			}

			m_layer = layer;
		}

		m_multisample_frame_buffer.bind();
	}

	void MultiSampleDepthFrameBuffer::bind_texture(const Uint32 layer)
	{
		m_frame_buffer.bind();

		if (layer != m_layer)
		{
			get_texture()->attach(GL_DEPTH_ATTACHMENT, 0, layer);

			if (m_stencil)
			{
				get_texture()->attach(GL_STENCIL_ATTACHMENT, 0,
					layer);
			}

			m_layer = layer;
		}
	}

	void MultiSampleDepthFrameBuffer::blit()
	{
		// blit from multisample buffer to final buffer, triggers resolve
		m_multisample_frame_buffer.bind(false);
		m_frame_buffer.bind(true);

		if (m_stencil)
		{
			glBlitFramebuffer(0, 0, get_width(), get_height(), 0, 0,
				get_width(), get_height(), GL_DEPTH_BUFFER_BIT |
					GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		}
		else
		{
			glBlitFramebuffer(0, 0, get_width(), get_height(), 0, 0,
				get_width(), get_height(), GL_DEPTH_BUFFER_BIT,
				GL_NEAREST);
		}
		
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void MultiSampleDepthFrameBuffer::clear(const glm::vec4 &color,
		const float depth)
	{
		if (m_stencil)
		{
			glClear(GL_STENCIL_BUFFER_BIT);
		}

		glClearBufferfv(GL_DEPTH, 0, &depth);
	}

	void MultiSampleDepthFrameBuffer::clear(const glm::vec4 &color)
	{
		if (m_stencil)
		{
			glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}
	}

	void MultiSampleDepthFrameBuffer::unbind()
	{
		m_multisample_frame_buffer.unbind();
	}

}
