/****************************************************************************
 *            cpurasterizer.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "cpurasterizer.hpp"
#include "submesh.hpp"
#include "alignedshort8array.hpp"
#include "simd/simd.hpp"

namespace eternal_lands
{

	CpuRasterizer::CpuRasterizer(const Uint32 width, const Uint32 height,
		const float threshold, const bool use_simd): m_width(width),
		m_height(height), m_threshold(threshold), m_use_simd(use_simd)
	{
	}

	CpuRasterizer::~CpuRasterizer() noexcept
	{
	}

	void CpuRasterizer::build_min_max_boxes(const SubMeshVector &sub_meshs,
		AlignedShort8Array &min_max_boxes)
	{
		glm::ivec4 min, max;
		Uint32 index;

		min_max_boxes.resize(sub_meshs.size());

		index = 0;

		BOOST_FOREACH(const SubMesh &sub_mesh, sub_meshs)
		{
			min = glm::ivec4(sub_mesh.get_bounding_box().get_min()
				* get_min_max_box_inverse_scale(), 1);
			max = glm::ivec4(sub_mesh.get_bounding_box().get_max()
				* get_min_max_box_inverse_scale(), 1);

			assert(min.x <= max.x);
			assert(min.y <= max.y);
			assert(min.z <= max.z);
			assert(min.w == 1);
			assert(max.w == 1);

			min_max_boxes.set_value_low(min, index);
			min_max_boxes.set_value_high(max, index);

			index++;
		}
	}

	void CpuRasterizer::append_min_max_box(const BoundingBox &bounding_box,
		AlignedShort8Array &min_max_boxes)
	{
		glm::ivec4 min, max;

		min = glm::ivec4(bounding_box.get_min() *
			get_min_max_box_inverse_scale(), 1);
		max = glm::ivec4(bounding_box.get_max() *
			get_min_max_box_inverse_scale(), 1);

		assert(min.x <= max.x);
		assert(min.y <= max.y);
		assert(min.z <= max.z);
		assert(min.w == 1);
		assert(max.w == 1);

		min_max_boxes.push_back(min, max);
	}

	bool CpuRasterizer::check_visibility(const glm::mat4x4 &matrix,
		const glm::ivec4 &min, const glm::ivec4 &max) const
	{
		glm::vec4 value;
		glm::vec2 vmin, vmax, tmp, size;

		value = glm::vec4(min.x, min.y, min.z, 1.0f);
		value = matrix * value;
		tmp = glm::vec2(value) / value.w;

		vmin = tmp;
		vmax = tmp;

		value = glm::vec4(max.x, min.y, min.z, 1.0f);
		value = matrix * value;
		tmp = glm::vec2(value) / value.w;

		vmin = glm::min(vmin, tmp);
		vmax = glm::max(vmax, tmp);

		value = glm::vec4(min.x, max.y, min.z, 1.0f);
		value = matrix * value;
		tmp = glm::vec2(value) / value.w;

		vmin = glm::min(vmin, tmp);
		vmax = glm::max(vmax, tmp);

		value = glm::vec4(max.x, max.y, min.z, 1.0f);
		value = matrix * value;
		tmp = glm::vec2(value) / value.w;

		vmin = glm::min(vmin, tmp);
		vmax = glm::max(vmax, tmp);

		value = glm::vec4(min.x, min.y, max.z, 1.0f);
		value = matrix * value;
		tmp = glm::vec2(value) / value.w;

		vmin = glm::min(vmin, tmp);
		vmax = glm::max(vmax, tmp);

		value = glm::vec4(max.x, min.y, max.z, 1.0f);
		value = matrix * value;
		tmp = glm::vec2(value) / value.w;

		vmin = glm::min(vmin, tmp);
		vmax = glm::max(vmax, tmp);

		value = glm::vec4(min.x, max.y, max.z, 1.0f);
		value = matrix * value;
		tmp = glm::vec2(value) / value.w;

		vmin = glm::min(vmin, tmp);
		vmax = glm::max(vmax, tmp);

		value = glm::vec4(max.x, max.y, max.z, 1.0f);
		value = matrix * value;
		tmp = glm::vec2(value) / value.w;

		vmin = glm::min(vmin, tmp);
		vmax = glm::max(vmax, tmp);

		vmin = glm::min(vmin, glm::vec2(1.0f));
		vmax = glm::min(vmax, glm::vec2(1.0f));

		vmin = glm::max(vmin, glm::vec2(-1.0f));
		vmax = glm::max(vmax, glm::vec2(-1.0f));

		size = vmax - vmin;

		size *= glm::vec2(get_width(), get_height());

		/**
		 * Scaled with four, because we do not map the range to
		 * (0 .. view_x) x (0 .. view_y).
		 * We use (-view_x .. view_x) x (-view_y .. view_y),
		 * so coverage values are four times bigger.
		 */
		return (size.x * size.y) > (get_threshold() * 4.0f);
	}

	BitSet64 CpuRasterizer::check_visibility(
		const glm::mat4x4 &projection_view_matrix,
		const glm::mat4x3 &world_matrix,
		const AlignedShort8Array &min_max_boxes) const
	{
		glm::mat4x4 matrix;
		BitSet64 result;
		Uint32 i, count;

		matrix = glm::mat4x4(world_matrix);

		matrix[0][0] *= get_min_max_box_scale();
		matrix[0][1] *= get_min_max_box_scale();
		matrix[0][2] *= get_min_max_box_scale();
		matrix[1][0] *= get_min_max_box_scale();
		matrix[1][1] *= get_min_max_box_scale();
		matrix[1][2] *= get_min_max_box_scale();
		matrix[2][0] *= get_min_max_box_scale();
		matrix[2][1] *= get_min_max_box_scale();
		matrix[2][2] *= get_min_max_box_scale();

		matrix = projection_view_matrix * matrix;

		count = std::min(min_max_boxes.size(), 32u);

		if (count == 0)
		{
			return 0xFFFFFFFF;
		}

		if (get_use_simd())
		{
			return SIMD::check_coverage_simple(
				glm::value_ptr(glm::transpose(matrix)),
				min_max_boxes.get_ptr_at(0), count,
				get_width(), get_height(), get_threshold());
		}

		for (i = 0; i < count; ++i)
		{
			result[i] = check_visibility(matrix,
				min_max_boxes.get_value_low(i),
				min_max_boxes.get_value_high(i));
		}

		return result;
	}

	BitSet64 CpuRasterizer::check_visibility(
		const glm::mat4x4 &projection_view_matrix,
		const glm::mat4x3 &world_matrix,
		const AlignedShort8Array &min_max_boxes,
		const SubMeshVector &sub_meshs) const
	{
		glm::mat4x4 matrix;
		BitSet64 result;
		Uint32 i, j, index, size, count;

		matrix = glm::mat4x4(world_matrix);

		matrix[0][0] *= get_min_max_box_scale();
		matrix[0][1] *= get_min_max_box_scale();
		matrix[0][2] *= get_min_max_box_scale();
		matrix[1][0] *= get_min_max_box_scale();
		matrix[1][1] *= get_min_max_box_scale();
		matrix[1][2] *= get_min_max_box_scale();
		matrix[2][0] *= get_min_max_box_scale();
		matrix[2][1] *= get_min_max_box_scale();
		matrix[2][2] *= get_min_max_box_scale();

		matrix = projection_view_matrix * matrix;

		size = sub_meshs.size();
		size = std::min(size, 32u);

		if (get_use_simd())
		{
			for (i = 0; i < size; ++i)
			{
				count = sub_meshs[i].get_min_max_boxes_count();
				index = sub_meshs[i].get_min_max_boxes_index();

				assert((index + count) <= min_max_boxes.size());

				result[i] = true;

				if (count > 0)
				{
					result[i] = SIMD::check_coverage(
						glm::value_ptr(matrix),
						min_max_boxes.get_ptr_at(index),
						count, get_width(),
						get_height(), get_threshold());
				}
			}

			return result;
		}

		for (i = 0; i < size; ++i)
		{
			count = sub_meshs[i].get_min_max_boxes_count();
			index = sub_meshs[i].get_min_max_boxes_index();

			assert((index + count) <= min_max_boxes.size());

			result[i] = count == 0;

			for (j = 0; j < count; ++j)
			{
				if (check_visibility(matrix,
					min_max_boxes.get_value_low(index + j),
					min_max_boxes.get_value_high(
						index + j)))
				{
					result[i] = true;

					break;
				}
			}
		}

		return result;
	}

}
