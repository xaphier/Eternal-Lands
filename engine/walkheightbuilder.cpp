/****************************************************************************
 *            walkheightbuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "walkheightbuilder.hpp"
#include "meshdatatool.hpp"
#include "boundingbox.hpp"
#include "submesh.hpp"
#include "globalvars.hpp"
#include "meshbuilder.hpp"
#include "abstractmesh.hpp"
#include "abstractwritememory.hpp"
#include "materialcache.hpp"

namespace eternal_lands
{

	namespace
	{

		void load_walk_height(const Uint16MultiArray2 &walk_height_page,
			const String &name, const glm::vec3 &scale,
			const float z_offset, const Uint16 walk_height_count,
			const bool use_simd,
			MeshDataToolSharedPtr &mesh_data_tool)
		{
			glm::vec4 data, position;
			glm::vec3 vmin, vmax;
			glm::vec2 size, uv;
			VertexSemanticTypeSet semantics;
			Uint32 vertex_count, index, x, y, xx, yy, i;
			Uint32 walk_height_size;
			Uint16 walk_height;

			vertex_count = walk_height_count * 4;

			semantics.insert(vst_position);

			mesh_data_tool = boost::make_shared<MeshDataTool>(name,
				vertex_count, 0, 0, semantics, pt_triangles,
				false, use_simd);

			index = 0;
			walk_height_size =
				WalkHeightBuilder::get_walk_height_size();

			for (y = 0; y < walk_height_size; ++y)
			{
				for (x = 0; x < walk_height_size; ++x)
				{
					walk_height = walk_height_page[x][y];

					if (walk_height == 0)
					{
						continue;
					}

					for (i = 0; i < 4; ++i)
					{
						xx = x + (i % 2);
						yy = y + (i / 2);
						uv = glm::vec2(xx, yy);

						position.x = uv.x;
						position.y = uv.y;
						position.z = walk_height;
						position.w = 1.0f;

						position.x *= scale.x;
						position.y *= scale.y;
						position.z *= scale.z;
						position.z += z_offset + 0.01f;

						mesh_data_tool->set_vertex_data(
							vst_position, index,
							position);

						++index;
					}
				}
			}
		}

	}

	WalkHeightBuilder::WalkHeightBuilder(
		const GlobalVarsConstSharedPtr &global_vars,
		const MeshBuilderConstSharedPtr &mesh_builder):
		m_global_vars(global_vars), m_mesh_builder(mesh_builder)
	{
	}

	WalkHeightBuilder::~WalkHeightBuilder() noexcept
	{
	}

	bool WalkHeightBuilder::get_walk_height(
		const Uint16MultiArray2 &walk_height_page,
		AbstractMeshSharedPtr &mesh) const
	{
		MeshDataToolSharedPtr mesh_data_tool;
		StringStream str;
		SubMeshVector sub_meshs;
		AbstractWriteMemorySharedPtr index_buffer;
		glm::vec3 vmin, vmax, scale;
		float min_z, max_z, height;
		Uint16* indices;
		Uint32 x, y, walk_height_size, index_count, index_offset, index;
		Uint32 vertex, min_vertex, max_vertex, walk_height_count;

		walk_height_size = get_walk_height_size();
		index_count = walk_height_size * walk_height_size * 6;

		assert(walk_height_page.shape()[0] == walk_height_size);
		assert(walk_height_page.shape()[1] == walk_height_size);

		scale.x = get_walk_height_world_scale();
		scale.y = get_walk_height_world_scale();
		scale.z = get_walk_height_world_scale_z();

		walk_height_count = 0;

		for (y = 0; y < walk_height_size; ++y)
		{
			for (x = 0; x < walk_height_size; ++x)
			{
				if (walk_height_page[x][y] == 0)
				{
					continue;
				}

				walk_height_count++;
			}
		}

		if (walk_height_count == 0)
		{
			return false;
		}

		index_count = walk_height_count * 6;

		str << UTF8("walk_height_") << get_walk_height_size();

		load_walk_height(walk_height_page, String(str.str()), scale,
			get_walk_height_world_offset_z(), walk_height_count,
			get_global_vars()->get_use_simd(), mesh_data_tool);

		mesh = get_mesh_builder()->get_mesh(vft_mesh, mesh_data_tool,
			String(str.str()));

		mesh->init(index_count, true);

		index_buffer = mesh->get_index_buffer();

		index = 0;
		indices = static_cast<Uint16*>(index_buffer->get_ptr());

		min_z = std::numeric_limits<float>::max();
		max_z = -std::numeric_limits<float>::max();

		vertex = 0;

		for (y = 0; y < walk_height_size; ++y)
		{
			for (x = 0; x < walk_height_size; ++x)
			{
				if (walk_height_page[x][y] == 0)
				{
					continue;
				}

				indices[index + 0] = vertex + 0;
				indices[index + 1] = vertex + 1;
				indices[index + 2] = vertex + 2;
				indices[index + 3] = vertex + 2;
				indices[index + 4] = vertex + 1;
				indices[index + 5] = vertex + 3;
				index += 6;
				vertex += 4;
				height = walk_height_page[x][y] * scale.z;
				height += get_walk_height_world_offset_z();
				min_z = std::min(min_z, height);
				max_z = std::max(max_z, height);
			}
		}

		min_z = std::min(min_z, max_z - 0.05f);

		vmin = glm::vec3(0.0f, 0.0f, min_z);
		vmax = glm::vec3(walk_height_size * scale.x,
			walk_height_size * scale.y, max_z);

		index_offset = 0;
		min_vertex = 0;
		max_vertex = walk_height_size * walk_height_size * 4 - 1;

		sub_meshs.push_back(SubMesh(BoundingBox(vmin, vmax),
			index_offset, index_count, min_vertex, max_vertex));

		mesh->set_sub_meshs(sub_meshs);

		return true;
	}

}
