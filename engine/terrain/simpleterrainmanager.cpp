/****************************************************************************
 *            simpleterrainmanager.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "simpleterrainmanager.hpp"
#include "rstartree.hpp"
#include "objectvisitor.hpp"
#include "codec/codecmanager.hpp"
#include "image.hpp"
#include "meshdatatool.hpp"
#include "submesh.hpp"
#include "indexbuilder.hpp"
#include "object.hpp"
#include "materialdescription.hpp"
#include "effectdescription.hpp"
#include "meshbuilder.hpp"
#include "globalvars.hpp"
#include "vertexstream.hpp"
#include "abstractmesh.hpp"
#include "alignedvec4array.hpp"

namespace eternal_lands
{

	SimpleTerrainManager::SimpleTerrainManager(
		const ImageSharedPtr &vector_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MaterialSharedPtrVector &materials)
	{
		m_object_tree.reset(new RStarTree());

		add_terrain_pages(String(UTF8("shaders/default.xml")),
			vector_map, normal_map, dudv_map, mesh_builder,
			materials, global_vars->get_low_quality_terrain(),
			global_vars->get_use_simd());
	}

	SimpleTerrainManager::~SimpleTerrainManager() throw()
	{
	}

	void SimpleTerrainManager::set_terrain_page(
		const ImageSharedPtr &vector_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const AbstractMeshSharedPtr &mesh,
		const glm::uvec2 &tile_offset,
		const glm::vec2 &position_scale,
		const Uint32 vertex_count, const Uint32 index_count,
		const bool low_quality)
	{
		VertexStreamSharedPtr vertex_stream;
		AlignedVec4Array vectors, normals;
		SubMeshVector sub_meshs;
		glm::vec4 vector, normal;
		glm::vec3 min, max, position;
		glm::vec2 uv;
		glm::ivec2 pos;
		Uint16 x, y, tile_size;

		vertex_stream = mesh->get_vertex_stream(1);

		min = glm::vec3(std::numeric_limits<float>::max());
		max = -min;

		vectors.reserve(vertex_stream->get_vertex_count());
		normals.reserve(vertex_stream->get_vertex_count());

		tile_size = get_tile_size();

		for (y = 0; y <= tile_size; ++y)
		{
			for (x = 0; x <= tile_size; ++x)
			{
				pos = tile_offset + glm::uvec2(x, y);

				vector = vector_map->get_pixel(pos.x, pos.y,
					0, 0, 0);

				normal = normal_map->get_pixel(pos.x, pos.y,
					0, 0, 0);

				uv = glm::vec2(dudv_map->get_pixel(pos.x,
					pos.y, 0, 0, 0));;

				vector.w = uv.x;
				normal.w = uv.y;

				vectors.push_back(vector);
				normals.push_back(normal);

				position = glm::vec3(glm::vec2(x, y) *
					position_scale, 0.0f);
				position += (glm::vec3(vector) *
					glm::vec3(2.0f, 2.0f, 1.0f) -
					glm::vec3(1.0f, 1.0f, 0.0f)) *
					get_vector_scale();

				min = glm::min(min, position);
				max = glm::max(max, position);
			}
		}

		vertex_stream->set(vst_morph_position, vectors);
		vertex_stream->set(vst_morph_normal, normals);

		min = glm::min(min, max - 0.05f);

		sub_meshs.push_back(SubMesh(BoundingBox(min, max), 0,
			index_count, 0, vertex_count - 1));

		mesh->set_sub_meshs(sub_meshs);
	}

	void SimpleTerrainManager::add_terrain_pages(const String &effect,
		const ImageSharedPtr &vector_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const MeshBuilderSharedPtr &mesh_builder,
		const MaterialSharedPtrVector &materials,
		const bool low_quality, const bool use_simd)
	{
		MeshDataToolSharedPtr mesh_data_tool;
		Transformation transformation;
		AbstractMeshSharedPtr mesh, mesh_clone;
		ObjectSharedPtr object;
		ObjectData object_data;
		glm::vec3 min, max;
		glm::uvec2 tile_offset, terrain_size;
		glm::vec2 position_scale, position, uv;
		Uint32Vector indices;
		Uint32 vertex_count, index_count, i, x, y, height, width;
		Uint32 index;
		VertexSemanticTypeSet semantics;

		position_scale = get_position_scale();

		vertex_count = get_tile_size() + 1;
		vertex_count *= get_tile_size() + 1;

		IndexBuilder::build_plane_indices(indices, get_tile_size(),
			false, 0, true);

		semantics.insert(vst_position);

		width = vector_map->get_width() / get_tile_size();
		height = vector_map->get_height() / get_tile_size();

		terrain_size.x = width * get_tile_size() + 1;
		terrain_size.y = height * get_tile_size() + 1;

		index_count = indices.size();

		mesh_data_tool = boost::make_shared<MeshDataTool>(
			String(UTF8("terrain")), vertex_count, index_count,
			1, semantics, pt_triangles, false, use_simd);

		for (i = 0; i < index_count; ++i)
		{
			mesh_data_tool->set_index_data(i, indices[i]);
		}

		index = 0;

		min = glm::vec3(std::numeric_limits<float>::max());
		max = -min;

		for (y = 0; y <= get_tile_size(); ++y)
		{
			for (x = 0; x <= get_tile_size(); ++x)
			{
				position = glm::vec2(x, y) * position_scale;
				uv = glm::vec2(x, y) /
					glm::vec2(get_tile_size());

				mesh_data_tool->set_vertex_data(vst_position,
					index, glm::vec4(position, 0.0f, 1.0f));

				min = glm::min(min, glm::vec3(position, 0.0f));
				max = glm::max(max, glm::vec3(position, 0.0f));

				++index;
			}
		}

		min = glm::min(min, max - 0.05f);

		mesh_data_tool->set_sub_mesh_data(0, SubMesh(
			BoundingBox(min, max), 0, index_count, 0,
			vertex_count - 1));

		mesh = mesh_builder->get_mesh(vft_simple_terrain,
			mesh_data_tool, String(UTF8("terrain")));

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				StringStream str;

				str << UTF8("terrain ") << x << UTF8("x") << y;

				object_data.set_name(String(str.str()));

				tile_offset.x = x;
				tile_offset.y = y;
				tile_offset *= get_tile_size();

				mesh_clone = mesh->clone(0x01, true);

				set_terrain_page(vector_map, normal_map,
					dudv_map, mesh_clone, tile_offset,
					position_scale, vertex_count,
					index_count, low_quality);

				transformation.set_translation(
					glm::vec3(glm::vec2(tile_offset) *
					position_scale, 0.0f));

				object_data.set_world_transformation(
					transformation);

				object = boost::make_shared<Object>(object_data,
					mesh_clone, materials);

				m_object_tree->add(object);
			}
		}
	}

	void SimpleTerrainManager::intersect(const Frustum &frustum,
		ObjectVisitor &visitor) const
	{
		m_object_tree->intersect(frustum, visitor);
	}

}
