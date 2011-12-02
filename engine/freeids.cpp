/****************************************************************************
 *            freeids.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "freeids.hpp"

namespace eternal_lands
{

	FreeIds::FreeIds()
	{
		m_next_free_id = 0;
	}

	FreeIds::~FreeIds() throw()
	{
	}

	void FreeIds::clear()
	{
		m_next_free_id = 0;
		m_free_ids.clear();
	}

	void FreeIds::set_next_free_id(const Uint32 id)
	{
		m_next_free_id = id;
	}

	Uint32 FreeIds::get_next_free_id()
	{
		Uint32Set::iterator it;
		Uint32 result;

		it = m_free_ids.begin();

		if (it != m_free_ids.end())
		{
			result = *it;

			m_free_ids.erase(it);

			return result;
		}

		result = m_next_free_id;

		m_next_free_id ++;

		return result;
	}

	void FreeIds::free_id(const Uint32 id)
	{
		m_free_ids.insert(id);
	}

}

