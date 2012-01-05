/****************************************************************************
 *            text.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "text.hpp"

namespace eternal_lands
{

	Text::Text(): m_hash(0), m_length(0)
	{
	}

	Text::~Text() throw()
	{
	}

	void Text::add(const Utf32String &str, const TextAttribute &attribute)
	{
		m_text.push_back(Utf32StringTextAttributePair(str, attribute));

		m_length += str.length();

		boost::hash_combine(m_hash, str);
	}

	bool Text::operator==(const Text &text) const
	{
		Uint32 i, count;

		if (text.get_text().size() != get_text().size())
		{
			return false;
		}

		if (text.get_hash() != get_hash())
		{
			return false;
		}

		count = get_text().size();

		for (i = 0; i < count; ++i)
		{
			if (get_text()[i].first != text.get_text()[i].first)
			{
				return false;
			}

			if (get_text()[i].second != text.get_text()[i].second)
			{
				return false;
			}
		}

		return true;
	}

	Utf32String Text::get_string() const
	{
		Utf32String result;
		Uint32 i, count;

		count = m_text.size();

		for (i = 0; i < count; ++i)
		{
			result += m_text[i].first;
		}

		return result;
	}

}
