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
#include "effectdescription.hpp"
#include "meshbuilder.hpp"
#include "globalvars.hpp"
#include "heightmapuvtool.hpp"

namespace eternal_lands
{

	SimpleTerrainManager::SimpleTerrainManager(
		const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache, const String &name):
		BasicTerrainManager(name)
	{
		ImageSharedPtr height_map;
		EffectDescription effect;

		m_object_tree.reset(new RStarTree());

		load_xml(file_system);

		height_map = codec_manager->load_image(get_height_map(),
			file_system, ImageCompressionTypeSet(), true);

		effect.load_xml(file_system,
			String(UTF8("shaders/simple_terrain.xml")));

		add_terrain_pages(effect, height_map, mesh_builder,
			effect_cache, texture_cache,
			global_vars->get_low_quality_terrain(),
			global_vars->get_use_simd());
	}

	SimpleTerrainManager::~SimpleTerrainManager() throw()
	{
	}

	void SimpleTerrainManager::set_terrain_page(const HeightMapUvTool &uvs,
		const ImageSharedPtr &height_map, const glm::uvec2 &tile_offset,
		const glm::uvec2 &terrain_size,
		const Uint32Vector &index_counts, const Uint32 vertex_count,
		MeshDataToolSharedPtr &mesh_data_tool)
	{
		glm::vec4 normal, tangent;
		glm::vec3 min, max, position;
		glm::vec2 uv;
		Uint32 index, i, count, offset;
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
				position.x = x + tile_offset.x;
				position.y = y + tile_offset.y;
				position.z = height_map->get_pixel_uint(
					x + tile_offset.x, y + tile_offset.y,
					0, 0, 0).r;

				min = glm::min(min, position);
				max = glm::max(max, position);

				uv = uvs.get_uv(x + tile_offset.x,
					y + tile_offset.y);
				uv.s /= terrain_size.x - 1;
				uv.t /= terrain_size.y - 1;

				mesh_data_tool->set_vertex_data(vst_position,
					index, glm::vec4(position, 1.0f));
				mesh_data_tool->set_vertex_data(vst_normal,
					index, normal);
				mesh_data_tool->set_vertex_data(vst_tangent,
					index, tangent);
				mesh_data_tool->set_vertex_data(
					vst_texture_coordinate_0, index,
					glm::vec4(uv, 0.0f, 1.0f));

				++index;
			}
		}

		min = glm::min(min, max - 0.05f);

		count = index_counts.size();
		offset = 0;

		for (i = 0; i < count; ++i)
		{
			mesh_data_tool->set_sub_mesh_data(i, SubMesh(
				BoundingBox(min, max), offset, index_counts[i],
				0, vertex_count - 1));

			offset += index_counts[i];
		}
	}

	void SimpleTerrainManager::add_terrain_pages(
		const EffectDescription &effect,
		const ImageSharedPtr &height_map,
		const MeshBuilderSharedPtr &mesh_builder,
		const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache,
		const bool low_quality, const bool use_simd)
	{
		MaterialEffectDescriptionVector materials;
		MaterialEffectDescription material;
		MeshDataToolSharedPtr mesh_data_tool;
		AbstractMeshSharedPtr mesh;
		ObjectSharedPtr object;
		ObjectData object_data;
		glm::vec4 scale_offset;
		glm::uvec2 tile_offset, terrain_size;
		Uint32Vector indices, index_counts;
		Uint16Array2Vector lods;
		Uint16Array4 lods_counts;
		Uint16Array4 lods_offsets;
		Uint16Array3 lods_distances;
		Uint16Array2 lod;
		Uint32 vertex_count, index_count, i, x, y, height, width;
		Uint32 restart_index, count;
		VertexSemanticTypeSet semantics;
		HeightMapUvTool uvs(height_map, get_height_scale());

		vertex_count = get_tile_size() + 1;
		vertex_count *= get_tile_size() + 1;

		index_count = 0;

		if (low_quality)
		{
			restart_index = IndexBuilder::build_plane_indices(
				indices, get_tile_size(), false, 0, false, 0);

			index_counts.push_back(indices.size() - index_count);
			index_count = indices.size();

			restart_index = IndexBuilder::build_plane_indices(
				indices, get_tile_size(), false, 1, true, 0);

			index_counts.push_back(indices.size() - index_count);
			index_count = indices.size();

			restart_index = IndexBuilder::build_plane_indices(
				indices, get_tile_size(), false, 1, false, 0xF);

			index_counts.push_back(indices.size() - index_count);
			index_count = indices.size();
		}
		else
		{
			restart_index = IndexBuilder::build_plane_indices(
				indices, get_tile_size(), false, 0, true, 0);

			index_counts.push_back(indices.size() - index_count);
			index_count = indices.size();

			restart_index = IndexBuilder::build_plane_indices(
				indices, get_tile_size(), false, 0, false, 0xF);

			index_counts.push_back(indices.size() - index_count);
			index_count = indices.size();
		}

		semantics.insert(vst_position);
		semantics.insert(vst_texture_coordinate_0);
		semantics.insert(vst_normal);
		semantics.insert(vst_tangent);

		width = height_map->get_width() / get_tile_size();
		height = height_map->get_height() / get_tile_size();

		terrain_size.x = width * get_tile_size() + 1;
		terrain_size.y = height * get_tile_size() + 1;

		object_data.set_world_transformation(get_transformation());

		count = index_counts.size();

		for (i = 0; i < count; ++i)
		{
			lod[0] = 0;
			lod[1] = std::max(1u, i);

			lods.push_back(lod);
		}

		lods_counts[0] = 1;
		lods_counts[1] = 1;
		lods_counts[2] = 1;
		lods_counts[3] = 1;
		
		lods_offsets[0] = std::min(0u, count - 1);
		lods_offsets[1] = std::min(1u, count - 1);
		lods_offsets[2] = std::min(2u, count - 1);
		lods_offsets[3] = std::min(3u, count - 1);

		lods_distances[0] = 10;
		lods_distances[1] = 20;
		lods_distances[2] = 40;

		uvs.buil_relaxed_uv();

		material.set_texture(get_albedo_map(0), stt_albedo_0);
		material.set_texture(get_albedo_map(1), stt_albedo_1);
		material.set_texture(get_albedo_map(2), stt_albedo_2);
		material.set_texture(get_albedo_map(3), stt_albedo_3);
		material.set_texture(get_blend_map(), stt_blend_0);

		material.set_effect_description(effect);

		scale_offset.x = width;
		scale_offset.y = height;
		scale_offset.z = 0.0f;
		scale_offset.w = 0.0f;

		material.set_texture_scale_offset(scale_offset);

		materials.push_back(material);

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				StringStream str;

				str << get_name() << UTF8(" terrain ") << x;
				str << UTF8("x") << y;

				mesh_data_tool =
					boost::make_shared<MeshDataTool>(
						String(str.str()), vertex_count,
						index_count, count, semantics,
						restart_index, pt_triangles,
						false, use_simd);

				for (i = 0; i < index_count; ++i)
				{
					mesh_data_tool->set_index_data(i,
						indices[i]);
				}

				object_data.set_name(String(str.str()));

				tile_offset.x = x;
				tile_offset.y = y;
				tile_offset *= get_tile_size();

				set_terrain_page(uvs, height_map, tile_offset,
					terrain_size, index_counts,
					vertex_count, mesh_data_tool);

				mesh = mesh_builder->get_mesh(
					vft_simple_terrain, mesh_data_tool,
					String(str.str()));

				object = boost::make_shared<Object>(object_data,
					mesh, materials, effect_cache,
					texture_cache, LodData(lods,
						lods_counts, lods_offsets,
						lods_distances));

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