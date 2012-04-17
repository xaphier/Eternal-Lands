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
		const Uint32 width, const Uint32 height, const Uint32 depth,
		const Uint16 mipmaps, const TextureTargetType target,
		const TextureFormatType format, const bool depth_buffer):
		AbstractFrameBuffer(name, width, height, depth)
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

			get_texture()->attach(GL_DEPTH_ATTACHMENT, 0);

			CHECK_GL_ERROR_NAME(get_name());

			if (TextureFormatUtil::get_stencil(format))
			{
				set_stencil_buffer(true);

				get_texture()->attach(GL_STENCIL_ATTACHMENT,
					0);
			}

			glDrawBuffer(GL_NONE);

			CHECK_GL_ERROR_NAME(get_name());
		}
		else
		{
			set_depth_buffer(depth_buffer);
			set_color_buffer(true);

			get_texture()->attach(GL_COLOR_ATTACHMENT0, 0);

			CHECK_GL_ERROR_NAME(get_name());

			if (get_depth_buffer())
			{
				set_stencil_buffer(true);

				m_depth_texture = boost::make_shared<Texture>(
					name);

				get_depth_texture()->set_target(target);

				get_depth_texture()->set_format(
					tft_depth24_stencil8);
				get_depth_texture()->set_wrap_s(twt_clamp);
				get_depth_texture()->set_wrap_t(twt_clamp);
				get_depth_texture()->set_wrap_r(twt_clamp);
				get_depth_texture()->set_mipmap_count(0);

				CHECK_GL_ERROR_NAME(get_name());

				get_depth_texture()->init(get_width(),
					get_height(), get_depth(), 0);

				CHECK_GL_ERROR_NAME(get_name());

				get_depth_texture()->attach(
					GL_DEPTH_ATTACHMENT, 0);

				CHECK_GL_ERROR_NAME(get_name());

				get_depth_texture()->attach(
					GL_STENCIL_ATTACHMENT, 0);
			}

			glDrawBuffer(GL_COLOR_ATTACHMENT0);

			CHECK_GL_ERROR_NAME(get_name());
		}

		glReadBuffer(GL_NONE);

		m_frame_buffer.check_status();
		m_frame_buffer.unbind();

		CHECK_GL_ERROR_NAME(get_name());
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
		if (get_depth_buffer())
		{
			glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);
		}

		if (get_color_buffer())
		{
			glClearBufferfv(GL_COLOR, 0, glm::value_ptr(color));
		}
	}

	void LayeredFrameBuffer::clear(const glm::vec4 &color,
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

	void LayeredFrameBuffer::unbind()
	{
		m_frame_buffer.unbind();
	}

}
