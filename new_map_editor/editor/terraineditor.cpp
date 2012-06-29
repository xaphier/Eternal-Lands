/****************************************************************************
 *            terraineditor.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terraineditor.hpp"
#include "image.hpp"
#include "abstractterrainmanager.hpp"

namespace eternal_lands
{

	namespace
	{
		
		String tmp;

	}

	TerrainEditor::TerrainEditor()
	{
		glm::uvec2 value;
		Uint32 i, j, index;

		for (i = 0; i < 256; ++i)
		{
			for (j = 0; j < 256; ++j)
			{
				value.x = j;
				value.y = i;

				index = value.x + value.y * 256;

				m_normals[index] =
					PackTool::decode_normal_optimized_uint8(
						value);
			}
		}

	}

	TerrainEditor::~TerrainEditor() noexcept
	{
	}

	void TerrainEditor::set_terrain_values(
		const TerrainValueVector &terrain_values)
	{
		Ivec2Set indices;
		glm::ivec2 index, min, max;
		Sint32 x, y;

		min = glm::ivec2(0);
		max = glm::ivec2(m_terrain_vector_image->get_width(),
			m_terrain_vector_image->get_height()) - 1;

		BOOST_FOREACH(const TerrainValue &terrain_value, terrain_values)
		{
			m_terrain_vector_image->set_pixel_packed_uint32(
				terrain_value.get_x(), terrain_value.get_y(),
				0, 0, 0, terrain_value.get_packed_value());

			for (y = -1; y < 2; ++y)
			{
				for (x = -1; x < 2; ++x)
				{
					index.x = terrain_value.get_x();
					index.y = terrain_value.get_y();
					index.x += x;
					index.y += y;

					index = glm::min(index, max);
					index = glm::max(index, min);

					indices.insert(index);
				}
			}
		}

		update_normals(indices);
	}

	void TerrainEditor::set_terrain_blend_map(const String &name,
		const Uint16 index)
	{
	}

	void TerrainEditor::set_terrain_vector_map(const String &name)
	{
	}

	void TerrainEditor::set_terrain_dudv_map(const String &name)
	{
	}

	const String &TerrainEditor::get_terrain_albedo_map(const Uint16 index)
		const
	{
		return tmp;
	}

	const String &TerrainEditor::get_terrain_blend_map(const Uint16 index)
		const
	{
		return tmp;
	}

	const String &TerrainEditor::get_terrain_vector_map() const
	{
		return tmp;
	}

	const String &TerrainEditor::get_terrain_dudv_map() const
	{
		return tmp;
	}

	void TerrainEditor::get_terrain_values(const glm::uvec2 &vertex,
		const glm::vec2 &size, const float attenuation_size,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, TerrainValueVector &terrain_values)
		const
	{
		glm::vec2 center, position, max_size;
		Sint64 temp;
		Uint32 x, y;
		Uint32 min_x, min_y, max_x, max_y;
		float distance, tmp;

		max_size = size + attenuation_size;

		tmp = static_cast<float>(vertex.x) - max_size.x;
		min_x = std::max(static_cast<Sint64>(tmp),
			static_cast<Sint64>(0));

		tmp = static_cast<float>(vertex.y) - max_size.y;
		min_y = std::max(static_cast<Sint64>(tmp),
			static_cast<Sint64>(0));

		tmp = static_cast<float>(vertex.x) + max_size.x;
		temp = m_terrain_vector_image->get_width();
		max_x = std::min(static_cast<Sint64>(tmp),
			static_cast<Sint64>(temp - 1));

		tmp = static_cast<float>(vertex.y) + max_size.y;
		temp = m_terrain_vector_image->get_height();
		max_y = std::min(static_cast<Sint64>(tmp),
			static_cast<Sint64>(temp - 1));

		terrain_values.clear();

		center = glm::vec2(vertex[0], vertex[1]);

		for (x = min_x; x <= max_x; x++)
		{
			for (y = min_y; y <= max_y; y++)
			{
				position = glm::vec2(x, y);

				distance = calc_distance(center, position,
					size, shape);

				if (calc_attenuation(distance,
					attenuation_size, attenuation) > 0.0f)
				{
					TerrainValue value(x, y);

					value.set_packed_value(
						m_terrain_vector_image->
							get_pixel_packed_uint32(
								x, y, 0, 0, 0));

					terrain_values.push_back(value);
				}
			}
		}
	}

	float TerrainEditor::calc_distance(const glm::vec2 &center,
		const glm::vec2 &position, const glm::vec2& size,
		const BrushShapeType shape)
	{
		glm::vec2 distance;

		switch (shape)
		{
			case bst_circle:
				return std::max(glm::distance(center, position)
					- size.x, 0.0f);
			case bst_rect:
				distance = glm::max(glm::abs(center - position)
					- size, glm::vec2(0.0f));
				return std::max(distance.x, distance.y);
		};

		return 0.0f;
	}

	float TerrainEditor::calc_attenuation(const float distance,
		const float attenuation_size,
		const BrushAttenuationType attenuation)
	{
		float tmp;

		if (distance <= 0.0f)
		{
			return 1.0f;
		}

		if (attenuation_size <= 0.0f)
		{
			return 0.0f;
		}

		tmp = std::max(1.0f - (distance / attenuation_size), 0.0f);

		switch (attenuation)
		{
			case bat_const:
				return 1.0f;
			case bat_linear:
				return tmp;
			case bat_quadratic:
				return tmp * tmp;
		};

		return 0.0f;
	}

	glm::vec3 TerrainEditor::calc_effect(const glm::vec3 &value,
		const glm::vec3 &data, const glm::vec3 &average,
		const glm::vec3 &strength, const BrushEffectType effect)
	{
		switch (effect)
		{
			case bet_add:
				return glm::mix(value, value + data, strength);
			case bet_set:
				return glm::mix(value, data, strength);
			case bet_smooth:
				return glm::mix(value, average, data *
					strength);
		};

		return data;
	}

	glm::vec3 TerrainEditor::calc_brush(const glm::vec3 &value,
		const glm::vec3 &data, const glm::vec3 &average,
		const glm::bvec3 &mask, const glm::vec2 &center,
		const glm::vec2 &position, const glm::vec2 &size,
		const float attenuation_size,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, const BrushEffectType effect)
	{
		glm::vec3 strength;
		float distance;

		strength = glm::vec3(mask);

		distance = calc_distance(center, position, size, shape);

		strength *= calc_attenuation(distance, attenuation_size,
			attenuation);

		return calc_effect(value, data, average, strength, effect);
	}

	void TerrainEditor::change_terrain_values(const glm::vec3 &data,
		const glm::bvec3 &mask, const glm::vec2 &size,
		const glm::uvec2 &vertex, const float attenuation_size,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, const BrushEffectType effect,
		TerrainValueVector &terrain_values) const
	{
		glm::vec3 average, value, normal;
		glm::vec2 center, position;
		glm::ivec2 index;

		if (terrain_values.size() < 1)
		{
			return;
		}

		center = glm::vec2(vertex);

		average = glm::vec3(0.0f);

		BOOST_FOREACH(const TerrainValue &terrain_value, terrain_values)
		{
			average += AbstractTerrainManager::get_offset_scaled_rgb10_a2(
				terrain_value.get_value());
		}

		average /= terrain_values.size();

		BOOST_FOREACH(TerrainValue &terrain_value, terrain_values)
		{
			value = AbstractTerrainManager::get_offset_scaled_rgb10_a2(
				terrain_value.get_value());

			index.x = terrain_value.get_x();
			index.y = terrain_value.get_y();
			position = index;

			value = calc_brush(value, data, average, mask, center,
				position, size, attenuation_size, attenuation,
				shape, effect);

			terrain_value.set_value(
				AbstractTerrainManager::get_value_scaled_rgb10_a2(
					value));
		}
	}

	void TerrainEditor::get_blend_values(const glm::uvec2 &vertex,
		const float radius, ImageValueVector &blend_values) const
	{
		Uvec4Array4 values;
		glm::vec2 centre, point;
		Uint32 x, y;
		Uint32 min_x, min_y, max_x, max_y;
		float tmp, temp, sqr_radius;

		tmp = static_cast<float>(vertex.x) - radius;
		min_x = boost::numeric_cast<Uint32>(std::max(0.0f, tmp));

		tmp = static_cast<float>(vertex.y) - radius;
		min_y = boost::numeric_cast<Uint32>(std::max(0.0f, tmp));

		tmp = static_cast<float>(vertex.x) + radius;
		temp = m_blend_images[0]->get_width() - 1.0f;
		max_x = boost::numeric_cast<Uint32>(std::min(temp, tmp));

		tmp = static_cast<float>(vertex.y) + radius;
		temp = m_blend_images[0]->get_height() - 1.0f;
		max_y = boost::numeric_cast<Uint32>(std::min(temp, tmp));

		blend_values.clear();

		centre = glm::vec2(vertex[0], vertex[1]);

		sqr_radius = radius * radius;

		for (x = min_x; x <= max_x; x++)
		{
			for (y = min_y; y <= max_y; y++)
			{
				point = glm::vec2(x, y);

				if (glm::distance2(centre, point) <= sqr_radius)
				{
					ImageValue value(x, y);

					values[0] = m_blend_images[0]->
						get_pixel_uint(x, y, 0, 0, 0);
					values[1] = m_blend_images[1]->
						get_pixel_uint(x, y, 0, 0, 0);
					values[2] = m_blend_images[2]->
						get_pixel_uint(x, y, 0, 0, 0);
					values[3] = m_blend_images[3]->
						get_pixel_uint(x, y, 0, 0, 0);

					//value.set_values(values);
					blend_values.push_back(value);
				}
			}
		}
	}

	float TerrainEditor::calc_effect(const float value, const float data,
		const float average, const float strength,
		const BrushEffectType effect)
	{
		switch (effect)
		{
			case bet_add:
				return glm::mix(value, value + data, strength);
			case bet_set:
				return glm::mix(value, data, strength);
			case bet_smooth:
				return glm::mix(value, average, data *
					strength);
		};

		return data;
	}

	float TerrainEditor::calc_brush(const glm::vec2 &center,
		const glm::vec2 &position, const glm::vec2 &size,
		const float value, const float attenuation_size,
		const float data, const float average,		
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, const BrushEffectType effect)
	{
		float strength, distance;

		distance = calc_distance(center, position, size, shape);

		strength = calc_attenuation(distance, attenuation_size,
			attenuation);

		return calc_effect(value, data, average, strength, effect);
	}

	void TerrainEditor::change_blend_values(const glm::vec2 &size,
		const glm::uvec2 &vertex, const float attenuation_size,
		const float data, const Uint16 layer, const bool mask, 
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, const BrushEffectType effect,
		ImageValueVector &terrain_values) const
	{
		glm::vec3 average, value, normal;
		glm::vec2 center, position;
		glm::ivec2 index;

		if (terrain_values.size() < 1)
		{
			return;
		}

		center = glm::vec2(vertex);
/*
		average = glm::vec3(0.0f);

		BOOST_FOREACH(const TerrainValue &terrain_value, terrain_values)
		{
			average += AbstractTerrainManager::get_offset_scaled_rgb10_a2(
				terrain_value.get_value());
		}

		average /= terrain_values.size();

		BOOST_FOREACH(TerrainValue &terrain_value, terrain_values)
		{
			value = AbstractTerrainManager::get_offset_scaled_rgb10_a2(
				terrain_value.get_value());

			index.x = terrain_value.get_x();
			index.y = terrain_value.get_y();
			position = index;

			value = calc_brush(value, data, average, mask, center,
				position, size, attenuation_size, attenuation,
				shape, effect);

			terrain_value.set_value(
				AbstractTerrainManager::get_value_scaled_rgb10_a2(
					value));
		}
*/
	}

	glm::vec4 TerrainEditor::get_blend_values(const glm::vec4 &blend)
	{
		glm::vec4 result;
		Uint32 i;
/*
		for (i = 0; i < 4; i++)
		{
			result[i] = get_blend_value(blend, i);
		}
*/
		return result;
	}

	void TerrainEditor::set_blend_value(const float value,
		const Uint32 index, glm::vec4 &blend)
	{
		glm::vec4 tmp;
		float t0, t1, sum;

		tmp = get_blend_values(blend);

		tmp[index] = 0.0f;

		sum = tmp[0] + tmp[1] + tmp[2] + tmp[3];

		if (sum > epsilon)
		{
			tmp *= (1.0f - value) / sum;
		}
		else
		{
			tmp = glm::vec4(1.0f - value) / 3.0f;
		}

		tmp[index] = value;

		assert(std::abs(glm::dot(tmp, glm::vec4(1.0f)) - 1.0f) <
			epsilon);

		t0 = tmp[0] + tmp[1];
		t1 = tmp[2] + tmp[3];

		blend[1] = tmp[2] + tmp[3];
		blend[1] += 1.0f - (tmp[0] + tmp[1]);
		blend[1] *= 0.5f;

		if (std::abs(t0) > epsilon)
		{
			blend[0] = tmp[1] / t0;
			blend[0] += 1.0f - tmp[0] / t0;
			blend[0] *= 0.5f;
		}

		if (std::abs(t1) > epsilon)
		{
			blend[2] = tmp[3] / t1;
			blend[2] += 1.0f - tmp[2] / t1;
			blend[2] *= 0.5f;
		}
/*
		assert(std::abs(value - get_blend_value(blend, index)) <
			epsilon);
*/
	}

	void TerrainEditor::init_terrain(const glm::uvec2 &size)
	{
		glm::uvec3 sizes, sizes_x4;
		Uint32 x, y;

		sizes = glm::uvec3(size, 1);
		sizes_x4 = glm::uvec3((size * 4u + 3u) / 4u, 1);

		m_terrain_vector_image = boost::make_shared<Image>(
			String(UTF8("terrain vector map")),
			false, tft_rgb10_a2, sizes, 0);

		m_terrain_normal_image = boost::make_shared<Image>(
			String(UTF8("terrain normal map")),
			false, tft_rgba8, sizes, 0);

		m_terrain_dudv_image = boost::make_shared<Image>(
			String(UTF8("terrain dudv map")),
			false, tft_rgba8, sizes, 0);

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				m_terrain_vector_image->set_pixel_uint(x, y, 0,
					0, 0, glm::uvec4(512, 512, 0, 0));
			}
		}

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				update_normal(glm::ivec2(x, y));
			}
		}
	}

	glm::uvec2 TerrainEditor::get_best_normal(const glm::vec3 &normal) const
	{
		glm::uvec2 value, result;
		float error, tmp;
		Uint32 i, j, index;

		error = std::numeric_limits<float>::max();

		for (i = 0; i < 256; ++i)
		{
			for (j = 0; j < 256; ++j)
			{
				value.x = j;
				value.y = i;

				index = value.x + value.y * 256;

				tmp = std::abs(1.0f - glm::dot(m_normals[index],
					normal));

				if (tmp <= error)
				{
					error = tmp;
					result = value;
				}
			}
		}

		return result;
	}

	glm::vec3 TerrainEditor::get_position(const glm::ivec2 &index) const
	{
		glm::vec3 offset;
		glm::vec2 position;

		offset = AbstractTerrainManager::get_offset_scaled_rgb10_a2(
			m_terrain_vector_image->get_pixel_uint(index.x,
				index.y, 0, 0, 0));

		position.x = index.x;
		position.y = index.y;
		position *= AbstractTerrainManager::get_patch_scale();

		return glm::vec3(position, 0.0f) + offset;
	}

	glm::vec3 TerrainEditor::get_direction(const glm::vec3 &centre,
		const glm::ivec2 &index) const
	{
		if ((index.x < 0) || (index.y < 0) ||
			(index.x >= static_cast<Sint32>(
				m_terrain_vector_image->get_width())) ||
			(index.y >= static_cast<Sint32>(
				m_terrain_vector_image->get_height())))
		{
			return glm::vec3(0.0f);
		}

		return glm::normalize(get_position(index) - centre);
	}

	glm::vec3 TerrainEditor::get_normal(const glm::ivec2 &index) const
	{
		glm::vec3 centre, d0, d1, d2, d3, d4, d5, d6, d7, n;
		glm::uvec3 sizes;
		glm::uvec2 value;

		centre = get_position(index);

		d0 = get_direction(centre, index + glm::ivec2(-1, -1));
		d1 = get_direction(centre, index + glm::ivec2( 0, -1));
		d2 = get_direction(centre, index + glm::ivec2( 1, -1));
		d3 = get_direction(centre, index + glm::ivec2( 1,  0));
		d4 = get_direction(centre, index + glm::ivec2( 1,  1));
		d5 = get_direction(centre, index + glm::ivec2( 0,  1));
		d6 = get_direction(centre, index + glm::ivec2(-1,  1));
		d7 = get_direction(centre, index + glm::ivec2(-1,  0));

		n = glm::cross(d0, d1);
		n += glm::cross(d1, d2);
		n += glm::cross(d2, d3);
		n += glm::cross(d3, d4);
		n += glm::cross(d4, d5);
		n += glm::cross(d5, d6);
		n += glm::cross(d6, d7);
		n += glm::cross(d7, d0);

		return glm::normalize(n);
	}

	void TerrainEditor::update_normal(const glm::ivec2 &index)
	{
		glm::vec3 normal;
		glm::uvec2 value;

		normal = get_normal(index);

		value = PackTool::encode_normal_optimized_uint8(normal);

		if (std::abs(1.0f - glm::dot(normal,
			PackTool::decode_normal_optimized_uint8(value)))
				> 0.01f)
		{
			value = get_best_normal(normal);
		}

		m_terrain_normal_image->set_pixel_uint(index.x,
			index.y, 0, 0, 0, glm::uvec4(value, 0, 0));
	}

	void TerrainEditor::update_normals(const Ivec2Set &indices)
	{
		BOOST_FOREACH(const glm::ivec2 &index, indices)
		{
			update_normal(index);
		}
	}

	const glm::vec3 &TerrainEditor::get_terrain_offset()
	{
		return AbstractTerrainManager::get_vector_scale();
	}

	const glm::vec3 &TerrainEditor::get_terrain_offset_min()
	{
		return AbstractTerrainManager::get_vector_min();
	}

	const glm::vec3 &TerrainEditor::get_terrain_offset_max()
	{
		return AbstractTerrainManager::get_vector_max();
	}

	glm::uvec2 TerrainEditor::get_vertex(const glm::vec3 &world_position)
		const
	{
		glm::vec2 tmp;
		glm::ivec2 index, min, max, result, size;
		Sint32 x, y;
		float distance, min_distance;

		min_distance = std::numeric_limits<float>::max();

		tmp = glm::vec2(world_position) +
			glm::vec2(AbstractTerrainManager::get_vector_min());
		min = glm::ivec2(tmp /
			AbstractTerrainManager::get_patch_scale());

		tmp = glm::vec2(world_position) +
			glm::vec2(AbstractTerrainManager::get_vector_max());
		max = glm::ivec2(tmp /
			AbstractTerrainManager::get_patch_scale());

		size.x = m_terrain_vector_image->get_width();
		size.y = m_terrain_vector_image->get_height();
		size -= 1;

		min = glm::clamp(min, glm::ivec2(0), size);
		max = glm::clamp(max, glm::ivec2(0), size);

		for (y = min.y; y <= max.y; ++y)
		{
			for (x = min.x; x <= max.x; ++x)
			{
				index.x = x;
				index.y = y;

				distance = glm::distance(get_position(index),
					world_position);

				if (distance < min_distance)
				{
					if (distance == 0.0f)
					{
						return glm::uvec2(result);
					}

					min_distance = distance;
					result = index;
				}
			}
		}

		return glm::uvec2(result);
	}

}
