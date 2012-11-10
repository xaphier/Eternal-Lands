/****************************************************************************
 *            imageupdate.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "imageupdate.hpp"
#include "image.hpp"

namespace eternal_lands
{

	ImageUpdate::ImageUpdate(): m_mipmap(0)
	{
	}

	ImageUpdate::ImageUpdate(const ImageSharedPtr &image,
		const Uint16 mipmap): m_image(image), m_mipmap(mipmap)
	{
		m_size = m_image->get_size();
	}

	ImageUpdate::ImageUpdate(const ImageSharedPtr &image,
		const glm::uvec3 &offset, const glm::uvec3 &size,
		const Uint16 mipmap): m_image(image), m_offset(offset),
		m_size(size), m_mipmap(mipmap)
	{
	}

	ImageUpdate::~ImageUpdate() noexcept
	{
	}

}
