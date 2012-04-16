/****************************************************************************
 *            clipmap.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "clipmap.hpp"

namespace eternal_lands
{

	namespace
	{

		Vec2Array4 dir = { {
			glm::vec2( 1.0f, 0.0f), glm::vec2(0.0f,  1.0f),
			glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f)
		} };

	}

	Clipmap::Clipmap()
	{
	}

	Clipmap::~Clipmap() throw()
	{
	}

	void Clipmap::rebuild(const glm::vec2 &terrain_world_size,
		const float view_distance, const float clipmap_world_size,
		const Uint16 clipmap_size)
	{
		glm::vec2 terrain_texture_size;

		m_slices = (std::log(view_distance / clipmap_world_size) /
			std::log(2.0f)) + 3;
		m_slices = std::min(m_slices, static_cast<Uint16>(6));
		m_slices = std::max(m_slices, static_cast<Uint16>(1));

/*
		m_terrain_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
			String(UTF8("terrain")), clipmap_size, clipmap_size,
			m_slices, 0, ttt_texture_2d_array, tft_rgb8, false);
*/
		m_texture_matrices.resize(m_slices);

		m_focus = glm::vec2(-1e7f);
		m_terrain_world_size = terrain_world_size;
		m_clipmap_world_size = clipmap_world_size;
		m_clipmap_size = clipmap_size;
		m_dir_index = 0;

		terrain_texture_size = (glm::vec2(terrain_world_size) *
			static_cast<float>(clipmap_size)) / clipmap_world_size;

		m_terrain_texture_size.x = terrain_texture_size.x;
		m_terrain_texture_size.y = terrain_texture_size.y;
		m_terrain_texture_size.z = 1.0f / terrain_texture_size.x;
		m_terrain_texture_size.w = 1.0f / terrain_texture_size.y;
	}

	bool Clipmap::update(const glm::vec3 &camera, const glm::vec3 &view_dir,
		const glm::vec2 &focus)
	{
		Uint16 dir_index;

		dir_index = get_dir_index(glm::normalize(glm::vec2(view_dir)));

		if ((dir_index != get_dir_index()) ||
			(glm::distance(focus, get_focus()) > 1.0f))
		{
			m_focus = focus;
			m_dir_index = dir_index;
			m_distance = glm::distance(glm::vec2(camera), focus);

			return true;
		}

		return false;
	}

	Uint16 Clipmap::get_dir_index(const glm::vec2 &view_dir) const
	{
		Uint16 dir_index;

		dir_index = get_dir_index();

		if (glm::dot(view_dir, dir[dir_index]) <= std::cos(45.5f))
		{
			return dir_index;
		}

		dir_index = (get_dir_index() + 1) % 4;

		if (glm::dot(view_dir, dir[dir_index]) <= std::cos(45.5f))
		{
			return dir_index;
		}

		dir_index = (get_dir_index() + 3) % 4;

		if (glm::dot(view_dir, dir[dir_index]) <= std::cos(45.5f))
		{
			return dir_index;
		}

		dir_index = (get_dir_index() + 2) % 4;

		assert(glm::dot(view_dir, dir[dir_index]) <= std::cos(45.5f));

		return dir_index;
	}

	void Clipmap::update_slice(const Uint16 slice)
	{
		glm::mat2x3 texture_matrix;
		glm::vec2 offset, scale;
		float clipmap_world_size;

		clipmap_world_size = get_clipmap_world_size() *
			static_cast<float>(1 << slice);

		offset = get_focus() - clipmap_world_size * 0.5f;

		if (!get_centered())
		{

			if (slice > 0)
			{
				offset += dir[get_dir_index()] *
					clipmap_world_size * 0.25f;
			}

			offset += dir[get_dir_index()] * (get_clipmap_world_size() * 0.5f - m_distance);
		}

		offset /= clipmap_world_size;
		scale = get_terrain_world_size() / clipmap_world_size;

		texture_matrix[0] = glm::vec3(scale.x, 0.0f, -offset.x);
		texture_matrix[1] = glm::vec3(0.0f, scale.y, -offset.y);

		m_texture_matrices[slice] = texture_matrix;

		texture_matrix = glm::mat2x3(glm::inverse(glm::mat3(
			texture_matrix)));
	}

}
