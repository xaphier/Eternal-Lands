/****************************************************************************
 *            extsimpleframebuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "extsimpleframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	ExtSimpleFrameBuffer::ExtSimpleFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height, const Uint16 mipmaps,
		const TextureFormatType format): AbstractFrameBuffer(name,
			width, height)
	{
		m_frame_buffer.bind();

		get_texture()->set_target(ttt_2d_texture);
		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(mipmaps);
		get_texture()->init(get_width(), get_height(), 0, mipmaps);

		if (TextureFormatUtil::get_depth(format))
		{
			get_texture()->attach_ext(GL_DEPTH_ATTACHMENT_EXT, 0,
				0);

			if (TextureFormatUtil::get_stencil(format))
			{
				get_texture()->attach_ext(
					GL_STENCIL_ATTACHMENT_EXT, 0, 0);
			}

			glDrawBuffer(GL_NONE);
		}
		else
		{
			get_texture()->attach_ext(GL_COLOR_ATTACHMENT0_EXT, 0,
				0);

			if (GLEW_EXT_packed_depth_stencil)
			{
				m_render_buffer.reset(new ExtRenderBuffer(
					get_width(), get_height(),
					tft_depth24_stencil8));
				m_render_buffer->bind_to_framebuffer(
					GL_DEPTH_ATTACHMENT_EXT);
				m_render_buffer->bind_to_framebuffer(
					GL_STENCIL_ATTACHMENT_EXT);
			}
			else
			{
				m_render_buffer.reset(new ExtRenderBuffer(
					get_width(), get_height(),
					tft_depth32));
				m_render_buffer->bind_to_framebuffer(
					GL_DEPTH_ATTACHMENT_EXT);
			}

			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		}

		CHECK_GL_ERROR();

		m_frame_buffer.unbind();
		m_frame_buffer.check_status();
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
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT |
			GL_COLOR_BUFFER_BIT);
	}

	void ExtSimpleFrameBuffer::unbind()
	{
		m_frame_buffer.unbind();
	}

}
