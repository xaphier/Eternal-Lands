/****************************************************************************
 *            simpleterrainmanager.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
#include "materialeffectdescription.hpp"
#include "meshbuilder.hpp"
#include "globalvars.hpp"

namespace eternal_lands
{

	SimpleTerrainManager::SimpleTerrainManager(
		const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache, const String &name):
		AbstractTerrainManager(name)
	{
		ImageSharedPtr height_map;

		load_xml(file_system);

		height_map = codec_manager->load_image(name, file_system,
			ImageCompressionTypeSet(), true);

		add_terrain_pages(height_map, mesh_builder, effect_cache,
			texture_cache, global_vars->get_use_simd());
	}

	SimpleTerrainManager::~SimpleTerrainManager() throw()
	{
	}

	void SimpleTerrainManager::set_terrain_page(
		const ImageSharedPtr &height_map, const glm::uvec2 &tile_offset,
		const Uint32Array2 index_counts, const Uint32 vertex_count,
		MeshDataToolSharedPtr &mesh_data_tool)
	{
		glm::vec4 normal, tangent, data;
		glm::vec3 min, max, position;
		glm::vec2 uv;
		Uint32 index, height;
		Uint16 x, y;

		normal = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		tangent = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

		index = 0;

		min = glm::vec3(std::numeric_limits<float>::max());
		max = -min;

		for (y = 0; y <= get_tile_size(); ++y)
		{
			for (x = 0; x <= get_tile_size(); ++x)
			{
				height = height_map->get_pixel_uint(
					x + tile_offset.x, y + tile_offset.y,
					0, 0, 0).r;

				position.x = x;
				position.y = y;
				position.z = height * get_height_scale();
				position *= get_scale();
				position += get_offset();

				min = glm::min(min, position);
				max = glm::max(max, position);

				uv = glm::vec2(x, y) /
					static_cast<float>(get_tile_size());

				data = glm::vec4(uv, 0.0f, 1.0f);

				mesh_data_tool->set_vertex_data(vst_position,
					index, glm::vec4(position, 1.0f));
				mesh_data_tool->set_vertex_data(vst_normal,
					index, normal);
				mesh_data_tool->set_vertex_data(vst_tangent,
					index, tangent);
				mesh_data_tool->set_vertex_data(
					vst_texture_coordinate_0, index, data);

				++index;
			}
		}

		mesh_data_tool->set_sub_mesh_data(0, SubMesh(
			BoundingBox(min, max), 0, index_counts[0], 0,
			vertex_count - 1));

		mesh_data_tool->set_sub_mesh_data(1, SubMesh(
			BoundingBox(min, max), index_counts[0], index_counts[1],
			0, vertex_count - 1));
	}

	void SimpleTerrainManager::add_terrain_pages(
		const ImageSharedPtr &height_map,
		const MeshBuilderSharedPtr &mesh_builder,
		const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache, const bool use_simd)
	{
		MaterialEffectDescriptionVector materials;
		MeshDataToolSharedPtr mesh_data_tool;
		AbstractMeshSharedPtr mesh;
		ObjectSharedPtr object;
		ObjectData object_data;
		Uint32Vector indices;
		glm::uvec2 tile_offset;
		Uint32Array2 index_counts;
		Uint32 vertex_count, index_count, i, x, y, height, width;
		Uint32 restart_index;
		VertexSemanticTypeSet semantics;

		vertex_count = get_tile_size() + 1;
		vertex_count *= get_tile_size() + 1;

		restart_index = IndexBuilder::build_plane_indices(
			indices, get_tile_size(), false, 0, true, 0);

		index_counts[0] = indices.size();

		restart_index = IndexBuilder::build_plane_indices(
			indices, get_tile_size(), false, 1, false, 0xF);

		index_counts[1] = indices.size() - index_counts[0];

		index_count = indices.size();

		semantics.insert(vst_position);
		semantics.insert(vst_texture_coordinate_0);
		semantics.insert(vst_normal);
		semantics.insert(vst_tangent);

		mesh_data_tool = boost::make_shared<MeshDataTool>(get_name(),
			vertex_count, index_count, 2, semantics, restart_index,
			pt_triangles, false, use_simd);

		for (i = 0; i < index_count; ++i)
		{
			mesh_data_tool->set_index_data(i, indices[i]);
		}

		width = height_map->get_width() / get_tile_size();
		height = height_map->get_height() / get_tile_size();

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				StringStream str;

				str << UTF8("terrain-") << x << UTF8("x") << y;

				object_data.set_name(String(str.str()));

				tile_offset.x = x;
				tile_offset.y = y;

				set_terrain_page(height_map, tile_offset,
					index_counts, vertex_count,
					mesh_data_tool);

				mesh = mesh_builder->get_mesh(
					vft_instanced_mesh, mesh_data_tool,
					get_name());

				object = boost::make_shared<Object>(object_data,
					mesh, materials, effect_cache,
					texture_cache);

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
