/****************************************************************************
 *            textattribute.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "textattribute.hpp"

namespace eternal_lands
{

	TextAttribute::TextAttribute(): m_color(1.0f), m_size(12),
		m_bold(false), m_italic(false)
	{
	}

	TextAttribute::~TextAttribute() throw()
	{
	}

}
