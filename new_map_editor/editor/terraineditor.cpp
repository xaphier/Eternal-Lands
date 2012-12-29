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
#include "meshdatatool.hpp"

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
		const DisplacementValueVector &displacement_values)
	{
		Ivec2Set indices;
		glm::ivec2 index, min, max;
		Sint32 x, y;

		min = glm::ivec2(0);
		max = glm::ivec2(m_displacement_map->get_width() - 1,
			m_displacement_map->get_height()) - 1;

		BOOST_FOREACH(const DisplacementValue &displacement_value,
			displacement_values)
		{
			m_displacement_map->set_pixel_packed_uint32(
				displacement_value.get_x(),
				displacement_value.get_y(), 0, 0, 0,
				displacement_value.get_packed_value());

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
				}
			}
		}

		update_normal_tangent_map(indices);
	}

	void TerrainEditor::set_blend_values(
		const ImageValueVector &blend_values, const Uint16 layer)
	{
		glm::uvec4 value;

		assert(get_blend_map()->get_depth() <= 4);

		assert(layer < (get_blend_map()->get_depth() * 4));

		BOOST_FOREACH(const ImageValue &blend_value, blend_values)
		{
			value = m_blend_map->get_pixel_uint(blend_value.get_x(),
				blend_value.get_y(), layer / 4, 0, 0);
			value[layer % 4] = blend_value.get_value();

			m_blend_map->set_pixel_uint(blend_value.get_x(),
				blend_value.get_y(), layer / 4, 0, 0, value);
		}
	}

	void TerrainEditor::set_blend_values(
		const ImageValuesVector &blend_values)
	{
		Uint16 i, count;

		assert(get_blend_map()->get_depth() <= 4);

		count = get_blend_map()->get_depth();

		BOOST_FOREACH(const ImageValues &blend_value, blend_values)
		{
			for (i = 0; i < count; ++i)
			{
				m_blend_map->set_pixel_packed_uint16(
					blend_value.get_x(),
					blend_value.get_y(), i, 0, 0,
					blend_value.get_packed_value(i));
			}
		}
	}

	void TerrainEditor::get_displacement_values(const Uint32 x,
		const Uint32 y, DisplacementValueVector &displacement_values)
		const
	{
		DisplacementValue value(x, y);

		value.set_packed_value(
			m_displacement_map->get_pixel_packed_uint32(x, y, 0, 0,
				0));

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
		temp = m_displacement_map->get_width();
		max_x = std::min(static_cast<Sint64>(tmp),
			static_cast<Sint64>(temp - 1));

		tmp = static_cast<float>(vertex.y) + max_size.y;
		temp = m_displacement_map->get_height();
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
		const Uint16 layer, ImageValueVector &blend_values) const
	{
		ImageValue value(x, y);

		assert(layer < (get_blend_map()->get_depth() * 4));

		value.set_value(m_blend_map->get_pixel_uint(x, y, layer / 4, 0,
			0)[layer % 4]);

		blend_values.push_back(value);
	}

	void TerrainEditor::get_blend_values(const Uint32 x, const Uint32 y,
		ImageValuesVector &blend_values) const
	{
		ImageValues value(x, y);
		Uint16 i, count;

		assert(get_blend_map()->get_depth() <= 4);

		count = get_blend_map()->get_depth();

		for (i = 0; i < count; ++i)
		{
			value.set_packed_value(
				m_blend_map->get_pixel_packed_uint16(x, y, i,
					0, 0), i);
		}

		blend_values.push_back(value);
	}

	void TerrainEditor::get_blend_values(const glm::uvec2 &vertex,
		const glm::vec2 &size, const float attenuation_size,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, const Uint16 layer,
		ImageValueVector &blend_values) const
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
		temp = m_blend_map->get_width();
		max_x = std::min(static_cast<Sint64>(tmp),
			static_cast<Sint64>(temp - 1));

		tmp = static_cast<float>(vertex.y) + max_size.y;
		temp = m_blend_map->get_height();
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
					get_blend_values(x, y, layer,
						blend_values);
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

	glm::vec3 TerrainEditor::calc_displacement_effect(
		const glm::vec3 &value, const glm::vec3 &data,
		const glm::vec3 &average, const glm::vec3 &strength,
		const DisplacementEffectType effect)
	{
		switch (effect)
		{
			case det_add:
				return glm::mix(value, value + data, strength);
			case det_set:
				return glm::mix(value, data, strength);
			case det_smooth:
				return glm::mix(value, average, data *
					strength);
		};

		return data;
	}

	glm::vec3 TerrainEditor::calc_displacement_brush(
		const glm::vec3 &value, const glm::vec3 &data,
		const glm::vec3 &average, const glm::bvec3 &mask,
		const glm::vec2 &center, const glm::vec2 &position,
		const glm::vec2 &size, const float attenuation_size,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, const DisplacementEffectType effect)
	{
		glm::vec3 strength;
		float distance;

		strength = glm::vec3(mask);

		distance = calc_distance(center, position, size, shape);

		strength *= calc_attenuation(distance, attenuation_size,
			attenuation);

		return calc_displacement_effect(value, data, average, strength,
			effect);
	}

	void TerrainEditor::change_displacement_values(const glm::vec3 &data,
		const glm::bvec3 &mask, const glm::vec2 &size,
		const glm::uvec2 &vertex, const float attenuation_size,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, const DisplacementEffectType effect,
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

			value = calc_displacement_brush(value, data, average,
				mask, center, position, size, attenuation_size,
				attenuation, shape, effect);

			displacement_value.set_value(
				AbstractTerrain::get_value_scaled_rgb10_a2(
					value));
		}
	}

	void TerrainEditor::change_blend_values(const glm::vec2 &size,
		const glm::uvec2 &vertex, const float attenuation_size,
		const float strength, const BrushAttenuationType attenuation,
		const BrushShapeType shape, const BlendEffectType effect,
		const Uint16 layer, ImageValueVector &blend_values) const
	{
		glm::vec2 center, position;
		glm::ivec2 index;
		float tmp, slope;
		Uint16 value;

		if (blend_values.size() < 1)
		{
			return;
		}

		assert(layer < get_layer_count());

		center = glm::vec2(vertex);

		BOOST_FOREACH(ImageValue &blend_value, blend_values)
		{
			value = blend_value.get_value();

			tmp = value / 15.0f;

			index.x = blend_value.get_x();
			index.y = blend_value.get_y();
			position = index;

			slope = get_blend_slope(index);

			tmp = calc_blend_brush(center, position, size, tmp,
				attenuation_size, strength, slope, attenuation,
				shape, effect);

			value = glm::clamp(tmp, 0.0f, 1.0f) * 15.0f + 0.5f;

			blend_value.set_value(value);
		}
	}

	float TerrainEditor::calc_blend_effect(const float value,
		const float data, const float slope, const float strength,
		const BlendEffectType effect)
	{
		float tmp;
	
		switch (effect)
		{
			case bet_set:
				return glm::mix(value, data, strength);
			case bet_slope:
				tmp = glm::smoothstep(data - 0.01f,
					data + 0.01f, slope);
				return glm::mix(value, tmp, strength);
			case bet_inverse_slope:
				tmp = glm::smoothstep(data - 0.01f,
					data + 0.01f, 1.0f - slope);
				return glm::mix(value, tmp, strength);
		};

		return data;
	}

	float TerrainEditor::calc_blend_brush(const glm::vec2 &center,
		const glm::vec2 &position, const glm::vec2 &size,
		const float value, const float attenuation_size,
		const float data, const float slope,
		const BrushAttenuationType attenuation,
		const BrushShapeType shape, const BlendEffectType effect)
	{
		float strength, distance;

		distance = calc_distance(center, position, size, shape);

		strength = calc_attenuation(distance, attenuation_size,
			attenuation);

		return calc_blend_effect(value, data, slope, strength, effect);
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

		m_displacement_map = boost::make_shared<Image>(
			String(UTF8("displacement map")), false, tft_rgb10_a2,
			tmp, 0, false);

		m_normal_tangent_map = boost::make_shared<Image>(
			String(UTF8("normal tangent map")), false, tft_rgba8,
			tmp, 0, false);

		m_dudv_map = boost::make_shared<Image>(
			String(UTF8("dudv map")), false, tft_rg16, tmp, 0,
			false);

		m_blend_map = boost::make_shared<Image>(String(
			UTF8("blend map")), false, tft_rgba4, m_size, 0, true);

		m_albedo_maps.resize(get_layer_count());
		m_specular_maps.resize(get_layer_count());
		m_gloss_maps.resize(get_layer_count());
		m_height_maps.resize(get_layer_count());
		m_material_data.clear();
		m_material_data.resize(get_layer_count());

		m_enabled = true;

		m_uv_tool.reset(new UvTool(m_displacement_map));
	}

	void TerrainEditor::init(const glm::vec3 &translation,
		const glm::uvec2 &size, const String &albedo_map,
		const String &specular_map, const String &gloss_map,
		const String &height_map, const glm::vec3 &default_specular,
		const float default_gloss, const float default_height,
		const float blend_size, const bool use_blend_size_texture,
		const bool use_specular_map, const bool use_gloss_map,
		const bool use_height_map)
	{
		Uint32 x, y, z, i;

		init(size);

		set_translation(translation);

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				m_displacement_map->set_pixel(x, y, 0, 0, 0,
					glm::vec4(0.5f, 0.5f, 0.0f, 0.0f));
			}
		}

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				update_normal_tangent_map(glm::ivec2(x, y));
			}
		}

		for (z = 0; z < m_size.z; ++z)
		{
			for (y = 0; y < m_size.y; ++y)
			{
				for (x = 0; x < m_size.x; ++x)
				{
					m_blend_map->set_pixel(x, y, z,
						0, 0, glm::vec4(0.0f));
				}
			}
		}

		m_albedo_maps.resize(get_layer_count());
		m_specular_maps.resize(get_layer_count());
		m_gloss_maps.resize(get_layer_count());
		m_height_maps.resize(get_layer_count());
		m_material_data.clear();
		m_material_data.resize(get_layer_count());

		for (i = 0; i < get_layer_count(); ++i)
		{
			set_albedo_map(albedo_map, i);
			set_specular_map(specular_map, i);
			set_gloss_map(gloss_map, i);
			set_height_map(height_map, i);
			set_default_specular(default_specular, i);
			set_default_gloss(default_gloss, i);
			set_default_height(default_height, i);
			set_blend_size(blend_size, i);
			set_use_blend_size_texture(use_blend_size_texture, i);
			set_use_specular_map(use_specular_map, i);
			set_use_gloss_map(use_gloss_map, i);
			set_use_height_map(use_height_map, i);
		}
	}

	void TerrainEditor::init(const ImageSharedPtr &displacement_map,
		const glm::vec3 &translation, const glm::uvec2 &size,
		const String &albedo_map, const String &specular_map,
		const String &gloss_map, const String &height_map,
		const glm::vec3 &default_specular, const float default_gloss,
		const float default_height, const float blend_size,
		const bool use_blend_size_texture, const bool use_specular_map,
		const bool use_gloss_map, const bool use_height_map)
	{
		init(translation, size, albedo_map, specular_map, gloss_map,
			height_map, default_specular, default_gloss,
			default_height, blend_size, use_blend_size_texture,
			use_specular_map, use_gloss_map, use_height_map);

		import_height_map(displacement_map);
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
			m_displacement_map->get_pixel_packed_uint32(index.x,
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
		glm::ivec2 idx, size;

		idx = index;
		size = glm::ivec2(m_size) - 1;

		p0 = get_position(idx);
		p1 = get_position(glm::min(idx + glm::ivec2(1, 0), size));
		p2 = get_position(glm::min(idx + glm::ivec2(0, 1), size));
		p3 = get_position(glm::min(idx + glm::ivec2(1, 1), size));

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
				m_displacement_map->get_width())) ||
			(index.y >= static_cast<Sint32>(
				m_displacement_map->get_height())))
		{
			return glm::vec3(0.0f);
		}

		return glm::normalize(get_position(index) - centre);
	}

	glm::vec3 TerrainEditor::get_normal(const glm::ivec2 &index) const
	{
		Vec3Array8 d;
		glm::vec3 centre, n;
		Uint32 i;

		centre = get_position(index);

		d[0] = get_direction(centre, index + glm::ivec2(-1, -1));
		d[1] = get_direction(centre, index + glm::ivec2( 0, -1));
		d[2] = get_direction(centre, index + glm::ivec2( 1, -1));
		d[3] = get_direction(centre, index + glm::ivec2( 1,  0));
		d[4] = get_direction(centre, index + glm::ivec2( 1,  1));
		d[5] = get_direction(centre, index + glm::ivec2( 0,  1));
		d[6] = get_direction(centre, index + glm::ivec2(-1,  1));
		d[7] = get_direction(centre, index + glm::ivec2(-1,  0));

		n = glm::vec3(0.0f);

		for (i = 0; i < 8; ++i)
		{
			n += glm::cross(d[i], d[(i + 1) % 8]);
		}

		return glm::normalize(n);
	}

	glm::vec2 TerrainEditor::get_uv(const glm::ivec2 &index) const
	{
		glm::vec2 uv, dudv;

		dudv = glm::vec2(m_dudv_map->get_pixel(index.x, index.y, 0,
			0, 0));

		dudv.x *= m_dudv_scale_offset.x;
		dudv.y *= m_dudv_scale_offset.y;
		dudv.x += m_dudv_scale_offset.z;
		dudv.y += m_dudv_scale_offset.w;

		uv = glm::vec2(index) * AbstractTerrain::get_patch_scale();

		return uv + dudv;
	}

	glm::vec2 TerrainEditor::get_uv_direction(const glm::vec2 &uv,
		const glm::ivec2 &index) const
	{
		if ((index.x < 0) || (index.y < 0) ||
			(index.x >= static_cast<Sint32>(
				m_dudv_map->get_width())) ||
			(index.y >= static_cast<Sint32>(
				m_dudv_map->get_height())))
		{
			return glm::vec2(0.0f);
		}

		return get_uv(index) - uv;
	}

	glm::uvec4 TerrainEditor::get_packed_normal_tangent(
		const glm::ivec2 &index) const
	{
		Vec3Array8 d;
		Vec2Array8 dudv;
		glm::vec4 temp;
		glm::uvec4 result;
		glm::vec3 centre, tangent, bitangent, normal;
		glm::vec2 uv;
		glm::uvec2 value;
		float r, tmp;
		Uint32 i, idx0, idx1;

		centre = get_position(index);

		d[0] = get_direction(centre, index + glm::ivec2(-1, -1));
		d[1] = get_direction(centre, index + glm::ivec2( 0, -1));
		d[2] = get_direction(centre, index + glm::ivec2( 1, -1));
		d[3] = get_direction(centre, index + glm::ivec2( 1,  0));
		d[4] = get_direction(centre, index + glm::ivec2( 1,  1));
		d[5] = get_direction(centre, index + glm::ivec2( 0,  1));
		d[6] = get_direction(centre, index + glm::ivec2(-1,  1));
		d[7] = get_direction(centre, index + glm::ivec2(-1,  0));

		uv = get_uv(index);

		dudv[0] = get_uv_direction(uv, index + glm::ivec2(-1, -1));
		dudv[1] = get_uv_direction(uv, index + glm::ivec2( 0, -1));
		dudv[2] = get_uv_direction(uv, index + glm::ivec2( 1, -1));
		dudv[3] = get_uv_direction(uv, index + glm::ivec2( 1,  0));
		dudv[4] = get_uv_direction(uv, index + glm::ivec2( 1,  1));
		dudv[5] = get_uv_direction(uv, index + glm::ivec2( 0,  1));
		dudv[6] = get_uv_direction(uv, index + glm::ivec2(-1,  1));
		dudv[7] = get_uv_direction(uv, index + glm::ivec2(-1,  0));

		tangent = glm::vec3(0.0f);
		bitangent = glm::vec3(0.0f);
		normal = glm::vec3(0.0f);

		for (i = 0; i < 8; ++i)
		{
			idx0 = i;
			idx1 = (i + 1) % 8;

			r = 1.0f;

			tmp = dudv[idx0].x * dudv[idx1].y -
				dudv[idx1].x * dudv[idx0].y;

			if (std::abs(tmp) > 0.0001f)
			{
				r = 1.0f / tmp;
			}

			tangent += glm::vec3(dudv[idx1].y * d[idx0] -
				dudv[idx0].y * d[idx1]) * r;

			bitangent += glm::vec3(dudv[idx0].x * d[idx1] -
				dudv[idx1].x * d[idx0]) * r;

			normal += glm::cross(d[idx0], d[idx1]);
		}

		tangent = glm::normalize(tangent);
		bitangent =  glm::normalize(bitangent);
		normal =  glm::normalize(normal);

		temp = MeshDataTool::get_gram_schmidth_orthogonalize_tangent(
			normal, tangent, bitangent);

		tangent = glm::vec3(temp);

		value = PackTool::encode_normal_optimized_uint8(normal);

		if (std::abs(1.0f - glm::dot(normal,
			PackTool::decode_normal_optimized_uint8(value)))
				> 0.01f)
		{
			value = get_best_normal(normal);
		}

		result.x = value.x;
		result.y = value.y;

		value = PackTool::encode_normal_optimized_uint8(tangent);

		if (std::abs(1.0f - glm::dot(tangent,
			PackTool::decode_normal_optimized_uint8(value)))
				> 0.01f)
		{
			value = get_best_normal(tangent);
		}

		result.z = value.x;
		result.w = value.y;

		return result;
	}

	glm::vec3 TerrainEditor::get_smooth_normal(const glm::vec2 &index)
		const
	{
		glm::vec3 p0, p1, p2, p3, t0, t1;
		glm::vec2 fract;
		glm::ivec2 idx, size;

		idx = index;
		size = glm::ivec2(m_size) - 1;

		p0 = get_normal(glm::clamp(idx, glm::ivec2(0), size));
		p1 = get_normal(glm::clamp(idx + glm::ivec2(1, 0),
			glm::ivec2(0), size));
		p2 = get_normal(glm::clamp(idx + glm::ivec2(0, 1),
			glm::ivec2(0), size));
		p3 = get_normal(glm::clamp(idx + glm::ivec2(1, 1),
			glm::ivec2(0), size));

		fract = glm::fract(index);

		t0 = glm::mix(p0, p1, fract.x);
		t1 = glm::mix(p2, p3, fract.x);

		return glm::mix(t0, t1, fract.y);
	}

	float TerrainEditor::get_blend_slope(const glm::ivec2 &index) const
	{
		glm::vec2 position, dudv;

		dudv = glm::vec2(m_dudv_map->get_pixel(index.x, index.y,
			0, 0, 0));

		dudv.x *= m_dudv_scale_offset.x;
		dudv.y *= m_dudv_scale_offset.y;
		dudv.x += m_dudv_scale_offset.z;
		dudv.y += m_dudv_scale_offset.w;

		dudv /= AbstractTerrain::get_patch_scale();

		position = glm::vec2(index) - dudv;

		return 1.0f - get_smooth_normal(position).z;
	}

	void TerrainEditor::update_normal_tangent_map(const glm::ivec2 &index)
	{
		glm::uvec4 value;

		value = get_packed_normal_tangent(index);

		m_normal_tangent_map->set_pixel_uint(index.x, index.y, 0, 0, 0,
			value);
	}

	void TerrainEditor::update_normal_tangent_map(const Ivec2Set &indices)
	{
		BOOST_FOREACH(const glm::ivec2 &index, indices)
		{
			update_normal_tangent_map(index);
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
		glm::vec3 position;
		glm::ivec2 index, min, max, size;
		Sint32 x, y;
		float distance, min_distance;

		position = world_position - get_translation();

		min_distance = std::numeric_limits<float>::max();

		min = glm::ivec2(glm::vec2(position -
			AbstractTerrain::get_vector_scale()) /
			AbstractTerrain::get_patch_scale());

		max = glm::ivec2(glm::vec2(position +
			AbstractTerrain::get_vector_scale()) /
			AbstractTerrain::get_patch_scale()) + 1;

		size.x = m_displacement_map->get_width();
		size.y = m_displacement_map->get_height();
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
					position);

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

	bool TerrainEditor::get_neares_data(const glm::vec3 &world_position,
		const glm::vec3 &dir, glm::vec3 &position, glm::vec3 &normal)
		const
	{
		glm::vec3 pos0, pos1, dir0, dir1, temp0, temp1, pos;
		glm::ivec2 index, min, max, size, result;
		Sint32 x, y;
		float distance, min_distance, dist0, dist1;

		pos = world_position - get_translation();
		dir0 = 1.0f - glm::min(glm::abs(dir), 1.0f);
		dir1 = 1.0f - dir0;
		pos0 = pos * dir0;
		pos1 = pos * dir1;

		min_distance = std::numeric_limits<float>::max();

		min = glm::ivec2(glm::vec2(pos -
			AbstractTerrain::get_vector_scale()) /
			AbstractTerrain::get_patch_scale());

		max = glm::ivec2(glm::vec2(pos +
			AbstractTerrain::get_vector_scale()) /
			AbstractTerrain::get_patch_scale()) + 1;

		size.x = m_displacement_map->get_width();
		size.y = m_displacement_map->get_height();
		size -= 1;

		min = glm::clamp(min, glm::ivec2(0), size);
		max = glm::clamp(max, glm::ivec2(0), size);

		for (y = min.y; y <= max.y; ++y)
		{
			for (x = min.x; x <= max.x; ++x)
			{
				index.x = x;
				index.y = y;

				dist0 = glm::distance(pos0,
					get_position(index) * dir0);

				dist1 = glm::distance(pos1,
					get_position(index) * dir1);

				distance = dist0 * dist0 + dist1;

				if (distance < min_distance)
				{
					min_distance = distance;
					result = index;
				}
			}
		}

		if (min_distance >= std::numeric_limits<float>::max())
		{
			return false;
		}

		normal = get_normal(result);
		position = get_position(result) + get_translation();

		return true;
	}

	void TerrainEditor::set_material(const String &albedo_map,
		const String &specular_map, const String &gloss_map,
		const String &height_map, const glm::vec3 &default_specular,
		const float default_gloss, const float default_height,
		const float blend_size, const bool use_blend_size_texture,
		const bool use_specular_map, const bool use_gloss_map,
		const bool use_height_map, const Uint16 index)
	{
		set_albedo_map(albedo_map, index);
		set_specular_map(specular_map, index);
		set_gloss_map(gloss_map, index);
		set_height_map(height_map, index);
		set_default_specular(default_specular, index);
		set_default_gloss(default_gloss, index);
		set_default_height(default_height, index);
		set_blend_size(blend_size, index);
		set_use_blend_size_texture(use_blend_size_texture, index);
		set_use_specular_map(use_specular_map, index);
		set_use_gloss_map(use_gloss_map, index);
		set_use_height_map(use_height_map, index);
	}

	void TerrainEditor::get_material(String &albedo_map,
		String &specular_map, String &gloss_map, String &height_map,
		glm::vec3 &default_specular, float &default_gloss,
		float &default_height, float &blend_size,
		bool &use_blend_size_texture, bool &use_specular_map,
		bool &use_gloss_map, bool &use_height_map, const Uint16 index)
		const
	{
		albedo_map = get_albedo_map(index);
		specular_map = get_specular_map(index);
		gloss_map = get_gloss_map(index);
		height_map = get_height_map(index);
		default_specular = get_default_specular(index);
		default_gloss = get_default_gloss(index);
		default_height = get_default_height(index);
		blend_size = get_blend_size(index);
		use_blend_size_texture = get_use_blend_size_texture(index);
		use_specular_map = get_use_specular_map(index);
		use_gloss_map = get_use_gloss_map(index);
		use_height_map = get_use_height_map(index);
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

				m_displacement_map->set_pixel(x, y, 0, 0, 0,
					glm::vec4(0.5f, 0.5f, height, 0));
			}
		}

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				update_normal_tangent_map(glm::ivec2(x, y));
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

	void TerrainEditor::get_all_blend_values(const Uint16 layer,
		ImageValueVector &blend_values) const
	{
		Uint32 x, y;

		blend_values.clear();

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				get_blend_values(x, y, layer, blend_values);
			}
		}
	}

	void TerrainEditor::get_all_blend_values(
		ImageValuesVector &blend_values) const
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
		const ImageSharedPtr &normal_tangent_map,
		const ImageSharedPtr &dudv_map,
		const ImageSharedPtr &blend_map,
		const StringVector &albedo_maps,
		const StringVector &specular_maps,
		const StringVector &gloss_maps,
		const StringVector &height_maps,
		const TerrainMaterialData &material_data,
		const glm::vec4 &dudv_scale_offset,
		const glm::vec3 &translation, const glm::uvec2 &size)
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

		m_displacement_map = displacement_map->decompress(false,
			true, false);

		m_uv_tool.reset(new UvTool(m_displacement_map));

		import_dudv_map(dudv_map, dudv_scale_offset);
		rebuild_normal_tangent_map();
		import_blend_map(blend_map);

		m_albedo_maps = albedo_maps;
		m_specular_maps = specular_maps;
		m_gloss_maps = gloss_maps;
		m_height_maps = height_maps;
		m_material_data = material_data;
		m_dudv_scale_offset = dudv_scale_offset;

		m_albedo_maps.resize(get_layer_count(), m_albedo_maps[0]);
		m_specular_maps.resize(get_layer_count(), m_specular_maps[0]);
		m_gloss_maps.resize(get_layer_count(), m_gloss_maps[0]);
		m_height_maps.resize(get_layer_count(), m_height_maps[0]);
		m_material_data.resize(get_layer_count());
		m_translation = translation;

		m_enabled = true;
	}

	void TerrainEditor::import_blend_map(const ImageSharedPtr &blend_map)
	{
		ImageSharedPtr blend_image;
		glm::vec4 value;
		glm::uvec3 size;
		Uint32 x, y, z;

		m_blend_map = boost::make_shared<Image>(
			String(UTF8("blend map")), false, tft_rgba4, m_size, 0,
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

					m_blend_map->set_pixel(x, y, z, 0, 0,
						value);
				}
			}
		}
	}

	void TerrainEditor::relax_uv(const AbstractProgressSharedPtr &progress,
		const Uint16 count, const bool use_simd)
	{
		m_uv_tool->relax_uv(progress, count, use_simd);

		m_uv_tool->convert(m_dudv_map, m_dudv_scale_offset);
	}

	void TerrainEditor::rebuild_normal_tangent_map()
	{
		Uint32 x, y;

		m_normal_tangent_map = boost::make_shared<Image>(
			String(UTF8("normal tangent map")), false, tft_rgba8,
			glm::uvec3(m_size.x, m_size.y, 0), 0, false);

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				update_normal_tangent_map(glm::ivec2(x, y));
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

		m_dudv_map = boost::make_shared<Image>(
			String(UTF8("dudv map")), false, tft_rg16,
			glm::uvec3(m_size.x, m_size.y, 0), 0, false);

		if (dudv_map.get() == nullptr)
		{
			return;
		}

		dudv_image = dudv_map->decompress(false, true, false);

		size = glm::min(m_size, dudv_image->get_size());
		size = glm::max(size, glm::uvec3(1));

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				value = dudv_image->get_pixel(x, y, 0, 0, 0);

				m_dudv_map->set_pixel(x, y, 0, 0, 0, value);
			}
		}

		m_uv_tool->import(m_dudv_map, m_dudv_scale_offset);
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
					value = m_blend_map->get_pixel(x, y,
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
					value = m_blend_map->get_pixel(x, y,
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
					if (mask[i] &&
						!get_use_blend_size_texture(i))
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
					value = m_blend_map->get_pixel(x, y,
						z, 0, 0);

					for (i = 0; i < 4; ++i)
					{
						if (mask[z * 4 + i])
						{
							value[i] = 0.0f;
						}
					}

					m_blend_map->set_pixel(x, y,
						z, 0, 0, value);
				}
			}
		}
	}

	void TerrainEditor::pack_layers()
	{
		Vec4Array16 values;
		glm::vec4 value;
		glm::vec3 default_specular;
		glm::uvec3 size;
		BitSet64 mask;
		String albedo_map, specular_map, gloss_map, height_map;
		Uint32 x, y, z, i, count, layer, channel;
		Sint32 index;
		float default_gloss, default_height, blend_size;
		bool use_blend_size_texture, use_specular_map, use_gloss_map;
		bool use_height_map;

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
					value = m_blend_map->get_pixel(x, y,
						z, 0, 0);

					for (i = 0; i < 4; ++i)
					{
						if (!mask[z * 4 + i + 1])
						{
							continue;
						}

						layer = index / 4;
						channel = index % 4;

						if (index >= 0)
						{
							values[layer][channel] =
								value[i];
						}

						index++;
					}
				}

				for (z = 0; z < size.z; ++z)
				{
					m_blend_map->set_pixel(x, y, z, 0, 0,
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
				get_material(albedo_map, specular_map,
					gloss_map, height_map,
					default_specular, default_gloss,
					default_height, blend_size,
					use_blend_size_texture,
					use_specular_map, use_gloss_map,
					use_height_map, i);

				set_material(albedo_map, specular_map,
					gloss_map, height_map,
					default_specular, default_gloss,
					default_height, blend_size,
					use_blend_size_texture,
					use_specular_map, use_gloss_map,
					use_height_map, index);

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
					value = m_blend_map->get_pixel(x, y,
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
		m_displacement_map.reset();
		m_normal_tangent_map.reset();
		m_dudv_map.reset();
		m_blend_map.reset();
		m_uv_tool.reset();
		m_material_data.clear();
		m_albedo_maps.clear();
		m_specular_maps.clear();
		m_gloss_maps.clear();
		m_height_maps.clear();
		m_size = glm::vec3(0);
		m_dudv_scale_offset = glm::vec4(0.0f);
		m_enabled = false;
	}

	glm::vec3 TerrainEditor::get_displacement_value(
		const glm::uvec2 &vertex) const
	{
		return AbstractTerrain::get_offset_scaled_rgb10_a2(
			m_displacement_map->get_pixel_packed_uint32(vertex.x,
				vertex.y, 0, 0, 0));
	}

	glm::vec3 TerrainEditor::get_normal_value(const glm::uvec2 &vertex)
		const
	{
		return get_normal(glm::ivec2(vertex));
	}

	void TerrainEditor::fill_blend_layer(const float strength,
		const BlendEffectType effect, const Uint16 layer)
	{
		Uint32 x, y, z, channel;

		assert(layer < (m_size.z * 4));

		channel = layer % 4;
		z = layer / 4;

		#pragma omp parallel for private(x)
		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				glm::vec4 value;
				glm::ivec2 index;
				float mask_value, slope;

				value = m_blend_map->get_pixel(x, y, z, 0, 0);

				index.x = x;
				index.y = y;

				mask_value = value[channel];

				slope = get_blend_slope(index);

				switch (effect)
				{
					case bet_set:
						mask_value = strength;
						break;
					case bet_slope:
						mask_value = slope;
						if (strength < 1.0f)
						{
							mask_value =
								glm::smoothstep(
								1.0f - strength,
								1.0f, slope);
						}
						break;
					case bet_inverse_slope:
						mask_value = 1.0f - slope;
						if (strength < 1.0f)
						{
							mask_value =
								glm::smoothstep(
								1.0f - strength,
								1.0f,
								1.0f - slope);
						}
						break;
				};

				value[channel] = glm::clamp(mask_value, 0.0f,
					1.0f);

				m_blend_map->set_pixel(x, y, z, 0, 0, value);
			}
		}		
	}

	Uint16 TerrainEditor::get_used_layer_count() const
	{
		glm::vec4 value;
		glm::uvec3 size;
		Uint32 x, y, z, i, result;

		size = glm::max(m_size, glm::uvec3(1));

		result = 0;

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				for (z = 0; z < size.z; ++z)
				{
					value = m_blend_map->get_pixel(x, y,
						z, 0, 0);

					for (i = 0; i < 4; ++i)
					{
						if (value[i] > 0.0f)
						{
							result = std::max(
								result,
								i + z * 4 + 1);
						}
					}
				}
			}
		}

		// +1 for the first layer that use no blending
		return result + 1;
	}

	ImageSharedPtr TerrainEditor::get_partial_blend_map(const Uint16 layer)
		const
	{
		ImageSharedPtr result;
		glm::uvec3 size;
		Uint32 x, y, z;
		Uint16 value;

		size.z = std::max(1, (layer + 2) / 4);
		size.z = std::min(size.z, m_size.z);
		size.x = std::max(m_size.x, 1u);
		size.y = std::max(m_size.y, 1u);

		result = boost::make_shared<Image>(String(UTF8("blend map")),
			false, tft_rgba4, size, 0, true);

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				for (z = 0; z < size.z; ++z)
				{
					value = m_blend_map->
						get_pixel_packed_uint16(x, y,
							z, 0, 0);
					result->set_pixel_packed_uint16(x, y,
						z, 0, 0, value);
				}
			}
		}

		return result;
	}

	void TerrainEditor::swap_blend_layers(const Uint16 idx0,
		const Uint16 idx1)
	{
		Uvec4Array4 values;
		glm::vec3 default_specular_0, default_specular_1;
		String albedo_map_0, specular_map_0, gloss_map_0, height_map_0;
		String albedo_map_1, specular_map_1, gloss_map_1, height_map_1;
		float default_gloss_0, default_height_0, blend_size_0;
		float default_gloss_1, default_height_1, blend_size_1;
		Uint32 x, y, z, l0, l1;
		bool use_blend_size_texture_0, use_specular_map_0;
		bool use_blend_size_texture_1, use_specular_map_1;
		bool use_gloss_map_0, use_height_map_0;
		bool use_gloss_map_1, use_height_map_1;

		assert(idx0 > 0);
		assert(idx1 > 0);

		l0 = idx0 - 1;
		l1 = idx1 - 1;

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				for (z = 0; z < m_size.z; ++z)
				{
					values[z] = m_blend_map->
						get_pixel_uint(x, y, z, 0, 0);
				}

				std::swap(values[l0 / 4][l0 % 4],
					values[l1 / 4][l1 % 4]);

				for (z = 0; z < m_size.z; ++z)
				{
					m_blend_map->set_pixel_uint(x, y, z, 0,
						0, values[z]);
				}
			}
		}

		get_material(albedo_map_0, specular_map_0, gloss_map_0,
			height_map_0, default_specular_0, default_gloss_0,
			default_height_0, blend_size_0,
			use_blend_size_texture_0, use_specular_map_0,
			use_gloss_map_0, use_height_map_0, idx0);

		get_material(albedo_map_1, specular_map_1, gloss_map_1,
			height_map_1, default_specular_1, default_gloss_1,
			default_height_1, blend_size_1,
			use_blend_size_texture_1, use_specular_map_1,
			use_gloss_map_1, use_height_map_1, idx1);

		set_material(albedo_map_0, specular_map_0, gloss_map_0,
			height_map_0, default_specular_0, default_gloss_0,
			default_height_0, blend_size_0,
			use_blend_size_texture_0, use_specular_map_0,
			use_gloss_map_0, use_height_map_0, idx1);

		set_material(albedo_map_1, specular_map_1, gloss_map_1,
			height_map_1, default_specular_1, default_gloss_1,
			default_height_1, blend_size_1,
			use_blend_size_texture_1, use_specular_map_1,
			use_gloss_map_1, use_height_map_1, idx0);
	}

	void TerrainEditor::move_blend_layer(const Uint16 idx0,
		const Uint16 idx1)
	{
		Uvec4Array4 values;
		glm::vec3 default_specular_0, default_specular_1;
		String albedo_map_0, specular_map_0, gloss_map_0, height_map_0;
		String albedo_map_1, specular_map_1, gloss_map_1, height_map_1;
		float default_gloss_0, default_height_0, blend_size_0;
		float default_gloss_1, default_height_1, blend_size_1;
		Uint32 x, y, z, i, min, max;
		bool use_blend_size_texture_0, use_specular_map_0;
		bool use_blend_size_texture_1, use_specular_map_1;
		bool use_gloss_map_0, use_height_map_0;
		bool use_gloss_map_1, use_height_map_1;

		assert(idx0 > 0);
		assert(idx1 > 0);

		min = std::min(idx0, idx1) - 1;
		max = std::max(idx0, idx1) - 1;

		for (y = 0; y < m_size.y; ++y)
		{
			for (x = 0; x < m_size.x; ++x)
			{
				for (z = 0; z < m_size.z; ++z)
				{
					values[z] = m_blend_map->
						get_pixel_uint(x, y, z, 0, 0);
				}

				for (i = min; i < max; ++i)
				{
					std::swap(values[i / 4][i % 4],
						values[max / 4][max % 4]);
				}

				for (z = 0; z < m_size.z; ++z)
				{
					m_blend_map->set_pixel_uint(x, y, z, 0,
						0, values[z]);
				}
			}
		}

		min = std::min(idx0, idx1);
		max = std::max(idx0, idx1);

		for (i = min; i < max; ++i)
		{
			get_material(albedo_map_0, specular_map_0, gloss_map_0,
				height_map_0, default_specular_0,
				default_gloss_0, default_height_0, blend_size_0,
				use_blend_size_texture_0, use_specular_map_0,
				use_gloss_map_0, use_height_map_0, i);

			get_material(albedo_map_1, specular_map_1, gloss_map_1,
				height_map_1, default_specular_1,
				default_gloss_1, default_height_1, blend_size_1,
				use_blend_size_texture_1, use_specular_map_1,
				use_gloss_map_1, use_height_map_1, max);

			set_material(albedo_map_0, specular_map_0, gloss_map_0,
				height_map_0, default_specular_0,
				default_gloss_0, default_height_0, blend_size_0,
				use_blend_size_texture_0, use_specular_map_0,
				use_gloss_map_0, use_height_map_0, max);

			set_material(albedo_map_1, specular_map_1, gloss_map_1,
				height_map_1, default_specular_1,
				default_gloss_1, default_height_1, blend_size_1,
				use_blend_size_texture_1, use_specular_map_1,
				use_gloss_map_1, use_height_map_1, i);
		}
	}

}
