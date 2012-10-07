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

	String AbstractRenderTarget::get_str(const CubeMapFaceType value)
	{
		switch (value)
		{
			case cmft_negative_x:
				return String(UTF8("negative_x"));
			case cmft_negative_y:
				return String(UTF8("negative_y"));
			case cmft_negative_z:
				return String(UTF8("negative_z"));
			case cmft_positive_x:
				return String(UTF8("positive_x"));
			case cmft_positive_y:
				return String(UTF8("positive_y"));
			case cmft_positive_z:
				return String(UTF8("positive_z"));
		}
		/**
		 * We should be never here. If so, it's a programming error,
		 * because we forgot to add all types to the switch or an
		 * invalid int was used (with a type cast)!
		 */
		VALUE_NOT_IN_SWITCH(value, UTF8("CubeMapFaceType"));
	}

	OutStream &operator<<(OutStream &str, const CubeMapFaceType value)
	{
		str << AbstractRenderTarget::get_str(value);

		return str;
	}

}
