/****************************************************************************
 *            instancebuilder.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "instancebuilder.hpp"
#include "meshdatatool.hpp"
#include "materialdescription.hpp"
#include "instancedata.hpp"
#include "instancingdata.hpp"
#include "subobject.hpp"
#include "submesh.hpp"
#include "cpurasterizer.hpp"

namespace eternal_lands
{

	namespace
	{

		void add_vertex(MeshDataTool &mesh_data_tool,
			const MeshDataToolSharedPtr &source,
			const glm::mat4x3 &world_matrix,
			const Uint32 source_index, const Uint32 dest_index,
			glm::vec3 &min, glm::vec3 &max)
		{
			glm::vec4 data;
			glm::vec3 normal, tangent, position;

			assert(source->get_vertex_count() > source_index);
			assert(mesh_data_tool.get_vertex_count() > dest_index);

			position = glm::vec3(source->get_vertex_data(
				vst_position, source_index));
			position = world_matrix * glm::vec4(position, 1.0f);
			mesh_data_tool.set_vertex_data(vst_position,
				dest_index, glm::vec4(position, 1.0f));

			min = glm::min(min, position);
			max = glm::max(max, position);

			data = source->get_vertex_data(vst_normal,
				source_index);
			normal = glm::vec3(data);
			normal = glm::normalize(glm::mat3(world_matrix) *
				normal);
			mesh_data_tool.set_vertex_data(vst_normal, dest_index,
				glm::vec4(normal, data.w));

			data = source->get_vertex_data(vst_tangent,
				source_index);
			tangent = glm::vec3(data);
			tangent = glm::normalize(glm::mat3(world_matrix) *
				tangent);
			mesh_data_tool.set_vertex_data(vst_tangent, dest_index,
				glm::vec4(tangent, data.w));

			data = source->get_vertex_data(vst_texture_coordinate_0,
				source_index);

			mesh_data_tool.set_vertex_data(vst_texture_coordinate_0,
				dest_index, data);

			mesh_data_tool.set_vertex_data(vst_color,
				dest_index, source->get_vertex_data(
					vst_color, source_index));
		}

		void build_sub_mesh_unpacked(MeshDataTool &mesh_data_tool,
			const InstancingData &instancing_data,
			const glm::vec3 &center, const Uint32 sub_mesh_index,
			const Uint32 base_vertex, Uint32 &vertex_offset,
			Uint32 &index_offset, glm::vec3 &min, glm::vec3 &max)
		{
			std::map<Uint32, Uint32> index_map;
			glm::mat4x3 world_matrix;
			Uint32Vector indices;
			Uint32 i, index, count, min_vertex, max_vertex, size;

			instancing_data.get_mesh_data_tool(
				)->get_triangle_indices(sub_mesh_index, indices,
				false);

			min_vertex = instancing_data.get_mesh_data_tool(
				)->get_sub_meshs()[sub_mesh_index
					].get_min_vertex_derived();
			max_vertex = instancing_data.get_mesh_data_tool(
				)->get_sub_meshs()[sub_mesh_index
					].get_max_vertex_derived();

			assert(max_vertex >= min_vertex);

			size = vertex_offset + max_vertex - min_vertex + 1;

			if (mesh_data_tool.get_vertex_count() < size)
			{
				mesh_data_tool.resize_vertices(size);
			}

			size = index_offset + indices.size();

			if (mesh_data_tool.get_index_count() < size)
			{
				mesh_data_tool.resize_indices(size);
			}

			world_matrix = instancing_data.get_world_transformation(
				).get_matrix();
			world_matrix[3] -= center;

			count = indices.size();

			for (i = 0; i < count; ++i)
			{
				index = indices[i];

				if (index_map.find(index) == index_map.end())
				{
					index_map[index] = vertex_offset;

					add_vertex(mesh_data_tool,
						instancing_data.get_mesh_data_tool(),
						world_matrix,
						index, vertex_offset, min, max);

					vertex_offset++;
				}

				assert(base_vertex <= index_map[index]);
				mesh_data_tool.set_index_data(index_offset,
					index_map[index] - base_vertex);

				index_offset++;
			}
		}

		void build_sub_mesh_packed(MeshDataTool &mesh_data_tool,
			const InstancingData &instancing_data,
			const glm::vec3 &center, const Uint32 sub_mesh_index,
			const Uint32 base_vertex, Uint32 &vertex_offset,
			Uint32 &index_offset, glm::vec3 &min, glm::vec3 &max)
		{
			BoundingBox bounding_box;
			Uint32Vector indices;
			glm::mat4x3 world_matrix;
			Uint32 count, min_vertex, max_vertex, size, offset;
			Sint32 index_vertex_offset;
			bool triangles;

			triangles = instancing_data.get_mesh_data_tool(
				)->get_primitive() == pt_triangles;

			if (triangles)
			{
				count = instancing_data.get_mesh_data_tool(
					)->get_sub_mesh_data(
						sub_mesh_index).get_count();
				offset = instancing_data.get_mesh_data_tool(
					)->get_sub_mesh_data(
						sub_mesh_index).get_offset();
				index_vertex_offset =
					instancing_data.get_mesh_data_tool(
						)->get_sub_meshs()[
							sub_mesh_index
						].get_base_vertex();
			}
			else
			{
				instancing_data.get_mesh_data_tool(
					)->get_triangle_indices(sub_mesh_index,
						indices, false);
				count = indices.size();
				offset = 0;
				index_vertex_offset = 0;
			}

			min_vertex = instancing_data.get_mesh_data_tool(
				)->get_sub_meshs()[sub_mesh_index
					].get_min_vertex_derived();
			max_vertex = instancing_data.get_mesh_data_tool(
				)->get_sub_meshs()[sub_mesh_index
					].get_max_vertex_derived();

			assert(max_vertex >= min_vertex);

			size = vertex_offset + max_vertex - min_vertex + 1;

			if (mesh_data_tool.get_vertex_count() < size)
			{
				mesh_data_tool.resize_vertices(size);
			}

			size = index_offset + count;

			if (mesh_data_tool.get_index_count() < size)
			{
				mesh_data_tool.resize_indices(size);
			}

			index_vertex_offset += vertex_offset - min_vertex -
				base_vertex;

			if (triangles)
			{
				mesh_data_tool.set_indices(
					instancing_data.get_mesh_data_tool(
						)->get_indices(),
					offset, index_offset, count,
					index_vertex_offset);
			}
			else
			{
				mesh_data_tool.set_indices(indices,
					offset, index_offset, count,
					index_vertex_offset);
			}

			index_offset += count;

			count = max_vertex - min_vertex + 1;

			world_matrix = instancing_data.get_world_transformation(
				).get_matrix();
			world_matrix[3] -= center;

			mesh_data_tool.transform_vertics(
				*instancing_data.get_mesh_data_tool(),
				vst_position, min_vertex, vertex_offset, count,
				world_matrix);

			mesh_data_tool.transform_vertics(
				*instancing_data.get_mesh_data_tool(),
				vst_normal, min_vertex, vertex_offset, count,
				glm::mat3x3(world_matrix));

			mesh_data_tool.transform_vertics(
				*instancing_data.get_mesh_data_tool(),
				vst_tangent, min_vertex, vertex_offset, count,
				glm::mat3x3(world_matrix));

			mesh_data_tool.copy_vertics(
				*instancing_data.get_mesh_data_tool(),
				vst_texture_coordinate_0, min_vertex,
				vertex_offset, count);

			mesh_data_tool.copy_vertics(
				*instancing_data.get_mesh_data_tool(),
				vst_color, min_vertex, vertex_offset, count);

			vertex_offset += count;

			bounding_box = instancing_data.get_mesh_data_tool(
				)->get_sub_meshs()[sub_mesh_index
					].get_bounding_box();

			bounding_box = bounding_box.transform(
				instancing_data.get_world_transformation());

			min = glm::min(min, bounding_box.get_min() - center);
			max = glm::max(max, bounding_box.get_max() - center);
		}

		bool build_sub_mesh(const glm::vec3 &center,
			const InstancingData &instancing_data,
			const MeshDataToolSharedPtr &mesh_data_tool,
			const String &material_name, const Uint32 index,
			const Uint32 base_vertex, glm::vec3 &min,
			glm::vec3 &max, Uint32 &vertex_offset,
			Uint32 &index_offset,
			AlignedSint16Vec8Array &min_max_boxes)
		{
			BoundingBox bounding_box;

			if (material_name !=
				instancing_data.get_material_names()[index])
			{
				return false;
			}

			bounding_box = instancing_data.get_bounding_box();

			bounding_box.set_center(bounding_box.get_center() -
				center);

			CpuRasterizer::append_min_max_box(bounding_box,
				min_max_boxes);

			if (instancing_data.get_mesh_data_tool(
				)->get_sub_mesh_data(index).get_packed())
			{
				build_sub_mesh_packed(*mesh_data_tool,
					instancing_data, center, index,
					base_vertex, vertex_offset,
					index_offset, min, max);
			}
			else
			{
				build_sub_mesh_unpacked(*mesh_data_tool,
					instancing_data, center, index,
					base_vertex, vertex_offset,
					index_offset, min, max);
			}

			return true;
		}

	}

	InstanceBuilder::InstanceBuilder(
		const InstancingDataVector &instancing_datas, const Uint32 id,
		const bool use_simd, const bool use_base_vertex):
		m_instancing_datas(instancing_datas), m_id(id),
		m_use_simd(use_simd), m_use_base_vertex(use_base_vertex)
	{
		assert(get_instancing_datas().size() > 1);
	}

	InstanceBuilder::~InstanceBuilder() noexcept
	{
	}

	glm::vec3 InstanceBuilder::get_center() const
	{
		BoundingBox bounding_box;
		Uint32 i;

		assert(get_instancing_datas().size() > 0);

		bounding_box = get_instancing_data(0).get_bounding_box();

		for (i = 1; i < get_instancing_datas().size(); ++i)
		{
			bounding_box.merge(
				get_instancing_data(i).get_bounding_box());
		}

		return bounding_box.get_center();
	}

	void InstanceBuilder::build_instance_sub_mesh(const glm::vec3 &center,
		const MeshDataToolSharedPtr &mesh_data_tool,
		const String &material, const Uint32 sub_mesh_index,
		const Uint32 base_vertex, Uint32 &vertex_offset,
		Uint32 &index_offset, SubObjectVector &sub_objects,
		Uint32 &min_max_boxes_index,
		AlignedSint16Vec8Array &min_max_boxes)
	{
		SubMesh sub_mesh;
		SubObject sub_object;
		glm::vec3 min, max;
		Uint32 count, i, min_vertex, max_vertex, offset;
		Uint32 min_max_boxes_count;

		sub_mesh.set_offset(index_offset);
		sub_mesh.set_min_vertex(vertex_offset - base_vertex);

		sub_object.set_material(sub_mesh_index);

		min_max_boxes_count = 0;

		BOOST_FOREACH(const InstancingData &instancing_data,
			get_instancing_datas())
		{
			count = instancing_data.get_material_names().size();

			sub_object.set_selection(
				instancing_data.get_selection());
			sub_object.set_id(instancing_data.get_id());

			for (i = 0; i < count; ++i)
			{
				offset = index_offset;
				min_vertex = vertex_offset;

				if (build_sub_mesh(center, instancing_data,
					mesh_data_tool, material, i,
					base_vertex, min, max, vertex_offset,
					index_offset, min_max_boxes))
				{
					min_max_boxes_count++;
					max_vertex = vertex_offset - 1;

					sub_object.set_offset(offset);
					sub_object.set_min_vertex(min_vertex -
						base_vertex);
					sub_object.set_count(index_offset -
						offset);
					sub_object.set_max_vertex(max_vertex -
						base_vertex);
					sub_object.set_base_vertex(base_vertex);

					sub_objects.push_back(sub_object);
				}	
			}
		}

		assert(index_offset > 0);
		assert(vertex_offset > 0);
		assert(sub_objects.size() > 0);

		sub_mesh.set_bounding_box(BoundingBox(min, max));
		sub_mesh.set_count(index_offset - sub_mesh.get_offset());
		sub_mesh.set_max_vertex(vertex_offset - 1 - base_vertex);
		sub_mesh.set_base_vertex(base_vertex);
		sub_mesh.set_min_max_boxes_index(min_max_boxes_index);
		sub_mesh.set_min_max_boxes_count(min_max_boxes_count);
		mesh_data_tool->set_sub_mesh_data(sub_mesh_index, sub_mesh);

		min_max_boxes_index += min_max_boxes_count;
	}

	void InstanceBuilder::build_instance()
	{
		SubMesh sub_mesh;
		Transformation transformation;
		glm::vec3 center;
		StringSet materials;
		MeshDataToolSharedPtr mesh_data_tool;
		StringVector material_names;
		SubObjectVector instanced_objects;
		VertexSemanticTypeSet semantics;
		AlignedSint16Vec8Array min_max_boxes;
		glm::vec4 texture_scale_offset;
		Uint32 index_count, vertex_count, sub_mesh_count, base_vertex;
		Uint32 vertex_offset, index_offset, sub_mesh_index;
		SelectionType selection;
		StringStream str;
		Uint32 i, count, min_max_boxes_index, min_max_boxes_count;

		center = get_center();

		index_count = 0;
		vertex_count = 0;
		min_max_boxes_count = 0;

		selection = st_none;

		texture_scale_offset = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);

		BOOST_FOREACH(const InstancingData &instancing_data,
			get_instancing_datas())
		{
			if (instancing_data.get_selection() != st_none)
			{
				selection = st_select;
			}

			mesh_data_tool = instancing_data.get_mesh_data_tool();
			index_count += mesh_data_tool->get_index_count();
			vertex_count += mesh_data_tool->get_vertex_count();
			min_max_boxes_count +=
				mesh_data_tool->get_sub_mesh_count();

			count = instancing_data.get_material_names().size();

			for (i = 0; i < count; ++i)
			{
				materials.insert(
					instancing_data.get_material_names(
						)[i]);
			}

			if (!str.str().empty())
			{
				str << std::endl;
			}

			str << instancing_data.get_name();
		}

		assert(materials.size() > 0);

		sub_mesh_count = materials.size();

		semantics.insert(vst_position);
		semantics.insert(vst_normal);
		semantics.insert(vst_color);
		semantics.insert(vst_tangent);
		semantics.insert(vst_texture_coordinate_0);

		mesh_data_tool = boost::make_shared<MeshDataTool>(
			String(str.str()), vertex_count, index_count,
			sub_mesh_count, semantics, pt_triangles, false,
			get_use_simd());

		base_vertex = 0;
		index_offset = 0;
		vertex_offset = 0;
		sub_mesh_index = 0;
		min_max_boxes_index = 0;

		min_max_boxes.reserve(min_max_boxes_count);

		BOOST_FOREACH(const String &material, materials)
		{
			if (get_use_base_vertex())
			{
				base_vertex = vertex_offset;
			}

			build_instance_sub_mesh(center, mesh_data_tool,
				material, sub_mesh_index, base_vertex,
				vertex_offset, index_offset, instanced_objects,
				min_max_boxes_index, min_max_boxes);

			material_names.push_back(material);
			sub_mesh_index++;

			assert(instanced_objects.size() > 0);
		}

		mesh_data_tool->set_min_max_boxes(min_max_boxes);

		assert(material_names.size() > 0);
		assert(instanced_objects.size() > 0);

		transformation.set_translation(center);

		m_instance_data.reset(new InstanceData(ObjectDescription(
			transformation, material_names, String(str.str()),
			1.0f, get_id(), selection, bt_disabled),
			mesh_data_tool, instanced_objects));
	}

	void InstanceBuilder::set_instance(InstanceDataVector &instances)
	{
		instances.push_back(m_instance_data);
	}

	void InstanceBuilder::remove_singles(
		InstancingDataVector &instancing_datas,
		ObjectDescriptionVector &uninstanced)
	{
		StringUint32Map material_names_map;
		StringUint32Map::iterator found;
		Uint32 i;
		bool single;

		BOOST_FOREACH(const InstancingData &instancing_data,
			instancing_datas)
		{
			BOOST_FOREACH(const String &material_name,
				instancing_data.get_material_names())
			{
				found = material_names_map.find(material_name);

				if (found != material_names_map.end())
				{
					found->second++;
					continue;
				}

				material_names_map.insert(
					std::pair<String, Uint32>(
						material_name, 1));
			}
		}

		for (i = 0; i < instancing_datas.size(); )
		{
			single = true;

			BOOST_FOREACH(const String &material_name,
				instancing_datas[i].get_material_names())
			{
				found = material_names_map.find(material_name);

				assert(found != material_names_map.end());

				single = found->second == 1;

				if (!single)
				{
					break;
				}
			}

			if (!single)
			{
				++i;
				continue;
			}

			uninstanced.push_back(instancing_datas[i]);
			instancing_datas.erase(instancing_datas.begin() + i);
		}
	}

}
