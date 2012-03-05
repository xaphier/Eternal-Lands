/****************************************************************************
 *            terraineditor.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terraineditor.hpp"
#include "height.hpp"
#include "image.hpp"

namespace eternal_lands
{

	void TerrainEditor::get_heights(const Uint16Array2 &vertex,
		const float radius, HeightVector &heights)
	{
		glm::vec2 centre, point;
		Uint32 x, y;
		Uint32 min_x, min_y, max_x, max_y;
		float tmp, temp, sqr_radius;

		tmp = static_cast<float>(vertex[0]) - radius;
		min_x = boost::numeric_cast<Uint32>(std::max(0.0f, tmp));

		tmp = static_cast<float>(vertex[1]) - radius;
		min_y = boost::numeric_cast<Uint32>(std::max(0.0f, tmp));

		tmp = static_cast<float>(vertex[0]) + radius;
//		temp = scene_page_read_write->get_terrain_width() - 1.0f;
		max_x = boost::numeric_cast<Uint32>(std::min(temp, tmp));

		tmp = static_cast<float>(vertex[1]) + radius;
//		temp = scene_page_read_write->get_terrain_height() - 1.0f;
		max_y = boost::numeric_cast<Uint32>(std::min(temp, tmp));

		heights.clear();

		centre = glm::vec2(vertex[0], vertex[1]);

		sqr_radius = radius * radius;

		for (x = min_x; x <= max_x; x++)
		{
			for (y = min_y; y <= max_y; y++)
			{
				point = glm::vec2(x, y);

				if (glm::distance2(centre, point) <= sqr_radius)
				{
					heights.push_back(Height(x, y, 0));
				}
			}
		}

//		scene_page_read_write->get_terrain_heights(heights);
	}

	void TerrainEditor::change_heights(const Uint16Array2 &vertex,
		const float strength, const float radius,
		const EditorBrushType brush_type, HeightVector &heights)
	{
		glm::vec2 centre;
		float average;
		Uint32 i;

		centre = glm::vec2(vertex[0], vertex[1]);

		if ((brush_type == ebt_linear_smooth) ||
			(brush_type == ebt_quadratic_smooth))
		{
			if (heights.size() < 2)
			{
				return;
			}

			average = 0.0f;

			BOOST_FOREACH(Height &height, heights)
			{
				average += height.get_value();
			}

			average /= heights.size() * 255.0f;
		}
		else
		{
			average = 0.0f;
		}

		for (i = 0; i < heights.size(); i++)
		{
			glm::vec2 point;
			float value;

			value = heights[i].get_value() / 255.0f;
			point[0] = heights[i].get_x();
			point[1] = heights[i].get_y();

			value = calc_brush_effect(centre, point, value,
				average, strength, radius, brush_type);

			heights[i].set_value(value * 255.0f + 0.5f);
		}
	}

	float TerrainEditor::calc_brush_effect(const glm::vec2 &centre,
		const glm::vec2 &point, const float value, const float average,
		const float strength, const float radius,
		const EditorBrushType brush_type)
	{
		float dist, tmp, result;

		result = value;

		switch (brush_type)
		{
			case ebt_set:
				result = strength;
				break;
			case ebt_const:
				result += strength;
				break;
			case ebt_linear:
				dist = glm::distance(centre, point);
				tmp = std::max(1.0f - dist / radius, 0.0f);
				result += tmp * strength;
				break;
			case ebt_quadratic:
				dist = glm::distance2(centre, point);
				tmp = std::max(1.0f - dist / (radius * radius), 0.0f);
				result += tmp * strength;
				break;
			case ebt_linear_smooth:
				dist = glm::distance(centre, point);
				tmp = std::max(1.0f - dist / radius, 0.0f);
				result -= (value - average) * strength * tmp;
				break;
			case ebt_quadratic_smooth:
				dist = glm::distance2(centre, point);
				tmp = std::max(1.0f - dist / (radius * radius), 0.0f);
				result -= (value - average) * strength * tmp;
				break;
		};

		return std::max(0.0f, std::min(result, 1.0f));
	}

	void TerrainEditor::get_blend_values(const Uint16Array2 &vertex,
		const float radius, ImageValueVector &blend_values) const
	{
		glm::vec2 centre, point;
		Uint32 x, y;
		Uint32 min_x, min_y, max_x, max_y;
		float tmp, temp, sqr_radius;

		tmp = static_cast<float>(vertex[0]) - radius;
		min_x = boost::numeric_cast<Uint32>(std::max(0.0f, tmp));

		tmp = static_cast<float>(vertex[1]) - radius;
		min_y = boost::numeric_cast<Uint32>(std::max(0.0f, tmp));

		tmp = static_cast<float>(vertex[0]) + radius;
		temp = m_blend_image->get_width() - 1.0f;
		max_x = boost::numeric_cast<Uint32>(std::min(temp, tmp));

		tmp = static_cast<float>(vertex[1]) + radius;
		temp = m_blend_image->get_height() - 1.0f;
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

					value.set_value(
						m_blend_image->get_pixel(x, y,
							0, 0, 0));
					blend_values.push_back(value);
				}
			}
		}
	}

	void TerrainEditor::change_blend_values(const Uint16Array2 &position,
		const Uint32 index, const float strength, const float radius,
		const EditorBrushType brush_type,
		ImageValueVector &blend_values)
	{
		glm::vec2 centre;
		float average;
		Uint32 i;

		centre = glm::vec2(position[0], position[1]);

		if ((brush_type == ebt_linear_smooth) ||
			(brush_type == ebt_quadratic_smooth))
		{
			if (blend_values.size() < 2)
			{
				return;
			}

			average = 0.0f;

			BOOST_FOREACH(ImageValue &blend_value, blend_values)
			{
				average += get_blend_value(
					blend_value.get_value(), index);
			}

			average /= blend_values.size();
		}
		else
		{
			average = 0.5f;
		}

		for (i = 0; i < blend_values.size(); i++)
		{
			glm::vec4 values;
			glm::vec2 point;
			float value;

			values = blend_values[i].get_value();
			value = get_blend_value(values, index);
			point[0] = blend_values[i].get_x();
			point[1] = blend_values[i].get_y();

			value = calc_brush_effect(centre, point, value,
				average, strength, radius, brush_type);

			set_blend_value(value, index, values);

			blend_values[i].set_value(values);
		}
	}

	float TerrainEditor::get_blend_value(const glm::vec4 &blend,
		const Uint32 index)
	{
		switch (index)
		{
			case 0:
				return (1.0f - blend[0]) * (1.0f - blend[1]);
			case 1:
				return blend[0] * (1.0f - blend[1]);
			case 2:
				return (1.0f - blend[2]) * blend[1];
			case 3:
				return blend[2] * blend[1];
			default:
				return 0.0f;
		}
	}

	glm::vec4 TerrainEditor::get_blend_values(const glm::vec4 &blend)
	{
		glm::vec4 result;
		Uint32 i;

		for (i = 0; i < 4; i++)
		{
			result[i] = get_blend_value(blend, i);
		}

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

		assert(std::abs(value - get_blend_value(blend, index)) <
			epsilon);
	}

	EditorBrushType TerrainEditor::get_brush_type(const int brush_type)
	{
		switch (brush_type)
		{
			case 0:
				return ebt_set;
			case 1:
				return ebt_const;
			case 2:
				return ebt_linear;
			case 3:
				return ebt_quadratic;
			case 4:
				return ebt_linear_smooth;
			case 5:
				return ebt_quadratic_smooth;
			default:
				return ebt_set;
		}
	}

	const String TerrainEditor::get_blend_image_name() const
	{
		if ((m_blend_image.get() != 0))
		{
			return m_blend_image->get_name();
		}
		else
		{
			return String(UTF8(""));
		}
	}

	void TerrainEditor::set_blend_image_name(const String &blend_image)
	{
		if ((m_blend_image.get() != 0) && !blend_image.get().empty())
		{
			m_blend_image->set_name(blend_image);
		}
	}

}
