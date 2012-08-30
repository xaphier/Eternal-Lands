/****************************************************************************
 *            framebufferbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "framebufferbuilder.hpp"
#include "framebuffer/extsimpleframebuffer.hpp"
#include "framebuffer/simpleframebuffer.hpp"
#include "framebuffer/multisampleframebuffer.hpp"
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
		const Uint16 samples, const bool depth_buffer)
	{
		if (get_global_vars()->get_opengl_3_0() && (samples > 1))
		{
			return AbstractFrameBufferSharedPtr(
				new MultiSampleFrameBuffer(name, width, height,
					samples, depth_buffer));
		}

		if (get_global_vars()->get_opengl_3_0())
		{
			return AbstractFrameBufferSharedPtr(
				new SimpleFrameBuffer(name, width, height,
					depth_buffer));
		}

		if (GLEW_EXT_framebuffer_object)
		{
			return AbstractFrameBufferSharedPtr(
				new ExtSimpleFrameBuffer(name, width, height,
					depth_buffer));
		}

		EL_THROW_EXCEPTION(OpenGlException()
			<< errinfo_message(UTF8("No framebuffer supported.")));
	}

}
