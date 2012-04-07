/****************************************************************************
 *            heightmapuvtool.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "heightmapuvtool.hpp"
#include "image.hpp"
#include "abstractterrainmanager.hpp"

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

	class HeightMapUvTool::Info
	{
		private:
			FloatArray8 m_half_distances;
			BitSet8 m_dirs;
			bool m_u;
			bool m_v;

		public:
			Info(const FloatArray8 &half_distances,
				const BitSet8 &dirs, const bool u,
				const bool v);
			~Info() throw();

			inline float get_half_distance(const Uint16 index)
				const
			{
				return m_half_distances[index];
			}

			inline bool get_dir(const Uint16 index) const
			{
				return m_dirs[index];
			}

			inline bool get_u() const
			{
				return m_u;
			}

			inline bool get_v() const
			{
				return m_v;
			}

	};

	HeightMapUvTool::Info::Info(const FloatArray8 &half_distances,
		const BitSet8 &dirs, const bool u, const bool v):
		m_half_distances(half_distances), m_dirs(dirs), m_u(u), m_v(v)
	{
	}

	HeightMapUvTool::Info::~Info() throw()
	{
	}

	HeightMapUvTool::HeightMapUvTool(const ImageSharedPtr &height_map,
		const glm::vec3 &offset_scale)
	{
		build_data(height_map, offset_scale);
	}

	HeightMapUvTool::~HeightMapUvTool() throw()
	{
	}

	void HeightMapUvTool::build_data(const ImageSharedPtr &height_map,
		const glm::vec3 &offset_scale, const Sint32 x, const Sint32 y)
	{
		FloatArray8 half_distances;
		BitSet8 dirs;
		glm::vec3 p0, p1;
		glm::ivec2 offset;
		glm::vec2 uv;
		Sint32 width, height, xx, yy, i;
		bool u, v;

		width = m_width;
		height = m_height;

		uv.s = x;
		uv.t = y;

		m_uvs.push_back(uv);

		p0 = AbstractTerrainManager::get_terrain_offset(
			height_map->get_pixel_uint(x, y, 0, 0, 0.0f),
			offset_scale);

		for (i = 0; i < 8; ++i)
		{
			offset = direction_indices[i];
			xx = x + offset.x;
			yy = y + offset.y;

			if ((xx < 0) || (yy < 0) || (xx > (width - 1)) ||
				(yy > (height - 1)))
			{
				dirs[i] = false;
				half_distances[i] = 0.0f;

				continue;
			}

			p1 = AbstractTerrainManager::get_terrain_offset(
				height_map->get_pixel_uint(xx, yy, 0, 0, 0.0f),
				offset_scale);

			dirs[i] = true;
			half_distances[i] = 0.5f * glm::distance(p0, p1);
		}

		u = (x == 0) || (x == (width - 1));
		v = (y == 0) || (y == (height - 1));

		m_infos.push_back(Info(half_distances, dirs, u, v));
	}

	void HeightMapUvTool::build_data(const ImageSharedPtr &height_map,
		const glm::vec3 &offset_scale)
	{
		Sint32 width, height, x, y;

		m_width = height_map->get_width();
		m_height = height_map->get_height();

		width = m_width;
		height = m_height;

		m_uvs.reserve(width * height);
		m_infos.reserve(width * height);

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				build_data(height_map, offset_scale, x, y);
			}
		}
	}

	float HeightMapUvTool::relax(const InfoVector &infos,
		const float damping, const float clamping, const Uint32 width,
		Vec2Vector &uvs)
	{
		glm::vec2 uv, dir, velocity, offset;
		float distance, factor, result;
		Uint32 i, j, count, index;

		result = 0.0f;
		count = infos.size();

		for (i = 0; i < count; ++i)
		{
			uv = uvs[i];
			velocity = glm::vec2(0.0f);

			for (j = 0; j < 8; ++j)
			{
				if (infos[i].get_dir(j))
				{
					index = i + direction_indices[j].x +
						direction_indices[j].y * width;
					dir = uvs[index] - uv;
					distance = glm::length(dir);
					dir /= std::max(distance, 1e-7f);
					factor = distance -
						infos[i].get_half_distance(j);
					velocity += dir * factor;
				}
			}

			offset = velocity * damping;
			offset = glm::clamp(offset, -clamping, clamping);
			result += glm::length2(offset);

			uv += offset;

			if (!infos[i].get_u())
			{
				uvs[i].s = uv.s;
			}

			if (!infos[i].get_v())
			{
				uvs[i].t = uv.t;
			}
		}

		return 1.0f;
 	}

	void HeightMapUvTool::buil_relaxed_uv()
	{
		Uint32 increasing;
		float movement, last;

		last = relax(m_infos, 0.05f, 10.0f, m_width, m_uvs);

		increasing = 0;

		while (true)
		{
			movement = relax(m_infos, 0.05f, 10.0f, m_width,
				m_uvs);

			if (movement >= last)
			{
				increasing += 1;
			}

			if (movement < 0.0001f)
			{
				break;
			}

			if (increasing >= 250)
			{
				break;
			}

			last = movement;
		}
	}

	/**
	 * Converts to signed rg 8 but image
	 */
	void HeightMapUvTool::convert()
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
