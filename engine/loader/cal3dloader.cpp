/****************************************************************************
 *            cal3dloader.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cal3dloader.hpp"
#include "meshdatatool.hpp"
#include "submesh.hpp"
#include "exceptions.hpp"
#include "logging.hpp"

namespace eternal_lands
{

	namespace
	{

		void calculate_face_and_vertex_count(CalCoreModel* core_model,
			Uint32 &index_count, Uint32 &vertex_count)
		{
			CalCoreMesh* core_mesh;
			CalCoreSubmesh* core_sub_mesh;
			Sint32 i, j;
			Sint32 core_mesh_count, core_sub_mesh_count;

			index_count = 0;
			vertex_count = 0;

			core_mesh_count = core_model->getCoreMeshCount();

			for (i = 0; i < core_mesh_count; i++)
			{
				core_mesh = core_model->getCoreMesh(i);
				core_sub_mesh_count =
					core_mesh->getCoreSubmeshCount();

				for (j = 0; j < core_sub_mesh_count; j++)
				{
					core_sub_mesh =
						core_mesh->getCoreSubmesh(j);
					index_count +=
						core_sub_mesh->getFaceCount()
							* 3;
					vertex_count +=
						core_sub_mesh->getVertexCount();
				}
			}
		}

		void load_mesh(MeshDataToolSharedPtr &mesh_data_tool,
			CalCoreSubmesh* core_sub_mesh, Uint32 &index_offset,
			Uint32 &vertex_offset, glm::vec3 &min, glm::vec3 &max,
			const Uint16 mesh_index)
		{
			glm::vec4 position, normal, tangent, uv;
			glm::vec4 index, weight, extra_index, extra_weight;
			float tmp_index, tmp_weight;
			Sint32 i, j, k, count, bone_count;

			std::vector<CalCoreSubmesh::Face>& vector_face =
				core_sub_mesh->getVectorFace();

			core_sub_mesh->enableTangents(0, true);

			count = core_sub_mesh->getFaceCount();

			for (i = 0; i < count; i++)
			{
				mesh_data_tool->set_index_data(
					index_offset + i * 3 + 0,
					vector_face[i].vertexId[0] + vertex_offset);
				mesh_data_tool->set_index_data(
					index_offset + i * 3 + 1,
					vector_face[i].vertexId[1] + vertex_offset);
				mesh_data_tool->set_index_data(
					index_offset + i * 3 + 2,
					vector_face[i].vertexId[2] + vertex_offset);
			}

			index_offset += count * 3;

			count = core_sub_mesh->getVertexCount();

			position = glm::vec4(glm::vec3(0.0f), 1.0f);

			normal = glm::vec4(0.0f);

			uv = glm::vec4(glm::vec3(0.0f), 1.0f);

			for (i = 0; i < count; i++)
			{
				position.x = core_sub_mesh->getVectorVertex()[i].position.x;
				position.y = core_sub_mesh->getVectorVertex()[i].position.y;
				position.z = core_sub_mesh->getVectorVertex()[i].position.z;
				position.w = mesh_index;

				min = glm::min(min, glm::vec3(position));
				max = glm::max(max, glm::vec3(position));

				normal.x = core_sub_mesh->getVectorVertex()[i].normal.x;
				normal.y = core_sub_mesh->getVectorVertex()[i].normal.y;
				normal.z = core_sub_mesh->getVectorVertex()[i].normal.z;

				tangent.x = core_sub_mesh->getVectorVectorTangentSpace()[0][i].tangent.x;
				tangent.y = core_sub_mesh->getVectorVectorTangentSpace()[0][i].tangent.y;
				tangent.z = core_sub_mesh->getVectorVectorTangentSpace()[0][i].tangent.z;
				tangent.w = core_sub_mesh->getVectorVectorTangentSpace()[0][i].crossFactor;

				uv.x = core_sub_mesh->getVectorVectorTextureCoordinate()[0][i].u;
				uv.y = core_sub_mesh->getVectorVectorTextureCoordinate()[0][i].v;

				index = glm::vec4(0.0f);
				weight = glm::vec4(0.0f);
				extra_index = glm::vec4(0.0f);
				extra_weight = glm::vec4(0.0f);
				bone_count = core_sub_mesh->getVectorVertex()[i].vectorInfluence.size();

				j = 0;
				k = 0;

				while (k < bone_count)
				{
					tmp_index = core_sub_mesh->getVectorVertex()[i].vectorInfluence[k].boneId;
					tmp_weight = core_sub_mesh->getVectorVertex()[i].vectorInfluence[k].weight;

					if (tmp_weight > 0.0f)
					{
						if (j < 4)
						{
							index[j] = tmp_index;
							weight[j] = tmp_weight;
						}
						else
						{
							extra_index[j - 4] = tmp_index;
							extra_weight[j - 4] = tmp_weight;
						}
						j++;
					}

					if (j >= 8)
					{
						break;
					}

					k++;
				}

				mesh_data_tool->set_vertex_data(
					vst_position, vertex_offset + i,
					position);
				mesh_data_tool->set_vertex_data(
					vst_normal, vertex_offset + i, normal);
				mesh_data_tool->set_vertex_data(
					vst_tangent, vertex_offset + i,
					tangent);
				mesh_data_tool->set_vertex_data(
					vst_texture_coordinate_0,
					vertex_offset + i, uv);
				mesh_data_tool->set_vertex_data(vst_bone_index,
					vertex_offset + i, index);
				mesh_data_tool->set_vertex_data(vst_bone_weight,
					vertex_offset + i, weight);

				mesh_data_tool->set_vertex_data(
					vst_extra_bone_index,
					vertex_offset + i, extra_index);
				mesh_data_tool->set_vertex_data(
					vst_extra_bone_weight,
					vertex_offset + i, extra_weight);
			}

			vertex_offset += count;
		}

	}

	Cal3dLoader::Cal3dLoader(CalCoreModel* core_model):
		m_core_model(core_model)
	{
		calculate_face_and_vertex_count(core_model, m_index_count,
			m_vertex_count);
	}

	Cal3dLoader::~Cal3dLoader() throw()
	{
	}

	void Cal3dLoader::load(MeshDataToolSharedPtr &mesh_data_tool,
		const bool use_extra_weight)
	{
		glm::vec3 min, max;
		CalCoreMesh* core_mesh;
		VertexSemanticTypeSet semantics;
		Uint32 offset, min_vertex;
		Uint32 index_offset, vertex_offset;
		Sint32 mesh_count, sub_mesh_count;
		Sint32 i, j;

		index_offset = 0;
		vertex_offset = 0;
		mesh_count = m_core_model->getCoreMeshCount();

		semantics.insert(vst_position);
		semantics.insert(vst_normal);
		semantics.insert(vst_tangent);
		semantics.insert(vst_texture_coordinate_0);
		semantics.insert(vst_bone_index);
		semantics.insert(vst_bone_weight);

		if (use_extra_weight)
		{
			semantics.insert(vst_extra_bone_index);
			semantics.insert(vst_extra_bone_weight);
		}

		mesh_data_tool = boost::make_shared<MeshDataTool>(
			m_vertex_count, m_index_count, mesh_count, semantics,
			std::numeric_limits<Uint32>::max(), pt_triangles,
			false);

		for (i = 0; i < mesh_count; i++)
		{
			core_mesh = m_core_model->getCoreMesh(i);
			sub_mesh_count = core_mesh->getCoreSubmeshCount();

			offset = index_offset;
			min_vertex = vertex_offset;
			min = glm::vec3(std::numeric_limits<float>::max());
			max = glm::vec3(-std::numeric_limits<float>::max());

			for (j = 0; j < sub_mesh_count; j++)
			{
				load_mesh(mesh_data_tool,
					core_mesh->getCoreSubmesh(j),
					index_offset, vertex_offset,
					min, max, i);
			}

			mesh_data_tool->set_sub_mesh_data(i, SubMesh(
				BoundingBox(min, max), offset,
				index_offset - offset, min_vertex,
				vertex_offset - min_vertex - 1));
		}
	}

}
