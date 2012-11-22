/****************************************************************************
 *            abstractrendertarget.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractrendertarget.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	AbstractRenderTarget::AbstractRenderTarget(const String &name,
		const Uint32 width, const Uint32 height, const Uint32 samples,
		const TextureFormatType format): m_name(name),
		m_width(width), m_height(height), m_samples(samples),
		m_format(format)
	{
		assert(!get_name().get().empty());
	}

	AbstractRenderTarget::~AbstractRenderTarget() noexcept
	{
	}

}
