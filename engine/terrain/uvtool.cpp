/****************************************************************************
 *            uvtool.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uvtool.hpp"
#include "image.hpp"
#include "abstractterrainmanager.hpp"
#include "simd/simd.hpp"

namespace eternal_lands
{

	namespace
	{

		boost::array<glm::ivec2, 8> direction_indices = {
			{
				glm::ivec2(-1, -1), glm::ivec2( 0, -1),
				glm::ivec2( 1, -1), glm::ivec2(-1,  0),
				glm::ivec2( 1,  0), glm::ivec2(-1,  1),
				glm::ivec2( 0,  1), glm::ivec2( 1,  1)
			}
		};

	}

	UvTool::UvTool(const ImageSharedPtr &vector_map,
		const glm::vec3 &offset_scale, const glm::vec2 &position_scale)
	{
		build_data(vector_map, offset_scale, position_scale);
	}

	UvTool::~UvTool() noexcept
	{
	}

	void UvTool::build_data(const ImageSharedPtr &vector_map,
		const glm::vec3 &offset_scale, const glm::vec2 &position_scale,
		const Sint32 x, const Sint32 y)
	{
		Vec4Array2 half_distances;
		glm::vec3 p0, p1;
		glm::ivec2 offset;
		glm::vec2 uv;
		Sint32 width, height, xx, yy, i;

		width = m_width;
		height = m_height;

		uv = glm::vec2(x, y) * position_scale;

		m_uvs.push_back(uv);

		p0 = glm::vec3(glm::vec2(x, y) * position_scale, 0.0f);
		p0 += glm::vec3(vector_map->get_pixel(x, y, 0, 0, 0.0f)) *
			offset_scale;

		for (i = 0; i < 8; ++i)
		{
			offset = direction_indices[i];
			xx = x + offset.x;
			yy = y + offset.y;

			if ((xx < 0) || (yy < 0) || (xx > (width - 1)) ||
				(yy > (height - 1)))
			{
				half_distances[i / 4][i % 4] = 0.0f;

				continue;
			}

			p1 = glm::vec3(glm::vec2(xx, yy) * position_scale,
				0.0f);
			p1 += glm::vec3(vector_map->get_pixel(xx, yy, 0, 0,
				0.0f)) * offset_scale;

			half_distances[i / 4][i % 4] = 0.5f *
				glm::distance(p0, p1);
		}

		m_half_distances.push_back(half_distances[0]);
		m_half_distances.push_back(half_distances[1]);
	}

	void UvTool::build_data(const ImageSharedPtr &vector_map,
		const glm::vec3 &offset_scale, const glm::vec2 &position_scale)
	{
		Sint32 width, height, x, y;

		m_width = vector_map->get_width();
		m_height = vector_map->get_height();

		width = m_width;
		height = m_height;

		m_uvs.reserve(width * height);
		m_half_distances.reserve(width * height * 2);

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				build_data(vector_map, offset_scale,
					position_scale, x, y);
			}
		}
	}

	void UvTool::relax(const AlignedVec4Array &half_distances,
		const Vec2Vector &uvs, const float damping,
		const float clamping, const Uint32 width, const Uint32 index,
		Vec2Vector &new_uvs)
	{
		glm::vec2 uv, dir, velocity, offset;
		float distance, factor;
		Uint32 i, idx;

		uv = uvs[index];
		velocity = glm::vec2(0.0f);

		for (i = 0; i < 8; ++i)
		{
			idx = index + direction_indices[i].x +
				direction_indices[i].y * width;
			dir = uvs[idx] - uv;
			distance = glm::length(dir);
			dir /= std::max(distance, 1e-7f);
			factor = distance - half_distances[i / 4][i % 4];
			velocity += dir * factor;
		}

		offset = velocity * damping;
		offset = glm::clamp(offset, -clamping, clamping);

		uv += offset;

		new_uvs[index] = uv;
	}

	void UvTool::relax_edge(
		const AlignedVec4Array &half_distances, const Vec2Vector &uvs,
		const glm::uvec2 &position, const glm::uvec2 &size,
		const float damping, const float clamping, Vec2Vector &new_uvs)
	{
		glm::vec2 uv, dir, velocity, offset;
		glm::ivec2 pos;
		float distance, factor;
		Uint32 i, index, idx;

		index = position.x + position.y * size.x;

		uv = uvs[index];
		velocity = glm::vec2(0.0f);

		for (i = 0; i < 8; ++i)
		{
			pos = glm::ivec2(position) + direction_indices[i];

			if (glm::all(glm::greaterThanEqual(pos, glm::ivec2(0)))
				&&
				glm::all(glm::lessThan(pos, glm::ivec2(size))))
			{
				idx = pos.x + pos.y * size.x;
				dir = uvs[idx] - uv;
				distance = glm::length(dir);
				dir /= std::max(distance, 1e-7f);
				factor = distance - get_half_distance(
					half_distances, index, i);
				velocity += dir * factor;
			}
		}

		offset = velocity * damping;
		offset = glm::clamp(offset, -clamping, clamping);

		uv += offset;

		if (!((position.x == 0) || (position.x == (size.x - 1))))
		{
			new_uvs[index].s = uv.s;
		}

		if (!((position.y == 0) || (position.y == (size.y - 1))))
		{
			new_uvs[index].t = uv.t;
		}
	}

	void UvTool::relax_default(
		const AlignedVec4Array &half_distances, const Vec2Vector &uvs,
		const float damping, const float clamping, const Uint32 width,
		const Uint32 height, Vec2Vector &new_uvs)
	{
		glm::uvec2 position, size;
		Uint32 i, j;

		size = glm::uvec2(width, height);
		position = glm::uvec2(0);

		for (i = 0; i < width; ++i)
		{
			position.x = i;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);
		}

		for (j = 1; j < (height - 1); ++j)
		{
			position.x = 0;
			position.y = j;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);

			#pragma omp parallel for
			for (i = 1; i < (width - 1); ++i)
			{
				relax(half_distances, uvs, damping, clamping,
					width, i + j * width, new_uvs);
			}

			position.x = width - 1;
			position.y = j;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);
		}

		for (i = 0; i < width; ++i)
		{
			position.x = i;
			position.y = height - 1;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);
		}
 	}

	void UvTool::relax_sse2(
		const AlignedVec4Array &half_distances, const Vec2Vector &uvs,
		const float damping, const float clamping, const Uint32 width,
		const Uint32 height, Vec2Vector &new_uvs)
	{
		glm::uvec2 position, size;
		Uint32 i;

		size = glm::uvec2(width, height);
		position = glm::uvec2(0);

		for (i = 0; i < width; ++i)
		{
			position.x = i;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);
		}

		#pragma omp parallel for
		for (i = 1; i < (height - 1); ++i)
		{
			SIMD::relax_uv_line(glm::value_ptr(uvs[i * width]),
				half_distances.get_ptr_at(i * width * 2),
				damping, clamping, width,
				glm::value_ptr(new_uvs[i * width]));
		}

		for (i = 0; i < width; ++i)
		{
			position.x = i;
			position.y = height - 1;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);
		}
	}

	void UvTool::relaxed_uv(const bool use_simd)
	{
		Vec2Vector new_uvs;
		Uint32 i;

		new_uvs.resize(m_uvs.size());

		if (use_simd)
		{
			for (i = 0; i < 512; ++i)
			{
				relax_sse2(m_half_distances, m_uvs, 0.015f,
					1.0f, m_width, m_height, new_uvs);

				std::swap(new_uvs, m_uvs);
			}

			return;
		}

		for (i = 0; i < 512; ++i)
		{
			relax_default(m_half_distances, m_uvs, 0.015f, 1.0f,
				m_width, m_height, new_uvs);

			std::swap(new_uvs, m_uvs);
		}
	}

	/**
	 * Converts to signed rg 8 but image
	 */
	void UvTool::convert()
	{
		glm::vec2 uv, max, tmp, diff;
		glm::ivec2 temp;
		Sint32 width, height, x, y, index;

		width = m_width;
		height = m_height;
		index = 0;

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				uv.s = x;
				uv.t = y;

				tmp = m_uvs[index] - uv;
				max = glm::max(max, tmp);
				max = glm::max(max, glm::abs(tmp));

				++index;
			}
		}

		index = 0;

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				uv.s = x;
				uv.t = y;

				tmp = m_uvs[index] - uv;
				tmp /= max;

				if (tmp.s < 0.0f)
				{
					temp.s = tmp.s * 128.0f - 0.5f;
					tmp.s = std::max(temp.s, -128) / 128.0f;
				}
				else
				{
					temp.s = tmp.s * 127.0f + 0.5f;
					tmp.s = std::min(temp.s, 128) / 127.0f;
				}

				if (tmp.t < 0.0f)
				{
					temp.t = tmp.t * 128.0f - 0.5f;
					tmp.t = std::max(temp.t, -128) / 128.0f;
				}
				else
				{
					temp.t = tmp.t * 127.0f + 0.5f;
					tmp.t = std::min(temp.t, 128) / 127.0f;
				}

				diff = glm::max(diff, glm::abs(m_uvs[index] -
					(uv + tmp * max))); 
				m_uvs[index] = uv + tmp * max;

				++index;
			}
		}
	}

}
