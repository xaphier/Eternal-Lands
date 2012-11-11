/****************************************************************************
 *            terraineditor.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terraineditor.hpp"
#include "image.hpp"
#include "abstractterrain.hpp"
#include "terrain/uvtool.hpp"
#include "imageupdate.hpp"

namespace eternal_lands
{

	TerrainEditor::TerrainEditor(): m_enabled(false)
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

	void TerrainEditor::set_displacement_values(
		const DisplacementValueVector &displacement_values,
		ImageUpdate &displacement_map, ImageUpdate &normal_map,
		ImageUpdate &dudv_map)
	{
		Ivec2Set indices;
		glm::uvec3 displacement_min, displacement_max;
		glm::uvec3 normal_min, normal_max;
		glm::ivec2 index, min, max;
		Sint32 x, y;

		min = glm::ivec2(0);
		max = glm::ivec2(m_displacement_image->get_width() - 1,
			m_displacement_image->get_height()) - 1;

		displacement_min = glm::uvec3(glm::uvec2(m_size), 0);
		displacement_max = glm::uvec3(0, 0, 1);
		normal_min = glm::uvec3(glm::uvec2(m_size), 0);
		normal_max = glm::uvec3(0, 0, 1);

		BOOST_FOREACH(const DisplacementValue &displacement_value,
			displacement_values)
		{
			m_displacement_image->set_pixel_packed_uint32(
				displacement_value.get_x(),
				displacement_value.get_y(), 0, 0, 0,
				displacement_value.get_packed_value());

			index.x = displacement_value.get_x();
			index.y = displacement_value.get_y();

			displacement_min = glm::min(displacement_min,
				glm::uvec3(index, 0));
			displacement_max = glm::max(displacement_max,
				glm::uvec3(index, 1));

			for (y = -1; y < 2; ++y)
			{
				for (x = -1; x < 2; ++x)
				{
					index.x = displacement_value.get_x();
					index.y = displacement_value.get_y();
					index.x += x;
					index.y += y;

					index = glm::min(index, max);
					index = glm::max(index, min);

					indices.insert(index);

					normal_min = glm::min(normal_min,
						glm::uvec3(index, 0));
					normal_max = glm::max(normal_max,
						glm::uvec3(index, 1));
				}
			}
		}

		update_normals(indices);


		displacement_min = glm::uvec3(0, 0, 0);
		displacement_max = glm::uvec3(glm::uvec2(m_size), 1);
		normal_min = glm::uvec3(0, 0, 0);
		normal_max = glm::uvec3(glm::uvec2(m_size), 1);

		displacement_map = ImageUpdate(m_displacement_image,
			displacement_min, displacement_max - displacement_min);
		normal_map = ImageUpdate(m_normal_image, normal_min,
			normal_max - normal_min);
		dudv_map = ImageUpdate(m_dudv_image, glm::uvec3(0),
			glm::uvec3(0));
	}

	void TerrainEditor::set_blend_values(
		const ImageValueVector &blend_values, ImageUpdate &blend_map)
	{
		glm::uvec3 blend_min, blend_max;
		glm::uvec2 index;
		Uint16 i, count;

		assert(get_blend_image()->get_depth() <= 4);

		count = get_blend_image()->get_depth();

		blend_min = glm::uvec3(glm::uvec2(m_size), 0);
		blend_max = glm::uvec3(0, 0, count);

		BOOST_FOREACH(const ImageValue &blend_value, blend_values)
		{
			for (i = 0; i < count; ++i)
			{
				m_blend_image->set_pixel_uint(
					blend_value.get_x(),
					blend_value.get_y(), i, 0, 0,
					blend_value.get_value(i));
			}

			index.x = blend_value.get_x();
			index.y = blend_value.get_y();

			blend_min = glm::min(blend_min, glm::uvec3(index, 0));
			blend_max = glm::max(blend_max, glm::uvec3(index,
				count));
		}

		blend_min = glm::uvec3(0);
		blend_max = glm::uvec3(m_size);

		blend_map = ImageUpdate(m_blend_image, blend_min,
			blend_max - blend_min);
	}

	void TerrainEditor::get_displacement_values(const Uint32 x,
		const Uint32 y, DisplacementValueVector &displacement_values)
		const
	{
		DisplacementValue value(x, y);

		value.set_packed_value(
			m_displacement_image->get_pixel_packed_uint32(x, y,
				0, 0, 0));

		displacement_values.push_back(value);
	}

	void TerrainEditor::get_displacement_values(const glm::uvec2 &vertex,
		const glm::vec2 &size, const float attenuation_size,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape,
		DisplacementValueVector &displacement_values) const
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
		temp = m_displacement_image->get_width();
		max_x = std::min(static_cast<Sint64>(tmp),
			static_cast<Sint64>(temp - 1));

		tmp = static_cast<float>(vertex.y) + max_size.y;
		temp = m_displacement_image->get_height();
		max_y = std::min(static_cast<Sint64>(tmp),
			static_cast<Sint64>(temp - 1));

		displacement_values.clear();

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
					get_displacement_values(x, y,
						displacement_values);
				}
			}
		}
	}

	void TerrainEditor::get_blend_values(const Uint32 x, const Uint32 y,
		ImageValueVector &blend_values) const
	{
		ImageValue value(x, y);
		Uint16 i, count;

		assert(get_blend_image()->get_depth() <= 4);

		count = get_blend_image()->get_depth();

		for (i = 0; i < count; ++i)
		{
			value.set_value(m_blend_image->get_pixel_uint(x, y, i,
				0, 0), i);
		}

		blend_values.push_back(value);
	}

	void TerrainEditor::get_blend_values(const glm::uvec2 &vertex,
		const glm::vec2 &size, const float attenuation_size,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, ImageValueVector &blend_values)
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
		temp = m_blend_image->get_width();
		max_x = std::min(static_cast<Sint64>(tmp),
			static_cast<Sint64>(temp - 1));

		tmp = static_cast<float>(vertex.y) + max_size.y;
		temp = m_blend_image->get_height();
		max_y = std::min(static_cast<Sint64>(tmp),
			static_cast<Sint64>(temp - 1));

		blend_values.clear();

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
					get_blend_values(x, y, blend_values);
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

	void TerrainEditor::change_displacement_values(const glm::vec3 &data,
		const glm::bvec3 &mask, const glm::vec2 &size,
		const glm::uvec2 &vertex, const float attenuation_size,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, const BrushEffectType effect,
		DisplacementValueVector &displacement_values) const
	{
		glm::vec3 average, value, normal;
		glm::vec2 center, position;
		glm::ivec2 index;

		if (displacement_values.size() < 1)
		{
			return;
		}

		center = glm::vec2(vertex);

		average = glm::vec3(0.0f);

		BOOST_FOREACH(const DisplacementValue &displacement_value,
			displacement_values)
		{
			average += AbstractTerrain::get_offset_scaled_rgb10_a2(
				displacement_value.get_packed_value());
		}

		average /= displacement_values.size();

		BOOST_FOREACH(DisplacementValue &displacement_value,
			displacement_values)
		{
			value = AbstractTerrain::get_offset_scaled_rgb10_a2(
				displacement_value.get_packed_value());

			index.x = displacement_value.get_x();
			index.y = displacement_value.get_y();
			position = index;

			value = calc_brush(value, data, average, mask, center,
				position, size, attenuation_size, attenuation,
				shape, effect);

			displacement_value.set_value(
				AbstractTerrain::get_value_scaled_rgb10_a2(
					value));
		}
	}

	void TerrainEditor::change_blend_values(const glm::vec2 &size,
		const glm::uvec2 &vertex, const float attenuation_size,
		const float data, const BrushAttenuationType attenuation,
		const BrushShapeType shape, const BrushEffectType effect,
		const int layer, ImageValueVector &blend_values) const
	{
		glm::vec4 value;
		glm::vec2 center, position;
		glm::ivec2 index;
		float average, tmp;
		Uint16 idx0, idx1;

		if (blend_values.size() < 1)
		{
			return;
		}

		center = glm::vec2(vertex);

		idx0 = layer / 4;
		idx1 = layer % 4;
		average = 0.0f;

		BOOST_FOREACH(const ImageValue &blend_value, blend_values)
		{
			average += blend_value.get_normalized_value(idx0)[idx1];
		}

		average /= blend_values.size();

		BOOST_FOREACH(ImageValue &blend_value, blend_values)
		{
			value = blend_value.get_normalized_value(idx0);

			tmp = value[idx1];

			index.x = blend_value.get_x();
			index.y = blend_value.get_y();
			position = index;

			tmp = calc_brush(center, position, size, tmp,
				attenuation_size, data, average, attenuation,
				shape, effect);

			value[idx1] = glm::clamp(tmp, 0.0f, 1.0f);

			blend_value.set_normalized_value(value, idx0);
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

	void TerrainEditor::init(const glm::uvec2 &size)
	{
		glm::uvec3 tmp;

		tmp = glm::uvec3(size, 0);

		tmp.x += AbstractTerrain::get_tile_size() - 1;
		tmp.y += AbstractTerrain::get_tile_size() - 1;

		tmp.x = (tmp.x - 1) / AbstractTerrain::get_tile_size();
		tmp.y = (tmp.y - 1) / AbstractTerrain::get_tile_size();

		tmp.x = tmp.x * AbstractTerrain::get_tile_size() + 1;
		tmp.y = tmp.y * AbstractTerrain::get_tile_size() + 1;

		m_size.x = tmp.x;
		m_size.y = tmp.y;
		m_size.z = 4;

		m_displacement_image = boost::make_shared<Image>(
			String(UTF8("displacement map")), false, tft_rgb10_a2,
			tmp, 0);

		m_normal_image = boost::make_shared<Image>(
			String(UTF8("normal map")), false, tft_rg8, tmp, 0);

		m_dudv_image = boost::make_shared<Image>(
			String(UTF8("dudv map")), false, tft_rg16, tmp, 0);

		m_blend_image = boost::make_shared<Image>(String(
			UTF8("blend map")), false, tft_rgba8, m_size, 0, true);

		m_albedo_maps.resize(get_layer_count());
		m_extra_maps.resize(get_layer_count());
		m_material_data.resize(get_layer_count() - 1);

		m_enabled = true;

		m_uv_tool.reset(new UvTool(m_displacement_image));
	}

	void TerrainEditor::init(const glm::uvec2 &size,
		const String &albedo_map, const String &extra_map,
		const bool use_blend_size_sampler, const bool use_extra_map)
	{
		Uint32 x, y, z, i;

		init(size);

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				m_displacement_image->set_pixel(x, y, 0, 0, 0,
					glm::vec4(0.5f, 0.5f, 0.0f, 0.0f));
			}
		}

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				update_normal(glm::ivec2(x, y));
			}
		}

		for (z = 0; z < m_size.z; ++z)
		{
			for (y = 0; y < m_size.y; ++y)
			{
				for (x = 0; x < m_size.x; ++x)
				{
					m_blend_image->set_pixel(x, y, z,
						0, 0, glm::vec4(0.0f));
				}
			}
		}

		for (i = 0; i < get_layer_count(); ++i)
		{
			set_albedo_map(albedo_map, i);
			set_extra_map(extra_map, i);
			set_use_blend_size_sampler(use_blend_size_sampler, i);
			set_use_extra_map(use_extra_map, i);
		}
	}

	void TerrainEditor::init(const ImageSharedPtr &height_map,
		const glm::uvec2 &size, const String &albedo_map,
		const String &extra_map, const bool use_blend_size_sampler,
		const bool use_extra_map)
	{
		init(size, albedo_map, extra_map, use_blend_size_sampler,
			use_extra_map);

		import_height_map(height_map);
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

		offset = AbstractTerrain::get_offset_scaled_rgb10_a2(
			m_displacement_image->get_pixel_packed_uint32(index.x,
				index.y, 0, 0, 0));

		position.x = index.x;
		position.y = index.y;
		position *= AbstractTerrain::get_patch_scale();

		return glm::vec3(position, 0.0f) + offset;
	}

	glm::vec3 TerrainEditor::get_smooth_position(const glm::vec2 &index)
		const
	{
		glm::vec3 p0, p1, p2, p3, t0, t1;
		glm::vec2 fract;
		glm::ivec2 idx;

		idx = index;

		p0 = get_position(idx);
		p1 = get_position(idx + glm::ivec2(1, 0));
		p2 = get_position(idx + glm::ivec2(0, 1));
		p3 = get_position(idx + glm::ivec2(1, 1));

		fract = glm::fract(index);

		t0 = glm::mix(p0, p1, fract.x);
		t1 = glm::mix(p2, p3, fract.x);

		return glm::mix(t0, t1, fract.y);
	}

	glm::vec3 TerrainEditor::get_direction(const glm::vec3 &centre,
		const glm::ivec2 &index) const
	{
		if ((index.x < 0) || (index.y < 0) ||
			(index.x >= static_cast<Sint32>(
				m_displacement_image->get_width())) ||
			(index.y >= static_cast<Sint32>(
				m_displacement_image->get_height())))
		{
			return glm::vec3(0.0f);
		}

		return glm::normalize(get_position(index) - centre);
	}

	glm::vec3 TerrainEditor::get_normal(const glm::ivec2 &index) const
	{
		glm::vec3 centre, d0, d1, d2, d3, d4, d5, d6, d7, n;
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

		m_normal_image->set_pixel_uint(index.x, index.y, 0, 0, 0,
			glm::uvec4(value, 0, 0));
	}

	void TerrainEditor::update_normals(const Ivec2Set &indices)
	{
		BOOST_FOREACH(const glm::ivec2 &index, indices)
		{
			update_normal(index);
		}
	}

	const glm::vec3 &TerrainEditor::get_terrain_offset_min()
	{
		return AbstractTerrain::get_vector_min();
	}

	const glm::vec3 &TerrainEditor::get_terrain_offset_max()
	{
		return AbstractTerrain::get_vector_max();
	}

	bool TerrainEditor::get_vertex(const glm::vec3 &world_position,
		glm::uvec2 &result) const
	{
		glm::ivec2 index, min, max, size;
		Sint32 x, y;
		float distance, min_distance;

		min_distance = std::numeric_limits<float>::max();

		min = glm::ivec2(glm::vec2(world_position -
			AbstractTerrain::get_vector_scale()) /
			AbstractTerrain::get_patch_scale());

		max = glm::ivec2(glm::vec2(world_position +
			AbstractTerrain::get_vector_scale()) /
			AbstractTerrain::get_patch_scale()) + 1;

		size.x = m_displacement_image->get_width();
		size.y = m_displacement_image->get_height();
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
						return true;
					}

					min_distance = distance;
					result = index;
				}
			}
		}

		return min_distance <= glm::length(
			AbstractTerrain::get_vector_scale());
	}

	void TerrainEditor::set_material(const String &albedo_map,
		const String &extra_map, const float blend_size,
		const bool use_blend_size_sampler, const bool use_blend_size,
		const bool use_extra_map, const Uint16 index)
	{
		set_albedo_map(albedo_map, index);
		set_extra_map(extra_map, index);
		set_use_blend_size_sampler(use_blend_size_sampler, index);
		set_use_extra_map(use_extra_map, index);

		if (index > 0)
		{
			set_blend_data(BlendData(blend_size, use_blend_size),
				index - 1);
		}
	}

	void TerrainEditor::get_material(String &albedo_map,
		String &extra_map, float &blend_size,
		bool &use_blend_size_sampler, bool &use_blend_size,
		bool &use_extra_map, const Uint16 index) const
	{
		albedo_map = get_albedo_map(index);
		extra_map = get_extra_map(index);
		use_blend_size_sampler = get_use_blend_size_sampler(index);
		use_extra_map = get_use_extra_map(index);

		blend_size = 1.0f;
		use_blend_size = false;

		if (index > 0)
		{
			blend_size = get_blend_size(index - 1);
			use_blend_size = get_use_blend_size(index - 1);
		}
	}

	void TerrainEditor::import_height_map(const ImageSharedPtr &height_map)
	{
		ImageSharedPtr height_image;
		glm::uvec3 size;
		float height;
		Uint32 x, y;

		height_image = height_map->decompress(false, true, false);

		size = height_image->get_size();
		size = glm::max(size, glm::uvec3(1));

		height = 0;

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				if ((x < size.x) && (y < size.y))
				{
					height = height_image->get_pixel(
						x, y, 0, 0, 0).r;
				}

				m_displacement_image->set_pixel(x, y, 0, 0, 0,
					glm::vec4(0.5f, 0.5f, height, 0));
			}
		}

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				update_normal(glm::ivec2(x, y));
			}
		}
	}

	void TerrainEditor::get_all_displacement_values(
		DisplacementValueVector	&displacement_values) const
	{
		Uint32 x, y;

		displacement_values.clear();

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				get_displacement_values(x, y,
					displacement_values);
			}
		}
	}

	void TerrainEditor::get_all_blend_values(
		ImageValueVector &blend_values) const
	{
		Uint32 x, y;

		blend_values.clear();

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				get_blend_values(x, y, blend_values);
			}
		}
	}

	void TerrainEditor::set(const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const ImageSharedPtr &blend_map,
		const StringVector &albedo_maps,
		const StringVector &extra_maps,
		const TerrainMaterialData &material_data,
		const glm::vec4 &dudv_scale_offset,
		const glm::uvec2 &size)
	{
		glm::uvec3 image_size;

		m_size.x = size.x;
		m_size.y = size.y;
		m_size.z = 4;

		image_size = displacement_map->get_size();

		if ((image_size.x != m_size.x) || (image_size.y != m_size.y) ||
			(image_size.z != 0))
		{
			EL_THROW_EXCEPTION(SizeErrorException()
				<< errinfo_message(UTF8("Image has wrong size"))
				<< errinfo_width(image_size.x)
				<< errinfo_height(image_size.y)
				<< errinfo_depth(image_size.z)
				<< errinfo_expected_width(m_size.x)
				<< errinfo_expected_height(m_size.y)
				<< errinfo_expected_depth(0)
				<< boost::errinfo_file_name(
					displacement_map->get_name()));
		}

		m_displacement_image = displacement_map->decompress(false,
			true, false);

		m_uv_tool.reset(new UvTool(m_displacement_image));

		rebuild_normal_map();
		import_dudv_map(dudv_map, dudv_scale_offset);
		import_blend_map(blend_map);

		m_albedo_maps = albedo_maps;
		m_extra_maps = extra_maps;
		m_material_data = material_data;
		m_dudv_scale_offset = dudv_scale_offset;

		m_albedo_maps.resize(get_layer_count());
		m_extra_maps.resize(get_layer_count());
		m_material_data.resize(get_layer_count() - 1);

		m_enabled = true;
	}

	void TerrainEditor::import_blend_map(const ImageSharedPtr &blend_map)
	{
		ImageSharedPtr blend_image;
		glm::vec4 value;
		glm::uvec3 size;
		Uint32 x, y, z;

		m_blend_image = boost::make_shared<Image>(
			String(UTF8("blend map")), false, tft_rgba8, m_size, 0,
			true);

		if (blend_map.get() == nullptr)
		{
			return;
		}

		blend_image = blend_map->decompress(false, true, false);

		size = glm::min(m_size, blend_image->get_size());
		size = glm::max(size, glm::uvec3(1));

		for (z = 0; z < size.z; ++z)
		{
			for (y = 0; y < size.y; ++y)
			{
				for (x = 0; x < size.x; ++x)
				{
					value = blend_image->get_pixel(x, y, z,
						0, 0);

					m_blend_image->set_pixel(x, y, z,
						0, 0, value);
				}
			}
		}
	}

	void TerrainEditor::relax_uv(const AbstractProgressSharedPtr &progress,
		const Uint16 count, const bool use_simd)
	{
		m_uv_tool->relax_uv(progress, count, use_simd);

		m_uv_tool->convert(m_dudv_image, m_dudv_scale_offset);
	}

	void TerrainEditor::rebuild_normal_map()
	{
		Uint32 x, y;

		m_normal_image = boost::make_shared<Image>(
			String(UTF8("normal map")), false, tft_rg8,
			glm::uvec3(m_size.x, m_size.y, 0), 0);

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				update_normal(glm::ivec2(x, y));
			}
		}
	}

	void TerrainEditor::import_dudv_map(const ImageSharedPtr &dudv_map,
		const glm::vec4 &dudv_scale_offset)
	{
		ImageSharedPtr dudv_image;
		glm::vec4 value;
		glm::uvec3 size;
		Uint32 x, y;

		m_dudv_image = boost::make_shared<Image>(
			String(UTF8("dudv map")), false, tft_rg16,
			glm::uvec3(m_size.x, m_size.y, 0), 0);

		if (dudv_map.get() == nullptr)
		{
			return;
		}

		dudv_image = dudv_map->decompress(false, true, false);

		size = glm::min(m_size, dudv_map->get_size());
		size = glm::max(size, glm::uvec3(1));

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				value = dudv_image->get_pixel(x, y, 0, 0, 0);

				m_dudv_image->set_pixel(x, y, 0, 0, 0, value);
			}
		}

		m_uv_tool->import(m_dudv_image, m_dudv_scale_offset);
	}

	BitSet64 TerrainEditor::get_used_layers() const
	{
		glm::vec4 value;
		glm::uvec3 size;
		BitSet64 result, any_none_zero, any_one;
		Uint32 x, y, z, i;

		assert((m_size.z * 4 + 1) <= result.size());

		size = glm::max(m_size, glm::uvec3(1));

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				any_one.reset();

				for (z = 0; z < size.z; ++z)
				{
					value = m_blend_image->get_pixel(x, y,
						z, 0, 0);

					for (i = 0; i < 4; ++i)
					{
						any_none_zero[z * 4 + i] =
							any_none_zero[z * 4 + i]
							| (value[i] > 0.0f);

						any_one[z * 4 + i] =
							any_one[z * 4 + i]
							| (value[i] >= 1.0f);
					}
				}

				result[0] = result[0] | any_one.none();
			}
		}

		for (i = 0; i < (m_size.z * 4); ++i)
		{
			result[i + 1] = any_none_zero[i];
		}

		return result;
	}

	void TerrainEditor::clear_invisible_layers()
	{
		boost::multi_array<BitSet64, 2> masks;
		BitSet64 mask;
		glm::vec4 value;
		glm::uvec3 size;
		Uint32 x, y, z, i, count;

		size = glm::max(m_size, glm::uvec3(1));

		masks.resize(boost::extents[size.x][size.y]);

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				for (z = 0; z < size.z; ++z)
				{
					value = m_blend_image->get_pixel(x, y,
						z, 0, 0);

					for (i = 0; i < 4; ++i)
					{
						mask[z * 4 + i] =
							value[i] >= 1.0f;
					}
				}

				count = 0;

				for (i = 0; i < (size.z * 4); ++i)
				{
					if (mask[i] && !get_use_blend_size(i))
					{
						count = i;
					}
				}

				mask.reset();

				for (i = 0; i < count; ++i)
				{
					mask[i] = true;
				}

				masks[x][y] = mask;
			}
		}

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				mask = masks[x][y];

				if (x > 0)
				{
					if (y > 0)
					{
						mask &= masks[x - 1][y - 1];
					}

					mask &= masks[x - 1][y];

					if (y < (m_size.y -1))
					{
						mask &= masks[x - 1][y + 1];
					}
				}

				if (y > 0)
				{
					mask &= masks[x][y - 1];
				}

				if (y < (m_size.y -1))
				{
					mask &= masks[x][y + 1];
				}

				if (x < (m_size.x -1))
				{
					if (y > 0)
					{
						mask &= masks[x + 1][y - 1];
					}

					mask &= masks[x + 1][y];

					if (y < (m_size.y -1))
					{
						mask &= masks[x + 1][y + 1];
					}
				}

				for (z = 0; z < size.z; ++z)
				{
					value = m_blend_image->get_pixel(x, y,
						z, 0, 0);

					for (i = 0; i < 4; ++i)
					{
						if (mask[z * 4 + i])
						{
							value[i] = 0.0f;
						}
					}

					m_blend_image->set_pixel(x, y,
						z, 0, 0, value);
				}
			}
		}
	}

	void TerrainEditor::pack_layers()
	{
		Vec4Array16 values;
		glm::vec4 value;
		glm::uvec3 size;
		BitSet64 mask;
		String albedo_map, extra_map;
		float blend_size;
		bool use_blend_size_sampler, use_blend_size, use_extra_map;
		Uint32 x, y, z, i, count;
		Sint32 index;

		mask = get_used_layers();

		size = glm::max(m_size, glm::uvec3(1));

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				if (mask[0])
				{
					index = 0;
				}
				else
				{
					index = -1;
				}

				for (z = 0; z < size.z; ++z)
				{
					value = m_blend_image->get_pixel(x, y,
						z, 0, 0);

					for (i = 0; i < 4; ++i)
					{
						if (mask[z * 4 + i + 1])
						{
							if (index >= 0)
							{
								values[index / 4][index % 4] = value[i];
							}

							index++;
						}
					}
				}

				for (z = 0; z < size.z; ++z)
				{
					m_blend_image->set_pixel(x, y, z, 0, 0,
						values[z]);
				}
			}
		}		

		index = 0;
		count = size.z * 4;

		for (i = 0; i < count; ++i)
		{
			if (mask[i])
			{
				get_material(albedo_map, extra_map, blend_size,
					use_blend_size_sampler, use_blend_size,
					use_extra_map, i);

				set_material(albedo_map, extra_map, blend_size,
					use_blend_size_sampler, use_blend_size,
					use_extra_map, index);

				index++;
			}
		}
	}

	void TerrainEditor::get_layer_usage(Uint32Vector &use_layer_pixels,
		Uint32 &pixels) const
	{
		glm::vec4 value;
		glm::uvec3 size;
		BitSet64 result, any_none_zero, any_one;
		Uint32 x, y, z, i;

		assert((m_size.z * 4 + 1) <= result.size());

		size = glm::max(m_size, glm::uvec3(1));

		use_layer_pixels.clear();
		use_layer_pixels.resize(get_layer_count(), 0);

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				for (z = 0; z < size.z; ++z)
				{
					value = m_blend_image->get_pixel(x, y,
						z, 0, 0);

					for (i = 0; i < 4; ++i)
					{
						if (value[i] > 0.0f)
						{
							use_layer_pixels[z * 4
								+ i + 1]++;
						}

						any_one[z * 4 + i] =
							value[i] >= 1.0f;
					}
				}

				if (any_one.none())
				{
					use_layer_pixels[0]++;
				}
			}
		}

		pixels = size.x * size.y;
	}

	void TerrainEditor::clear()
	{
		m_displacement_image.reset();
		m_normal_image.reset();
		m_dudv_image.reset();
		m_blend_image.reset();
		m_uv_tool.reset();
		m_material_data.clear();
		m_albedo_maps.clear();
		m_extra_maps.clear();
		m_size = glm::vec3(0);
		m_dudv_scale_offset = glm::vec4(0.0f);
		m_enabled = false;
	}

	glm::vec3 TerrainEditor::get_displacement_value(
		const glm::uvec2 &vertex) const
	{
		return AbstractTerrain::get_offset_scaled_rgb10_a2(
			m_displacement_image->get_pixel_packed_uint32(vertex.x,
				vertex.y, 0, 0, 0));
	}

	glm::vec3 TerrainEditor::get_normal_value(const glm::uvec2 &vertex)
		const
	{
		return get_normal(glm::ivec2(vertex));
	}

}
