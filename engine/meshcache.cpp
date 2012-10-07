/****************************************************************************
 *            meshcache.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "meshcache.hpp"
#include "meshdatatool.hpp"
#include "materialdescription.hpp"
#include "meshdatacache.hpp"
#include "meshbuilder.hpp"
#include "submesh.hpp"

namespace eternal_lands
{

	class MeshCache::MeshCacheItem
	{
		public:
			AbstractMeshSharedPtr m_mesh;
			StringVector m_materials;
	};

	MeshCache::MeshCache(const MeshBuilderWeakPtr &mesh_builder,
		const MeshDataCacheWeakPtr &mesh_data_cache):
		m_mesh_builder(mesh_builder),
		m_mesh_data_cache(mesh_data_cache)
	{
		assert(!m_mesh_builder.expired());
		assert(!m_mesh_data_cache.expired());
	}

	MeshCache::~MeshCache() noexcept
	{
	}

	void MeshCache::load_mesh(const String &name,
		AbstractMeshSharedPtr &mesh, StringVector &materials) const
	{
		MeshDataToolSharedPtr mesh_data_tool;

		get_mesh_data_cache()->get_mesh_data(name, mesh_data_tool,
			materials);

		mesh = get_mesh_builder()->get_mesh(vft_mesh, mesh_data_tool,
			name);
	}

	void MeshCache::get_mesh(const String &name,
		AbstractMeshSharedPtr &mesh, StringVector &materials)
	{
		MeshCache::MeshCacheItem tmp;
		MeshCacheMap::iterator found;

		found = m_mesh_cache.find(name);

		if (found == m_mesh_cache.end())
		{
			load_mesh(name, tmp.m_mesh, tmp.m_materials);

			m_mesh_cache[name] = tmp;

			mesh = tmp.m_mesh;
			materials = tmp.m_materials;
		}
		else
		{
			mesh = found->second.m_mesh;
			materials = found->second.m_materials;
		}
	}

	void MeshCache::get_mesh(const String &name,
		AbstractMeshSharedPtr &mesh)
	{
		StringVector materials;

		get_mesh(name, mesh, materials);
	}

}
