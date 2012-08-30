/****************************************************************************
 *            extsimpleframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "extsimpleframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	ExtSimpleFrameBuffer::ExtSimpleFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height,
		const bool depth_buffer):
		AbstractFrameBuffer(name, width, height)
	{
		DEBUG_CHECK_GL_ERROR();

		m_frame_buffer.bind();

		if (depth_buffer)
		{
			if (GLEW_EXT_packed_depth_stencil)
			{
				m_render_buffer.reset(new ExtRenderBuffer(name,
					get_width(), get_height(),
					tft_depth24_stencil8));
			}
			else
			{
				m_render_buffer.reset(new ExtRenderBuffer(name,
					get_width(), get_height(),
					tft_depth32));

			}

			CHECK_GL_ERROR_NAME(get_name());

			attach_depth_render_buffer();
		}

		CHECK_GL_ERROR_NAME(get_name());

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		CHECK_GL_ERROR_NAME(get_name());

//		m_frame_buffer.check_status();
		m_frame_buffer.unbind();

		DEBUG_CHECK_GL_ERROR();
	}

	ExtSimpleFrameBuffer::~ExtSimpleFrameBuffer() noexcept
	{
	}

	void ExtSimpleFrameBuffer::detach_texture(
		const FrameBufferAttachmentType attachment)
	{
		GLenum gl_attachment;

		switch (attachment)
		{
			case fbat_depth:
				gl_attachment = GL_DEPTH_ATTACHMENT_EXT;
				break;
			case fbat_stencil:
				gl_attachment = GL_STENCIL_ATTACHMENT_EXT;
				break;
			case fbat_color_0:
				gl_attachment = GL_COLOR_ATTACHMENT0_EXT;
				break;
			case fbat_color_1:
				gl_attachment = GL_COLOR_ATTACHMENT1_EXT;
				break;
			case fbat_color_2:
				gl_attachment = GL_COLOR_ATTACHMENT2_EXT;
				break;
			case fbat_color_3:
				gl_attachment = GL_COLOR_ATTACHMENT3_EXT;
				break;
			case fbat_color_4:
				gl_attachment = GL_COLOR_ATTACHMENT4_EXT;
				break;
			case fbat_color_5:
				gl_attachment = GL_COLOR_ATTACHMENT5_EXT;
				break;
			case fbat_color_6:
				gl_attachment = GL_COLOR_ATTACHMENT6_EXT;
				break;
			case fbat_color_7:
				gl_attachment = GL_COLOR_ATTACHMENT7_EXT;
				break;
			default:
				return;
		}

		glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, gl_attachment,
			0, 0, 0, 0);
	}

	void ExtSimpleFrameBuffer::attach_texture(
		const TextureSharedPtr &texture,
		const FrameBufferAttachmentType attachment,
		const Uint16 layer)
	{
		GLenum gl_attachment;

		switch (attachment)
		{
			case fbat_depth:
				gl_attachment = GL_DEPTH_ATTACHMENT_EXT;
				break;
			case fbat_stencil:
				gl_attachment = GL_STENCIL_ATTACHMENT_EXT;
				break;
			case fbat_color_0:
				gl_attachment = GL_COLOR_ATTACHMENT0_EXT;
				break;
			case fbat_color_1:
				gl_attachment = GL_COLOR_ATTACHMENT1_EXT;
				break;
			case fbat_color_2:
				gl_attachment = GL_COLOR_ATTACHMENT2_EXT;
				break;
			case fbat_color_3:
				gl_attachment = GL_COLOR_ATTACHMENT3_EXT;
				break;
			case fbat_color_4:
				gl_attachment = GL_COLOR_ATTACHMENT4_EXT;
				break;
			case fbat_color_5:
				gl_attachment = GL_COLOR_ATTACHMENT5_EXT;
				break;
			case fbat_color_6:
				gl_attachment = GL_COLOR_ATTACHMENT6_EXT;
				break;
			case fbat_color_7:
				gl_attachment = GL_COLOR_ATTACHMENT7_EXT;
				break;
			default:
				return;
		}

		texture->attach_ext(gl_attachment, 0, layer);
	}

	void ExtSimpleFrameBuffer::bind()
	{
		m_frame_buffer.bind();
	}

	void ExtSimpleFrameBuffer::clear(const glm::vec4 &color,
		const Uint16 index)
	{
		assert(index == 0);

		if (!get_color())
		{
			return;
		}

		glClearColor(color.r, color.g, color.b, color.a);

		glClear(GL_COLOR_BUFFER_BIT);
	}

	void ExtSimpleFrameBuffer::clear(const float depth,
		const GLint stencil)
	{
		GLbitfield mask;

		mask = 0;

		if (get_depth())
		{
			mask |= GL_DEPTH_BUFFER_BIT;

			glClearDepth(depth);
		}

		if (get_stencil())
		{
			mask |= GL_STENCIL_BUFFER_BIT;

			glClearStencil(stencil);
		}

		if (mask == 0)
		{
			return;
		}

		glClear(mask);
	}

	void ExtSimpleFrameBuffer::unbind()
	{
		m_frame_buffer.unbind();
	}

	void ExtSimpleFrameBuffer::blit_to_back_buffer(const glm::uvec4 &rect,
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
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);

		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT + layer);
		glDrawBuffer(GL_BACK);

		glBlitFramebufferEXT(0, 0, get_width(), get_height(), rect.x,
			rect.y, rect.z, rect.w, mask, GL_NEAREST);

		glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0);
	}

	void ExtSimpleFrameBuffer::do_attach_depth_render_buffer(
		bool &depth, bool &stencil)
	{
		if (m_render_buffer.get() == 0)
		{
			return;
		}

		m_render_buffer->attach(GL_DEPTH_ATTACHMENT_EXT);

		depth = true;

		if (GLEW_EXT_packed_depth_stencil)
		{
			m_render_buffer->attach(GL_STENCIL_ATTACHMENT_EXT);

			stencil = true;
		}
	}

	void ExtSimpleFrameBuffer::set_draw_buffer(const Uint16 index,
		const bool enabled)
	{
		if (enabled)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + index);
		}
		else
		{
			glDrawBuffer(GL_NONE);
		}
	}

	void ExtSimpleFrameBuffer::blit_buffers()
	{
	}

}
