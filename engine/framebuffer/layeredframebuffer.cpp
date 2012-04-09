/****************************************************************************
 *            layeredframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "layeredframebuffer.hpp"
#include "exceptions.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	LayeredFrameBuffer::LayeredFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height, const Uint32 layers,
		const Uint16 mipmaps, const TextureFormatType format,
		const bool depth): AbstractFrameBuffer(name, width, height)
	{
		DEBUG_CHECK_GL_ERROR();

		m_frame_buffer.bind();

		if (layers > 0)
		{
			get_texture()->set_target(ttt_2d_texture_array);
		}
		else
		{
			get_texture()->set_target(ttt_2d_texture);
		}

		DEBUG_CHECK_GL_ERROR();

		get_texture()->set_format(format);
		get_texture()->set_wrap_s(twt_clamp);
		get_texture()->set_wrap_t(twt_clamp);
		get_texture()->set_wrap_r(twt_clamp);

		get_texture()->set_mipmap_count(mipmaps);

		DEBUG_CHECK_GL_ERROR();

		get_texture()->init(get_width(), get_height(), layers, mipmaps);

		DEBUG_CHECK_GL_ERROR();

		if (TextureFormatUtil::get_depth(format))
		{
			m_depth = true;
			m_color = false;

			get_texture()->attach(GL_DEPTH_ATTACHMENT, 0);

			DEBUG_CHECK_GL_ERROR();

			if (TextureFormatUtil::get_stencil(format))
			{
				get_texture()->attach(GL_STENCIL_ATTACHMENT,
					0);
			}

			glDrawBuffer(GL_NONE);

			DEBUG_CHECK_GL_ERROR();
		}
		else
		{
			m_depth = depth;
			m_color = true;

			get_texture()->attach(GL_COLOR_ATTACHMENT0, 0);

			DEBUG_CHECK_GL_ERROR();

			if (m_depth)
			{
				m_depth_texture = boost::make_shared<Texture>(
					name);

				if (layers > 0)
				{
					get_depth_texture()->set_target(
						ttt_2d_texture_array);
				}
				else
				{
					get_depth_texture()->set_target(
						ttt_2d_texture);
				}

				get_depth_texture()->set_format(
					tft_depth24_stencil8);
				get_depth_texture()->set_wrap_s(twt_clamp);
				get_depth_texture()->set_wrap_t(twt_clamp);
				get_depth_texture()->set_wrap_r(twt_clamp);
				get_depth_texture()->set_mipmap_count(0);

				DEBUG_CHECK_GL_ERROR();

				get_depth_texture()->init(get_width(),
					get_height(), layers, 0);

				DEBUG_CHECK_GL_ERROR();

				get_depth_texture()->attach(
					GL_DEPTH_ATTACHMENT, 0);

				DEBUG_CHECK_GL_ERROR();

				get_depth_texture()->attach(
					GL_STENCIL_ATTACHMENT, 0);
			}

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			DEBUG_CHECK_GL_ERROR();
		}

		glReadBuffer(GL_NONE);

		m_frame_buffer.check_status();
		m_frame_buffer.unbind();

		DEBUG_CHECK_GL_ERROR();
	}

	LayeredFrameBuffer::~LayeredFrameBuffer() throw()
	{
	}

	void LayeredFrameBuffer::bind(const Uint32 layer)
	{
		m_frame_buffer.bind();
	}

	void LayeredFrameBuffer::bind_texture(const Uint32 layer)
	{
		m_frame_buffer.bind();
	}

	void LayeredFrameBuffer::blit()
	{
	}

	void LayeredFrameBuffer::clear(const glm::vec4 &color)
	{
		if (m_depth)
		{
			glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
		}

		if (m_color)
		{
			glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
		}
	}

	void LayeredFrameBuffer::clear(const glm::vec4 &color,
		const float depth)
	{
		if (m_depth)
		{
			glClearBufferfi(GL_DEPTH_STENCIL, 0, depth, 0);
		}

		if (m_color)
		{
			glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
		}
	}

	void LayeredFrameBuffer::unbind()
	{
		m_frame_buffer.unbind();
	}

}
