/****************************************************************************
 *            instancebuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "instancebuilder.hpp"
#include "meshdatatool.hpp"
#include "materialdescription.hpp"
#include "instancedata.hpp"
#include "instancingdata.hpp"
#include "subobject.hpp"

namespace eternal_lands
{

	namespace
	{

		void add_vertex(MeshDataTool &mesh_data_tool,
			const MeshDataToolSharedPtr &source,
			const glm::mat4x3 &world_matrix,
			const glm::vec4 &color, const Uint32 source_index,
			const Uint32 dest_index, glm::vec3 &min, glm::vec3 &max,
			const float material_index)
		{
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

			normal = glm::vec3(source->get_vertex_data(
				vst_normal, source_index));
			normal = glm::mat3(world_matrix) * normal;
			mesh_data_tool.set_vertex_data(vst_normal, dest_index,
				glm::vec4(normal, material_index));

			tangent = glm::vec3(source->get_vertex_data(
				vst_tangent, source_index));
			tangent = glm::mat3(world_matrix) * tangent;
			mesh_data_tool.set_vertex_data(vst_tangent, dest_index,
				glm::vec4(tangent, 1.0f));

			mesh_data_tool.set_vertex_data(vst_color, dest_index,
				color);

			mesh_data_tool.set_vertex_data(vst_texture_coordinate_0,
				dest_index, source->get_vertex_data(
					vst_texture_coordinate_0,
					source_index));

			mesh_data_tool.set_vertex_data(vst_layer_index,
				dest_index, source->get_vertex_data(
					vst_layer_index, source_index));
		}

		void build_sub_mesh_unpacked(MeshDataTool &mesh_data_tool,
			const InstancingData &instancing_data,
			const Uint32 sub_mesh_index, Uint32 &vertex_offset,
			Uint32 &index_offset, glm::vec3 &min, glm::vec3 &max,
			const glm::vec3 &center, const float material_index)
		{
			std::map<Uint32, Uint32> index_map;
			glm::mat4x3 world_matrix;
			Uint32Vector indices;
			Uint32 i, index, count, min_vertex, max_vertex;

			instancing_data.get_mesh_data_tool(
				)->get_triangle_indices(sub_mesh_index, indices,
				false);

			min_vertex = instancing_data.get_mesh_data_tool(
				)->get_sub_meshs(
					)[sub_mesh_index].get_min_vertex();
			max_vertex = instancing_data.get_mesh_data_tool(
				)->get_sub_meshs(
					)[sub_mesh_index].get_max_vertex();

			count = max_vertex - min_vertex + 1;

			if (mesh_data_tool.get_vertex_count()
				< (vertex_offset + count))
			{
				mesh_data_tool.resize_vertices(
					vertex_offset + count);
			}

			count = indices.size();

			world_matrix = instancing_data.get_world_matrix();
			world_matrix[3] -= center;

			for (i = 0; i < count; i++)
			{
				index = indices[i];

				if (index_map.find(index) == index_map.end())
				{
					index_map[index] = vertex_offset;

					add_vertex(mesh_data_tool,
						instancing_data.get_mesh_data_tool(),
						world_matrix,
						instancing_data.get_color(),
						index, vertex_offset, min, max,
						material_index);

					vertex_offset++;
				}

				mesh_data_tool.set_index_data(index_offset,
					index_map[index]);

				index_offset++;
			}
		}

		void build_sub_mesh_packed(MeshDataTool &mesh_data_tool,
			const InstancingData &instancing_data,
			const Uint32 sub_mesh_index, Uint32 &vertex_offset,
			Uint32 &index_offset, glm::vec3 &min, glm::vec3 &max,
			const glm::vec3 &center, const float material_index)
		{
			Uint32Vector indices;
			glm::mat4x3 world_matrix;
			Uint32 i, count, min_vertex, max_vertex;

			instancing_data.get_mesh_data_tool(
				)->get_triangle_indices(sub_mesh_index, indices,
				false);

			min_vertex = instancing_data.get_mesh_data_tool(
				)->get_sub_meshs(
					)[sub_mesh_index].get_min_vertex();
			max_vertex = instancing_data.get_mesh_data_tool(
				)->get_sub_meshs(
					)[sub_mesh_index].get_max_vertex();

			world_matrix = instancing_data.get_world_matrix();
			world_matrix[3] -= center;

			count = indices.size();

			for (i = 0; i < count; i++)
			{
				mesh_data_tool.set_index_data(index_offset,
					indices[i] - min_vertex +
						vertex_offset);

				index_offset++;
			}

			count = max_vertex - min_vertex + 1;

			if (mesh_data_tool.get_vertex_count()
				< (vertex_offset + count))
			{
				mesh_data_tool.resize_vertices(
					vertex_offset + count);
			}

			for (i = 0; i < count; i++)
			{
				add_vertex(mesh_data_tool,
					instancing_data.get_mesh_data_tool(),
					world_matrix,
					instancing_data.get_color(),
					i + min_vertex, vertex_offset, min, max,
					material_index);

				vertex_offset++;
			}
		}

		bool build_sub_mesh(const glm::vec3 &center,
			const InstancingData &instancing_data,
			const MeshDataToolSharedPtr &mesh_data_tool,
			const MaterialDescription &material, const Uint32 index,
			glm::vec3 &min, glm::vec3 &max, Uint32 &vertex_offset,
			Uint32 &index_offset)
		{
			float material_index;
			bool high_index;

			high_index = false;

			if (material != instancing_data.get_materials()[index])
			{
				return false;
			}

			material_index = 0.0f;

			if (high_index)
			{
				material_index = 1.0f;
			}

			if (instancing_data.get_mesh_data_tool(
				)->get_sub_mesh_data(index).get_packed())
			{
				build_sub_mesh_packed(*mesh_data_tool,
					instancing_data, index, vertex_offset,
					index_offset, min, max, center,
					material_index);
			}
			else
			{
				build_sub_mesh_unpacked(*mesh_data_tool,
					instancing_data, index, vertex_offset,
					index_offset, min, max, center,
					material_index);
			}

			return true;
		}

	}

	InstanceBuilder::InstanceBuilder(
		const InstancingDataVector &instancing_datas, const Uint32 id):
		m_instancing_datas(instancing_datas), m_id(id)
	{
		assert(get_instancing_datas().size() > 1);
	}

	InstanceBuilder::~InstanceBuilder() throw()
	{
	}

	glm::vec3 InstanceBuilder::get_center() const
	{
		BoundingBox bounding_box;
		Uint32 i;

		assert(get_instancing_datas().size() > 1);

		bounding_box = get_instancing_data(0).get_bounding_box();

		for (i = 1; i < get_instancing_datas().size(); i++)
		{
			bounding_box.merge(
				get_instancing_data(i).get_bounding_box());
		}

		return bounding_box.get_center();
	}

	void InstanceBuilder::build_instance_sub_mesh(const glm::vec3 &center,
		const MeshDataToolSharedPtr &mesh_data_tool,
		const MaterialDescription &material,
		const Uint32 sub_mesh_index, Uint32 &vertex_offset,
		Uint32 &index_offset, SubObjectVector &sub_objects)
	{
		SubMesh sub_mesh;
		SubObject sub_object;
		glm::vec3 min, max;
		Uint32 count, i;

		sub_mesh.set_offset(index_offset);
		sub_mesh.set_min_vertex(vertex_offset);

		sub_object.set_offset(index_offset);
		sub_object.set_min_vertex(vertex_offset);
		sub_object.set_material(sub_mesh_index);

		BOOST_FOREACH(const InstancingData &instancing_data,
			get_instancing_datas())
		{
			count = instancing_data.get_materials().size();

			sub_object.set_selection(
				instancing_data.get_selection());
			sub_object.set_id(instancing_data.get_id());

			for (i = 0; i < count; i++)
			{
				if (build_sub_mesh(center, instancing_data,
					mesh_data_tool, material, i, min, max,
					vertex_offset, index_offset))
				{
					sub_object.set_count(index_offset -
						sub_object.get_offset());
					sub_object.set_max_vertex(
						vertex_offset - 1);

					sub_objects.push_back(sub_object);

					sub_object.set_offset(index_offset);
					sub_object.set_min_vertex(vertex_offset);
				}	
			}
		}

		sub_mesh.set_bounding_box(BoundingBox(min, max));
		sub_mesh.set_count(index_offset - sub_mesh.get_offset());
		sub_mesh.set_max_vertex(vertex_offset - 1);
		mesh_data_tool->set_sub_mesh_data(sub_mesh_index, sub_mesh);
	}

	void InstanceBuilder::build_instance()
	{
		SubMesh sub_mesh;
		glm::mat4x3 world_matrix;
		glm::vec3 center;
		std::set<MaterialDescription> material_set;
		MeshDataToolSharedPtr mesh_data_tool;
		MaterialDescriptionVector materials;
		SubObjectVector instanced_objects;
		VertexSemanticTypeSet semantics;
		Uint32 index_count, vertex_count, sub_mesh_count;
		Uint32 vertex_offset, index_offset, sub_mesh_index;
		SelectionType selection;

		center = get_center();

		index_count = 0;
		vertex_count = 0;

		selection = st_none;

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

			BOOST_FOREACH(const MaterialDescription &material,
				instancing_data.get_materials())
			{
				material_set.insert(material);
			}
		}

		sub_mesh_count = material_set.size();

		semantics.insert(vst_position);
		semantics.insert(vst_normal);
		semantics.insert(vst_color);
		semantics.insert(vst_tangent);
		semantics.insert(vst_texture_coordinate_0);

		mesh_data_tool = boost::make_shared<MeshDataTool>(vertex_count,
			index_count, sub_mesh_count, semantics, 0,
			pt_triangles, false);

		index_offset = 0;
		vertex_offset = 0;
		sub_mesh_index = 0;

		BOOST_FOREACH(const MaterialDescription &material, material_set)
		{
			build_instance_sub_mesh(center, mesh_data_tool,
				material, sub_mesh_index, vertex_offset,
				index_offset, instanced_objects);

			materials.push_back(material);
			sub_mesh_index++;
		}

		mesh_data_tool->resize_vertices(vertex_offset);

		world_matrix[3] = center;

		m_instance_data.reset(new InstanceData(ObjectData(world_matrix,
			glm::vec4(0.0f), String(L"Instance"), 1.0f,
			get_id(), selection, false), mesh_data_tool, materials,
			instanced_objects));
	}

	void InstanceBuilder::set_instance(InstanceDataVector &instances)
	{
		instances.push_back(m_instance_data);
	}

}
