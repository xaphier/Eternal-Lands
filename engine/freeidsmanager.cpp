/****************************************************************************
 *            freeidsmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "freeidsmanager.hpp"

namespace eternal_lands
{

	FreeIdsManager::FreeIdsManager(): m_free_ids(boost::assign::list_of
		(FreeIds(it_3d_object))(FreeIds(it_2d_object))
		(FreeIds(it_tile_object))(FreeIds(it_water_object))
		(FreeIds(it_dynamic_object))(FreeIds(it_light_object))
		(FreeIds(it_particle_object)))
	{
	}

	FreeIdsManager::~FreeIdsManager() noexcept
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

		m_free_light_ids.clear();
		m_free_particle_ids.clear();
	}

	Uint32 FreeIdsManager::get_next_free_object_id(const IdType type)
	{
		RANGE_CECK_MAX(type, m_free_ids.size() - 1,
			UTF8("type value too big"));

		return m_free_ids[type].get_next_free_id();
	}

	void FreeIdsManager::free_object_id(const Uint32 id)
	{
		Uint16 type;

		type = FreeIds::get_type_value(id);

		RANGE_CECK_MAX(type, m_free_ids.size() - 1,
			UTF8("type value too big"));

		m_free_ids[type].free_id(id);
	}

	void FreeIdsManager::use_object_id(const Uint32 id)
	{
		Uint16 type;

		type = FreeIds::get_type_value(id);

		RANGE_CECK_MAX(type, m_free_ids.size() - 1,
			UTF8("type value too big"));

		m_free_ids[type].use_id(id);
	}

	Uint32 FreeIdsManager::use_typeless_object_id(const Uint32 typeless_id,
		const IdType type)
	{
		RANGE_CECK_MAX(type, m_free_ids.size() - 1,
			UTF8("type value too big"));

		return m_free_ids[type].use_typeless_id(typeless_id);
	}

	Uint32 FreeIdsManager::get_object_id(const Uint32 typeless_id,
		const IdType type)
	{
		RANGE_CECK_MAX(type, m_free_ids.size() - 1,
			UTF8("type value too big"));

		return m_free_ids[type].get_id(typeless_id);
	}

	Uint32 FreeIdsManager::get_next_free_light_id()
	{
		return m_free_light_ids.get_next_free_id();
	}

	void FreeIdsManager::free_light_id(const Uint32 id)
	{
		m_free_light_ids.free_id(id);
	}

	void FreeIdsManager::use_light_id(const Uint32 id)
	{
		m_free_light_ids.use_id(id);
	}

	Uint32 FreeIdsManager::get_next_free_particle_id()
	{
		return m_free_particle_ids.get_next_free_id();
	}

	void FreeIdsManager::free_particle_id(const Uint32 id)
	{
		m_free_particle_ids.free_id(id);
	}

	void FreeIdsManager::use_particle_id(const Uint32 id)
	{
		m_free_particle_ids.use_id(id);
	}

	Uint32 FreeIdsManager::get_next_free_decal_id()
	{
		return m_free_decal_ids.get_next_free_id();
	}

	void FreeIdsManager::free_decal_id(const Uint32 id)
	{
		m_free_decal_ids.free_id(id);
	}

	void FreeIdsManager::use_decal_id(const Uint32 id)
	{
		m_free_decal_ids.use_id(id);
	}

	bool FreeIdsManager::get_is_object_id_free(const Uint32 id) const
	{
		Uint16 type;

		type = FreeIds::get_type_value(id);

		RANGE_CECK_MAX(type, m_free_ids.size() - 1,
			UTF8("type value too big"));

		return m_free_ids[type].get_is_id_free(id);
	}

	bool FreeIdsManager::get_is_light_id_free(const Uint32 id) const
	{
		return m_free_light_ids.get_is_id_free(id);
	}

	bool FreeIdsManager::get_is_particle_id_free(const Uint32 id) const
	{
		return m_free_particle_ids.get_is_id_free(id);
	}

	bool FreeIdsManager::get_is_decal_id_free(const Uint32 id) const
	{
		return m_free_decal_ids.get_is_id_free(id);
	}

}
