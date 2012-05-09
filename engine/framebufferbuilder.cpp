/****************************************************************************
 *            framebufferbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "framebufferbuilder.hpp"
#include "framebuffer/extsimpleframebuffer.hpp"
#include "framebuffer/simpleframebuffer.hpp"
#include "framebuffer/multisampleframebuffer.hpp"
#include "framebuffer/layeredframebuffer.hpp"
#include "framebuffer/filterframebuffer.hpp"
#include "globalvars.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	FrameBufferBuilder::FrameBufferBuilder(
		const GlobalVarsSharedPtr &global_vars):
			m_global_vars(global_vars)
	{
	}

	FrameBufferBuilder::~FrameBufferBuilder() noexcept
	{
	}

	AbstractFrameBufferSharedPtr FrameBufferBuilder::build(
		const String &name, const Uint32 width, const Uint32 height,
		const Uint32 depth, const Uint16 mipmaps,
		const TextureTargetType target,	const TextureFormatType format,
		const bool depth_buffer)
	{
		if (get_global_vars()->get_opengl_3_0())
		{
			return AbstractFrameBufferSharedPtr(
				new SimpleFrameBuffer(name, width, height,
					depth, mipmaps, target, format,
					depth_buffer));
		}

		if (GLEW_EXT_framebuffer_object)
		{
			return AbstractFrameBufferSharedPtr(
				new ExtSimpleFrameBuffer(name, width, height,
					depth, mipmaps, target, format,
					depth_buffer));
		}

		EL_THROW_EXCEPTION(OpenGlException()
			<< errinfo_message(UTF8("No framebuffer supported.")));
	}

	AbstractFrameBufferSharedPtr FrameBufferBuilder::build_filter(
		const String &name, const Uint32 width, const Uint32 height,
		const TextureFormatType format)
	{
		if (get_global_vars()->get_opengl_3_0())
		{
			return AbstractFrameBufferSharedPtr(
				new FilterFrameBuffer(name, width, height,
					format));
		}

		EL_THROW_EXCEPTION(OpenGlException()
			<< errinfo_message(UTF8("OpenGL 3.0 needed")));
	}

	AbstractFrameBufferSharedPtr FrameBufferBuilder::build(
		const String &name, const Uint32 width, const Uint32 height,
		const Uint32 depth, const Uint16 mipmaps, const Uint16 samples,
		const TextureTargetType target, const TextureFormatType format,
		const bool layered, const bool depth_buffer)
	{
		if (!get_global_vars()->get_opengl_3_0())
		{
			EL_THROW_EXCEPTION(OpenGlException()
				<< errinfo_message(UTF8("OpenGL 3.0 needed")));
		}

		if ((samples == 0) && layered)
		{
			return AbstractFrameBufferSharedPtr(
				new LayeredFrameBuffer(name, width, height,
					depth, mipmaps, target, format,
					depth_buffer));
		}

		if (samples == 0)
		{
			return AbstractFrameBufferSharedPtr(
				new SimpleFrameBuffer(name, width, height,
					depth, mipmaps, target, format,
					depth_buffer));
		}

		return AbstractFrameBufferSharedPtr(
			new MultiSampleFrameBuffer(name, width, height,
				depth, mipmaps, samples, target, format,
				depth_buffer));
	}

}
