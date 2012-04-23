/****************************************************************************
 *            instancesbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "instancesbuilder.hpp"
#include "materialdescriptioncache.hpp"
#include "instancingdata.hpp"
#include "freeids.hpp"
#include "instancebuilder.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	InstancesBuilder::InstancesBuilder(
		const MeshDataCacheWeakPtr &mesh_data_cache,
		const MaterialDescriptionCacheWeakPtr
			&material_description_cache,
		const float max_size, const bool use_simd,
		const bool use_base_vertex): m_mesh_data_cache(mesh_data_cache),
		m_material_description_cache(material_description_cache),
		m_max_size(max_size), m_use_simd(use_simd),
		m_use_base_vertex(use_base_vertex)
	{
		assert(!m_mesh_data_cache.expired());
		assert(!m_material_description_cache.expired());
	}

	InstancesBuilder::~InstancesBuilder() throw()
	{
	}

	void InstancesBuilder::add(const ObjectDescription &object_description)
	{
		std::auto_ptr<InstancingData> instancing_data;
		Sint16Sint16Pair index;
		bool ok;

		ok = true;

		try
		{
			instancing_data.reset(new InstancingData(
				get_mesh_data_cache(), object_description));

			BOOST_FOREACH(const String &name,
				instancing_data->get_material_names())
			{
				if (!get_material_description_cache(
					)->get_has_material_description(name))
				{
					LOG_ERROR(lt_material, UTF8("Object "
						"'%1%' using invalid material"
						" '%2%'."),
						object_description.get_name() %
						name);
					ok = false;
				}
			}

			if (!ok)
			{
				return;
			}

			index = instancing_data->get_index(m_max_size);

			m_instancing_datas[index].push_back(instancing_data);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While checking object '%1%' "
				"caught exception '%2%'"),
				object_description.get_name() %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While checking object '%1%' "
				"caught exception '%2%'"),
				object_description.get_name() %
				exception.what());
		}
	}

	void InstancesBuilder::build(FreeIds &free_ids,
		InstanceDataVector &instances,
		ObjectDescriptionVector &uninstanced)
	{
		boost::ptr_vector<InstanceBuilder> builders;
		Sint16Sint16PairInstancingDataVectorMap::iterator it, end;
		Uint32 i, count;

		end = m_instancing_datas.end();

		for (it = m_instancing_datas.begin(); it != end; ++it)
		{ 
			if (it->second.size() == 0)
			{
				continue;
			}

			InstanceBuilder::remove_singles(it->second,
				uninstanced);

			if (it->second.size() == 0)
			{
				continue;
			}

			if (it->second.size() == 1)
			{
				uninstanced.push_back(it->second[0]);

				continue;
			}

			builders.push_back(new InstanceBuilder(it->second,
				free_ids.get_next_free_id(), get_use_simd(),
				get_use_base_vertex()));
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
