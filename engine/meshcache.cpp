/****************************************************************************
 *            meshcache.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
			MaterialDescriptionVector m_materials;
	};

	MeshCache::MeshCache(const MeshBuilderWeakPtr &mesh_builder,
		const MeshDataCacheWeakPtr &mesh_data_cache):
		m_mesh_builder(mesh_builder),
		m_mesh_data_cache(mesh_data_cache)
	{
		MeshCache::MeshCacheItem tmp;

		assert(!m_mesh_builder.expired());
		assert(!m_mesh_data_cache.expired());

		init_rect(tmp.m_mesh);

		m_mesh_cache[String(UTF8("rect"))] = tmp;
	}

	MeshCache::~MeshCache() throw()
	{
	}

	void MeshCache::init_rect(AbstractMeshSharedPtr &mesh) const
	{
		MeshDataToolSharedPtr mesh_data_tool;
		VertexSemanticTypeSet semantics;

		semantics.insert(vst_position);

		mesh_data_tool = boost::make_shared<MeshDataTool>(4, 6,
			1, semantics, 0xFFFFFFFF, pt_triangles, false);

		mesh_data_tool->set_vertex_data(vst_position, 0,
			glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		mesh_data_tool->set_vertex_data(vst_position, 1,
			glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		mesh_data_tool->set_vertex_data(vst_position, 2,
			glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		mesh_data_tool->set_vertex_data(vst_position, 3,
			glm::vec4(1.0f, 1.0f, 0.0f, 0.0f));

		mesh_data_tool->set_index_data(0, 0);
		mesh_data_tool->set_index_data(1, 1);
		mesh_data_tool->set_index_data(2, 2);
		mesh_data_tool->set_index_data(3, 1);
		mesh_data_tool->set_index_data(4, 3);
		mesh_data_tool->set_index_data(5, 2);

		mesh_data_tool->set_sub_mesh_data(0, SubMesh(BoundingBox(
			glm::vec3(0.0f), glm::vec3(1.0f, 1.0f, 0.01f)),
				0, 6, 0, 3));

		mesh = get_mesh_builder()->get_mesh(vft_rect, mesh_data_tool);
	}

	void MeshCache::load_mesh(const String &name,
		AbstractMeshSharedPtr &mesh,
		MaterialDescriptionVector &materials) const
	{
		MeshDataToolSharedPtr mesh_data_tool;

		get_mesh_data_cache()->get_mesh_data(name, mesh_data_tool,
			materials);

		mesh = get_mesh_builder()->get_mesh(vft_mesh, mesh_data_tool);
	}

	void MeshCache::get_mesh(const String &name,
		AbstractMeshSharedPtr &mesh,
		MaterialDescriptionVector &materials)
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
		MaterialDescriptionVector materials;

		get_mesh(name, mesh, materials);
	}

}
