/****************************************************************************
 *            simpleframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "simpleframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	SimpleFrameBuffer::SimpleFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height, const Uint32 depth,
		const Uint16 mipmaps, const TextureTargetType target,
		const TextureFormatType format, const bool depth_buffer):
		AbstractFrameBuffer(name, width, height, depth), m_layer(0)
	{
		DEBUG_CHECK_GL_ERROR();

		m_frame_buffer.bind();

		get_texture()->set_target(target);
		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(mipmaps);

		DEBUG_CHECK_GL_ERROR();

		get_texture()->init(get_width(), get_height(), get_depth(),
			mipmaps);

		DEBUG_CHECK_GL_ERROR();

		set_stencil_buffer(false);

		if (TextureFormatUtil::get_depth(format))
		{
			set_depth_buffer(true);
			set_color_buffer(false);

			get_texture()->attach(GL_DEPTH_ATTACHMENT, 0, 0);

			DEBUG_CHECK_GL_ERROR();

			if (TextureFormatUtil::get_stencil(format))
			{
				set_stencil_buffer(true);

				get_texture()->attach(GL_STENCIL_ATTACHMENT, 0,
					0);
			}

			glDrawBuffer(GL_NONE);

			DEBUG_CHECK_GL_ERROR();
		}
		else
		{
			set_depth_buffer(depth_buffer);
			set_color_buffer(true);

			get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, 0);

			DEBUG_CHECK_GL_ERROR();

			if (get_depth_buffer())
			{
				set_stencil_buffer(true);

				m_render_buffer.reset(new RenderBuffer(
					get_width(), get_height(), 0,
					tft_depth24_stencil8));

				DEBUG_CHECK_GL_ERROR();

				m_render_buffer->bind_to_framebuffer(
					GL_DEPTH_ATTACHMENT);

				DEBUG_CHECK_GL_ERROR();

				m_render_buffer->bind_to_framebuffer(
					GL_STENCIL_ATTACHMENT);
			}

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			DEBUG_CHECK_GL_ERROR();
		}

		glReadBuffer(GL_NONE);

		m_frame_buffer.check_status();
		m_frame_buffer.unbind();

		DEBUG_CHECK_GL_ERROR();
	}

	SimpleFrameBuffer::~SimpleFrameBuffer() throw()
	{
	}

	void SimpleFrameBuffer::do_bind(const Uint32 layer)
	{
		m_frame_buffer.bind();

		if (layer != get_layer())
		{
			get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, layer);
			m_layer = layer;
		}
	}

	void SimpleFrameBuffer::bind(const Uint32 layer)
	{
		do_bind(layer);
	}

	void SimpleFrameBuffer::bind_texture(const Uint32 layer)
	{
		do_bind(layer);
	}

	void SimpleFrameBuffer::blit()
	{
	}

	void SimpleFrameBuffer::clear(const glm::vec4 &color)
	{
		if (get_depth_buffer())
		{
			glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
		}

		if (get_color_buffer())
		{
			glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
		}
	}

	void SimpleFrameBuffer::clear(const glm::vec4 &color,
		const float depth)
	{
		if (get_depth_buffer())
		{
			glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, 0);
		}

		if (get_color_buffer())
		{
			glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
		}
	}

	void SimpleFrameBuffer::unbind()
	{
		m_frame_buffer.unbind();
	}

}
