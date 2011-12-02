/****************************************************************************
 *            xmlutils.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "xmlutils.hpp"
#include "../engine/prerequisites.hpp"
#include "../engine/shader/shadersource.hpp"
#include "../engine/exceptions.hpp"

namespace eternal_lands
{

	void XmlUtils::load(const gchar* name)
	{
		ShaderSource ss;

		ss.load_xml(String(utf8_to_string(name)));
	}

}
