/****************************************************************************
 *            multisampleframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "multisampleframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	MultiSampleFrameBuffer::MultiSampleFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height, const Uint16 samples,
		const bool depth_buffer):
		AbstractFrameBuffer(name, width, height), m_samples(samples),
		m_draw_buffer(0xFFFF)
	{
		CHECK_GL_ERROR();

		m_multisample_frame_buffer.bind();

		if (depth_buffer)
		{
			m_render_buffer.reset(new RenderBuffer(name,
				get_width(), get_height(), samples,
				tft_depth24_stencil8));

			CHECK_GL_ERROR_NAME(get_name());

			attach_depth_render_buffer();
		}

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

//		m_frame_buffer.check_status();
		m_multisample_frame_buffer.unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	MultiSampleFrameBuffer::~MultiSampleFrameBuffer() noexcept
	{
	}

	void MultiSampleFrameBuffer::bind()
	{
		m_multisample_frame_buffer.bind();
	}

	void MultiSampleFrameBuffer::detach_texture(
		const FrameBufferAttachmentType attachment)
	{
		FrameBufferAttachmentTypeRenderBufferMap::iterator found;
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

		CHECK_GL_ERROR_NAME(get_name());

		m_frame_buffer.bind();

		CHECK_GL_ERROR_NAME(get_name());

		glFramebufferTextureLayer(GL_FRAMEBUFFER, gl_attachment,
			0, 0, 0);

		CHECK_GL_ERROR_NAME(get_name());

		m_multisample_frame_buffer.bind();
/*
		CHECK_GL_ERROR_NAME(get_name());

		glFramebufferTextureLayer(GL_FRAMEBUFFER, gl_attachment,
			0, 0, 0);

		CHECK_GL_ERROR_NAME(get_name());

		found = m_render_buffers.find(attachment);

		if (found == m_render_buffers.end())
		{
			return;
		}

		m_render_buffers.erase(found);
*/	}

	void MultiSampleFrameBuffer::attach_texture(
		const TextureSharedPtr &texture,
		const FrameBufferAttachmentType attachment,
		const Uint16 layer)
	{
		FrameBufferAttachmentTypeRenderBufferMap::iterator found;
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

		CHECK_GL_ERROR_NAME(get_name());

		m_frame_buffer.bind();

		CHECK_GL_ERROR_NAME(get_name());

		texture->attach(gl_attachment, 0, layer);

		CHECK_GL_ERROR_NAME(get_name());

		m_multisample_frame_buffer.bind();

		CHECK_GL_ERROR_NAME(get_name());

		found = m_render_buffers.find(attachment);

		if (found != m_render_buffers.end())
		{
			if (found->second->get_format() ==
				texture->get_format())
			{
				return;
			}
		}

		boost::shared_ptr<RenderBuffer> render_buffer;

		render_buffer.reset(new RenderBuffer(texture->get_name(),
			get_width(), get_height(), get_samples(),
			texture->get_format()));

		m_render_buffers.insert(
			FrameBufferAttachmentTypeRenderBufferPair(attachment,
				render_buffer));

		CHECK_GL_ERROR_NAME(get_name());

		render_buffer->attach(gl_attachment);

		CHECK_GL_ERROR_NAME(get_name());
	}

	void MultiSampleFrameBuffer::clear(const float depth,
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

	void MultiSampleFrameBuffer::clear(const glm::vec4 &color,
		const Uint16 index)
	{
		if (get_color())
		{
			glClearBufferfv(GL_COLOR, index, glm::value_ptr(color));
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void MultiSampleFrameBuffer::unbind()
	{
		m_multisample_frame_buffer.unbind();
	}

	void MultiSampleFrameBuffer::blit_to_back_buffer(const glm::uvec4 &rect,
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

		glBlitFramebuffer(0, 0, get_width(), get_height(), rect.x,
			rect.y, rect.z, rect.w, mask, GL_NEAREST);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}

	void MultiSampleFrameBuffer::do_attach_depth_render_buffer(
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

	void MultiSampleFrameBuffer::set_draw_buffer(const Uint16 index,
		const bool enabled)
	{
		if (enabled)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + index);

			m_draw_buffer = index;
		}
		else
		{
			glDrawBuffer(GL_NONE);

			m_draw_buffer = 0xFFFF;
		}
	}

	void MultiSampleFrameBuffer::blit_buffers()
	{
		// blit from multisample buffer to final buffer, triggers resolve
		m_multisample_frame_buffer.bind(false);
		m_frame_buffer.bind(true);

		CHECK_GL_ERROR_NAME(get_name());

		for (int i = 0; i < 1; ++i)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + i);
			glReadBuffer(GL_COLOR_ATTACHMENT0 + i);

			glBlitFramebuffer(0, 0, get_width(), get_height(), 0,
				0, get_width(), get_height(),
				GL_COLOR_BUFFER_BIT, GL_NEAREST);
		}

		if (m_draw_buffer != 0xFFFF)
		{
			glDrawBuffer(GL_COLOR_ATTACHMENT0 + m_draw_buffer);
		}
		else
		{
			glDrawBuffer(GL_NONE);

			m_draw_buffer = 0xFFFF;
		}

		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		CHECK_GL_ERROR_NAME(get_name());
	}

}
