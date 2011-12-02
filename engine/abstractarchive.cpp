/****************************************************************************
 *            abstractarchive.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractarchive.hpp"

namespace eternal_lands
{

	AbstractArchive::AbstractArchive(const String &name): m_name(name)
	{
	}

	AbstractArchive::~AbstractArchive() throw()
	{
	}

}
