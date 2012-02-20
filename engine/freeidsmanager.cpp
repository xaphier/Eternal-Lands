/****************************************************************************
 *            freeidsmanager.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "freeidsmanager.hpp"

namespace eternal_lands
{

	FreeIdsManager::FreeIdsManager()
	{
		m_free_ids[it_2d_object] = FreeIds(it_2d_object);
		m_free_ids[it_3d_object] = FreeIds(it_3d_object);
		m_free_ids[it_tile_object] = FreeIds(it_tile_object);
		m_free_ids[it_water_object] = FreeIds(it_water_object);
		m_free_ids[it_dynamic_object] = FreeIds(it_dynamic_object);
		m_free_ids[it_light_object] = FreeIds(it_light_object);
	}

	FreeIdsManager::~FreeIdsManager() throw()
	{
	}

	void FreeIdsManager::clear()
	{
		Uint32 i, count;

		count = m_free_ids.size();

		for (i = 0; i < count; ++i)
		{
			m_free_ids[i].clear();
		}
	}

	void FreeIdsManager::clear(const IdType type)
	{
		RANGE_CECK(type, m_free_ids.size(), UTF8("type value too big"));

		m_free_ids[type].clear();
	}

	Uint32 FreeIdsManager::get_next_free_id(const IdType type)
	{
		RANGE_CECK(type, m_free_ids.size(), UTF8("type value too big"));

		return m_free_ids[type].get_next_free_id();
	}

	void FreeIdsManager::free_id(const Uint32 id)
	{
		Uint16 type;

		type = FreeIds::get_type_value(id);

		RANGE_CECK(type, m_free_ids.size(), UTF8("type value too big"));

		m_free_ids[type].free_id(id);
	}

	Uint32 FreeIdsManager::use_typeless_id(const Uint32 typeless_id,
		const IdType type)
	{
		RANGE_CECK(type, m_free_ids.size(), UTF8("type value too big"));

		return m_free_ids[type].use_typeless_id(typeless_id);
	}	

}
