/****************************************************************************
 *            textattribute.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "textattribute.hpp"

namespace eternal_lands
{

	TextAttribute::TextAttribute(): m_color(1.0f), m_size(12),
		m_spacing(0.0f), m_rise(0.0f)
	{
	}

	TextAttribute::~TextAttribute() throw()
	{
	}

}
