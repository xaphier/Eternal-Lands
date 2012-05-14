/****************************************************************************
 *			cdlodquadtree.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cdlodquadtree.hpp"
#include "alignedvec4array.hpp"
#include "frustum.hpp"

namespace eternal_lands
{

	namespace
	{

		const glm::uvec2 quad_orders[32] =
		{
			// index 0
			glm::uvec2(0, 0),
			glm::uvec2(0, 1),
			glm::uvec2(1, 0),
			glm::uvec2(1, 1),
			// index 1
			glm::uvec2(0, 0),
			glm::uvec2(1, 0),
			glm::uvec2(0, 1),
			glm::uvec2(1, 1),
			// index 2
			glm::uvec2(0, 1),
			glm::uvec2(0, 0),
			glm::uvec2(1, 1),
			glm::uvec2(1, 0),
			// index 3
			glm::uvec2(0, 1),
			glm::uvec2(1, 1),
			glm::uvec2(0, 0),
			glm::uvec2(1, 0),
			// index 4
			glm::uvec2(1, 1),
			glm::uvec2(1, 0),
			glm::uvec2(0, 1),
			glm::uvec2(0, 0),
			// index 5
			glm::uvec2(1, 1),
			glm::uvec2(0, 1),
			glm::uvec2(1, 0),
			glm::uvec2(0, 0),
			// index 6
			glm::uvec2(1, 0),
			glm::uvec2(1, 1),
			glm::uvec2(0, 0),
			glm::uvec2(0, 1),
			// index 7
			glm::uvec2(1, 0),
			glm::uvec2(0, 0),
			glm::uvec2(1, 1),
			glm::uvec2(0, 1)
		};

	}

	Uint16 CdLodQuadTree::get_quad_order(const glm::vec2 &dir) noexcept
	{
		Uint16 result;

		result = 0;

		if (dir.x < 0.0f)
		{
			result += 4;
		}

		if (dir.y < 0.0f)
		{
			result += 2;
		}

		if (std::abs(dir.x) < std::abs(dir.y))
		{
			result += 1;
		}

		return result;
	}

	const glm::uvec2 &CdLodQuadTree::get_quad_order(const Uint16 quad_order,
		const Uint16 index) noexcept
	{
		assert(quad_order < 8);
		assert(index < 4);

		return quad_orders[quad_order * 4 + index];
	}

	CdLodQuadTree::CdLodQuadTree(const ImageSharedPtr height_map,
		const glm::vec3 &scale, const glm::vec3 &offset)
	{
	}

	CdLodQuadTree::~CdLodQuadTree() noexcept
	{
	}

	float get_height_scale()
	{
		return 32.0f;
	}

	void CdLodQuadTree::calculate_lod_params()
	{
		Uint32 i, size;
		float range_end, morph_range, morph_start;
		float range, cur_range;

		assert(m_lod_count <= get_max_patch_count());

		size = m_patch_size;

		range = 2.0f * std::sqrt(2.0f) * m_cell_size * m_patch_size;

		cur_range = 0.0f;

		for (i = 0; i < m_lod_count; ++i)
		{
			m_lods[i].range_start = cur_range;
			cur_range += range;

			range_end = cur_range;
			morph_range = range * m_morph_zone_ratio;
			morph_start = range_end - morph_range;

			m_lods[i].morph_params = glm::vec2(1.0f / morph_range,
				-morph_start / morph_range);

			m_lods[i].patch_size = size;

			range *= 2.0f;
			size *= 2;
		}

		m_lods[m_lod_count - 1].morph_params = glm::vec2(0.0f);
	}

	void CdLodQuadTree::add_patch_to_queue(const glm::uvec2 &position,
		const Uint16 level) const
	{
/*
		glm::vec4 data;

		if (patchCount < MAX_PATCH_COUNT)
		{
			data.x = position.x;
			data.y = position.y;
			data.z = m_lods[level].patch_size;
			data.w = m_lods[level].patch_size * 2.0f;

			data.x = m_lods[level].morph_params.x;
			data.y = m_lods[level].morph_params.y;
		}
*/
	}

	void CdLodQuadTree::select_quads_for_drawing(const Frustum &frustum,
		const glm::vec3 &camera, const glm::uvec2 &position,
		const PlanesMask mask, const Uint16 level) const
	{
		BoundingBox box;
		glm::vec3 min, max;
		glm::vec2 size, center, half_size, distance;
		PlanesMask out_mask;
		Uint32 offset;
		Uint16 i, index;
		bool intersect;

		if (glm::any(glm::greaterThanEqual(position, m_grid_size - 1u)))
		{
			return;
		}

		size = glm::vec2(glm::min(glm::uvec2(m_lods[level].patch_size),
			m_grid_size - position)) * m_cell_size;
		min = glm::vec3(m_start, 0.0f) + m_cell_size *
			glm::vec3(position, 0.0f);
		max = min + glm::vec3(size, get_height_scale());

		box.set_min_max(min, max);

		if (frustum.intersect(box, mask, out_mask) == it_outside)
		{
			return;
		}

		if ((level == 0) || ((m_lods[level].range_start +
			get_height_scale()) < camera.z))
		{
			add_patch_to_queue(position, level);
			return;
		}

		/**
		 * Check whether patch intersects it's LOD sphere and thus
		 * require subdivision
		 */
		center = glm::vec2(box.get_center());
		half_size = glm::vec2(box.get_half_size());
		distance = glm::abs(center - glm::vec2(camera));
		distance = glm::max(distance - half_size, glm::vec2(0.0f));

		intersect = glm::dot(distance, distance) <
			(m_lods[level].range_start * m_lods[level].range_start);

		if (!intersect)
		{
			add_patch_to_queue(position, level);

			return;
		}

		offset = m_lods[level].patch_size / 2;

		index = get_quad_order(glm::vec2(position + offset) -
			glm::vec2(camera));

		for (i = 0; i < 4; ++i)
		{
			select_quads_for_drawing(frustum, camera, position +
					get_quad_order(offset, index, i),
				out_mask, level - 1);
		}
	}

}
