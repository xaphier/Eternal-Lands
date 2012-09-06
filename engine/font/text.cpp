/****************************************************************************
 *            text.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "text.hpp"
#include "textattribute.hpp"

namespace eternal_lands
{

	Text::Text(): m_hash(0)
	{
	}

	Text::~Text() noexcept
	{
	}

	void Text::add(const Uint32Vector &str, const TextAttribute &attribute)
	{
/*
		m_text_attributes[m_string.length()] = TextAttributeUint32Pair(
			attribute, str.length());

		m_string += str;

		boost::hash_combine(m_hash, str);
*/
	}

	bool Text::operator==(const Text &text) const
	{
		if (text.m_text_attributes.size() != m_text_attributes.size())
		{
			return false;
		}

		if (text.get_hash() != get_hash())
		{
			return false;
		}

		if (text.get_string() != get_string())
		{
			return false;
		}

		return text.m_text_attributes == m_text_attributes;
	}

	const TextAttribute &Text::get_text_attribute(const Uint32 index,
		Uint32 &size) const
	{
		Uint32TextAttributeUint32PairMap::const_iterator found;

		found = m_text_attributes.find(index);

		assert(found != m_text_attributes.end());

		size = found->second.second;

		return found->second.first;
	}

	void Text::clear()
	{
		m_text_attributes.clear();
		m_string.clear();
		m_hash = 0;
	}

}
