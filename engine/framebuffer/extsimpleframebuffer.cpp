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
		const Uint32 width, const Uint32 height, const Uint32 depth,
		const Uint16 mipmaps, const TextureTargetType target,
		const TextureFormatType format, const bool depth_buffer):
		AbstractFrameBuffer(name, width, height, depth)
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

			get_texture()->attach_ext(GL_DEPTH_ATTACHMENT_EXT, 0,
				0);

			DEBUG_CHECK_GL_ERROR();

			if (TextureFormatUtil::get_stencil(format))
			{
				set_stencil_buffer(true);

				get_texture()->attach_ext(
					GL_STENCIL_ATTACHMENT_EXT, 0, 0);
			}

			glDrawBuffer(GL_NONE);

			DEBUG_CHECK_GL_ERROR();
		}
		else
		{
			set_depth_buffer(depth_buffer);
			set_color_buffer(true);

			get_texture()->attach_ext(GL_COLOR_ATTACHMENT0_EXT, 0,
				0);

			DEBUG_CHECK_GL_ERROR();

			if (get_depth_buffer())
			{
				if (GLEW_EXT_packed_depth_stencil)
				{
					set_stencil_buffer(true);

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

	void ExtSimpleFrameBuffer::do_bind(const Uint32 layer)
	{
		m_frame_buffer.bind();

		if (layer != get_layer())
		{
			get_texture()->attach_ext(GL_COLOR_ATTACHMENT0_EXT, 0,
				layer);
			m_layer = layer;
		}

		m_frame_buffer.check_status();
	}

	void ExtSimpleFrameBuffer::bind(const Uint32 layer)
	{
		do_bind(layer);
	}

	void ExtSimpleFrameBuffer::bind_texture(const Uint32 layer)
	{
		do_bind(layer);
	}

	void ExtSimpleFrameBuffer::blit()
	{
	}

	void ExtSimpleFrameBuffer::clear(const glm::vec4 &color)
	{
		GLenum mask;

		mask = 0;

		if (get_depth_buffer())
		{
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		if (get_stencil_buffer())
		{
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		if (get_color_buffer())
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

		if (get_depth_buffer())
		{
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		if (get_stencil_buffer())
		{
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		if (get_color_buffer())
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
