/****************************************************************************
 *            filterframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "filterframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	FilterFrameBuffer::FilterFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height,
		const TextureFormatType format): AbstractFrameBuffer(name,
			width, height, 1)
	{
		CHECK_GL_ERROR();

		get_texture()->set_target(ttt_texture_2d_array);

		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(0);
		get_texture()->init(get_width(), get_height(), 1, 0);

		CHECK_GL_ERROR_NAME(get_name());

		m_frame_buffer.bind();

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		CHECK_GL_ERROR_NAME(get_name());

		get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, 0);

		CHECK_GL_ERROR_NAME(get_name());

		m_frame_buffer.check_status();

		m_frame_buffer.unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	FilterFrameBuffer::~FilterFrameBuffer() noexcept
	{
	}

	void FilterFrameBuffer::bind(const Uint32 layer)
	{
		assert(layer == 0);

		m_frame_buffer.bind();
	}

	void FilterFrameBuffer::bind_texture(const Uint32 layer)
	{
		assert(layer == 0);

		m_frame_buffer.bind();
	}

	void FilterFrameBuffer::blit()
	{
	}

	void FilterFrameBuffer::clear(const glm::vec4 &color)
	{
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
	}

	void FilterFrameBuffer::clear(const glm::vec4 &color,
		const float depth)
	{
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
	}

	void FilterFrameBuffer::unbind()
	{
		m_frame_buffer.unbind();
	}

	void FilterFrameBuffer::blit_to_back_buffer(const glm::uvec4 &rect,
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
