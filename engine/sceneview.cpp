/****************************************************************************
 *            sceneview.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "sceneview.hpp"
#include "globalvars.hpp"
#include "boundingbox.hpp"

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

			return glm::mix(split_uni, split_log, 0.25f);
		}

	}

	SceneView::SceneView(const GlobalVarsSharedPtr &global_vars):
		m_global_vars(global_vars), m_fov(40.0f),
		m_aspect(4.0f / 3.0f), m_z_near(0.5f),
		m_shadow_map_count(1), m_layer_count(4),
		m_shadow_view_index(0), m_exponential_shadow_maps(false)
	{
		set_default_view();
	}

	SceneView::~SceneView() noexcept
	{
	}

	void SceneView::build_shadow_matrices(const glm::vec3 &light_direction,
		const float scene_max_height, const Uint16 index)
	{
		Vec3Array8 corner_points;
		glm::mat4x4 shadow_view_matrix, shadow_projection_matrix;
		glm::mat4x4 shadow_projection_view_matrix, matrix;
		glm::mat4x4 shadow_texture_matrix;
		glm::vec4 temp;
		glm::vec3 dir, up, left, center, pos, scale, offset;
		float shadow_z_offset, z_far, x, y, radius, diameter;

		matrix = glm::inverse(get_split_projection_view_matrices(
			)[index]);

		temp = matrix * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		corner_points[0] = glm::vec3(temp) / temp.w;
		temp = matrix * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f);
		corner_points[1] = glm::vec3(temp) / temp.w;
		temp = matrix * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f);
		corner_points[2] = glm::vec3(temp) / temp.w;
		temp = matrix * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f);
		corner_points[3] = glm::vec3(temp) / temp.w;
		temp = matrix * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f);
		corner_points[4] = glm::vec3(temp) / temp.w;
		temp = matrix * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f);
		corner_points[5] = glm::vec3(temp) / temp.w;
		temp = matrix * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f);
		corner_points[6] = glm::vec3(temp) / temp.w;
		temp = matrix * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f);
		corner_points[7] = glm::vec3(temp) / temp.w;

		BOOST_FOREACH(const glm::vec3 &point, corner_points)
		{
			center += point;
		}

		center /= static_cast<float>(corner_points.size());

		radius = 0.0f;

		BOOST_FOREACH(const glm::vec3 &point, corner_points)
		{
			radius = std::max(radius, glm::distance(center,
				point) * 0.6f);
		}

		/**
		 * rounding to prevent flickering
		 */
		radius = std::ceil(radius * 25.0f) / 25.0f;

		dir = glm::normalize(light_direction);

		shadow_z_offset = 0.0f;

		if (std::abs(dir.z) > epsilon)
		{
			shadow_z_offset = -scene_max_height / dir.z;
		}

		up = glm::vec3(0.0f, 0.0f, 1.0);

		if (std::abs(glm::dot(up, dir)) >= 1.0f)
		{
			up = glm::vec3(1.0f, 0.0f, 0.0f);
		}

		/**
		 * rounding to prevent flickering
		 */
		center = glm::round(center * 25.0f) / 25.0f;

		left = glm::normalize(glm::cross(dir, up));
		up = glm::normalize(glm::cross(dir, left));

		/**
		 * Stabilizing shadows
		 */
		diameter = radius * 2.0f;

		x = std::ceil(glm::dot(center, up) * m_shadow_map_size /
			diameter) * diameter / m_shadow_map_size;
		y = std::ceil(glm::dot(center, left) * m_shadow_map_size /
			diameter) * diameter / m_shadow_map_size;

		z_far = std::abs(shadow_z_offset) + radius;

		center = up * x + left * y + dir * glm::dot(center, dir);

		pos = center + dir * z_far;

		shadow_view_matrix = glm::mat4x4(1.0f);

		shadow_view_matrix[0][0] = -left.x;
		shadow_view_matrix[1][0] = -left.y;
		shadow_view_matrix[2][0] = -left.z;
		shadow_view_matrix[0][1] = -up.x;
		shadow_view_matrix[1][1] = -up.y;
		shadow_view_matrix[2][1] = -up.z;
		shadow_view_matrix[0][2] = dir.x;
		shadow_view_matrix[1][2] = dir.y;
		shadow_view_matrix[2][2] = dir.z;

		shadow_view_matrix = glm::translate(shadow_view_matrix,	-pos);

		shadow_projection_matrix = glm::ortho(-radius, radius,
			-radius, radius, 1.0f, z_far + radius);

		shadow_projection_view_matrix = shadow_projection_matrix
			* shadow_view_matrix;

		m_shadow_cameras[index] = glm::inverse(shadow_view_matrix) *
			glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		/**
		* 0.1f as scale value for the depth, because we use exp
		* function for esm shadows.
		*/
		m_shadow_distance_transforms[index] = -0.2f *
			glm::transpose(shadow_view_matrix)[2];

		m_shadow_view_matrices[index] = shadow_view_matrix;

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
		m_shadow_cameras.resize(get_shadow_map_count());
		m_shadow_distance_transforms.resize(get_shadow_map_count());
		m_shadow_view_matrices.resize(get_shadow_map_count());

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
