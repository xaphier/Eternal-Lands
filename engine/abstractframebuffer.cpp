/****************************************************************************
 *            abstractframebuffer.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractframebuffer.hpp"
#include "texture.hpp"

namespace eternal_lands
{

	AbstractFrameBuffer::AbstractFrameBuffer(const String &name,
		const Uint32 width, const Uint32 height):
		m_texture(boost::make_shared<Texture>(name)), m_width(width),
		m_height(height)
	{
	}

	AbstractFrameBuffer::~AbstractFrameBuffer() throw()
	{
	}

}
