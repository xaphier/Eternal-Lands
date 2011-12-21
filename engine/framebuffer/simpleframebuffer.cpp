/****************************************************************************
 *            simpleframebuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "simpleframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	SimpleFrameBuffer::SimpleFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height, const Uint32 layers,
		const Uint16 mipmaps, const TextureFormatType format):
		AbstractFrameBuffer(name, width, height), m_layer(0)
	{
		m_frame_buffer.bind();

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

		m_depth = true;

		if (TextureFormatUtil::get_depth(format))
		{
			m_stencil = false;
			m_color = false;

			get_texture()->attach(GL_DEPTH_ATTACHMENT, 0, 0);

			if (TextureFormatUtil::get_stencil(format))
			{
				m_stencil = true;

				get_texture()->attach(GL_STENCIL_ATTACHMENT, 0,
					0);
			}

			glDrawBuffer(GL_NONE);
		}
		else
		{
			m_stencil = true;
			m_color = true;

			get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, 0);

			m_render_buffer.reset(new RenderBuffer(get_width(),
				get_height(), 0, tft_depth24_stencil8));
			m_render_buffer->bind_to_framebuffer(
				GL_DEPTH_ATTACHMENT);
			m_render_buffer->bind_to_framebuffer(
				GL_STENCIL_ATTACHMENT);

			glDrawBuffer(GL_COLOR_ATTACHMENT0);
		}

		glReadBuffer(GL_NONE);
		CHECK_GL_ERROR();

		m_frame_buffer.check_status();
		m_frame_buffer.unbind();
	}

	SimpleFrameBuffer::~SimpleFrameBuffer() throw()
	{
	}

	void SimpleFrameBuffer::do_bind(const Uint32 layer)
	{
		m_frame_buffer.bind();

		if (layer != m_layer)
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

		if (mask != 0)
		{
			glClear(mask);
		}

		if (m_color)
		{
			glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
		}
	}

	void SimpleFrameBuffer::unbind()
	{
		m_frame_buffer.unbind();
	}

}
