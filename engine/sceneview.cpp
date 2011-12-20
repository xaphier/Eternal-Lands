/****************************************************************************
 *            sceneview.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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

			return glm::mix(split_uni, split_log, 0.5f);
		}

		glm::mat4x4 build_shadow_cropp_matrix(
			const glm::mat4 &shadow_projection_view_matrix,
			const BoundingBox &split_box,
			const BoundingBox &receiver_box,
			const BoundingBox &caster_box)
		{
			BoundingBox bounding_box;
			glm::mat4 matrix;
			glm::vec3 min, max, offset, scale;
			glm::vec3 split_min, split_max, caster_min, caster_max;
			glm::vec3 receiver_min, receiver_max;

			bounding_box = split_box.transform(
				shadow_projection_view_matrix);

			split_min = bounding_box.get_min();
			split_max = bounding_box.get_max();

			bounding_box = receiver_box.transform(
				shadow_projection_view_matrix);

			receiver_min = bounding_box.get_min();
			receiver_max = bounding_box.get_max();

			bounding_box = caster_box.transform(
				shadow_projection_view_matrix);

			caster_min = bounding_box.get_min();
			caster_max = bounding_box.get_max();

			min.x = glm::max(glm::max(caster_min.x,
				receiver_min.x), split_min.x);  
			max.x = glm::min(glm::min(caster_max.x,
				receiver_max.x), split_max.x);  
			min.y = glm::max(glm::max(caster_min.y,
				receiver_min.y), split_min.y);  
			max.y = glm::min(glm::min(caster_max.y,
				receiver_max.y), split_max.y);  
			min.z = glm::min(caster_min.z, split_min.z);  
			max.z = glm::min(receiver_max.z, split_max.z);  

			// Create the crop matrix.  
			scale.x = 2.0f / (max.x - min.x);
			scale.y = 2.0f / (max.y - min.y);
			scale.z = 1.0f / (max.z - min.z);
			offset.x = -0.5f * (max.x + min.x) * scale.x;  
			offset.y = -0.5f * (max.y + min.y) * scale.y;
			offset.z = -min.z * scale.z;
			// Use default near-plane value

			matrix = glm::translate(offset);
			matrix = glm::scale(matrix, scale);

			return matrix;
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
		const BoundingBox &split_box, const BoundingBox &receiver_box,
		const BoundingBox &caster_box, const Uint16 index)
	{
		glm::mat4x4 shadow_projection_matrix;
		glm::mat4x4 shadow_projection_view_matrix;
		glm::mat4x4 shadow_texture_matrix;
		glm::vec3 scale, offset;

		if (receiver_box.get_empty())
		{
			m_shadow_projection_matrices[index] = glm::mat4x4();
			m_shadow_projection_view_matrices[index] =
				glm::mat4x4();
			m_shadow_texture_matrices[index] = glm::mat4x4();

			return;
		}

		shadow_projection_matrix = build_shadow_cropp_matrix(
			basic_projection_view_matrix, split_box, receiver_box,
			caster_box) * basic_projection_matrix;

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
		const SubFrustumsBoundingBoxes &split_boxes,
		const SubFrustumsBoundingBoxes &receiver_boxes,
		const float scene_max_height)
	{
		BoundingBox caster_box;
		glm::mat4x4 shadow_view_matrix, basic_projection_matrix;
		glm::mat4x4 basic_projection_view_matrix;
		glm::vec3 target, dir, pos, up, left, shadow_extrusion;
		float shadow_extrusion_distance;
		Uint16 i;

		// Calculate look at position
		target = glm::vec3(m_focus);

		// Calculate direction, which same as directional light direction
		dir = glm::normalize(light_direction); // backwards since point down -z

		// Calculate position
		// We want to be in the -ve direction of the light direction
		// far enough to project for the dir light extrusion distance
		pos = target + dir * 50.0f;

		// get Cam orientation
		up = glm::vec3(0.0f, 0.0f, 1.0);

		// Check it's not coincident with dir
		if (std::abs(glm::dot(up, dir)) >= 1.0f)
		{
			// Use camera up
			up = glm::vec3(1.0f, 0.0f, 0.0f);
		}

		// cross twice to rederive, only direction is unaltered
		left = glm::normalize(glm::cross(dir, up));
		up = glm::normalize(glm::cross(dir, left));

		shadow_view_matrix = glm::lookAt(pos, target, -up);

		basic_projection_matrix =
			glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, get_z_near(),
				get_shadow_z_far());

		basic_projection_view_matrix = basic_projection_matrix
			* shadow_view_matrix;

		m_shadow_view_matrix = shadow_view_matrix;

		m_shadow_camera = glm::vec4(0.0, 0.0, 0.0f, 1.0f);
		m_shadow_camera = glm::inverse(shadow_view_matrix) *
			m_shadow_camera;

		for (i = 0; i < get_shadow_map_count(); ++i)
		{
			shadow_extrusion_distance = std::max(scene_max_height -
				receiver_boxes[i].get_min().z, 0.0f) / dir.z;

			shadow_extrusion = dir * shadow_extrusion_distance;

			caster_box = receiver_boxes[i];

			caster_box.set_center(shadow_extrusion +
				caster_box.get_center());

			build_shadow_matrix(basic_projection_matrix,
				basic_projection_view_matrix, split_boxes[i],
				receiver_boxes[i], caster_box, i);
		}
	}

	void SceneView::update_shadow_matrices(
		const SubFrustumsBoundingBoxes &split_boxes,
		const SubFrustumsBoundingBoxes &receiver_boxes,
		const SubFrustumsBoundingBoxes &caster_boxes)
	{
		BoundingBox split_box, receiver_box, caster_box;
		glm::mat4x4 basic_projection_matrix;
		glm::mat4x4 basic_projection_view_matrix;
		Uint16 i;

		basic_projection_matrix =
			glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, get_z_near(),
				get_shadow_z_far());

		basic_projection_view_matrix = basic_projection_matrix
			* get_shadow_view_matrix();

		for (i = 0; i < get_shadow_map_count(); ++i)
		{
			build_shadow_matrix(basic_projection_matrix,
				basic_projection_view_matrix, split_boxes[i],
				receiver_boxes[i], caster_boxes[i], i);

			split_box.merge(split_boxes[i]);
			receiver_box.merge(receiver_boxes[i]);
			caster_box.merge(caster_boxes[i]);
		}

		if (receiver_box.get_empty())
		{
			m_shadow_projection_matrix = glm::mat4x4();
			m_shadow_projection_view_matrix = glm::mat4x4();

			return;
		}

		m_shadow_projection_matrix = build_shadow_cropp_matrix(
			basic_projection_view_matrix, split_box, receiver_box,
			caster_box) * basic_projection_matrix;

		m_shadow_projection_view_matrix =
			get_shadow_projection_matrix() *
			get_shadow_view_matrix();
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
