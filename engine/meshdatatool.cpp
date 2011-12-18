/****************************************************************************
 *            meshdatatool.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "meshdatatool.hpp"
#include "vertexelement.hpp"
#include "vertexelements.hpp"
#include "packtool.hpp"
#include "abstractwritememorybuffer.hpp"
#include "triangles.hpp"
#include "exceptions.hpp"
#include "submesh.hpp"
#include "vertexstream.hpp"
#include "vertexstreams.hpp"
#include "alignedvec4array.hpp"
#include "simd/simd.hpp"

namespace eternal_lands
{

	namespace
	{

		struct triangle_data
		{
			bool added;
			float score;
			Uint32 vertices[3];
		};

		struct vertex_data
		{
			float score;
			Uint32Set remaining_triangles;
		};

		float calculate_average_cache_miss_ratio(
			const Uint32Vector &indices, const Uint32 offset,
			const Uint32 count, const Uint32 cache_size)
		{
			Uint32Vector cache(cache_size, 0xFFFFFFFF);
			Uint32 i, j, cache_ptr, cache_misses;
			bool cache_hit;

			if (count <= cache_size)
			{
				return -1.0f;
			}

			cache_ptr = 0;
			cache_misses = 0;

			for (i = 0 ; i < count; ++i)
			{
				cache_hit = false;

				for (j = 0 ; j < cache_size; j++)
				{
					cache_hit |= (cache[j] == indices[offset + i]);
				}

				if (!cache_hit)
				{
					cache[cache_ptr] = indices[offset + i];
					cache_ptr = (cache_ptr + 1) % cache_size;
					cache_misses++;
				}
			}

			return (cache_misses / (count / 3.0f));
		}

		static inline float calc_new_score(const Uint32 count)
		{
			if (count == 0)
			{
				return 0.0f;
			}

			return 2.0f / std::sqrt(count);
		}

		bool optimize_vertex_cache_order(Uint32Vector &indices,
			const Uint32 offset, const Uint32 count,
			const Uint32 cache_size)
		{
			FloatVector cache_score(cache_size + 3, 0.75);
			Uint32Vector cache_idx(cache_size + 3, 0xFFFFFFFF);
			Uint32Vector grow_cache_idx(cache_size + 3, 0xFFFFFFFF);
			Uint32Set::iterator it;
			Uint32 i, j, index;
			Uint32 triangle_count, vertex_count;
			Uint32 triangles_left, best_idx;
			Uint32 a, b, c;
			float best_score, old_score, new_score;

			if ((count < 3) || (count % 3 != 0) || (cache_size < 4))
			{
				return false;
			}

			triangle_count = count / 3;
			vertex_count = 0;

			for (i = 0; i < count; ++i)
			{
				if (indices[offset + i] > vertex_count)
				{
					vertex_count = indices[offset + i];
				}
			}
			vertex_count++;

			for (i = 3; i < cache_size; ++i)
			{
				cache_score[i] = powf((cache_size - i) /
					(cache_size - 3.0), 1.5);
			}

			for (i = 0; i < 3; ++i)
			{
				cache_score[cache_size + i] = 0.0;
			}
			triangles_left = triangle_count;

			std::vector<triangle_data> t(triangle_count);
			std::vector<vertex_data> v(vertex_count);

			for (i = 0; i < vertex_count; ++i)
			{
				v[i].score = 0.0;
				v[i].remaining_triangles.clear();
			}
			for (i = 0; i < triangle_count; ++i)
			{
				t[i].added = false;
				t[i].score = 0.0;
				t[i].vertices[0] = indices[offset + i * 3 + 0];
				t[i].vertices[1] = indices[offset + i * 3 + 1];
				t[i].vertices[2] = indices[offset + i * 3 + 2];

				index = indices[offset + i * 3 + 0];
				v[index].remaining_triangles.insert(i);
				index = indices[offset + i * 3 + 1];
				v[index].remaining_triangles.insert(i);
				index = indices[offset + i * 3 + 2];
				v[index].remaining_triangles.insert(i);
			}
			for (i = 0; i < vertex_count; ++i)
			{
				v[i].score = calc_new_score(v[i].remaining_triangles.size());
			}
			for (i = 0; i < triangle_count; ++i)
			{
				t[i].score = v[t[i].vertices[0]].score +
					v[t[i].vertices[1]].score +
					v[t[i].vertices[2]].score;
			}

			while (triangles_left > 0)
			{
				best_score = 0.0;
				best_idx = 0xFFFFFFFF;
				for (i = 0; i < triangle_count; ++i)
				{
					if (!t[i].added)
					{
						if (t[i].score > best_score)
						{
							best_score = t[i].score;
							best_idx = i;
						}
					}
				}
				a = t[best_idx].vertices[0];
				b = t[best_idx].vertices[1];
				c = t[best_idx].vertices[2];
				indices[offset + (triangle_count - triangles_left) * 3 + 0] = a;
				indices[offset + (triangle_count - triangles_left) * 3 + 1] = b;
				indices[offset + (triangle_count - triangles_left) * 3 + 2] = c;
				for (i = 0; i < 3; ++i)
				{
					index = t[best_idx].vertices[i];
					v[index].remaining_triangles.erase(best_idx);
				}
				t[best_idx].added = true;
				triangles_left--;
				grow_cache_idx[0] = a;
				grow_cache_idx[1] = b;
				grow_cache_idx[2] = c;
				j = 3;
				for (i = 0; i < cache_size; ++i)
				{
					grow_cache_idx[i + 3] = 0xFFFFFFFF;
					if ((cache_idx[i] != a) &&
						(cache_idx[i] != b) &&
						(cache_idx[i] != c))
					{
						grow_cache_idx[j++] = cache_idx[i];
					}
				}
				cache_idx.swap(grow_cache_idx);
				for (i = 0; i < cache_size; ++i)
				{
					if (cache_idx[i] < 0xFFFFFFFF)
					{
						index = cache_idx[i];
						if (v[index].remaining_triangles.size() > 0)
						{
							old_score = v[index].score;
							new_score = cache_score[i] +
								calc_new_score(v[index].remaining_triangles.size());
							v[index].score = new_score;
							for (it = v[index].remaining_triangles.begin();
								it != v[index].remaining_triangles.end(); it++)
							{
								t[*it].score += new_score - old_score;
							}
						}
					}
				}
			}

			return true;
		}

		glm::vec3 get_normal(const Vec3Array3 &positions)
		{
			glm::vec3 p0, p1;

			p0 = positions[1] - positions[0];
			p1 = positions[2] - positions[0];

			return glm::normalize(glm::cross(p0, p1));
		}

		void get_tangent(const Vec3Array3 &positions,
			const Vec2Array3 &texture_coords, glm::vec3 &tangent,
			glm::vec3 &bitangent)
		{
			glm::vec3 p0, p1;
			glm::vec2 t0, t1;
			float r;

			p0 = positions[1] - positions[0];
			p1 = positions[2] - positions[0];

			t0 = texture_coords[1] - texture_coords[0];
			t1 = texture_coords[2] - texture_coords[0];

			r = 1.0f / (t0.x * t1.y - t0.y * t1.x);

			tangent = glm::vec3(t1.y * p0 - t1.x * p1) * r;
			bitangent = glm::vec3(t0.x * p1 - t0.y * p0) * r;
		}

		glm::vec4 get_gram_schmidth_orthogonalize_tangent(
			const glm::vec3 &normal, const glm::vec3 &tangent,
			const glm::vec3 &bitangent)
		{
			glm::vec4 result;
        
			// Gram-Schmidt orthogonalize
			result = glm::vec4(glm::normalize(tangent - normal *
				glm::dot(normal, tangent)), 0.0f);
        
			// Calculate handedness
			result.w = (glm::dot(glm::cross(normal, tangent),
				bitangent) < 0.0f) ? -1.0f : 1.0f;

			return result;
		}

	}

	MeshDataTool::MeshDataTool(const Uint32 vertex_count,
		const Uint32 index_count, const Uint32 sub_mesh_count,
		const VertexSemanticTypeSet &semantics,
		const Uint32 restart_index, const PrimitiveType primitive_type,
		const bool use_restart_index)
	{
		if ((primitive_type != pt_triangles) &&
			(primitive_type != pt_triangle_fan) &&
			(primitive_type != pt_triangle_strip))
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("Only triangle "
					"primitives supported"))
				<< errinfo_string_value(
					PrimitiveUtil::get_str(
					primitive_type)));
		}

		m_vertex_count = vertex_count;
		m_restart_index = restart_index;
		m_primitive_type = primitive_type;
		m_use_restart_index = use_restart_index;

		BOOST_FOREACH(const VertexSemanticType semantic, semantics)
		{
			m_vertices[semantic].resize(vertex_count);
		}

		m_sub_meshs.resize(sub_mesh_count);

		resize_indices(index_count);
	}

	MeshDataTool::~MeshDataTool() throw()
	{
	}

	void MeshDataTool::resize_vertices(const Uint32 vertex_count)
	{
		VertexSemanticTypeAlignedVec4ArrayMap::iterator it, end;

		m_vertex_count = vertex_count;

		end = m_vertices.end();

		for (it = m_vertices.begin(); it != end; ++it)
		{
			it->second.resize(vertex_count);
		}
	}

	void MeshDataTool::set_vertex_data(
		const VertexSemanticType vertex_semantic, const Uint32 index,
		const glm::vec4 &data)
	{
		VertexSemanticTypeAlignedVec4ArrayMap::iterator found;

		assert(index < get_vertex_count());

		found = m_vertices.find(vertex_semantic);

		if (found != m_vertices.end())
		{
			found->second[index] = data;
		}
	}

	glm::vec4 MeshDataTool::get_vertex_data(
		const VertexSemanticType vertex_semantic, const Uint32 index)
		const
	{
		VertexSemanticTypeAlignedVec4ArrayMap::const_iterator found;

		assert(index < get_vertex_count());

		found = m_vertices.find(vertex_semantic);

		if (found != m_vertices.end())
		{
			return found->second[index];
		}
		else
		{
			return glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	void MeshDataTool::set_sub_mesh_data(const Uint32 index,
		const SubMesh &data)
	{
		assert(index < m_sub_meshs.size());

		m_sub_meshs[index] = data;
	}

	const SubMesh &MeshDataTool::get_sub_mesh_data(const Uint32 index)
		const
	{
		assert(index < m_sub_meshs.size());

		return m_sub_meshs[index];
	}

	void MeshDataTool::optimize()
	{
		Uint32 i, count;

		if (get_primitive_type() == pt_triangles)
		{
			count = get_sub_meshs().size();

			for (i = 0; i < count; ++i)
			{
				optimize_vertex_cache_order(m_indices,
					get_sub_meshs()[i].get_offset(),
					get_sub_meshs()[i].get_count(), 32);
			}
		}

		vertex_optimize();
	}

	void MeshDataTool::vertex_optimize()
	{
		Triangles triangles(get_indices(), get_sub_meshs(),
			get_primitive_type(), get_restart_index(),
			get_use_restart_index());
		VertexSemanticTypeAlignedVec4ArrayMap::iterator it, end;
		AlignedVec4Array vertices;
		Uint32Vector indices;
		std::map<Uint32, Uint32> index_map;
		Uint32 i, index, count, value;

		count = get_index_count();

		for (i = 0; i < count; ++i)
		{
			index = get_index_data(i);

			if ((index == get_restart_index()) &&
				get_use_restart_index())
			{
				indices.push_back(index);

				continue;
			}

			if (index_map.find(index) == index_map.end())
			{
				value = index_map.size();

				index_map[index] = value;
			}
			else
			{
				value = index_map[index];
			}

			indices.push_back(value);
		}

		assert(get_vertex_count() == index_map.size());

		vertices.resize(get_vertex_count());

		count = get_vertex_count();

		end = m_vertices.end();

		for (it = m_vertices.begin(); it != end; ++it)
		{
			for (i = 0; i < count; ++i)
			{
				index = index_map[i];
				vertices[index] = it->second[i];
			}

			std::swap(it->second, vertices);
		}

		std::swap(m_indices, indices);
		update_sub_meshs_packed();
	}

	void MeshDataTool::build_normal(const bool morph_target)
	{
		Triangles triangles(get_indices(), get_sub_meshs(),
			get_primitive_type(), get_restart_index(),
			get_use_restart_index());
		Vec3Array3 positions;
		Vec3Vector normals;
		glm::vec3 v_normal;
		Uint32 i, j, index, count;
		VertexSemanticType position, normal;

		if (morph_target)
		{
			position = vst_morph_position;
			normal = vst_morph_normal;
		}
		else
		{
			position = vst_position;
			normal = vst_normal;
		}

		normals.resize(get_index_count(), glm::vec3(0.0f));

		count = get_sub_meshs().size();

		for (i = 0; i < count; ++i)
		{
			triangles.start(i, true);

			while (triangles.next_triangle())
			{
				for (j = 0; j < 3; j++)
				{
					index = triangles.get_current_index(j);

					positions[j] = glm::vec3(
						get_vertex_data(position,
							index));
				}

				v_normal = get_normal(positions);

				for (j = 0; j < 3; j++)
				{
					index = triangles.get_current_index(j);

					normals[j] += v_normal;
				}
			}
		}

		count = normals.size();

		for (i = 0; i < count; ++i)
		{
			set_vertex_data(normal, i, glm::vec4(
				glm::normalize(normals[i]), 0.0f));
		}
	}

	void MeshDataTool::build_tangent(const bool morph_target,
		const bool second_texture_coord,
		const bool gram_schmidth_orthogonalize)
	{
		Triangles triangles(get_indices(), get_sub_meshs(),
			get_primitive_type(), get_restart_index(),
			get_use_restart_index());
		Vec3Array3 positions;
		Vec2Array3 texture_coords;
		Vec3Vector tangents, bitangents;
		glm::vec3 v_tangent, v_bitangent, v_normal;
		Uint32 i, j, index, count;
		VertexSemanticType position, normal, tangent, texture_coord;

		if (morph_target)
		{
			position = vst_morph_position;
			normal = vst_morph_normal;
			tangent = vst_morph_tangent;

			if (second_texture_coord)
			{
				texture_coord = vst_morph_texture_coordinate_1;
			}
			else
			{
				texture_coord = vst_morph_texture_coordinate_0;
			}
		}
		else
		{
			position = vst_position;
			normal = vst_normal;
			tangent = vst_tangent;

			if (second_texture_coord)
			{
				texture_coord = vst_texture_coordinate_1;
			}
			else
			{
				texture_coord = vst_texture_coordinate_0;
			}
		}

		tangents.resize(get_index_count(), glm::vec3(0.0f));
		bitangents.resize(get_index_count(), glm::vec3(0.0f));

		count = get_sub_meshs().size();

		for (i = 0; i < count; ++i)
		{
			triangles.start(i, true);

			while (triangles.next_triangle())
			{
				for (j = 0; j < 3; j++)
				{
					index = triangles.get_current_index(j);

					positions[j] = glm::vec3(
						get_vertex_data(position,
							index));
					texture_coords[j] = glm::vec2(
						get_vertex_data(texture_coord,
							index));
				}

				get_tangent(positions, texture_coords,
					v_tangent, v_bitangent);

				for (j = 0; j < 3; j++)
				{
					index = triangles.get_current_index(j);

					tangents[j] += v_tangent;
					bitangents[j] += v_bitangent;
				}
			}
		}

		count = tangents.size();

		for (i = 0; i < count; ++i)
		{
			if (gram_schmidth_orthogonalize)
			{
				v_normal = glm::vec3(get_vertex_data(normal,
					i));

				set_vertex_data(tangent, i,
					get_gram_schmidth_orthogonalize_tangent(
						v_normal, tangents[i],
						bitangents[i]));
			}
			else
			{
				set_vertex_data(tangent, i, glm::vec4(
					glm::normalize(tangents[i]), 1.0f));
			}
		}
	}

	void MeshDataTool::build_scale_morph(const float scale)
	{
		Triangles triangles(get_indices(), get_sub_meshs(),
			get_primitive_type(), get_restart_index(),
			get_use_restart_index());
		Vec3Array3 position;
		Vec3Vector direction, positions;
		glm::vec3 normal, tmp;
		Uint32Vector indices;
		Uint32 i, j, index, idx, count;

		indices.resize(get_vertex_count());
		count = get_vertex_count();

		for (i = 0; i < count; ++i)
		{
			tmp = glm::vec3(get_vertex_data(vst_position, i));

			index = positions.size();

			for (j = 0; j < positions.size(); j++)
			{
				if (glm::all(glm::equalEpsilon(tmp,
					positions[j], 0.01f)))
				{
					index = j;
					break;
				}
			}

			if (index == positions.size())
			{
				positions.push_back(tmp);
			}

			indices[i] = index;
		}

		direction.resize(positions.size());
		count = get_sub_meshs().size();

		for (i = 0; i < count; ++i)
		{
			triangles.start(i, true);

			while (triangles.next_triangle())
			{
				for (j = 0; j < 3; j++)
				{
					index = triangles.get_current_index(j);

					position[j] = glm::vec3(get_vertex_data(
						vst_position, index));
				}

				normal = glm::normalize(glm::cross(
					position[0] - position[1],
					position[0] - position[2]));

				for (j = 0; j < 3; j++)
				{
					index = triangles.get_current_index(j);
					idx = indices[index];
					direction[idx] += normal;
				}
			}
		}

		count = direction.size();

		for (i = 0; i < count; ++i)
		{
			direction[i] = glm::normalize(direction[i]);
		}

		count = get_sub_meshs().size();

		for (i = 0; i < count; ++i)
		{
			triangles.start(i, true);

			while (triangles.next_triangle())
			{
				for (j = 0; j < 3; j++)
				{
					index = triangles.get_current_index(j);
					idx = indices[index];
					position[j] = direction[idx] * scale;
					position[j] += glm::vec3(
						get_vertex_data(vst_position,
							index));
					set_vertex_data(vst_morph_position,
						index, glm::vec4(position[j],
							1.0f));
				}
			}
		}

		build_normal(true);
		build_tangent(true, false, true);
	}

	void MeshDataTool::update_sub_meshs_bounding_box()
	{
		Triangles triangles(get_indices(), get_sub_meshs(),
			get_primitive_type(), get_restart_index(),
			get_use_restart_index());
		glm::vec3 min, max, position;
		Uint32 i, j, index, count;

		count = get_sub_meshs().size();

		for (i = 0; i < count; ++i)
		{
			min = glm::vec3(std::numeric_limits<float>::max());
			max = glm::vec3(-std::numeric_limits<float>::max());

			triangles.start(i, true);

			while (triangles.next_triangle())
			{
				for (j = 0; j < 3; j++)
				{
					index = triangles.get_current_index(j);
					position = glm::vec3(get_vertex_data(
						vst_position, index));
					min = glm::min(min, position);
					max = glm::max(max, position);
				}
			}

			m_sub_meshs[i].set_bounding_box(BoundingBox(min, max));
		}
	}

	void MeshDataTool::update_sub_meshs_packed()
	{
		Triangles triangles(get_indices(), get_sub_meshs(),
			get_primitive_type(), get_restart_index(),
			get_use_restart_index());
		Uint32Set indices;
		Uint32 i, j, count, min_vertex, max_vertex;
		bool packed;

		count = get_sub_meshs().size();

		for (i = 0; i < count; ++i)
		{
			indices.clear();

			triangles.start(i, true);

			while (triangles.next_triangle())
			{
				for (j = 0; j < 3; j++)
				{
					indices.insert(
						triangles.get_current_index(j));
				}
			}

			packed = true;

			if (indices.size() > 0)
			{
				min_vertex = *indices.begin();
				max_vertex = *indices.rbegin();

				m_sub_meshs[i].set_min_vertex(min_vertex);
				m_sub_meshs[i].set_max_vertex(max_vertex);

				packed = (max_vertex - min_vertex + 1) ==
					indices.size();
			}

			m_sub_meshs[i].set_packed(packed);
		}
	}

	void MeshDataTool::disable_restart_index()
	{
		if (get_use_restart_index())
		{
			convert_to_triangles();
		}
	}

	void MeshDataTool::convert_to_triangles()
	{
		Uint32Vector indices;
		Uint32 i, count;

		if (get_primitive_type() == pt_triangles)
		{
			return;
		}

		count = get_sub_meshs().size();

		for (i = 0; i < count; ++i)
		{
			get_triangle_indices(i, indices, true);
		}

		std::swap(indices, m_indices);
	}

	void MeshDataTool::get_triangle_indices(const Uint32 sub_mesh_index,
		Uint32Vector &indices, const bool use_base_vertex) const
	{
		Triangles triangles(get_indices(), get_sub_meshs(),
			get_primitive_type(), get_restart_index(),
			get_use_restart_index());

		triangles.start(sub_mesh_index, use_base_vertex);

		assert((indices.size() % 3) == 0);

		while (triangles.next_triangle())
		{
			indices.push_back(triangles.get_current_index(0));
			indices.push_back(triangles.get_current_index(1));
			indices.push_back(triangles.get_current_index(2));
		}

		assert((indices.size() % 3) == 0);
	}

	void MeshDataTool::write_vertex_semantic_to_stream(
		const VertexSemanticType semantic, const Uint32 index,
		OutStream &str)
	{
		glm::vec4 data;

		if (m_vertices.find(semantic) != m_vertices.end())
		{
			data = get_vertex_data(semantic, index);

			str << semantic << ": " << data[0] << ", " << data[1];
			str << ", " << data[2] << ", " << data[3] << std::endl;
		}
	}

	void MeshDataTool::write_to_stream(OutStream &str)
	{
		Triangles triangles(get_indices(), get_sub_meshs(),
			get_primitive_type(), get_restart_index(),
			get_use_restart_index());
		Uint32 i, count;

		count = get_vertex_count();

		str << "vertex count: " << count << std::endl;

		for (i = 0; i < count; ++i)
		{
			write_vertex_semantic_to_stream(vst_position, i, str);
			write_vertex_semantic_to_stream(vst_normal, i, str);
			write_vertex_semantic_to_stream(vst_tangent, i, str);
			write_vertex_semantic_to_stream(vst_color, i, str);
			write_vertex_semantic_to_stream(vst_bone_weight, i, str);
			write_vertex_semantic_to_stream(vst_bone_index, i, str);
			write_vertex_semantic_to_stream(vst_extra_bone_weight, i, str);
			write_vertex_semantic_to_stream(vst_extra_bone_index, i, str);
			write_vertex_semantic_to_stream(vst_texture_coordinate_0, i, str);
			write_vertex_semantic_to_stream(vst_texture_coordinate_1, i, str);
			write_vertex_semantic_to_stream(vst_layer_index, i, str);
			write_vertex_semantic_to_stream(vst_morph_position, i, str);
			write_vertex_semantic_to_stream(vst_morph_normal, i, str);
			write_vertex_semantic_to_stream(vst_morph_tangent, i, str);
			write_vertex_semantic_to_stream(vst_morph_texture_coordinate_0, i, str);
			write_vertex_semantic_to_stream(vst_morph_texture_coordinate_1, i, str);
		}

		str << "primitive: " << get_primitive_type() << std::endl;
		str << "restart index: " << get_restart_index() << std::endl;
		str << "use restart index: " << get_use_restart_index() << std::endl;

		count = get_index_count();

		str << "index count: " << count << std::endl;

		for (i = 0; i < count / 3; ++i)
		{
			str << get_index_data(i * 3 + 0) << ", ";
			str << get_index_data(i * 3 + 1) << ", ";
			str << get_index_data(i * 3 + 2) << ", ";
			str << std::endl;
		}

		count = get_sub_meshs().size();

		str << "sub mesh count: " << count << std::endl;

		for (i = 0; i < count; ++i)
		{
			str << "bounding box: " << get_sub_meshs()[i].get_bounding_box() << std::endl;
			str << "offset: " << get_sub_meshs()[i].get_offset() << std::endl;
			str << "count: " << get_sub_meshs()[i].get_count() << std::endl;
			str << "min_vertex: " << get_sub_meshs()[i].get_min_vertex() << std::endl;
			str << "max_vertex: " << get_sub_meshs()[i].get_max_vertex() << std::endl;
			str << "base_vertex: " << get_sub_meshs()[i].get_base_vertex() << std::endl;

			triangles.start(i, true);

			while (triangles.next_triangle())
			{
				str << triangles.get_current_index(0) << ", ";
				str << triangles.get_current_index(1) << ", ";
				str << triangles.get_current_index(2) << ", ";
				str << std::endl;
			}
		}
	}

	void MeshDataTool::write_vertex_stream(VertexStream &stream) const
	{
		VertexSemanticTypeAlignedVec4ArrayMap::const_iterator found;
		VertexSemanticTypeAlignedVec4ArrayMap::const_iterator end;

		end = m_vertices.end();

		BOOST_FOREACH(const VertexElement &element,
			stream.get_vertex_elements().get_vertex_elements())
		{
			found = m_vertices.find(element.get_semantic());

			if (found != end)
			{
				stream.set(found->first, found->second);
			}
		}
	}

	void MeshDataTool::write_vertex_streams(VertexStreams &streams) const
	{
		BOOST_FOREACH(VertexStream &stream, streams.get_streams())
		{
			write_vertex_stream(stream);
		}
	}

	void MeshDataTool::write_index_buffer(const bool use_16_bit_indices,
		const AbstractWriteMemoryBufferSharedPtr &buffer) const
	{
		void* src;
		Uint32 i;

		src = buffer->get_ptr();

		if (use_16_bit_indices)
		{
			assert(buffer->get_size() >= (sizeof(Uint16) *
				get_index_count()));
		}
		else
		{
			assert(buffer->get_size() >= (sizeof(Uint32) *
				get_index_count()));
		}

		for (i = 0; i < get_index_count(); ++i)
		{
			if (use_16_bit_indices)
			{
				static_cast<Uint16*>(src)[i] =
					get_index_data(i);
			}
			else
			{
				static_cast<Uint32*>(src)[i] =
					get_index_data(i);
			}
		}
	}

	void MeshDataTool::get_bounding_box(const glm::mat4x3 &matrix,
		BoundingBox &bounding_box)
	{
		Uint32 i;

		if (get_sub_meshs().size() == 0)
		{
			EL_THROW_EXCEPTION(ArraySizeErrorException()
				<< errinfo_size(0));
		}

		bounding_box = get_sub_meshs()[0].get_bounding_box().transform(
			matrix);

		for (i = 1; i < get_sub_meshs().size(); ++i)
		{
			bounding_box.merge(get_sub_meshs()[i].get_bounding_box(
				).transform(matrix));
		}
	}

	void MeshDataTool::copy_vertics(const MeshDataTool &mesh_data_tool,
		const VertexSemanticType semantic, const Uint32 source_index,
		const Uint32 dest_index, const Uint32 count)
	{
		VertexSemanticTypeAlignedVec4ArrayMap::const_iterator source;
		VertexSemanticTypeAlignedVec4ArrayMap::iterator dest;
		glm::vec4 data;
		Uint32 i;

		dest = m_vertices.find(semantic);

		if (dest == m_vertices.end())
		{
			return;
		}

		assert((dest_index + count) <= dest->second.size());

		source = mesh_data_tool.m_vertices.find(semantic);

		if (source == mesh_data_tool.m_vertices.end())
		{
			data = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			if (SIMD::get_supported())
			{
				SIMD::fill(data, count, glm::value_ptr(
					dest->second[dest_index]));

				return;
			}

			for (i = 0; i < count; ++i)
			{
				dest->second[dest_index + i] = data;
			}

			return;
		}

		assert((source_index + count) <= source->second.size());

		memcpy(glm::value_ptr(dest->second[dest_index]),
			glm::value_ptr(source->second[source_index]),
			count * sizeof(float) * 4);
	}

	void MeshDataTool::transform_vertics(
		const MeshDataTool &mesh_data_tool,
		const VertexSemanticType semantic, const Uint32 source_index,
		const Uint32 dest_index, const Uint32 count,
		const glm::mat4x3 &matrix)
	{
		VertexSemanticTypeAlignedVec4ArrayMap::const_iterator source;
		VertexSemanticTypeAlignedVec4ArrayMap::iterator dest;
		glm::vec4 data;
		Uint32 i;

		dest = m_vertices.find(semantic);

		if (dest == m_vertices.end())
		{
			return;
		}

		assert((dest_index + count) <= dest->second.size());

		source = mesh_data_tool.m_vertices.find(semantic);

		if (source == mesh_data_tool.m_vertices.end())
		{
			data = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			if (SIMD::get_supported())
			{
				SIMD::fill(data, count, glm::value_ptr(
					dest->second[dest_index]));

				return;
			}

			for (i = 0; i < count; ++i)
			{
				dest->second[dest_index + i] = data;
			}

			return;
		}

		assert((source_index + count) <= source->second.size());

		if (SIMD::get_supported())
		{
			SIMD::transform(
				glm::value_ptr(source->second[source_index]),
				count, matrix,
				glm::value_ptr(dest->second[dest_index]));

			return;
		}

		for (i = 0; i < count; ++i)
		{
			data = source->second[source_index + i];
			data.w = 1.0f;

			dest->second[dest_index + i] =
				glm::vec4(matrix * data, 1.0f);
		}
	}

	void MeshDataTool::transform_vertics(
		const MeshDataTool &mesh_data_tool,
		const VertexSemanticType semantic, const Uint32 source_index,
		const Uint32 dest_index, const Uint32 count,
		const glm::mat3x3 &matrix)
	{
		VertexSemanticTypeAlignedVec4ArrayMap::const_iterator source;
		VertexSemanticTypeAlignedVec4ArrayMap::iterator dest;
		glm::vec4 data;
		glm::vec3 tmp;
		Uint32 i;

		dest = m_vertices.find(semantic);

		if (dest == m_vertices.end())
		{
			return;
		}

		assert((dest_index + count) <= dest->second.size());

		source = mesh_data_tool.m_vertices.find(semantic);

		if (source == mesh_data_tool.m_vertices.end())
		{
			data = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

			if (SIMD::get_supported())
			{
				SIMD::fill(data, count, glm::value_ptr(
					dest->second[dest_index]));

				return;
			}

			for (i = 0; i < count; ++i)
			{
				dest->second[dest_index + i] = data;
			}

			return;
		}

		assert((source_index + count) <= source->second.size());

		if (SIMD::get_supported())
		{
			SIMD::transform(
				glm::value_ptr(source->second[source_index]),
				count, matrix,
				glm::value_ptr(dest->second[dest_index]));

				return;
		}

		for (i = 0; i < count; ++i)
		{
			data = source->second[source_index + i];

			dest->second[dest_index + i] =
				glm::vec4(matrix * glm::vec3(data), 1.0f);
		}
	}

	void MeshDataTool::fill_vertics(const VertexSemanticType semantic,
		const Uint32 index, const Uint32 count, const glm::vec4 &data)
	{
		VertexSemanticTypeAlignedVec4ArrayMap::iterator found;
		Uint32 i;

		found = m_vertices.find(semantic);

		if (found == m_vertices.end())
		{
			return;
		}

		assert((index + count) <= found->second.size());

		if (SIMD::get_supported())
		{
			SIMD::fill(data, count,
				glm::value_ptr(found->second[index]));

				return;
		}

		for (i = 0; i < count; ++i)
		{
			found->second[index + i] = data;
		}
	}

	void MeshDataTool::set_indices(const Uint32Vector &indices,
		const Uint32 source_index, const Uint32 dest_index,
		const Uint32 count, const Sint32 offset)
	{
		Uint32 i;

		for (i = 0; i < count; ++i)
		{
			m_indices[dest_index + i] = indices[source_index + i]
				+ offset;
		}
	}

}
