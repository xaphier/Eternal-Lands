/****************************************************************************
 *            multisampleframebuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "multisampleframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	MultiSampleFrameBuffer::MultiSampleFrameBuffer(const String &name,
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

		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(mipmaps);
		get_texture()->init(get_width(), get_height(), layers, mipmaps);

		m_multisample_frame_buffer.bind();

		m_depth_render_buffer.reset(new RenderBuffer(width, height,
			samples, tft_depth24_stencil8));
		m_depth_render_buffer->bind_to_framebuffer(
			GL_DEPTH_ATTACHMENT);
		m_depth_render_buffer->bind_to_framebuffer(
			GL_STENCIL_ATTACHMENT);

		m_render_buffer.reset(new RenderBuffer(width, height, samples,
			format));
		m_render_buffer->bind_to_framebuffer(GL_COLOR_ATTACHMENT0);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		m_multisample_frame_buffer.check_status();
		m_multisample_frame_buffer.unbind();

		m_frame_buffer.bind();
		get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, 0);
		m_layer = 0;

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_NONE);

		m_frame_buffer.check_status();
		m_frame_buffer.unbind();
	}

	MultiSampleFrameBuffer::~MultiSampleFrameBuffer() throw()
	{
	}

	void MultiSampleFrameBuffer::bind(const Uint32 layer)
	{
		if (layer != m_layer)
		{
			m_frame_buffer.bind();
			get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, layer);
			m_layer = layer;
		}

		m_multisample_frame_buffer.bind();
	}

	void MultiSampleFrameBuffer::bind_texture(const Uint32 layer)
	{
		m_frame_buffer.bind();

		if (layer != m_layer)
		{
			get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, layer);
			m_layer = layer;
		}
	}

	void MultiSampleFrameBuffer::blit()
	{
		// blit from multisample buffer to final buffer, triggers resolve
		m_multisample_frame_buffer.bind(false);
		m_frame_buffer.bind(true);

		glBlitFramebuffer(0, 0, get_width(), get_height(), 0, 0,
			get_width(), get_height(), GL_COLOR_BUFFER_BIT,
			GL_NEAREST);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void MultiSampleFrameBuffer::clear(const glm::vec4 &color)
	{
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
	}

	void MultiSampleFrameBuffer::unbind()
	{
		m_multisample_frame_buffer.unbind();
	}

}
