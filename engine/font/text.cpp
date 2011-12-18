/****************************************************************************
 *            text.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "text.hpp"

namespace eternal_lands
{

	Text::Text()
	{
	}

	Text::~Text() throw()
	{
	}

	void Text::add(const String &str, const TextAttribute &attribute)
	{
		m_text.push_back(StringTextAttributePair(str, attribute));
	}

	String Text::get_string() const
	{
		StringType result;
		Uint32 i, count;

		count = m_text.size();

		for (i = 0; i < count; ++i)
		{
			result += m_text[i].first;
		}

		return String(result);
	}

}
