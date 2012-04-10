/****************************************************************************
 *            abstractframebuffer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractframebuffer.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	AbstractFrameBuffer::AbstractFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height, const Uint32 depth):
		m_texture(boost::make_shared<Texture>(name)), m_width(width),
		m_height(height), m_depth(depth), m_depth_buffer(false),
		m_stencil_buffer(false), m_color_buffer(false)
	{
	}

	AbstractFrameBuffer::~AbstractFrameBuffer() throw()
	{
	}

}
