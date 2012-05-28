/****************************************************************************
 *			cdlodquadtree.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cdlodquadtree.hpp"
#include "alignedvec4array.hpp"
#include "frustum.hpp"
#include "shader/mappeduniformbuffer.hpp"
#include "image.hpp"

namespace eternal_lands
{

	namespace
	{

		const boost::array<Uvec2Array4, 8> quad_orders =
			boost::assign::list_of
				// index 0
				(boost::assign::list_of
					(glm::uvec2(0, 0))
					(glm::uvec2(0, 1))
					(glm::uvec2(1, 0))
					(glm::uvec2(1, 1)))
				// index 1
				(boost::assign::list_of
					(glm::uvec2(0, 0))
					(glm::uvec2(1, 0))
					(glm::uvec2(0, 1))
					(glm::uvec2(1, 1)))
				// index 2
				(boost::assign::list_of
					(glm::uvec2(0, 1))
					(glm::uvec2(0, 0))
					(glm::uvec2(1, 1))
					(glm::uvec2(1, 0)))
				// index 3
				(boost::assign::list_of
					(glm::uvec2(0, 1))
					(glm::uvec2(1, 1))
					(glm::uvec2(0, 0))
					(glm::uvec2(1, 0)))
				// index 4
				(boost::assign::list_of
					(glm::uvec2(1, 0))
					(glm::uvec2(1, 1))
					(glm::uvec2(0, 0))
					(glm::uvec2(0, 1)))
				// index 5
				(boost::assign::list_of
					(glm::uvec2(1, 0))
					(glm::uvec2(0, 0))
					(glm::uvec2(1, 1))
					(glm::uvec2(0, 1)))
				// index 6
				(boost::assign::list_of
					(glm::uvec2(1, 1))
					(glm::uvec2(1, 0))
					(glm::uvec2(0, 1))
					(glm::uvec2(0, 0)))
				// index 7
				(boost::assign::list_of
					(glm::uvec2(1, 1))
					(glm::uvec2(0, 1))
					(glm::uvec2(1, 0))
					(glm::uvec2(0, 0)));

		void get_min_max(const ImageSharedPtr &image,
			const glm::vec3 &scale, const glm::uvec2 &offset,
			const Uint32 size, glm::vec3 &min, glm::vec3 &max)
		{
			glm::vec3 tmp;
			glm::uvec2 index, count;
			Uint32 x, y;

			min = glm::vec3(std::numeric_limits<float>::max());
			max = glm::vec3(-std::numeric_limits<float>::max());

			if (glm::any(glm::lessThan(glm::uvec2(
				image->get_sizes()), offset + size)))
			{
				return;
			}

			count = glm::uvec2(image->get_sizes()) - offset;
			count = glm::min(count, size);

			for (y = 0; y < count.y; ++y)
			{
				for (x = 0; x < count.x; ++x)
				{
					index = offset + glm::uvec2(x, y);

					tmp = glm::vec3(image->get_pixel(
						index.x, index.y, 0, 0, 0));

					min = glm::min(min, tmp);
					max = glm::max(max, tmp);
				}
			}

			min.x = min.x * 2.0f - 1.0f;
			min.y = min.y * 2.0f - 1.0f;
			max.x = max.x * 2.0f - 1.0f;
			max.y = max.y * 2.0f - 1.0f;

			min *= scale;
			max *= scale;
		}

	}

	const Uvec2Array4 &CdLodQuadTree::get_quad_order(const glm::vec2 &dir)
		noexcept
	{
		Uint16 index;

		index = 0;

		if (dir.x < 0.0f)
		{
			index += 4;
		}

		if (dir.y < 0.0f)
		{
			index += 2;
		}

		if (std::abs(dir.x) < std::abs(dir.y))
		{
			index += 1;
		}

		assert(index < 8);

		return quad_orders[index];
	}

	CdLodQuadTree::CdLodQuadTree(const ImageSharedPtr &vector_map,
		const glm::vec3 &scale)
	{
		Uint32 max_grid_size;

		m_grid_size.x = vector_map->get_width();
		m_grid_size.y = vector_map->get_height();

		max_grid_size = std::max(m_grid_size.x, m_grid_size.y);

		m_lod_count = 1;

		while ((m_lod_count < get_max_lod_count()) &&
			(max_grid_size < (m_patch_size * 1 << m_lod_count)))
		{
			m_lod_count++;
		}
	}

	CdLodQuadTree::~CdLodQuadTree() noexcept
	{
	}

	void CdLodQuadTree::init_min_max(const ImageSharedPtr &image,
		const glm::vec3 &scale, const glm::uvec2 &position,
		const Uint16 level, glm::vec3 &min, glm::vec3 &max)
	{
		glm::vec3 tmin, tmax;

		if (level == 0)
		{
			get_min_max(image, scale, position, m_patch_size,
				min, max);

			m_lods[level].min_max[position.x][position.y][0] = min;
			m_lods[level].min_max[position.x][position.y][1] = max;

			return;
		}

		init_min_max(image, scale, position * 2u + glm::uvec2(0, 0),
			level - 1, min, max);

		init_min_max(image, scale, position * 2u + glm::uvec2(0, 1),
			level - 1, tmin, tmax);

		min = glm::min(min, tmin);
		max = glm::max(max, tmax);

		init_min_max(image, scale, position * 2u + glm::uvec2(1, 0),
			level - 1, tmin, tmax);

		min = glm::min(min, tmin);
		max = glm::max(max, tmax);

		init_min_max(image, scale, position * 2u + glm::uvec2(1, 1),
			level - 1, tmin, tmax);

		min = glm::min(min, tmin);
		max = glm::max(max, tmax);

		m_lods[level].min_max[position.x][position.y][0] = min;
		m_lods[level].min_max[position.x][position.y][1] = max;
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
		const Uint16 level, MappedUniformBuffer &instances,
		Uint32 &instance_index) const
	{
		glm::mat2x4 data;

		if (instance_index < get_max_patch_count())
		{
			data[0].x = position.x;
			data[0].y = position.y;
			data[0].z = m_lods[level].patch_size;
			data[0].w = m_lods[level].patch_size * 2.0f;

			data[1].x = m_lods[level].morph_params.x;
			data[1].y = m_lods[level].morph_params.y;
			data[1].z = level;
			data[1].w = 0.0f;

			instances.set_parameter(apt_terrain_instances,
				data, instance_index);

			instance_index++;
		}
	}

	void CdLodQuadTree::select_quads_for_drawing(const Frustum &frustum,
		const glm::vec3 &camera, const glm::uvec2 &position,
		const PlanesMask mask, const Uint16 level,
		MappedUniformBuffer &instances, Uint32 &instance_index) const
	{
		BoundingBox box;
		glm::vec3 min, max;
		glm::vec2 size, center, half_size, distance;
		glm::uvec2 pos;
		PlanesMask out_mask;
		Uint32 offset;
		Uint16 i;
		bool intersect;

		if (glm::any(glm::greaterThanEqual(position, m_grid_size - 1u)))
		{
			return;
		}

		size = glm::vec2(glm::min(glm::uvec2(m_lods[level].patch_size),
			m_grid_size - position)) * m_cell_size;
		min = m_cell_size * glm::vec3(position, 0.0f);
		max = min + glm::vec3(size, 0.0f);

		pos = position / m_lods[level].patch_size;
		min += m_lods[level].min_max[pos.x][pos.y][0];
		max += m_lods[level].min_max[pos.x][pos.y][1];

		box.set_min_max(min, max);

		if (frustum.intersect(box, mask, out_mask) == it_outside)
		{
			return;
		}

		if ((level == 0) || ((m_lods[level].range_start +
			get_max_z()) < camera.z))
		{
			add_patch_to_queue(position, level, instances,
				instance_index);
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
			add_patch_to_queue(position, level, instances,
				instance_index);

			return;
		}

		offset = m_lods[level].patch_size / 2;

		const Uvec2Array4 &quad_order = get_quad_order(
			glm::vec2(position + offset) - glm::vec2(camera));

		for (i = 0; i < 4; ++i)
		{
			select_quads_for_drawing(frustum, camera, position +
				quad_order[i] * offset, out_mask, level - 1,
				instances, instance_index);
		}
	}

	void CdLodQuadTree::select_quads_for_drawing(const Frustum &frustum,
		const glm::vec3 &camera, MappedUniformBuffer &instances,
		Uint32 &instance_count) const
	{
		glm::vec4 terrain_lod_offset;
		float dist;
		Uint32 x, y, level, step;
		PlanesMask mask;

		level = m_lod_count - 1;

		step = m_lods[level].patch_size;

		mask = frustum.get_planes_mask();

		instance_count = 0;

		dist = std::abs(camera.z - m_max.z * 0.5f);
		terrain_lod_offset = glm::vec4(-camera.x, -camera.y, 0.0f,
			0.0f);

		if (dist > (m_max.z * 0.5f))
		{
			terrain_lod_offset.z = dist - m_max.z * 0.5f;
		}

		instances.set_parameter(apt_terrain_lod_offset,
			terrain_lod_offset, 0);

		for (y = 0; y < (m_grid_size.y - 1); y += step)
		{
			for (x = 0; x < (m_grid_size.x - 1); x += step)
			{
				select_quads_for_drawing(frustum, camera,
					glm::uvec2(x, y), mask, level,
					instances, instance_count);
			}
		}
	}

}
