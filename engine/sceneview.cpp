/****************************************************************************
 *            sceneview.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "sceneview.hpp"
#include "globalvars.hpp"
#include "boundingbox.hpp"
#include "convexbody.hpp"
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

			return glm::mix(split_uni, split_log, 0.15f);
		}

		glm::mat4x4 build_shadow_cropp_matrix(const glm::mat4x3 &matrix,
			const ConvexBody &convex_body, const glm::vec3 &dir,
			const float max_height)
		{
			BoundingBox bounding_box;
			glm::mat4x4 result;
			glm::vec3 min, max, offset, scale;

			bounding_box = convex_body.get_transformed(
				matrix).get_bounding_box();

			min = bounding_box.get_min();
			max = bounding_box.get_max();

			if (std::abs(dir.z) > epsilon)
			{
				min.z -= max_height / dir.z;
			}

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

		glm::mat4x4 build_shadow_cropp_matrix(const glm::mat4x3 &matrix,
			const ConvexBody &convex_body)
		{
			BoundingBox bounding_box;
			glm::mat4x4 result;
			glm::vec3 min, max, offset, scale;

			bounding_box = convex_body.get_transformed(
				matrix).get_bounding_box();

			min = bounding_box.get_min();
			max = bounding_box.get_max();

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
		m_shadow_map_count(1), m_exponential_shadow_maps(false)
	{
		set_default_view();
	}

	SceneView::~SceneView() throw()
	{
	}

	void SceneView::build_shadow_matrix(
		const glm::mat4x4 &basic_projection_matrix,
		const glm::mat4x4 &basic_projection_view_matrix,
		const ConvexBody &convex_body, const glm::vec3 &dir,
		const float max_height, const Uint16 index)
	{
		glm::mat4x4 shadow_projection_matrix;
		glm::mat4x4 shadow_projection_view_matrix;
		glm::mat4x4 shadow_texture_matrix;
		glm::vec3 scale, offset;

		if (convex_body.get_bounding_box().get_empty())
		{
			m_shadow_projection_matrices[index] = glm::mat4x4();
			m_shadow_projection_view_matrices[index] =
				glm::mat4x4();
			m_shadow_texture_matrices[index] = glm::mat4x4();

			return;
		}

		shadow_projection_matrix = build_shadow_cropp_matrix(
			glm::mat4x3(basic_projection_view_matrix), convex_body,
			dir, max_height) * basic_projection_matrix;

		shadow_projection_view_matrix =	shadow_projection_matrix *
			get_shadow_view_matrix();

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

	void SceneView::build_shadow_matrix(
		const glm::mat4x4 &basic_projection_matrix,
		const glm::mat4x4 &basic_projection_view_matrix,
		const ConvexBody &convex_body, const Uint16 index)
	{
		glm::mat4x4 shadow_projection_matrix;
		glm::mat4x4 shadow_projection_view_matrix;
		glm::mat4x4 shadow_texture_matrix;
		glm::vec3 scale, offset;

		if (convex_body.get_bounding_box().get_empty())
		{
			m_shadow_projection_matrices[index] = glm::mat4x4();
			m_shadow_projection_view_matrices[index] =
				glm::mat4x4();
			m_shadow_texture_matrices[index] = glm::mat4x4();

			return;
		}

		shadow_projection_matrix = build_shadow_cropp_matrix(
			glm::mat4x3(basic_projection_view_matrix), convex_body)
				* basic_projection_matrix;

		shadow_projection_view_matrix =	shadow_projection_matrix *
			get_shadow_view_matrix();

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
		const SubFrustumsConvexBodys &convex_bodys,
		const float scene_max_height)
	{
		glm::mat4x4 shadow_view_matrix, basic_projection_matrix;
		glm::mat4x4 basic_projection_view_matrix;
		glm::vec3 dir, up, left, target, pos;
		Uint16 i;

		dir = glm::normalize(light_direction);

		up = glm::vec3(0.0f, 0.0f, 1.0);

		if (std::abs(glm::dot(up, dir)) >= 1.0f)
		{
			up = glm::vec3(1.0f, 0.0f, 0.0f);
		}

		left = glm::normalize(glm::cross(dir, up));
		up = glm::normalize(glm::cross(dir, left));

		target = glm::vec3(m_focus);
		pos = target + dir * 50.0f;

		shadow_view_matrix = glm::lookAt(pos, target, -up);

		basic_projection_matrix = glm::mat4x4(1.0f);
		basic_projection_matrix[2][2] = -1.0f;

		basic_projection_view_matrix = basic_projection_matrix
			* shadow_view_matrix;

		m_shadow_view_matrix = shadow_view_matrix;

		m_shadow_camera = glm::vec4(0.0, 0.0, 0.0f, 1.0f);
		m_shadow_camera = glm::inverse(shadow_view_matrix) *
			m_shadow_camera;

		for (i = 0; i < get_shadow_map_count(); ++i)
		{
			build_shadow_matrix(basic_projection_matrix,
				basic_projection_view_matrix, convex_bodys[i],
				dir, scene_max_height, i);
		}
	}

	void SceneView::update_shadow_matrices(
		const SubFrustumsConvexBodys &convex_bodys)
	{
		glm::mat4x4 basic_projection_matrix;
		glm::mat4x4 basic_projection_view_matrix;
		Uint16 i;

		basic_projection_matrix = glm::mat4x4(1.0f);
		basic_projection_matrix[2][2] = -1.0f;

		basic_projection_view_matrix = basic_projection_matrix
			* get_shadow_view_matrix();

		for (i = 0; i < get_shadow_map_count(); ++i)
		{
			build_shadow_matrix(basic_projection_matrix,
				basic_projection_view_matrix, convex_bodys[i],
				i);
		}
	}

	void SceneView::update()
	{
		float z_near, z_far;
		Uint32 shadow_map_size;
		Uint16 i;

		m_z_far = get_global_vars()->get_view_distance();

		m_shadow_z_far = std::min(get_global_vars(
			)->get_shadow_distance(), get_z_far());

		m_projection_matrix = glm::perspective(get_fov(),
			get_aspect(), get_z_near(), get_z_far());

		glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(m_view_matrix));
		glGetIntegerv(GL_VIEWPORT, glm::value_ptr(m_view_port));

		m_projection_view_matrix = m_projection_matrix *
			get_view_matrix();

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
				shadow_map_size = 1536;
				break;
			case 3:
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
		m_projection_view_matrices.resize(get_shadow_map_count());

		m_shadow_map_width = shadow_map_size;
		m_shadow_map_height = shadow_map_size;

		for (i = 0; i < get_shadow_map_count(); ++i)
		{
			z_near = get_split_point(get_z_near(),
				get_shadow_z_far(), i,
				get_shadow_map_count());
			z_far = get_split_point(get_z_near(),
				get_shadow_z_far(), i + 1,
				get_shadow_map_count());

			z_near = std::max(z_near - 1.5f, get_z_near());
			z_far = std::min(z_far + 1.5f, get_shadow_z_far());

			m_projection_view_matrices[i] =
				glm::perspective(get_fov(), get_aspect(),
					z_near, z_far) * get_view_matrix();
		}

		for (i = 0; i < 4; ++i)
		{
			m_split_distances[i] = get_split_point(get_z_near(),
				get_shadow_z_far(), i + 1,
				get_shadow_map_count());
		}
	}

}
