/****************************************************************************
 *            simpleframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "simpleframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	SimpleFrameBuffer::SimpleFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height,
		const bool depth_buffer):
		AbstractFrameBuffer(name, width, height)
	{
		CHECK_GL_ERROR();

		m_frame_buffer.bind();

		if (depth_buffer)
		{
			m_render_buffer.reset(new RenderBuffer(name,
				get_width(0), get_height(0), 0,
				tft_depth24_stencil8));

			CHECK_GL_ERROR_NAME(get_name());

			attach_depth_render_buffer();
		}

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

//		m_frame_buffer.check_status();
		m_frame_buffer.unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	SimpleFrameBuffer::~SimpleFrameBuffer() noexcept
	{
	}

	void SimpleFrameBuffer::bind()
	{
		m_frame_buffer.bind();
	}

	void SimpleFrameBuffer::detach_texture(
		const FrameBufferAttachmentType attachment)
	{
		GLenum gl_attachment;

		switch (attachment)
		{
			case fbat_depth:
				gl_attachment = GL_DEPTH_ATTACHMENT;
				break;
			case fbat_stencil:
				gl_attachment = GL_STENCIL_ATTACHMENT;
				break;
			case fbat_color_0:
				gl_attachment = GL_COLOR_ATTACHMENT0;
				break;
			case fbat_color_1:
				gl_attachment = GL_COLOR_ATTACHMENT1;
				break;
			case fbat_color_2:
				gl_attachment = GL_COLOR_ATTACHMENT2;
				break;
			case fbat_color_3:
				gl_attachment = GL_COLOR_ATTACHMENT3;
				break;
			case fbat_color_4:
				gl_attachment = GL_COLOR_ATTACHMENT4;
				break;
			case fbat_color_5:
				gl_attachment = GL_COLOR_ATTACHMENT5;
				break;
			case fbat_color_6:
				gl_attachment = GL_COLOR_ATTACHMENT6;
				break;
			case fbat_color_7:
				gl_attachment = GL_COLOR_ATTACHMENT7;
				break;
			default:
				return;
		}

		glFramebufferTextureLayer(GL_FRAMEBUFFER, gl_attachment,
			0, 0, 0);
	}

	void SimpleFrameBuffer::attach_texture(
		const TextureSharedPtr &texture,
		const FrameBufferAttachmentType attachment,
		const Uint16 layer, const Uint16 mipmap)
	{
		GLenum gl_attachment;

		switch (attachment)
		{
			case fbat_depth:
				gl_attachment = GL_DEPTH_ATTACHMENT;
				break;
			case fbat_stencil:
				gl_attachment = GL_STENCIL_ATTACHMENT;
				break;
			case fbat_color_0:
				gl_attachment = GL_COLOR_ATTACHMENT0;
				break;
			case fbat_color_1:
				gl_attachment = GL_COLOR_ATTACHMENT1;
				break;
			case fbat_color_2:
				gl_attachment = GL_COLOR_ATTACHMENT2;
				break;
			case fbat_color_3:
				gl_attachment = GL_COLOR_ATTACHMENT3;
				break;
			case fbat_color_4:
				gl_attachment = GL_COLOR_ATTACHMENT4;
				break;
			case fbat_color_5:
				gl_attachment = GL_COLOR_ATTACHMENT5;
				break;
			case fbat_color_6:
				gl_attachment = GL_COLOR_ATTACHMENT6;
				break;
			case fbat_color_7:
				gl_attachment = GL_COLOR_ATTACHMENT7;
				break;
			default:
				return;
		}

		texture->attach(gl_attachment, mipmap, layer);
	}

	void SimpleFrameBuffer::clear(const float depth,
		const GLint stencil)
	{
		DEBUG_CHECK_GL_ERROR();

		if (get_depth() && get_stencil())
		{
			glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, stencil);

			DEBUG_CHECK_GL_ERROR();

			return;
		}

		if (get_depth())
		{
			glClearBufferfv(GL_DEPTH, 0, &depth);

			DEBUG_CHECK_GL_ERROR();
		}

		if (get_stencil())
		{
			glClearBufferiv(GL_STENCIL, 0, &stencil);

			DEBUG_CHECK_GL_ERROR();
		}
	}

	void SimpleFrameBuffer::clear(const glm::vec4 &color,
		const Uint16 index)
	{
		if (get_color())
		{
			glClearBufferfv(GL_COLOR, index, glm::value_ptr(color));
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

		if (depth)
		{
			mask |= GL_DEPTH_BUFFER_BIT;
		}

		if (stencil)
		{
			mask |= GL_STENCIL_BUFFER_BIT;
		}

		if (mask == 0)
		{
			return;
		}

		m_frame_buffer.bind(false);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		glReadBuffer(GL_COLOR_ATTACHMENT0 + layer);
		glDrawBuffer(GL_BACK);

		glBlitFramebuffer(0, 0, get_width(0), get_height(0), rect.x,
			rect.y, rect.z, rect.w, mask, GL_NEAREST);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	void SimpleFrameBuffer::do_attach_depth_render_buffer(
		bool &depth, bool &stencil)
	{
		if (m_render_buffer.get() == 0)
		{
			return;
		}

		m_render_buffer->attach(GL_DEPTH_ATTACHMENT);
		m_render_buffer->attach(GL_STENCIL_ATTACHMENT);

		depth = true;
		stencil = true;
	}

	void SimpleFrameBuffer::set_draw_buffers(const glm::bvec4 &enabled)
	{
		GLenum buffers[4];

		buffers[0] = enabled[0] ? GL_COLOR_ATTACHMENT0 : GL_NONE;
		buffers[1] = enabled[1] ? GL_COLOR_ATTACHMENT1 : GL_NONE;
		buffers[2] = enabled[2] ? GL_COLOR_ATTACHMENT2 : GL_NONE;
		buffers[3] = enabled[3] ? GL_COLOR_ATTACHMENT3 : GL_NONE;

		glDrawBuffers(4, buffers);
	}

	void SimpleFrameBuffer::blit_buffers()
	{
	}

}
