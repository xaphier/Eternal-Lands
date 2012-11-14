/****************************************************************************
 *            simpleterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "simpleterrain.hpp"
#include "rstartree.hpp"
#include "objectvisitor.hpp"
#include "codec/codecmanager.hpp"
#include "image.hpp"
#include "imageupdate.hpp"
#include "meshdatatool.hpp"
#include "submesh.hpp"
#include "indexbuilder.hpp"
#include "object.hpp"
#include "materialdescription.hpp"
#include "effect/effectdescription.hpp"
#include "meshbuilder.hpp"
#include "globalvars.hpp"
#include "vertexstream.hpp"
#include "abstractmesh.hpp"
#include "alignedarrays.hpp"
#include "material.hpp"
#include "terrainvisitor.hpp"
#include "terrainrenderingdata.hpp"
#include "boundingboxvisitor.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	namespace
	{

		glm::vec4 get_low_quality_pixel_3(const ImageSharedPtr &image,
			const Uint32 x, const Uint32 y)
		{
			glm::vec4 result;
			Uint32 x0, y0, x1, y1;

			x0 = std::min(x, image->get_width() - 1);
			y0 = std::min(y, image->get_height() - 1);
			x1 = std::min(x + 1, image->get_width() - 1);
			y1 = std::min(y + 1, image->get_height() - 1);

			result = image->get_pixel(x0, y0, 0, 0, 0);
			result += image->get_pixel(x1, y0, 0, 0, 0);
			result += image->get_pixel(x0, y1, 0, 0, 0);
			result += image->get_pixel(x1, y1, 0, 0, 0);

			return result * 0.25f;
		}

		glm::vec2 get_low_quality_pixel_2(const ImageSharedPtr &image,
			const Uint32 x, const Uint32 y)
		{
			glm::vec2 result;
			Uint32 x0, y0, x1, y1;

			x0 = std::min(x, image->get_width() - 1);
			y0 = std::min(y, image->get_height() - 1);
			x1 = std::min(x + 1, image->get_width() - 1);
			y1 = std::min(y + 1, image->get_height() - 1);

			result = glm::vec2(image->get_pixel(x0, y0, 0, 0, 0));
			result += glm::vec2(image->get_pixel(x1, y0, 0, 0,
				0));
			result += glm::vec2(image->get_pixel(x0, y1, 0, 0,
				0));
			result += glm::vec2(image->get_pixel(x1, y1, 0,
				0, 0));

			return result * 0.25f;
		}

	}

	SimpleTerrain::SimpleTerrain(const GlobalVarsSharedPtr &global_vars,
		const EffectCacheSharedPtr &effect_cache,
		const MeshBuilderSharedPtr &mesh_builder,
		const MaterialBuilderSharedPtr &material_builder,
		const MaterialCacheSharedPtr &material_cache,
		const String &material, const String &effect):
		AbstractTerrain(global_vars, effect_cache, material_builder,
			material_cache, material, effect)
	{
		m_object_tree.reset(new RStarTree());

		init_terrain_pages(mesh_builder, global_vars->get_use_simd());
	}

	SimpleTerrain::~SimpleTerrain() noexcept
	{
	}

	void SimpleTerrain::set_terrain_page(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const AbstractMeshSharedPtr &mesh,
		const glm::uvec2 &tile_offset,
		const glm::vec2 &position_scale)
	{
		VertexStreamSharedPtr vertex_stream;
		AlignedVec4Array vectors, normals;
		SubMeshVector sub_meshs;
		glm::vec4 data;
		glm::vec3 vector, min, max, position;
		glm::vec2 normal, dudv;
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

				data = displacement_map->get_pixel(pos.x,
					pos.y, 0, 0, 0);

				vector = AbstractTerrain::get_offset_scaled_0_1(
					data);

				normal = glm::vec2(normal_map->get_pixel(pos.x,
					pos.y, 0, 0, 0));

				dudv = glm::vec2(dudv_map->get_pixel(pos.x,
					pos.y, 0, 0, 0));;

				vectors.push_back(data);

				data.x = normal.x;
				data.y = normal.y;
				data.z = dudv.x;
				data.w = dudv.y;

				normals.push_back(data);

				position = glm::vec3(glm::vec2(x, y) *
					position_scale, 0.0f);
				position += vector;

				min = glm::min(min, position);
				max = glm::max(max, position);
			}
		}

		vertex_stream->set(vst_morph_position, vectors);
		vertex_stream->set(vst_morph_normal, normals);

		min = glm::min(min, max - 0.05f);

		sub_meshs.push_back(SubMesh(BoundingBox(min, max), 0,
			mesh->get_index_count(), 0,
			mesh->get_vertex_count() - 1));

		mesh->set_sub_meshs(sub_meshs);
	}

	void SimpleTerrain::set_terrain_page_low_quality(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const AbstractMeshSharedPtr &mesh,
		const glm::uvec2 &tile_offset,
		const glm::vec2 &position_scale)
	{
		VertexStreamSharedPtr vertex_stream;
		AlignedVec4Array vectors, normals;
		SubMeshVector sub_meshs;
		glm::vec4 data;
		glm::vec3 vector, min, max, position;
		glm::vec2 normal, dudv;
		glm::ivec2 pos;
		Uint16 x, y, tile_size;

		vertex_stream = mesh->get_vertex_stream(1);

		min = glm::vec3(std::numeric_limits<float>::max());
		max = -min;

		vectors.reserve(vertex_stream->get_vertex_count());
		normals.reserve(vertex_stream->get_vertex_count());

		tile_size = get_tile_size() / 2;

		for (y = 0; y <= tile_size; ++y)
		{
			for (x = 0; x <= tile_size; ++x)
			{
				pos = (tile_offset + glm::uvec2(x, y)) * 2u;

				data = get_low_quality_pixel_3(
					displacement_map, pos.x, pos.y);

				vector = AbstractTerrain::get_offset_scaled_0_1(
					data);

				normal = get_low_quality_pixel_2(normal_map,
					pos.x, pos.y);

				dudv = get_low_quality_pixel_2(dudv_map,
					pos.x, pos.y);

				vectors.push_back(data);

				data.x = normal.x;
				data.y = normal.y;
				data.z = dudv.x;
				data.w = dudv.y;

				normals.push_back(data);

				position = glm::vec3(glm::vec2(x, y) *
					position_scale, 0.0f);
				position += vector;

				min = glm::min(min, position);
				max = glm::max(max, position);
			}
		}

		vertex_stream->set(vst_morph_position, vectors);
		vertex_stream->set(vst_morph_normal, normals);

		min = glm::min(min, max - 0.05f);

		sub_meshs.push_back(SubMesh(BoundingBox(min, max), 0,
			mesh->get_index_count(), 0,
			mesh->get_vertex_count() - 1));

		mesh->set_sub_meshs(sub_meshs);
	}

	void SimpleTerrain::init_terrain_pages(
		const MeshBuilderSharedPtr &mesh_builder, const bool use_simd)
	{
		MeshDataToolSharedPtr mesh_data_tool;
		Transformation transformation;
		AbstractMeshSharedPtr mesh_clone;
		ObjectSharedPtr object;
		ObjectData object_data;
		MaterialSharedPtrVector materials;
		Uint32Vector indices;
		glm::vec3 min, max;
		glm::vec2 position_scale, position, uv;
		Uint32 vertex_count, index_count, i, x, y;
		Uint32 index, tile_size;
		VertexSemanticTypeSet semantics;

		materials.push_back(get_terrain_material());

		position_scale = glm::vec2(get_patch_scale());

		tile_size = get_tile_size();

		if (get_low_quality_terrain())
		{
			tile_size /= 2;
			position_scale *= 2;
		}

		vertex_count = tile_size + 1;
		vertex_count *= tile_size + 1;

		IndexBuilder::build_plane_indices(tile_size, false, 0, true,
			indices);

		semantics.insert(vst_position);

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

		for (y = 0; y <= tile_size; ++y)
		{
			for (x = 0; x <= tile_size; ++x)
			{
				position = glm::vec2(x, y) * position_scale;
				uv = glm::vec2(x, y) / glm::vec2(tile_size);

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

		m_mesh = mesh_builder->get_mesh(vft_simple_terrain,
			mesh_data_tool, String(UTF8("terrain")));
	}

	void SimpleTerrain::add_terrain_page(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const glm::uvec2 &position)
	{
		MaterialSharedPtrVector materials;
		AbstractMeshSharedPtr mesh_clone;
		ObjectData object_data;
		ObjectSharedPtr object;
		Transformation transformation;
		StringStream str;
		glm::uvec2 tile_offset;
		glm::vec2 position_scale;
		Uint32 tile_size;

		materials.push_back(get_terrain_material());

		str << UTF8("terrain ") << position.x << UTF8("x");
		str << position.y;

		object_data.set_name(String(str.str()));

		tile_offset = position;

		tile_size = get_tile_size();
		position_scale = glm::vec2(get_patch_scale());

		if (get_low_quality_terrain())
		{
			tile_size /= 2;
			position_scale *= 2;
		}

		tile_offset *= tile_size;

		mesh_clone = m_mesh->clone(0x01, true);

		if (get_low_quality_terrain())
		{
			set_terrain_page_low_quality(displacement_map,
				normal_map, dudv_map, mesh_clone, tile_offset,
				position_scale);
		}
		else
		{
			set_terrain_page(displacement_map, normal_map,
				dudv_map, mesh_clone, tile_offset,
				position_scale);
		}

		transformation.set_translation(
			glm::vec3(glm::vec2(tile_offset) * position_scale,
			0.0f));

		object_data.set_world_transformation(
			transformation);

		object_data.set_id(position.x + (position.y << 16));

		object = boost::make_shared<Object>(object_data,
			mesh_clone, materials);

		LOG_DEBUG(lt_terrain, UTF8("adding page <%1%, %2%> at %3%"),
			position.x % position.y % glm::to_string(
				transformation.get_translation()));

		m_object_tree->add(object);
	}

	void SimpleTerrain::intersect(const Frustum &frustum,
		ObjectVisitor &visitor) const
	{
		m_object_tree->intersect(frustum, visitor);
	}

	void SimpleTerrain::intersect(const Frustum &frustum,
		const glm::vec3 &camera, BoundingBox &bounding_box) const
	{
		BoundingBoxVisitor visitor;

		m_object_tree->intersect(frustum, visitor);

		bounding_box = visitor.get_bounding_box();
	}

	void SimpleTerrain::intersect(const Frustum &frustum,
		const glm::vec3 &camera, TerrainVisitor &terrain) const
	{
		terrain.set_instances(0);
	}

	void SimpleTerrain::init_rendering_data(
		TerrainRenderingData &rendering_data) const
	{
		rendering_data.set_mesh(AbstractMeshSharedPtr());
		rendering_data.set_offset(0);
		rendering_data.set_max_instances(0);
	}

	void SimpleTerrain::do_set_geometry_maps(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map)
	{
		ImageSharedPtr displacement_map_tmp, normal_map_tmp;
		ImageSharedPtr dudv_map_tmp;
		Uint32 x, y, height, width;

		m_object_tree->clear();

		set_terrain_size((glm::vec2(displacement_map->get_size())
			-1.0f) * get_patch_scale());

		displacement_map_tmp = displacement_map->decompress(false,
			true, false);
		normal_map_tmp = normal_map->decompress(false, true, false);
		dudv_map_tmp = dudv_map->decompress(false, true, false);

		width = displacement_map->get_width() / get_tile_size();
		height = displacement_map->get_height() / get_tile_size();

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				add_terrain_page(displacement_map_tmp,
					normal_map_tmp, dudv_map_tmp,
					glm::uvec2(x, y));
			}
		}

		set_bounding_box(m_object_tree->get_bounding_box());
	}

	void SimpleTerrain::do_update_geometry_maps(
		const ImageUpdate &displacement_map,
		const ImageUpdate &normal_map, const ImageUpdate &dudv_map)
	{
		
		ImageSharedPtr displacement_map_tmp, normal_map_tmp;
		ImageSharedPtr dudv_map_tmp;
		Uint32 x, y, height, width;

		m_object_tree->clear();

		set_terrain_size((glm::vec2(displacement_map.get_image(
			)->get_size()) -1.0f) * get_patch_scale());

		displacement_map_tmp = displacement_map.get_image(
			)->decompress(false, true, false);
		normal_map_tmp = normal_map.get_image()->decompress(false,
			true, false);
		dudv_map_tmp = dudv_map.get_image()->decompress(false, true,
			false);

		width = displacement_map.get_image()->get_width() /
			get_tile_size();
		height = displacement_map.get_image()->get_height() /
			get_tile_size();

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				add_terrain_page(displacement_map_tmp,
					normal_map_tmp, dudv_map_tmp,
					glm::uvec2(x, y));
			}
		}

		set_bounding_box(m_object_tree->get_bounding_box());
	}

	void SimpleTerrain::clear()
	{
		m_object_tree->clear();
		set_bounding_box(m_object_tree->get_bounding_box());
	}

}
