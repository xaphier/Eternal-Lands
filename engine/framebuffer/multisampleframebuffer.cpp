/****************************************************************************
 *            multisampleframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "multisampleframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	MultiSampleFrameBuffer::MultiSampleFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height, const Uint32 depth,
		const Uint16 mipmaps, const Uint16 samples,
		const TextureTargetType target,	const TextureFormatType format,
		const bool depth_buffer): AbstractFrameBuffer(name, width,
			height, depth), m_layer(0)
	{
		CHECK_GL_ERROR();

		get_texture()->set_target(target);

		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(mipmaps);

		set_depth_buffer(true);
		set_color_buffer(true);
		set_stencil_buffer(true);

		CHECK_GL_ERROR_NAME(get_name());

		get_texture()->init(get_width(), get_height(), get_depth(),
			mipmaps);

		CHECK_GL_ERROR_NAME(get_name());

		m_multisample_frame_buffer.bind();

		CHECK_GL_ERROR_NAME(get_name());

		m_depth_render_buffer.reset(new RenderBuffer(width, height,
			samples, tft_depth24_stencil8));

		CHECK_GL_ERROR_NAME(get_name());

		m_depth_render_buffer->bind_to_framebuffer(
			GL_DEPTH_ATTACHMENT);

		CHECK_GL_ERROR_NAME(get_name());

		m_depth_render_buffer->bind_to_framebuffer(
			GL_STENCIL_ATTACHMENT);

		CHECK_GL_ERROR_NAME(get_name());

		m_render_buffer.reset(new RenderBuffer(width, height, samples,
			format));

		CHECK_GL_ERROR_NAME(get_name());

		m_render_buffer->bind_to_framebuffer(GL_COLOR_ATTACHMENT0);

		CHECK_GL_ERROR_NAME(get_name());

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		m_multisample_frame_buffer.check_status();
		m_multisample_frame_buffer.unbind();

		CHECK_GL_ERROR_NAME(get_name());

		m_frame_buffer.bind();

		CHECK_GL_ERROR_NAME(get_name());

		get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, 0);
		m_layer = 0;

		CHECK_GL_ERROR_NAME(get_name());

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_NONE);

		m_frame_buffer.check_status();
		m_frame_buffer.unbind();

		CHECK_GL_ERROR_NAME(get_name());
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

			m_frame_buffer.check_status();
		}

		m_multisample_frame_buffer.bind();

		m_multisample_frame_buffer.check_status();
	}

	void MultiSampleFrameBuffer::bind_texture(const Uint32 layer)
	{
		m_frame_buffer.bind();

		if (layer != m_layer)
		{
			get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, layer);
			m_layer = layer;
		}

		m_frame_buffer.check_status();
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
		glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
	}

	void MultiSampleFrameBuffer::clear(const glm::vec4 &color,
		const float depth)
	{
		glClear(GL_STENCIL_BUFFER_BIT);
		glClearBufferfv(GL_DEPTH, 0, &depth);
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
	}

	void MultiSampleFrameBuffer::unbind()
	{
		m_multisample_frame_buffer.unbind();
	}

}
