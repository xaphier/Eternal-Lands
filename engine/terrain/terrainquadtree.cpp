/****************************************************************************
 *            terrainquadtree.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainquadtree.hpp"
#include "image.hpp"

namespace eternal_lands
{

	TerrainQuadTree::TerrainQuadTree()
	{
	}

	TerrainQuadTree::~TerrainQuadTree() noexcept
	{
	}

	void TerrainQuadTree::init(const ImageSharedPtr &terrain_image)
	{
	}

	void TerrainQuadTree::fill_node(const glm::uvec4 &indices,
		const Uint32 index, const Uint32 count)
	{
		Vec4Array3 min, max;
		glm::vec4 data, value;
		Uint32 i, j;

		for (i = 0; i < count; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				value = m_data[indices[i] + j];
				min[j][i] = std::min(std::min(value[0],
					value[1]), std::min(value[2],
					value[3]));

				value = m_data[indices[i] + j + 3];
				max[j][i] = std::max(std::max(value[0],
					value[1]), std::max(value[2],
					value[3]));
			}
		}

		for (i = 0; i < 3; ++i)
		{
			m_data[index + i] = min[i];
			m_data[index + i + 3] = max[i];
		}

		data.x = count;
		data.y = 0;

		static_cast<glm::uvec4>(m_data[index + 6]) = indices;
		static_cast<glm::uvec4>(m_data[index + 7]) = data;
	}

	void TerrainQuadTree::fill_leaf(const glm::uvec4 &indices,
		const Uint32 index, const Uint32 count, const Uint32 line)
	{
		Vec4Array3 min, max;
		glm::vec4 data, value0, value1, value2, value3, t0, t1;
		Uint32 i, j;

		for (i = 0; i < count; ++i)
		{
			value0 = m_data[indices[i] + 0];
			value1 = m_data[indices[i] + 1];
			value2 = m_data[indices[i] + line + 0];
			value3 = m_data[indices[i] + line + 1];

			t0 = glm::min(glm::min(value0, value1),
				glm::min(value2, value3));
			t1 = glm::max(glm::max(value0, value1),
				glm::max(value2, value3));

			for (j = 0; j < 3; ++j)
			{
				min[j][i] = t0[j];
				max[j][i] = t1[j];
			}
		}

		for (i = count; i < 4; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				min[j][i] = min[j][count];
				max[j][i] = max[j][count];
			}
		}

		for (i = 0; i < 3; ++i)
		{
			m_data[index + i] = min[i];
			m_data[index + i + 3] = max[i];
		}

		data.x = count;
		data.y = 1;

		static_cast<glm::uvec4>(m_data[index + 6]) = indices;
		static_cast<glm::uvec4>(m_data[index + 7]) = data;
	}

	void TerrainQuadTree::fill_leaf(const glm::uvec2 &indices,
		const glm::uvec2 &counts, const Uint32 index,
		const Uint32 line)
	{
		Vec4Array3 min, max;
		glm::vec4 data, value0, value1, value2, value3, t0, t1;
		glm::uvec2 count;
		Uint32 i, j, k, x, y, idx;

		idx = 0;

		for (i = 0; i < (counts.y + 1); ++i)
		{
			for (j = 0; j < (count.x + 1); ++j)
			{
				x = indices.x + j + 0;
				y = indices.y + i + 0;

				value0 =  m_data[x + y * line];

				x = indices.x + j + 1;
				y = indices.y + i + 0;

				value1 =  m_data[x + y * line];

				x = indices.x + j + 0;
				y = indices.y + i + 1;

				value2 =  m_data[x + y * line];

				x = indices.x + j + 1;
				y = indices.y + i + 1;

				value3 =  m_data[x + y * line];

				t0 = glm::min(glm::min(value0, value1),
					glm::min(value2, value3));
				t1 = glm::max(glm::max(value0, value1),
					glm::max(value2, value3));

				for (k = 0; k < 3; ++k)
				{
					min[k][idx] = t0[k];
					max[k][idx] = t1[k];
				}

				idx++;
			}
		}

		for (i = idx; i < 4; ++i)
		{
			for (j = 0; j < 3; ++j)
			{
				min[j][i] = min[j][idx];
				max[j][i] = min[j][idx];
			}
		}

		for (i = 0; i < 3; ++i)
		{
			m_data[index + i] = min[i];
			m_data[index + i + 3] = max[i];
		}

		data.x = idx;
		data.y = 1;

		static_cast<glm::uvec4>(m_data[index + 6]) =
			glm::uvec4(indices, 0, 0);
		static_cast<glm::uvec4>(m_data[index + 7]) = data;
	}

	Uint32 TerrainQuadTree::calculate_data_size(const Uint32 width,
		const Uint32 height)
	{
		Uint32 result, w, h;

		result = width * height;

		w = width - 1;
		h = height - 1;

		while ((w * h) > 1)
		{
			w = (w + 1) / 2;
			h = (h + 1) / 2;

			result += w * h * 8;
		}

		return result + 8;
	}

}
