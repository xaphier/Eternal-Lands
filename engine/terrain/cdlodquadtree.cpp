/****************************************************************************
 *            cdlodquadtree.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cdlodquadtree.hpp"
#include "alignedarrays.hpp"
#include "frustum.hpp"
#include "shader/mappeduniformbuffer.hpp"
#include "image.hpp"
#include "logging.hpp"
#include "abstractterrain.hpp"

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

	CdLodQuadTree::CdLodQuadTree(const bool low_quality):
		m_low_quality(low_quality)
	{
		BOOST_FOREACH(LodDescription &lod, m_lods)
		{
			lod.range_start = 0.0f;
			lod.patch_scale = 0;
		}

		m_patch_scale = 0;
		m_lod_count = 0;
	}

	CdLodQuadTree::~CdLodQuadTree() noexcept
	{
	}

	void CdLodQuadTree::get_min_max_rgb10_a2(
		const ImageConstSharedPtr &image, const glm::uvec2 &offset,
		const Uint32 size, glm::vec3 &min, glm::vec3 &max)
	{
		glm::vec3 tmp;
		glm::uvec2 index, count;
		Uint32 x, y;

		min = glm::vec3(std::numeric_limits<float>::max());
		max = glm::vec3(-std::numeric_limits<float>::max());

		if (glm::any(glm::lessThan(glm::uvec2(image->get_size()),
			offset + size)))
		{
			return;
		}

		count = glm::uvec2(image->get_size()) - offset;
		count = glm::min(count, size);

		for (y = 0; y < count.y; ++y)
		{
			for (x = 0; x < count.x; ++x)
			{
				index = offset + glm::uvec2(x, y);

				tmp = AbstractTerrain::
					get_offset_scaled_rgb10_a2(
						image->get_pixel_packed_uint32(
							index.x, index.y, 0,
							0, 0));

				min = glm::min(min, tmp);
				max = glm::max(max, tmp);
			}
		}
	}

	void CdLodQuadTree::get_min_max(const ImageConstSharedPtr &image,
		const glm::uvec2 &offset, const Uint32 size, glm::vec3 &min,
		glm::vec3 &max)
	{
		glm::vec3 tmp;
		glm::uvec2 index, count;
		Uint32 x, y;

		min = glm::vec3(std::numeric_limits<float>::max());
		max = glm::vec3(-std::numeric_limits<float>::max());

		if (glm::any(glm::lessThan(glm::uvec2(image->get_size()),
			offset + size)))
		{
			return;
		}

		count = glm::uvec2(image->get_size()) - offset;
		count = glm::min(count, size);

		for (y = 0; y < count.y; ++y)
		{
			for (x = 0; x < count.x; ++x)
			{
				index = offset + glm::uvec2(x, y);

				tmp = AbstractTerrain::get_offset_scaled_0_1(
					image->get_pixel(index.x, index.y, 0,
						0, 0));

				min = glm::min(min, tmp);
				max = glm::max(max, tmp);
			}
		}
	}

	void CdLodQuadTree::init(const ImageConstSharedPtr &displacement_map,
		const glm::vec3 &translation, const float patch_scale)
	{
		glm::vec3 min, max;
		glm::uvec2 size;
		Uint32 x, y, level, step;
		Uint32 max_grid_size;

		m_patch_scale = patch_scale;

		m_grid_size.x = displacement_map->get_width();
		m_grid_size.y = displacement_map->get_height();

		max_grid_size = std::max(get_grid_size().x, get_grid_size().y);
		max_grid_size--;

		m_lod_count = 1;

		while ((get_lod_count() < get_max_lod_count()) &&
			(max_grid_size >= (get_patch_size() *
				(1 << get_lod_count()))))
		{
			m_lod_count++;
		}

		calculate_lod_params();

		size = (get_grid_size() + get_patch_size() - 2u) /
			get_patch_size();

		if ((displacement_map->get_format() == GL_RGBA) &&
			(displacement_map->get_type() ==
				GL_UNSIGNED_INT_10_10_10_2))
		{
			update_level_zero_rgb10_a2(displacement_map,
				translation, glm::uvec2(0), size);
		}
		else
		{
			update_level_zero(displacement_map, translation,
				glm::uvec2(0), size);
		}

		level = get_lod_count() - 1;

		step = m_lods[level].patch_scale * get_patch_size();

		size = (get_grid_size() + step - 2u) / step;

		m_min = glm::vec3(std::numeric_limits<float>::max());
		m_max = glm::vec3(-std::numeric_limits<float>::max());

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				min = glm::vec3(
					std::numeric_limits<float>::max());
				max = glm::vec3(
					-std::numeric_limits<float>::max());

				init_min_max(glm::uvec2(x, y), level, min,
					max);

				m_min = glm::min(m_min, min);
				m_max = glm::max(m_max, max);
			}
		}
	}

	void CdLodQuadTree::clear()
	{
		BOOST_FOREACH(LodDescription &lod, m_lods)
		{
			lod.range_start = 0.0f;
			lod.patch_scale = 0;
		}

		m_patch_scale = 0;
		m_lod_count = 0;
	}

	void CdLodQuadTree::update_level_zero_rgb10_a2(
		const ImageConstSharedPtr &displacement_map,
		const glm::vec3 &translation, const glm::uvec2 &offset,
		const glm::uvec2 &size)
	{
		Uint32 x, y;

		#pragma omp parallel for private(x)
		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				glm::vec3 min, max;

				min = glm::vec3(std::numeric_limits<
					float>::max());
				max = glm::vec3(-std::numeric_limits<
					float>::max());

				get_min_max_rgb10_a2(displacement_map,
					glm::uvec2(x, y) * get_patch_size(),
					get_patch_size() + 1, min, max);

				m_lods[0].min_max[x][y][0] = min + translation;
				m_lods[0].min_max[x][y][1] = max + translation;
			}
		}
	}

	void CdLodQuadTree::update_level_zero(
		const ImageConstSharedPtr &displacement_map,
		const glm::vec3 &translation, const glm::uvec2 &offset,
		const glm::uvec2 &size)
	{
		Uint32 x, y;

		#pragma omp parallel for private(x)
		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				glm::vec3 min, max;

				min = glm::vec3(std::numeric_limits<
					float>::max());
				max = glm::vec3(-std::numeric_limits<
					float>::max());

				get_min_max(displacement_map,
					glm::uvec2(x, y) * get_patch_size(),
					get_patch_size() + 1, min, max);

				m_lods[0].min_max[x][y][0] = min + translation;
				m_lods[0].min_max[x][y][1] = max + translation;
			}
		}
	}

	void CdLodQuadTree::init_min_max(const glm::uvec2 &position,
		const Uint16 level, glm::vec3 &min, glm::vec3 &max)
	{
		glm::vec3 tmin, tmax;

		if ((position.x >= m_lods[level].min_max.shape()[0]) ||
			(position.y >= m_lods[level].min_max.shape()[1]))
		{
			return;
		}

		if (level == 0)
		{
			min = m_lods[level].min_max[position.x][position.y][0];
			max = m_lods[level].min_max[position.x][position.y][1];

			return;
		}

		init_min_max(position * 2u + glm::uvec2(0, 0), level - 1, min,
			max);

		init_min_max(position * 2u + glm::uvec2(0, 1), level - 1, tmin,
			tmax);

		min = glm::min(min, tmin);
		max = glm::max(max, tmax);

		init_min_max(position * 2u + glm::uvec2(1, 0), level - 1, tmin,
			tmax);

		min = glm::min(min, tmin);
		max = glm::max(max, tmax);

		init_min_max(position * 2u + glm::uvec2(1, 1), level - 1, tmin,
			tmax);

		min = glm::min(min, tmin);
		max = glm::max(max, tmax);

		m_lods[level].min_max[position.x][position.y][0] = min;
		m_lods[level].min_max[position.x][position.y][1] = max;
	}

	void CdLodQuadTree::calculate_lod_params()
	{
		glm::uvec2 size;
		Uint32 i, patch_scale, patch_size;
		float range_end, morph_range, morph_start;
		float range, cur_range;

		assert(get_lod_count() <= get_max_lod_count());

		patch_scale = 1;

		range = 2.0f * std::sqrt(2.0f) * get_patch_scale() *
			get_patch_size();

		cur_range = 5.0f;

		if (!m_low_quality)
		{
			cur_range += 20.0f;
		}

		for (i = 0; i < get_lod_count(); ++i)
		{
			m_lods[i].range_start = cur_range;
			cur_range += range;

			range_end = cur_range;
			morph_range = range * get_morph_zone_ratio();
			morph_start = range_end - morph_range;

			m_lods[i].morph_params = glm::vec2(1.0f / morph_range,
				-morph_start / morph_range);

			m_lods[i].patch_scale = patch_scale;

			patch_size = patch_scale * get_patch_size();

			size = (get_grid_size() + patch_size - 2u) / patch_size;

			m_lods[i].min_max.resize(
				boost::extents[size.x][size.y]);

			range *= 2.0f;
			patch_scale *= 2;
		}

		m_lods[0].range_start = 0.0f;

		m_lods[get_lod_count() - 1].morph_params = glm::vec2(0.0f);
	}

	void CdLodQuadTree::add_patch_to_queue(const glm::uvec2 &position,
		const AbstractWriteMemorySharedPtr &buffer,
		const Uint64 offset, const Uint16 level,
		const Uint16 max_instances, Uint32 &instance_index) const
	{
		float* ptr;

		ptr = reinterpret_cast<float*>(static_cast<Uint8*>(
			buffer->get_ptr()) + offset);

		if (instance_index < max_instances)
		{
			ptr[instance_index * 8 + 0] = position.x;
			ptr[instance_index * 8 + 1] = position.y;
			ptr[instance_index * 8 + 2] =
				m_lods[level].patch_scale;
			ptr[instance_index * 8 + 3] =
				m_lods[level].patch_scale * 2.0f;
			ptr[instance_index * 8 + 4] =
				m_lods[level].morph_params.x;
			ptr[instance_index * 8 + 5] =
				m_lods[level].morph_params.y;
			ptr[instance_index * 8 + 6] = level;
			ptr[instance_index * 8 + 7] = m_lods[level].patch_scale
				* get_patch_size();

			instance_index++;

			return;
		}

		LOG_ERROR(lt_rendering,
			UTF8("instance index %1% too big (%2%)"),
			instance_index % max_instances);

		instance_index++;
	}

	void CdLodQuadTree::select_quads_for_drawing(const Frustum &frustum,
		const glm::vec3 &camera, const glm::uvec2 &position,
		const AbstractWriteMemorySharedPtr &buffer,
		const Uint64 offset, const BitSet64 mask, const Uint16 level,
		const Uint16 max_instances, BoundingBox &bounding_box,
		Uint32 &instance_index) const
	{
		BoundingBox box;
		glm::vec3 min, max;
		glm::vec2 size, center, half_size, distance;
		glm::uvec2 pos;
		BitSet64 out_mask;
		Uint32 patch_offset, patch_size;
		Uint16 i;
		bool intersect;

		if (glm::any(glm::greaterThanEqual(position,
			get_grid_size() - 1u)))
		{
			return;
		}

		patch_size = m_lods[level].patch_scale * get_patch_size();

		size = glm::vec2(glm::min(glm::uvec2(patch_size),
			get_grid_size() - position)) * get_patch_scale();
		min = get_patch_scale() * glm::vec3(position, 0.0f);
		max = min + glm::vec3(size, 0.0f);

		pos = position / patch_size;
		min += m_lods[level].min_max[pos.x][pos.y][0] - 0.5f;
		max += m_lods[level].min_max[pos.x][pos.y][1] + 0.5f;

		min = glm::min(min, max - 0.05f);

		box.set_min_max(min, max);
		box.scale(1.05f);

		if (frustum.intersect(box, mask, out_mask) == it_outside)
		{
			return;
		}

		if ((level == 0) || ((m_lods[level].range_start +
			get_max_z()) < camera.z))
		{
			bounding_box.merge(box);

			add_patch_to_queue(position, buffer, offset, level,
				max_instances, instance_index);

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
			bounding_box.merge(box);

			add_patch_to_queue(position, buffer, offset, level,
				max_instances, instance_index);

			return;
		}

		patch_offset = patch_size / 2;

		const Uvec2Array4 &quad_order = get_quad_order(
			glm::vec2(position + patch_offset) - glm::vec2(camera));

		for (i = 0; i < 4; ++i)
		{
			select_quads_for_drawing(frustum, camera, position +
				quad_order[i] * patch_offset, buffer, offset,
				out_mask, level - 1, max_instances,
				bounding_box, instance_index);
		}
	}

	void CdLodQuadTree::select_quads_for_drawing(const Frustum &frustum,
		const glm::vec3 &camera,
		const AbstractWriteMemorySharedPtr &buffer,
		const Uint64 offset, const Uint16 max_instances,
		BoundingBox &bounding_box, glm::vec4 &terrain_lod_offset,
		Uint32 &instances) const
	{
		float dist, half_max_z;
		Uint32 x, y, level, step;
		BitSet64 mask;

		if (get_lod_count() == 0)
		{
			bounding_box = BoundingBox();
			instances = 0;

			return;
		}

		level = get_lod_count() - 1;

		step = m_lods[level].patch_scale * get_patch_size();

		mask = frustum.get_planes_mask();

		instances = 0;

		half_max_z = get_max_z() * 0.5f;
		dist = std::abs(camera.z - half_max_z);
		terrain_lod_offset = glm::vec4(-camera.x, -camera.y, 0.0f,
			0.0f);

		if (dist > half_max_z)
		{
			terrain_lod_offset.z = dist - half_max_z;
		}

		terrain_lod_offset.w = get_patch_scale();

		for (y = 0; y < (get_grid_size().y - 1); y += step)
		{
			for (x = 0; x < (get_grid_size().x - 1); x += step)
			{
				select_quads_for_drawing(frustum, camera,
					glm::uvec2(x, y), buffer, offset, mask,
					level, max_instances, bounding_box,
					instances);
			}
		}

		if (instances > max_instances)
		{
			instances = max_instances;
		}
	}

	void CdLodQuadTree::select_bounding_box(const Frustum &frustum,
		const glm::vec3 &camera, const glm::uvec2 &position,
		const BitSet64 mask, const Uint16 level,
		const Uint16 max_instances, BoundingBox &bounding_box,
		Uint32 &instance_index) const
	{
		BoundingBox box;
		glm::vec3 min, max;
		glm::vec2 size, center, half_size, distance;
		glm::uvec2 pos;
		BitSet64 out_mask;
		Uint32 offset, patch_size;
		Uint16 i;
		IntersectionType intersect;

		if (glm::any(glm::greaterThanEqual(position,
			get_grid_size() - 1u)))
		{
			return;
		}

		patch_size = m_lods[level].patch_scale * get_patch_size();

		size = glm::vec2(glm::min(glm::uvec2(patch_size),
			get_grid_size() - position)) * get_patch_scale();
		min = get_patch_scale() * glm::vec3(position, 0.0f);
		max = min + glm::vec3(size, 0.0f);

		pos = position / patch_size;
		min += m_lods[level].min_max[pos.x][pos.y][0];
		max += m_lods[level].min_max[pos.x][pos.y][1];

		min = glm::min(min, max - 0.05f);

		box.set_min_max(min, max);
		box.scale(1.05f);

		intersect = frustum.intersect(box, mask, out_mask);

		if (intersect == it_outside)
		{
			return;
		}

		if ((level == 0) || (intersect == it_inside))
		{
			bounding_box.merge(box);

			return;
		}

		offset = patch_size / 2;

		const Uvec2Array4 &quad_order = get_quad_order(
			glm::vec2(position + offset) - glm::vec2(camera));

		for (i = 0; i < 4; ++i)
		{
			select_bounding_box(frustum, camera, position +
				quad_order[i] * offset, out_mask, level - 1,
				max_instances, bounding_box, instance_index);
		}
	}

	void CdLodQuadTree::select_bounding_box(const Frustum &frustum,
		const glm::vec3 &camera, const Uint16 max_instances,
		BoundingBox &bounding_box) const
	{
		Uint32 x, y, level, step, instances;
		BitSet64 mask;

		if (get_lod_count() == 0)
		{
			bounding_box = BoundingBox();

			return;
		}

		level = get_lod_count() - 1;

		step = m_lods[level].patch_scale * get_patch_size();

		mask = frustum.get_planes_mask();

		instances = 0;

		for (y = 0; y < (get_grid_size().y - 1); y += step)
		{
			for (x = 0; x < (get_grid_size().x - 1); x += step)
			{
				select_bounding_box(frustum, camera,
					glm::uvec2(x, y), mask, level,
					max_instances, bounding_box,
					instances);
			}
		}
	}

}
