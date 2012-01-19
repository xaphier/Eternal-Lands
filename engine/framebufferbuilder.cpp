/****************************************************************************
 *            framebufferbuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

	FrameBufferBuilder::~FrameBufferBuilder() throw()
	{
	}

	AbstractFrameBufferSharedPtr FrameBufferBuilder::build(
		const String &name, const Uint32 width, const Uint32 height,
		const Uint16 mipmaps, const TextureFormatType format)
	{
		if (get_global_vars()->get_opengl_3_0())
		{
			return AbstractFrameBufferSharedPtr(
				new SimpleFrameBuffer(name, width, height, 0,
					mipmaps, format));
		}

		if (GLEW_EXT_framebuffer_object)
		{
			return AbstractFrameBufferSharedPtr(
				new ExtSimpleFrameBuffer(name, width, height,
					mipmaps, format));
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
		const Uint32 layers, const Uint16 mipmaps, const Uint16 samples,
		const TextureFormatType format, const bool layered)
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
					layers, mipmaps, format));
		}

		if (samples == 0)
		{
			return AbstractFrameBufferSharedPtr(
				new SimpleFrameBuffer(name, width, height,
					layers, mipmaps, format));
		}

		return AbstractFrameBufferSharedPtr(
			new MultiSampleFrameBuffer(name, width, height,
				layers, mipmaps, samples, format));
	}

}
