/****************************************************************************
 *            heightmaptool.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "heightmaptool.hpp"
#include "image.hpp"

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

	class HeightMapTool::Info
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

	HeightMapTool::HeightMapTool(const ImageSharedPtr &height_map,
		const float scale)
	{
		build_data(height_map, scale);
	}

	HeightMapTool::~HeightMapTool() throw()
	{
	}

	void HeightMapTool::build_data(const ImageSharedPtr &height_map,
		const float scale)
	{
		FloatArray8 half_distances;
		BitSet8 dirs;
		glm::ivec2 offset;
		glm::vec2 uv;
		Sint32 width, height, x, y, xx, yy, i;
		float h0, h1;
		bool u, v;

		m_width = height_map->get_width();
		m_height = height_map->get_height();

		width = m_width;
		height = m_height;

		m_uvs.reserve(width * height);
		m_infos.reserve(width * height);
		m_velocities.reserve(width * height);

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				uv.s = static_cast<float>(x + 0.0) /
					static_cast<float>(width - 1);
				uv.t = static_cast<float>(y + 0.0) /
					static_cast<float>(height - 1);

				m_uvs.push_back(uv);
				m_velocities.push_back(glm::vec2(0.0f));

				h0 = height_map->get_pixel(x, y, 0, 0, 0.0f).r
					* scale;

				for (i = 0; i < 8; ++i)
				{
					offset = direction_indices[i];
					xx = x + offset.x;
					yy = y + offset.y;

					if ((xx < 0) || (yy < 0) ||
						(xx > (width - 1)) ||
						(yy > (height - 1)))
					{
						dirs[i] = false;
						half_distances[i] = 0.0f;

						continue;
					}

					h1 = height_map->get_pixel(xx, yy, 0,
						0, 0.0f).r * scale;

					dirs[i] = true;
					half_distances[i] = 0.5f * std::sqrt(
						glm::length2(glm::vec2(offset))
						+ (h0 - h1) * (h0 - h1));
				}

				u = (x == 0) || (x == (width - 1));
				v = (y == 0) || (y == (height - 1));

				m_infos.push_back(Info(half_distances,
					dirs, u, v));
			}
		}
	}

	float HeightMapTool::relax(const InfoVector &infos, const float damping,
		const float clamping, const Uint32 width, Vec2Vector &uvs,
		Vec2Vector &velocities)
	{
		glm::vec2 uv, dir;
		float distance, factor, result;
		Uint32 i, j, index, count;

		result = 0.0f;
		count = infos.size();

		for (i = 0; i < count; ++i)
		{
			uv = uvs[i];

			for (j = 0; j < 8; ++j)
			{
				if (infos[i].get_dir(j))
				{
					index = i + direction_indices[j].x +
						direction_indices[j].y * width;
					dir = uvs[index] - uv;
					distance = glm::length(dir);
					dir /= std::max(distance, 1e-7f);
					factor = infos[i].get_half_distance(j)
						- distance;
					dir *= factor;
					velocities[i] += dir;
				}
			}
		}

 		for (i = 0; i < count; ++i)
		{
			uv = velocities[i] * damping;
			uv = glm::clamp(uv, -clamping, clamping);
			result += glm::length2(uv);
			uv += uvs[i];
			uv = glm::clamp(uv, 0.0f, 1.0f);		

			if (infos[i].get_u())
			{
				uvs[i].s = uv.s;
			}

			if (infos[i].get_v())
			{
				uvs[i].t = uv.t;
			}
		}

		return result;
 	}

	void HeightMapTool::buil_relaxed_uv()
	{
		Uint32 increasing;
		float movement, last;

		last = relax(m_infos, 0.2f, 0.00015f, m_width, m_uvs,
			m_velocities);
		increasing = 0;

		while (true)
		{
			movement = relax(m_infos, 0.2f, 0.00015f, m_width,
				m_uvs, m_velocities);

			if (movement >= last)
			{
				increasing += 1;
			}

			if (movement < 0.0001f)
			{
				break;
			}

			if (increasing >= 100)
			{
				break;
			}

			last = movement;
		}
	}

}
