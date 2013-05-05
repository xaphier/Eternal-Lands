/****************************************************************************
 *            tilebuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "tilebuilder.hpp"
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

		Uint32 get_low_detail_offset() noexcept
		{
			return 0;
		}

		Uint32 get_high_detail_scale() noexcept
		{
			return 8;
		}

		bool get_high_detail_tile(const Uint16 tile) noexcept
		{
			return tile == 0;
		}

		Uint32 get_low_detail_vertex_count() noexcept
		{
			return (TileBuilder::get_tile_size() + 1) *
				(TileBuilder::get_tile_size() + 1);
		}

		Uint32 get_high_detail_offset() noexcept
		{
			return get_low_detail_vertex_count();
		}

		 Uint32 get_high_detail_tile_size() noexcept
		{
			return get_high_detail_scale() *
				TileBuilder::get_tile_size();
		}

		Uint32 get_high_detail_vertex_count() noexcept
		{
			return (get_high_detail_tile_size() + 1) *
				(get_high_detail_tile_size() + 1);
		}

		Uint32 get_vertex_count() noexcept
		{
			return get_low_detail_vertex_count() +
				get_high_detail_vertex_count();
		}

		void load_tile(const String &name, const float scale,
			const float uv_scale, const float z_size,
			const bool use_simd,
			MeshDataToolSharedPtr &mesh_data_tool)
		{
			glm::vec4 normal, tangent, data, position;
			glm::vec3 vmin, vmax;
			glm::vec2 size, uv;
			VertexSemanticTypeSet semantics;
			Uint32 vertex_count, index, x, y, tile_size;

			vertex_count = get_vertex_count();

			semantics.insert(vst_position);
			semantics.insert(vst_texture_coordinate);
			semantics.insert(vst_normal);
			semantics.insert(vst_tangent);

			mesh_data_tool = boost::make_shared<MeshDataTool>(name,
				vertex_count, 0, 0, semantics, pt_triangles,
				false, use_simd);

			normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

			index = 0;

			tile_size = TileBuilder::get_tile_size();

			for (y = 0; y <= tile_size; ++y)
			{
				for (x = 0; x <= tile_size; ++x)
				{
					uv = glm::vec2(x, y);

					position.x = uv.x;
					position.y = uv.y;
					position.z = 0.0f;
					position.w = 1.0f;

					position.x *= scale;
					position.y *= scale;

					data = glm::vec4(uv * uv_scale, 0.0f,
						1.0f);

					mesh_data_tool->set_vertex_data(
						vst_position, index, position);

					mesh_data_tool->set_vertex_data(
						vst_normal, index, normal);
					mesh_data_tool->set_vertex_data(
						vst_tangent, index, tangent);
					mesh_data_tool->set_vertex_data(
						vst_texture_coordinate,
						index, data);

					++index;
				}
			}

			assert(index == get_high_detail_offset());

			tile_size = get_high_detail_tile_size();

			for (y = 0; y <= tile_size; ++y)
			{
				for (x = 0; x <= tile_size; ++x)
				{
					uv = glm::vec2(x, y) / glm::vec2(
						get_high_detail_scale());

					position.x = uv.x;
					position.y = uv.y;
					position.z = 0.0f;
					position.w = 1.0f;

					position.x *= scale;
					position.y *= scale;

					data = glm::vec4(uv * uv_scale, 0.0f,
						1.0f);

					mesh_data_tool->set_vertex_data(
						vst_position, index, position);

					mesh_data_tool->set_vertex_data(
						vst_normal, index, normal);
					mesh_data_tool->set_vertex_data(
						vst_tangent, index, tangent);
					mesh_data_tool->set_vertex_data(
						vst_texture_coordinate,
						index, data);

					++index;
				}
			}
		}

		Uint32 get_vertex(const Uint32 x, const Uint32 y,
			const Uint32 tile_size) noexcept
		{
			return x + y * (tile_size + 1);
		}

		void build_tile_index(const Uint32 x, const Uint32 y,
			const bool detailed, const bool use_base_vertex,
			Uint32 &index, Uint32 &min_vertex, Uint32 &max_vertex,
			Uint16* indices)
		{
			Uint32 i, j, xx, yy, size, offset, tile_size, vertex;

			size = 1;
			offset = 0;
			tile_size = TileBuilder::get_tile_size();

			if (detailed)
			{
				if (!use_base_vertex)
				{
					offset = get_high_detail_offset();
				}

				size = get_high_detail_scale();
				tile_size = get_high_detail_tile_size();
			}

			for (j = 0; j < size; ++j)
			{
				for (i = 0; i < size; ++i)
				{
					xx = x * size + i;
					yy = y * size + j;

					indices[index + 0] = get_vertex(xx + 0,
						yy + 0, tile_size) + offset;
					indices[index + 1] = get_vertex(xx + 1,
						yy + 0, tile_size) + offset;
					indices[index + 2] = get_vertex(xx + 0,
						yy + 1, tile_size) + offset;

					indices[index + 3] = get_vertex(xx + 0,
						yy + 1, tile_size) + offset;
					indices[index + 4] = get_vertex(xx + 1,
						yy + 0, tile_size) + offset;
					indices[index + 5] = get_vertex(xx + 1,
						yy + 1, tile_size) + offset;

					index += 6;
				}
			}

			xx = x * size;
			yy = y * size;
			vertex = get_vertex(xx + 0, yy + 0, tile_size) + offset;
			min_vertex = std::min(min_vertex, vertex);

			xx = x * size + size - 1;
			yy = y * size + size - 1;
			vertex = get_vertex(xx + 1, yy + 1, tile_size) + offset;
			max_vertex = std::max(max_vertex, vertex);
		}

	}

	TileBuilder::TileBuilder(const GlobalVarsConstSharedPtr &global_vars,
		const MeshBuilderConstSharedPtr &mesh_builder,
		const MaterialCacheSharedPtr &material_cache):
		m_global_vars(global_vars), m_mesh_builder(mesh_builder),
		m_material_cache(material_cache)
	{
		MeshDataToolSharedPtr mesh_data_tool;
		StringStream str;

		assert(get_vertex_count() <=
			std::numeric_limits<Uint16>::max());

		str << UTF8("tile_") << get_tile_size();

		load_tile(String(str.str()), get_tile_world_scale(),
			get_tile_uv_scale(), 0.1f, global_vars->get_use_simd(),
			mesh_data_tool);

		m_mesh = mesh_builder->get_mesh(vft_mesh, mesh_data_tool,
			String(str.str()));
	}

	TileBuilder::~TileBuilder() noexcept
	{
	}

	void TileBuilder::get_tile(const Uint16MultiArray2 &tile_page,
		AbstractMeshSharedPtr &mesh, MaterialSharedPtrVector &materials,
		BitSet64 &blend_mask) const
	{
		SubMeshVector sub_meshs;
		AbstractWriteMemorySharedPtr index_buffer;
		Uint32Set::const_iterator it, end;
		Uint32Set used_materials;
		glm::vec3 vmin, vmax;
		glm::uvec2 min, max;
		Uint16* indices;
		Uint32 x, y, tile_size, tile, index_count, index_offset, index;
		Uint32 min_vertex, max_vertex, base_vertex;
		bool detailed, use_base_vertex;

		use_base_vertex = get_global_vars()->get_opengl_3_2();

		tile_size = get_tile_size();

		assert(tile_page.shape()[0] == tile_size);
		assert(tile_page.shape()[1] == tile_size);

		index_count = 0;

		for (y = 0; y < tile_size; ++y)
		{
			for (x = 0; x < tile_size; ++x)
			{
				tile = tile_page[x][y];

				if (tile == std::numeric_limits<Uint16>::max())
				{
					continue;
				}

				if (get_high_detail_tile(tile))
				{
					index_count += 6 *
						get_high_detail_scale() *
						get_high_detail_scale();
				}
				else
				{
					index_count += 6;
				}

				used_materials.insert(tile);
			}
		}

		mesh = m_mesh->clone(all_bits_set, false);

		mesh->init(index_count, true);

		index_buffer = mesh->get_index_buffer();

		end = used_materials.end();
		index = 0;
		index_offset = 0;
		indices = static_cast<Uint16*>(index_buffer->get_ptr());
		materials.clear();
		blend_mask.reset();

		for (it = used_materials.begin(); it != end; ++it)
		{
			StringStream material;

			min_vertex = std::numeric_limits<Uint32>::max();
			max_vertex = 0;

			index_offset = index;
			detailed = get_high_detail_tile(*it);

			for (y = 0; y < tile_size; ++y)
			{
				for (x = 0; x < tile_size; ++x)
				{
					tile = tile_page[x][y];

					if (tile != *it)
					{
						continue;
					}

					build_tile_index(x, y, detailed,
						use_base_vertex, index,
						min_vertex, max_vertex,
						indices);

					min = glm::min(min,
						glm::uvec2(x + 0, y + 0));
					max = glm::max(max,
						glm::uvec2(x + 1, y + 1));
				}
			}

			index_count = index - index_offset;

			vmin = glm::vec3(glm::vec2(min) *
				get_tile_world_scale(), -0.05f);
			vmax = glm::vec3(glm::vec2(max) *
				get_tile_world_scale(), 0.0f);
			base_vertex = detailed && use_base_vertex ?
				get_high_detail_offset() :
				get_low_detail_offset();

			sub_meshs.push_back(SubMesh(BoundingBox(vmin, vmax),
				index_offset, index_count, min_vertex,
				max_vertex, base_vertex, true));

			material << UTF8("tile") << *it;

			blend_mask[materials.size()] = false;//*it == 0;

			materials.push_back(get_material_cache()->get_material(
				String(material.str())));
		}

		mesh->set_sub_meshs(sub_meshs);
	}

}
