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
#include "framebuffer/filterframebuffer.hpp"

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
		if (GLEW_VERSION_3_0)
		{
			return AbstractFrameBufferSharedPtr(
				new SimpleFrameBuffer(name, width, height, 0,
					mipmaps, format));
		}
		else
		{
			return AbstractFrameBufferSharedPtr(
				new ExtSimpleFrameBuffer(name, width, height,
					mipmaps, format));
		}
	}

	AbstractFrameBufferSharedPtr FrameBufferBuilder::build_filter(
		const String &name, const Uint32 width, const Uint32 height,
		const TextureFormatType format)
	{
		return AbstractFrameBufferSharedPtr(new FilterFrameBuffer(name,
			width, height, format));
	}

	AbstractFrameBufferSharedPtr FrameBufferBuilder::build(
		const String &name, const Uint32 width, const Uint32 height,
		const Uint32 layers, const Uint16 mipmaps, const Uint16 samples,
		const TextureFormatType format)
	{
		if (samples == 0)
		{
			return AbstractFrameBufferSharedPtr(
				new SimpleFrameBuffer(name, width, height,
					layers, mipmaps, format));
		}
		else
		{
			return AbstractFrameBufferSharedPtr(
				new MultiSampleFrameBuffer(name, width, height,
					layers, mipmaps, samples, format));
		}
	}

}
