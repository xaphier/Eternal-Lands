/****************************************************************************
 *            clipmapterrain.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "clipmapterrain.hpp"
#include "texturetargetutil.hpp"
#include "textureformatutil.hpp"
#include "materialdescription.hpp"
#include "materialbuilder.hpp"

namespace eternal_lands
{

	namespace
	{

		Vec2Array4 dir = { {
			glm::vec2( 1.0f, 0.0f), glm::vec2(0.0f,  1.0f),
			glm::vec2(-1.0f, 0.0f), glm::vec2(0.0f, -1.0f)
		} };

	}

	ClipmapTerrain::ClipmapTerrain(
		const MaterialBuilderWeakPtr &material_builder):
		m_material_builder(material_builder), m_world_size(16.0f),
		m_distance(0.0f), m_size(512), m_dir_index(0), m_slices(4)
	{
	}

	ClipmapTerrain::~ClipmapTerrain() noexcept
	{
	}

	void ClipmapTerrain::rebuild(const glm::vec2 &terrain_world_size,
		const float view_distance, const float world_size,
		const Uint16 size, const Uint16 slices)
	{
		glm::vec2 terrain_texture_size;

		m_focus = glm::vec2(-1e7f);
		m_terrain_world_size = terrain_world_size;
		m_world_size = world_size;
		m_size = size;
		m_slices = slices;
		m_dir_index = 0;

		m_texture_matrices.resize(m_slices);

		terrain_texture_size = (glm::vec2(terrain_world_size) *
			static_cast<float>(size)) / world_size;

		m_terrain_texture_size.x = terrain_texture_size.x;
		m_terrain_texture_size.y = terrain_texture_size.y;
		m_terrain_texture_size.z = 1.0f / terrain_texture_size.x;
		m_terrain_texture_size.w = 1.0f / terrain_texture_size.y;
	}

	bool ClipmapTerrain::update(const glm::vec3 &camera,
		const glm::vec3 &view_dir, const glm::vec2 &focus)
	{
#if	0
		glm::vec2 dir;
		Uint16 dir_index;

		dir = glm::vec2(view_dir);
		dir_index = get_dir_index();

		if (std::abs(glm::dot(dir, dir)) > epsilon)
		{
			dir_index = get_dir_index(glm::normalize(dir));
		}

		if ((dir_index != get_dir_index()) ||
			(glm::distance(focus, get_focus()) > 1.0f))
		{
			m_focus = focus;
			m_dir_index = dir_index;
			m_distance = glm::distance(glm::vec2(camera), focus);

			return true;
		}
#else
		if (glm::distance(focus, get_focus()) > 1.0f)
		{
			m_focus = focus;
			m_distance = glm::distance(glm::vec2(camera), focus);

			return true;
		}
#endif
		return false;
	}

	Uint16 ClipmapTerrain::get_dir_index(const glm::vec2 &view_dir) const
		noexcept
	{
		Uint16 dir_index;

		dir_index = get_dir_index();

		assert(dir_index < 4);

		if (glm::dot(view_dir, dir[dir_index]) >= 0.7f)
		{
			return dir_index;
		}

		dir_index = (get_dir_index() + 1) % 4;

		if (glm::dot(view_dir, dir[dir_index]) >= 0.7f)
		{
			return dir_index;
		}

		dir_index = (get_dir_index() + 3) % 4;

		if (glm::dot(view_dir, dir[dir_index]) >= 0.7f)
		{
			return dir_index;
		}

		dir_index = (get_dir_index() + 2) % 4;

		assert(glm::dot(view_dir, dir[dir_index]) >= 0.7f);

		return dir_index;
	}

	void ClipmapTerrain::update_slice(const Uint16 slice)
	{
		glm::mat2x3 texture_matrix;
		glm::vec2 offset, scale;
		float world_size;

		world_size = get_world_size() *	static_cast<float>(1 << slice);

		offset = get_focus() - world_size * 0.5f;
/*
		offset -= dir[get_dir_index()] * world_size * 0.495f;
//		offset += dir[get_dir_index()] * get_world_size() * 0.5f;
//		offset -= dir[get_dir_index()] *
//			(get_world_size() * 0.5f - m_distance);
		offset += dir[get_dir_index()] * m_distance * 1.05f;
*/
		offset /= world_size;
		scale = get_terrain_world_size() / world_size;

		texture_matrix[0] = glm::vec3(scale.x, 0.0f, -offset.x);
		texture_matrix[1] = glm::vec3(0.0f, scale.y, -offset.y);

		m_texture_matrices[slice] = texture_matrix;
	}

}
