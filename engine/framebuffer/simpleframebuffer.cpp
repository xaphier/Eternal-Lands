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
		CHECK_GL_ERROR();

		m_frame_buffer.bind();

		get_texture()->set_target(target);
		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(mipmaps);

		CHECK_GL_ERROR_NAME(get_name());

		get_texture()->init(get_width(), get_height(), get_depth(),
			mipmaps);

		CHECK_GL_ERROR_NAME(get_name());

		set_stencil_buffer(false);

		if (TextureFormatUtil::get_depth(format))
		{
			set_depth_buffer(true);
			set_color_buffer(false);

			get_texture()->attach(GL_DEPTH_ATTACHMENT, 0, 0);

			CHECK_GL_ERROR_NAME(get_name());

			if (TextureFormatUtil::get_stencil(format))
			{
				set_stencil_buffer(true);

				get_texture()->attach(GL_STENCIL_ATTACHMENT, 0,
					0);
			}

			glDrawBuffer(GL_NONE);

			CHECK_GL_ERROR_NAME(get_name());
		}
		else
		{
			set_depth_buffer(depth_buffer);
			set_color_buffer(true);

			get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, 0);

			CHECK_GL_ERROR_NAME(get_name());

			if (get_depth_buffer())
			{
				set_stencil_buffer(true);

				m_render_buffer.reset(new RenderBuffer(
					get_width(), get_height(), 0,
					tft_depth24_stencil8));

				CHECK_GL_ERROR_NAME(get_name());

				m_render_buffer->bind_to_framebuffer(
					GL_DEPTH_ATTACHMENT);

				CHECK_GL_ERROR_NAME(get_name());

				m_render_buffer->bind_to_framebuffer(
					GL_STENCIL_ATTACHMENT);
			}

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			CHECK_GL_ERROR_NAME(get_name());
		}

		glReadBuffer(GL_NONE);

		m_frame_buffer.check_status();
		m_frame_buffer.unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	SimpleFrameBuffer::~SimpleFrameBuffer() noexcept
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

		m_frame_buffer.check_status();
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

	void SimpleFrameBuffer::blit_to_back_buffer(const glm::uvec4 &rect,
		const Uint16 layer, const bool color, const bool depth,
		const bool stencil)
	{
		GLbitfield mask;

		mask = 0;

		if (color)
		{
			mask |= GL_COLOR_BUFFER_BIT;
		}

		if (color)
		{
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		if (color)
		{
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		if (mask == 0)
		{
			return;
		}

		m_frame_buffer.bind(false);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, 0);

		glReadBuffer(GL_COLOR_ATTACHMENT0 + layer);
		glDrawBuffer(GL_BACK);

		glBlitFramebuffer(0, 0, get_width(), get_height(), rect.x,
			rect.y, rect.z, rect.w, mask, GL_NEAREST);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

}
