/****************************************************************************
 *            filterframebuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			width, height)
	{
		get_texture()->set_target(ttt_2d_texture_array);

		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(0);
		get_texture()->init(get_width(), get_height(), 1, 0);

		m_frame_buffer.bind();

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glReadBuffer(GL_COLOR_ATTACHMENT0);

		get_texture()->attach(GL_COLOR_ATTACHMENT0, 0, 0);

		m_frame_buffer.check_status();

		m_frame_buffer.unbind();
	}

	FilterFrameBuffer::~FilterFrameBuffer() throw()
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

	void FilterFrameBuffer::clear(const glm::vec4 &color,
		const float depth)
	{
		glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
	}

	void FilterFrameBuffer::unbind()
	{
		m_frame_buffer.unbind();
	}

}
