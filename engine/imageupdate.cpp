/****************************************************************************
 *            imageupdate.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "imageupdate.hpp"
#include "readwritememory.hpp"

namespace eternal_lands
{

	ImageUpdate::ImageUpdate(const String &name,
		const AbstractReadMemorySharedPtr &buffer,
		const glm::uvec3 &offset, const glm::uvec3 &size,
		const CubeMapFaceType face, const GLenum format,
		const GLenum type, const Uint16 mipmap,
		const bool compressed): m_name(name), m_buffer(buffer),
		m_offset(offset), m_size(size), m_face(face),
		m_format(format), m_type(type), m_mipmap(mipmap),
		m_compressed(compressed)
	{
	}

	ImageUpdate::~ImageUpdate() noexcept
	{
	}

}
