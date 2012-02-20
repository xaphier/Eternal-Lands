/****************************************************************************
 *            freeids.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "freeids.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	FreeIds::FreeIds(const Uint16 type): m_type(type)
	{
		m_next_free_typeless_id = 0;
	}

	FreeIds::~FreeIds() throw()
	{
	}

	void FreeIds::clear()
	{
		m_next_free_typeless_id = 0;
		m_free_typeless_ids.clear();
	}

	Uint32 FreeIds::get_next_free_id()
	{
		Uint32Set::iterator it;
		Uint32 result;

		it = m_free_typeless_ids.begin();

		if (it != m_free_typeless_ids.end())
		{
			result = *it;

			m_free_typeless_ids.erase(it);

			return get_id(result);
		}

		result = m_next_free_typeless_id;

		m_next_free_typeless_id++;

		return get_id(result);
	}

	void FreeIds::free_id(const Uint32 id)
	{
		m_free_typeless_ids.insert(get_typeless_id(id));
	}

	void FreeIds::free_typeless_id(const Uint32 typeless_id)
	{
		m_free_typeless_ids.insert(typeless_id);
	}

	void FreeIds::use_id(const Uint32 id)
	{
		Uint32Set::iterator found;
		Uint32 i, typeless_id;

		typeless_id = get_typeless_id(id);

		if (m_next_free_typeless_id <= typeless_id)
		{
			for (i = m_next_free_typeless_id; i < typeless_id; ++i)
			{
				m_free_typeless_ids.insert(i);
			}

			m_next_free_typeless_id = typeless_id + 1;

			return;
		}

		found = m_free_typeless_ids.find(typeless_id);

		if (found != m_free_typeless_ids.end())
		{
			m_free_typeless_ids.erase(found);

			return;
		}

		EL_THROW_EXCEPTION(DuplicateItemException()
			<< errinfo_item_id(id));
	}

}
