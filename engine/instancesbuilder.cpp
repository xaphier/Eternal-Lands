/****************************************************************************
 *            instancesbuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "instancesbuilder.hpp"
#include "materialeffectdescription.hpp"
#include "instancingdata.hpp"
#include "freeids.hpp"
#include "instancebuilder.hpp"

namespace eternal_lands
{

	InstancesBuilder::InstancesBuilder(
		const MeshDataCacheWeakPtr &mesh_data_cache,
		const float max_size, const bool use_simd,
		const bool use_base_vertex): m_mesh_data_cache(mesh_data_cache),
		m_max_size(max_size), m_use_simd(use_simd),
		m_use_base_vertex(use_base_vertex)
	{
		assert(!m_mesh_data_cache.expired());
	}

	InstancesBuilder::~InstancesBuilder() throw()
	{
	}

	void InstancesBuilder::add(const ObjectData &object_data)
	{
		std::auto_ptr<InstancingData> instancing_data;
		Sint16Sint16Pair index;

		instancing_data.reset(new InstancingData(
			get_mesh_data_cache(), object_data));

		index = instancing_data->get_index(m_max_size);

		m_instancing_datas[index].push_back(instancing_data);
	}

	void InstancesBuilder::add(const ObjectData &object_data,
		const MaterialEffectDescriptionVector &materials)
	{
		std::auto_ptr<InstancingData> instancing_data;
		Sint16Sint16Pair index;

		instancing_data.reset(new InstancingData(
			get_mesh_data_cache(), object_data, materials));

		index = instancing_data->get_index(m_max_size);

		m_instancing_datas[index].push_back(instancing_data);
	}

	void InstancesBuilder::build(FreeIds &free_ids,
		InstanceDataVector &instances,
		ObjectDataVector &uninstanced)
	{
		boost::ptr_vector<InstanceBuilder> builders;
		Sint16Sint16PairInstancingDataVectorMap::iterator it, end;
		Uint32 i, count;

		end = m_instancing_datas.end();

		for (it = m_instancing_datas.begin(); it != end; ++it)
		{ 
			if (it->second.size() <= 1)
			{
				BOOST_FOREACH(const InstancingData
					&instancing_data, it->second)
				{
					uninstanced.push_back(instancing_data);
				}
			}
			else
			{
				builders.push_back(new InstanceBuilder(
					it->second,
					free_ids.get_next_free_id(),
					get_use_simd(), get_use_base_vertex()));
			}
		}

		count = builders.size();

		for (i = 0; i < count; ++i)
		{
			builders[i].build_instance();
		}

		for (i = 0; i < count; ++i)
		{
			builders[i].set_instance(instances);
		}
	}

	void InstancesBuilder::clear()
	{
		m_instancing_datas.clear();
	}

}
