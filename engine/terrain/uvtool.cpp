/****************************************************************************
 *            uvtool.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "uvtool.hpp"
#include "image.hpp"
#include "abstractterrain.hpp"
#include "abstractprogress.hpp"
#include "simd/simd.hpp"

namespace eternal_lands
{

	namespace
	{

		/* For SSE2 usage */
		boost::array<glm::ivec2, 8> direction_indices = {
			{
				glm::ivec2(-1, -1), glm::ivec2( 0, -1),
				glm::ivec2(-1,  1), glm::ivec2( 0,  1),
				glm::ivec2( 1, -1), glm::ivec2( 1,  1),
				glm::ivec2(-1,  0), glm::ivec2( 1,  0)
			}
		};

	}

	UvTool::UvTool(const ImageSharedPtr &displacement_map):
		m_displacement_map(displacement_map),
		m_width(displacement_map->get_width()),
		m_height(displacement_map->get_height())
	{
		build_uv();
		build_half_size();
	}

	UvTool::~UvTool() noexcept
	{
	}

	void UvTool::build_half_size(const Sint32 x, const Sint32 y)
	{
		Vec4Array2 half_distances;
		glm::vec3 p0, p1;
		glm::ivec2 offset;
		Sint32 width, height, xx, yy, i;
		Uint32 index;

		width = m_width;
		height = m_height;

		p0 = glm::vec3(glm::vec2(x, y) *
			AbstractTerrain::get_patch_scale(), 0.0f);
		p0 += AbstractTerrain::get_offset_scaled_0_1(
			m_displacement_map->get_pixel(x, y, 0, 0, 0));

		index = (x + y * m_width) * 2;

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

			p1 = glm::vec3(glm::vec2(xx, yy) *
				AbstractTerrain::get_patch_scale(), 0.0f);
			p1 += AbstractTerrain::get_offset_scaled_0_1(
				m_displacement_map->get_pixel(xx, yy, 0, 0, 0));

			half_distances[i / 4][i % 4] = 0.5f *
				glm::distance(p0, p1);
		}

		m_half_distances[index + 0] = half_distances[0];
		m_half_distances[index + 1] = half_distances[1];
	}

	void UvTool::build_half_size()
	{
		Uint32 x, y;

		m_half_distances.resize(m_width * m_height * 2);

		#pragma omp parallel for private(x)
		for (y = 0; y < m_height; ++y)
		{
			for (x = 0; x < m_width; ++x)
			{
				build_half_size(x, y);
			}
		}
	}

	void UvTool::build_uv()
	{
		glm::vec2 uv;
		Uint32 x, y;

		m_uvs.resize(m_width * m_height);

		for (y = 0; y < m_height; ++y)
		{
			for (x = 0; x < m_width; ++x)
			{
				uv = glm::vec2(x, y);
				uv *= AbstractTerrain::get_patch_scale();

				m_uvs[x + y * m_width] = uv;
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
			factor = distance - get_half_distance(half_distances,
				index, i);
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

		if ((position.x > 0) && (position.x < (size.x - 1)))
		{
			uv.s += offset.s;
		}

		if ((position.y > 0) && (position.y < (size.y - 1)))
		{
			uv.t += offset.t;
		}

		new_uvs[index] = uv;
	}

	void UvTool::relax_default(
		const AlignedVec4Array &half_distances, const Vec2Vector &uvs,
		const float damping, const float clamping, const Uint32 width,
		const Uint32 height, Vec2Vector &new_uvs)
	{
		glm::uvec2 size;
		Uint32 i, j;

		size = glm::uvec2(width, height);

		#pragma omp parallel for
		for (i = 0; i < width; ++i)
		{
			glm::uvec2 position;

			position.x = i;
			position.y = 0;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);
		}

		#pragma omp parallel for private(i)
		for (j = 1; j < (height - 1); ++j)
		{
			glm::uvec2 position;

			position.x = 0;
			position.y = j;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);

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

		#pragma omp parallel for
		for (i = 0; i < width; ++i)
		{
			glm::uvec2 position;

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
		glm::uvec2 size;
		Uint32 i;

		size = glm::uvec2(width, height);

		#pragma omp parallel for
		for (i = 0; i < width; ++i)
		{
			glm::uvec2 position;

			position.x = i;
			position.y = 0;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);
		}

		#pragma omp parallel for
		for (i = 1; i < (height - 1); ++i)
		{
			glm::uvec2 position;

			SIMD::relax_uv_line(glm::value_ptr(uvs[i * width]),
				half_distances.get_ptr_at(i * width * 2),
				damping, clamping, width,
				glm::value_ptr(new_uvs[i * width]));

			position.x = 0;
			position.y = i;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);

			position.x = width - 1;
			position.y = i;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);
		}

		#pragma omp parallel for
		for (i = 0; i < width; ++i)
		{
			glm::uvec2 position;

			position.x = i;
			position.y = height - 1;

			relax_edge(half_distances, uvs, position, size,
				damping, clamping, new_uvs);
		}
	}

	void UvTool::relax_uv(const AbstractProgressSharedPtr &progress,
		const Uint16 count, const bool use_simd)
	{
		Vec2Vector new_uvs;
		Uint16 i;

		build_half_size();

		new_uvs.resize(m_uvs.size());

		progress->set_text(String(UTF8("relax uv")));
		progress->init(0, count / 100);

		if (progress->get_canceled())
		{
			return;
		}

		if (use_simd)
		{
			for (i = 0; i < count; ++i)
			{
				relax_sse2(m_half_distances, m_uvs, 0.05f,
					AbstractTerrain::get_patch_scale(),
					m_width, m_height, new_uvs);

				std::swap(new_uvs, m_uvs);

				if ((i % 100) == 0)
				{
					if (progress->get_canceled())
					{
						return;
					}

					progress->stepp(1);
				}
			}

			return;
		}

		for (i = 0; i < count; ++i)
		{
			relax_default(m_half_distances, m_uvs, 0.05f,
				AbstractTerrain::get_patch_scale(),
				m_width, m_height, new_uvs);

			std::swap(new_uvs, m_uvs);

			if ((i % 100) == 0)
			{
				if (progress->get_canceled())
				{
					return;
				}

				progress->stepp(1);
			}
		}
	}

	/**
	 * Converts to image
	 */
	void UvTool::convert(ImageSharedPtr &dudv_map,
		glm::vec4 &dudv_scale_offset) const
	{
		glm::vec2 uv, min, max, tmp, diff, scale;
		Sint32 width, height, x, y, index;

		width = m_width;
		height = m_height;
		index = 0;

		CECK_TABLE_SIZES_EQUAL(dudv_map->get_size(),
			glm::uvec3(m_width, m_height, 0),
			UTF8("Image has wrong size"));

		min = glm::vec2(std::numeric_limits<float>::max());
		max = glm::vec2(-std::numeric_limits<float>::max());

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				uv.s = x;
				uv.t = y;

				uv *= AbstractTerrain::get_patch_scale();

				tmp = m_uvs[index] - uv;
				min = glm::min(min, tmp);
				max = glm::max(max, tmp);

				++index;
			}
		}

		diff = glm::max(max - min, 0.001f);

		dudv_scale_offset.x = diff.x;
		dudv_scale_offset.y = diff.y;
		dudv_scale_offset.z = min.x;
		dudv_scale_offset.w = min.y;

		scale = 1.0f / diff;

		index = 0;

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				uv.s = x;
				uv.t = y;

				uv *= AbstractTerrain::get_patch_scale();

				tmp = m_uvs[index] - uv;
				tmp -= min;
				tmp *= scale;

				dudv_map->set_pixel(x, y, 0, 0, 0,
					glm::vec4(tmp, 0.0f, 1.0f));

				++index;
			}
		}
	}

	/**
	 * Converts to unsigned rg 16 bit image
	 */
	ImageSharedPtr UvTool::convert(glm::vec4 &dudv_scale_offset) const
	{
		ImageSharedPtr dudv_map;

		dudv_map = boost::make_shared<Image>(String(UTF8("Dudv map")),
			false, tft_rg16, glm::uvec3(m_width, m_height, 0),
			0, false);			

		convert(dudv_map, dudv_scale_offset);

		return dudv_map;
	}

	/**
	 * Imports from unsigned rg 16 bit image
	 */
	void UvTool::import(const ImageSharedPtr &dudv_map,
		const glm::vec4 &dudv_scale_offset)
	{
		glm::vec2 uv, min, tmp, diff;
		Sint32 width, height, x, y, index;

		width = m_width;
		height = m_height;
		index = 0;

		CECK_TABLE_SIZES_EQUAL(dudv_map->get_size(),
			glm::uvec3(m_width, m_height, 0),
			UTF8("Image has wrong size"));

		diff.x = dudv_scale_offset.x;
		diff.y = dudv_scale_offset.y;
		min.x = dudv_scale_offset.z;
		min.y = dudv_scale_offset.w;

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				uv.s = x;
				uv.t = y;

				uv *= AbstractTerrain::get_patch_scale();

				tmp = glm::vec2(dudv_map->get_pixel(x, y, 0,
					0, 0));

				tmp = tmp * diff + min;
				m_uvs[index] = uv + tmp;

				++index;
			}
		}
	}

}
