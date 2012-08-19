/****************************************************************************
 *            sceneview.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "sceneview.hpp"
#include "globalvars.hpp"
#include "boundingbox.hpp"
#include "frustum.hpp"
#include "glmutil.hpp"

namespace eternal_lands
{

	namespace
	{

		float get_split_point(const float z_near, const float z_far,
			const Uint16 index, const Uint16 count)
		{
			float scale, split_uni, split_log;

			scale = static_cast<float>(index) /
				static_cast<float>(count);

			split_uni = z_near + (z_far - z_near) * scale;
			split_log = z_near * std::pow(z_far / z_near, scale);

			return glm::mix(split_uni, split_log, 0.5f);
		}

		float build_min_max(const glm::mat4x4 &matrix,
			const Vec3Array8 &corner_points,
			glm::vec3 &min, glm::vec3 &max)
		{
			BoundingBox bounding_box;
			glm::vec3 center;
			float radius;

			BOOST_FOREACH(const glm::vec3 &point, corner_points)
			{
				center += point;
			}

			center /= static_cast<float>(corner_points.size());

			radius = 0.0f;

			BOOST_FOREACH(const glm::vec3 &point, corner_points)
			{
				radius = std::max(radius,
					glm::distance(center, point) * 0.5f);
			}

			radius = std::ceil(radius * 10.0f) / 10.0f;
			center = glm::vec3(glm::ivec3(center * 10.0f)) / 10.0f;

			bounding_box.set_center(center);
			bounding_box.set_half_size(glm::vec3(radius));

			bounding_box = bounding_box.transform(matrix);

			min = bounding_box.get_min();
			max = bounding_box.get_max();

			return radius;
		}

		glm::mat4x4 build_shadow_cropp_matrix(
			const glm::mat4x4 &matrix,
			const Vec3Array8 &corner_points,
			const float shadow_map_size,
			const float z_offset)
		{
			glm::mat4x4 result;
			glm::vec3 min, max, offset, scale;
			float radius, world_units_per_texel;

			radius = build_min_max(matrix, corner_points, min, max);

			min.z += z_offset;

			world_units_per_texel = (2.0f + radius * 2.0f) /
				shadow_map_size;

			max += world_units_per_texel * 0.5;
			min -= world_units_per_texel * 0.5;

			min /= world_units_per_texel;
			min = glm::floor(min);
			min *= world_units_per_texel;

			max /= world_units_per_texel;
			max = glm::floor(max);
			max *= world_units_per_texel;

			// Create the crop matrix.
			scale.x = 2.0f / (max.x - min.x);
			scale.y = 2.0f / (max.y - min.y);
			scale.z = 1.0f / (max.z - min.z);
			offset.x = -0.5f * (max.x + min.x) * scale.x;  
			offset.y = -0.5f * (max.y + min.y) * scale.y;
			offset.z = -min.z * scale.z;
			// Use default near-plane value

			result = glm::translate(offset);
			result = glm::scale(result, scale);

			return result;
		}

	}

	SceneView::SceneView(const GlobalVarsSharedPtr &global_vars):
		m_global_vars(global_vars), m_fov(40.0f),
		m_aspect(4.0f / 3.0f), m_z_near(1.5f),
		m_shadow_map_count(1), m_layer_count(4),
		m_exponential_shadow_maps(false)
	{
		set_default_view();
	}

	SceneView::~SceneView() noexcept
	{
	}

	void SceneView::build_shadow_matrix(
		const glm::dmat4x4 &shadow_view_matrix,
		const glm::dmat4x4 &basic_projection_matrix,
		const glm::dmat4x4 &basic_projection_view_matrix,
		const Vec3Array8 &corner_points, const Uint16 index)
	{
		glm::dmat4x4 shadow_cropp_matrix;
		glm::dmat4x4 shadow_projection_matrix;
		glm::dmat4x4 shadow_projection_view_matrix;
		glm::dmat4x4 shadow_texture_matrix;
		glm::dvec3 scale, offset;

		shadow_cropp_matrix = glm::dmat4x4(build_shadow_cropp_matrix(
			glm::mat4x4(basic_projection_view_matrix),
			corner_points, m_shadow_map_size,
			get_shadow_z_offset()));

		shadow_projection_matrix = shadow_cropp_matrix *
			basic_projection_matrix;

		shadow_projection_view_matrix =	shadow_projection_matrix *
			shadow_view_matrix;

		scale = glm::vec3(0.5f);
		offset = glm::vec3(0.5f);

		shadow_texture_matrix = glm::translate(offset);
		shadow_texture_matrix = glm::scale(shadow_texture_matrix,
			scale);

		m_shadow_projection_matrices[index] = shadow_projection_matrix;
		m_shadow_projection_view_matrices[index] =
			shadow_projection_view_matrix;
		m_shadow_texture_matrices[index] = shadow_texture_matrix *
			shadow_projection_view_matrix;
	}

	void SceneView::build_shadow_matrices(const glm::vec3 &light_direction,
		const Vec3Array8Vector &corner_points,
		const float scene_max_height)
	{
		Frustum frustum;
		glm::dmat4x4 shadow_view_matrix, basic_projection_matrix;
		glm::dmat4x4 basic_projection_view_matrix;
		glm::dvec3 dir, up, left, target, pos;
		Uint16 i;

		dir = glm::normalize(glm::dvec3(light_direction));
		dir = glm::round(dir * 100.0) / 100.0;

		m_shadow_z_offset = 0.0f;

		if (std::abs(dir.z) > epsilon)
		{
			m_shadow_z_offset = -scene_max_height / dir.z;
		}

		up = glm::vec3(0.0f, 0.0f, 1.0);

		if (std::abs(glm::dot(up, dir)) >= 1.0f)
		{
			up = glm::vec3(1.0f, 0.0f, 0.0f);
		}

		frustum = Frustum(build_projection_matrix(get_z_near(),
				get_shadow_z_far()) * m_view_matrix);

		target = glm::ivec3(glm::ceil(m_focus * 10.0f)) / 10;

		left = glm::normalize(glm::cross(dir, up));
		up = glm::normalize(glm::cross(dir, left));

		pos = target + dir * 60.0;

		m_shadow_dir = dir;
		m_shadow_up = up;

		shadow_view_matrix = glm::dmat4x4(1);

		shadow_view_matrix[0][0] = -left.x;
		shadow_view_matrix[1][0] = -left.y;
		shadow_view_matrix[2][0] = -left.z;
		shadow_view_matrix[0][1] = -up.x;
		shadow_view_matrix[1][1] = -up.y;
		shadow_view_matrix[2][1] = -up.z;
		shadow_view_matrix[0][2] = dir.x;
		shadow_view_matrix[1][2] = dir.y;
		shadow_view_matrix[2][2] = dir.z;

		for (i = 0; i < 3; ++i)
		{
			m_shadow_view_rotation_matrix[i] =
				glm::vec3(shadow_view_matrix[i]);
		}

		shadow_view_matrix = glm::translate(shadow_view_matrix,
			-glm::dvec3(pos));

		basic_projection_matrix = glm::mat4x4(1.0);
		basic_projection_matrix[2][2] = -1.0;

		basic_projection_view_matrix = basic_projection_matrix
			* shadow_view_matrix;

		m_shadow_camera = glm::inverse(shadow_view_matrix) *
			glm::dvec4(0.0, 0.0, 0.0, 1.0);

		/**
		 * 0.3 for a maximum shadow distance of 250 units, because
		 * we use exp function for esm shadows.
		 */
		m_shadow_distance_transform =
			-0.3 * glm::transpose(shadow_view_matrix)[2];

		m_shadow_view_matrix = shadow_view_matrix;

		for (i = 0; i < get_shadow_map_count(); ++i)
		{
			build_shadow_matrix(shadow_view_matrix,
				basic_projection_matrix,
				basic_projection_view_matrix, corner_points[i],
				i);
		}
	}

	void SceneView::update_shadow_matrices(
		const Vec3Array8Vector &corner_points)
	{
		glm::dmat4x4 basic_projection_matrix;
		glm::dmat4x4 basic_projection_view_matrix;
		Uint16 i;

		basic_projection_matrix = glm::mat4x4(1.0f);
		basic_projection_matrix[2][2] = -1.0f;

		basic_projection_view_matrix = basic_projection_matrix
			* glm::dmat4x4(get_shadow_view_matrix());

		for (i = 0; i < get_shadow_map_count(); ++i)
		{
			build_shadow_matrix(glm::dmat4x4(
				get_shadow_view_matrix()),
				basic_projection_matrix,
				basic_projection_view_matrix, corner_points[i],
				i);
		}
	}

	glm::mat4 SceneView::build_projection_matrix(const float z_near,
		const float z_far) const
	{
		switch (get_projection())
		{
			case pt_perspective:
				return glm::perspective(get_fov(),
					get_aspect(), z_near, z_far);
			case pt_ortho:
				return glm::ortho(get_ortho().x, get_ortho().y,
					get_ortho().z, get_ortho().w, z_near,
					z_far);
		}

		return glm::mat4();
	}

	void SceneView::update()
	{
		glm::mat4 projection_matrix;
		glm::mat4 projection_view_matrix;
		float z_near, z_far;
		Uint32 shadow_map_size;
		Uint16 i;

		m_z_far = get_global_vars()->get_view_distance();
		m_z_params.x = m_z_near;
		m_z_params.y = m_z_far;
		m_z_params.z = m_z_near + m_z_far;
		m_z_params.w = m_z_far - m_z_near;

		m_shadow_z_far = std::min(get_global_vars(
			)->get_shadow_distance(), get_z_far());

		projection_matrix = build_projection_matrix(get_z_near(),
			get_z_far());

		projection_view_matrix = projection_matrix * m_view_matrix;

		m_layer_count = 4;

		for (i = 0; i < 3; ++i)
		{
			m_view_rotation_matrix[i] =
				glm::normalize(glm::vec3(m_view_matrix[i]));
		}

		m_projection_matrix = projection_matrix;
		m_projection_view_matrix = projection_view_matrix;

		m_camera = glm::vec4(0.0, 0.0, 0.0f, 1.0f);
		m_camera = glm::inverse(m_view_matrix) * m_camera;

		m_view_dir = glm::vec4(0.0, 0.0, 1.0f, 1.0f);
		m_view_dir = glm::inverse(m_view_matrix) * m_view_dir;
		m_view_dir = glm::vec4(glm::normalize(glm::vec3(m_view_dir) -
			glm::vec3(m_camera)), 0.0f);

		switch (get_global_vars()->get_shadow_map_size())
		{
			case 0:
				shadow_map_size = 512;
				break;
			case 1:
				shadow_map_size = 1024;
				break;
			case 2:
				shadow_map_size = 2048;
				break;
			default:
				shadow_map_size = 512;
				break;
		}

		m_shadow_map_count = get_global_vars()->get_shadow_map_count();
		m_exponential_shadow_maps =
			get_global_vars()->get_exponential_shadow_maps();

		if (!get_exponential_shadow_maps())
		{
			m_shadow_map_count = std::min(get_shadow_map_count(),
				static_cast<Uint16>(1));
		}

		m_shadow_projection_matrices.resize(get_shadow_map_count());
		m_shadow_projection_view_matrices.resize(
			get_shadow_map_count());
		m_shadow_texture_matrices.resize(get_shadow_map_count());
		m_split_projection_view_matrices.resize(get_shadow_map_count());

		m_shadow_map_size = shadow_map_size;

		for (i = 0; i < get_shadow_map_count(); ++i)
		{
			z_near = get_split_point(get_z_near(),
				get_shadow_z_far(), i,
				get_shadow_map_count());
			z_far = get_split_point(get_z_near(),
				get_shadow_z_far(), i + 1,
				get_shadow_map_count());

			z_near = std::max(z_near - 0.5f, get_z_near());
			z_far = std::min(z_far + 0.5f, get_shadow_z_far());

			m_split_projection_view_matrices[i] =
				build_projection_matrix(z_near, z_far) *
				m_view_matrix;
		}

		for (i = 0; i < 4; ++i)
		{
			m_split_distances[i] = get_split_point(get_z_near(),
				get_shadow_z_far(), i + 1,
				get_shadow_map_count());
		}
	}

}
