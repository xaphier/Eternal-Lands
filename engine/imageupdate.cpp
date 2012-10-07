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
		m_sizes = m_image->get_sizes();
	}

	ImageUpdate::ImageUpdate(const ImageSharedPtr &image,
		const glm::uvec3 &offsets, const glm::uvec3 &sizes,
		const Uint16 mipmap): m_image(image), m_offsets(offsets),
		m_sizes(sizes), m_mipmap(mipmap)
	{
	}

	ImageUpdate::~ImageUpdate() noexcept
	{
	}

}
