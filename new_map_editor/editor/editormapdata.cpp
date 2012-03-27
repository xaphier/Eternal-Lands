/****************************************************************************
 *            editormapdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editormapdata.hpp"
#include "lightdata.hpp"
#include "objectdata.hpp"
#include "exceptions.hpp"
#include "particledata.hpp"
#include "image.hpp"
#include "editorscene.hpp"
#include "freeidsmanager.hpp"
#include "materialdescription.hpp"
#include "materialdescriptioncache.hpp"
#include "meshdatacache.hpp"
#include "abstractmaploader.hpp"

namespace eternal_lands
{

	EditorMapData::EditorMapData(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system)
	{
		m_scene.reset(new EditorScene(global_vars, file_system));
		m_scene->set_main_light_ambient(glm::vec3(0.1f));
		m_scene->set_main_light_color(glm::vec3(0.8f));
		m_scene->set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		m_scene->set_lights(false);
	}

	EditorMapData::~EditorMapData() throw()
	{
	}

	void EditorMapData::set_draw_lights(const bool draw_lights)
	{
		m_scene->set_draw_lights(draw_lights);
	}

	void EditorMapData::set_draw_light_spheres(
		const bool draw_light_spheres)
	{
		m_scene->set_draw_light_spheres(draw_light_spheres);
	}

	void EditorMapData::set_lights_enabled(const bool enabled)
	{
		if (enabled)
		{
			m_scene->set_main_light_color(glm::vec3(0.1f));
			m_scene->set_lights(true);
		}
		else
		{
			m_scene->set_main_light_color(glm::vec3(0.9f));
			m_scene->set_lights(false);
		}
	}

	void EditorMapData::init()
	{
		m_scene->init();
	}

	void EditorMapData::set_view_matrix(const glm::mat4 &view_matrix)
	{
		m_scene->set_view_matrix(view_matrix);
	}

	void EditorMapData::set_perspective(const float fov, const float aspect,
		const float z_near, const float z_far)
	{
		m_scene->set_perspective(fov, aspect, z_near, z_far);
	}

	void EditorMapData::set_view_port(const glm::uvec4 &view_port)
	{
		m_scene->set_view_port(view_port);
	}

	const glm::mat4 &EditorMapData::get_projection_matrix() const
	{
		return m_scene->get_projection_matrix();
	}

	void EditorMapData::set_ambient(const glm::vec3 &color)
	{
		m_scene->set_ambient(color);
	}

	const glm::vec3 &EditorMapData::get_ambient() const
	{
		return m_scene->get_ambient();
	}

	void EditorMapData::add_object(const EditorObjectData &object)
	{
		m_objects.insert(std::pair<Uint32,
			EditorObjectData>(object.get_id(), object));
		m_scene->add_object(object, object.get_material_names());
	}

	void EditorMapData::add_light(const LightData &light)
	{
		m_lights.insert(std::pair<Uint32,
			LightData>(light.get_id(), light));
		m_scene->add_light(light);
	}

	void EditorMapData::add_particle(const ParticleData &particle)
	{
		m_particles.insert(std::pair<Uint32,
			ParticleData>(particle.get_id(), particle));
//		m_scene->add_particle(particle);
	}

	void EditorMapData::modify_object(const EditorObjectData &object)
	{
		m_objects.erase(object.get_id());
		m_scene->remove_object(object.get_id());

		m_objects.insert(std::pair<Uint32,
			EditorObjectData>(object.get_id(), object));
		m_scene->add_object(object, object.get_material_names());
	}

	void EditorMapData::modify_light(const LightData &light)
	{
		m_lights.erase(light.get_id());
		m_scene->remove_light(light.get_id());

		m_lights.insert(std::pair<Uint32,
			LightData>(light.get_id(), light));
		m_scene->add_light(light);
	}

	void EditorMapData::modify_particle(const ParticleData &particle)
	{
		m_particles.erase(particle.get_id());
//		m_scene->remove_particle(particle.get_id());

		m_particles.insert(std::pair<Uint32,
			ParticleData>(particle.get_id(), particle));
//		m_scene->add_particle(particle);
	}

	bool EditorMapData::remove_object(const Uint32 id)
	{
		m_objects.erase(id);
		m_scene->remove_object(id);

		if ((id == m_id) && (m_renderable == rt_object))
		{
			m_renderable = rt_none;

			return true;
		}

		return false;
	}

	bool EditorMapData::remove_light(const Uint32 id)
	{
		m_lights.erase(id);
		m_scene->remove_light(id);

		if ((id == m_id) && (m_renderable == rt_light))
		{
			m_renderable = rt_none;

			return true;
		}

		return false;
	}

	bool EditorMapData::remove_particle(const Uint32 id)
	{
		m_particles.erase(id);
//		m_scene->remove_particle(id);

		if ((id == m_id) && (m_renderable == rt_particle))
		{
			m_renderable = rt_none;

			return true;
		}

		return false;
	}

	bool EditorMapData::get_object(const Uint32 id,
		EditorObjectData &object) const
	{
		std::map<Uint32, EditorObjectData>::const_iterator found;

		found = m_objects.find(id);

		if (found == m_objects.end())
		{
			return false;
		}

		object = found->second;
		return true;
	}

	bool EditorMapData::get_light(const Uint32 id,
		LightData &light) const
	{
		std::map<Uint32, LightData>::const_iterator found;

		found = m_lights.find(id);

		if (found == m_lights.end())
		{
			return false;
		}

		light = found->second;
		return true;
	}

	bool EditorMapData::get_particle(const Uint32 id,
		ParticleData &particle) const
	{
		std::map<Uint32, ParticleData>::const_iterator found;

		found = m_particles.find(id);

		if (found == m_particles.end())
		{
			return false;
		}

		particle = found->second;
		return true;
	}

	void EditorMapData::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
		MaterialDescriptionVector materials;
		StringStream str;
		String file_name;
		Transformation transformation;
		glm::vec3 offset;
		Uint32 id;

		RANGE_CECK(x, m_tile_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK(y, m_tile_map.shape()[1],
			UTF8("index value too big"));

		id = m_scene->get_free_ids()->get_object_id(x + (y << 10),
			it_tile_object);

		m_scene->remove_object(id);

		m_tile_map[x][y] = tile;

		if (tile == 255)
		{
			return;
		}

		m_scene->get_free_ids()->use_object_id(id);

		offset.x = x * AbstractMapLoader::get_tile_size();
		offset.y = y * AbstractMapLoader::get_tile_size();

		if ((tile == 0) || (tile > 230))
		{
			offset.z = -0.2501f;
		}
		else
		{
			offset.z = -0.0011f;
		}

		assert(glm::all(glm::lessThanEqual(glm::abs(offset),
			glm::vec3(1e7f))));

		transformation.set_translation(offset);
		transformation.set_scale(3.0f);

		if ((tile != 0) && (tile != 240))
		{
			str << UTF8("tile") << static_cast<Uint16>(tile);
		}
		else
		{
			if (tile == 240)
			{
				str << UTF8("lava");
			}
			else
			{
				str << UTF8("water");
			}
		}

		materials.push_back(m_scene->get_scene_resources(
			).get_material_description_cache(
			)->get_material_description(String(str.str())));

		m_scene->add_object(ObjectData(transformation,
			String(UTF8("plane_4")), 0.0f, id, st_none,
			bt_disabled), materials);
	}

	Uint16 EditorMapData::get_tile(const Uint16 x, const Uint16 y) const
	{
		RANGE_CECK(x, m_tile_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK(y, m_tile_map.shape()[1],
			UTF8("index value too big"));

		return m_tile_map[x][y];
	}

	Uint16Array2 EditorMapData::get_tile_offset(const glm::vec2 &point)
		const
	{
		Uint16Array2 result;

		result[0] = std::max(0.0f, point.x / 3);
		result[1] = std::max(0.0f, point.y / 3);

		return result;
	}

	void EditorMapData::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		RANGE_CECK(x, m_height_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK(y, m_height_map.shape()[1],
			UTF8("index value too big"));

		m_height_map[x][y] = height;
	}

	void EditorMapData::set_heights(const HeightVector &heights)
	{
		BOOST_FOREACH(const Height &height, heights)
		{
			set_height(height.get_x(), height.get_y(),
				height.get_value());
		}
	}

	void EditorMapData::set_terrain_heights(const HeightVector &heights,
		const Uint16 id)
	{
		glm::uvec4 data;

		BOOST_FOREACH(const Height &height, heights)
		{
			data = m_height_image->get_pixel_uint(height.get_x(),
				height.get_y(), 0, 0, 0);

			data.x = height.get_value();

			m_height_image->set_pixel_uint(height.get_x(),
				height.get_y(), 0, 0, 0, data);
		}
	}

	void EditorMapData::set_terrain_albedo_map(const String &name,
		const Uint16 index, const Uint16 id)
	{
	}

	void EditorMapData::set_terrain_blend_map(const String &name,
		const Uint16 id)
	{
	}

	void EditorMapData::set_terrain_height_map(const String &name,
		const Uint16 id)
	{
	}

	void EditorMapData::set_terrain_dudv_map(const String &name,
		const Uint16 id)
	{
	}

	String tmp;

	const String &EditorMapData::get_terrain_albedo_map(const Uint16 index,
		const Uint16 id) const
	{
		return tmp;
	}

	const String &EditorMapData::get_terrain_blend_map(const Uint16 id)
		const
	{
		return tmp;
	}

	const String &EditorMapData::get_terrain_height_map(const Uint16 id)
		const
	{
		return tmp;
	}

	const String &EditorMapData::get_terrain_dudv_map(const Uint16 id)
		const
	{
		return tmp;
	}

	Uint32 EditorMapData::get_free_object_id() const
	{
		return m_scene->get_free_ids()->get_next_free_object_id(
			it_3d_object);
	}

	Uint32 EditorMapData::get_free_light_id() const
	{
		return m_scene->get_free_ids()->get_next_free_light_id();
	}

	void EditorMapData::load_map(const String &name)
	{
		m_scene->load_map(name, *this);
	}

	void EditorMapData::draw()
	{
		m_scene->cull();
		m_scene->draw();
	}

	void EditorMapData::select(const Uint16Array2 &position,
		const Uint16Array2 &half_size)
	{
		Uint32 typeless_id;
		SelectionType selection;
		IdType type;
		bool selected;

		m_id = m_scene->pick(glm::vec2(position[0], position[1]),
			glm::vec2(half_size[0], half_size[1]), selection);

		selected = FreeIdsManager::get_id_type(m_id, typeless_id, type);
		m_renderable = rt_none;

		if (!selected)
		{
			return;
		}

		switch (type)
		{
			case it_3d_object:
			case it_2d_object:
				m_renderable = rt_object;
				break;
			case it_tile_object:
			case it_water_object:
			case it_dynamic_object:
				break;
			case it_light_object:
				m_id = typeless_id;
				m_renderable = rt_light;
				break;
			case it_particle_object:
				m_id = typeless_id;
				m_renderable = rt_particle;
				break;
			default:
				break;
		}
	}

	StringVector EditorMapData::get_materials() const
	{
		return m_scene->get_scene_resources(
			).get_material_description_cache(
			)->get_material_description_names();
	}

	StringVector EditorMapData::get_default_materials(const String &name)
		const
	{
		MaterialDescriptionVector materials;
		StringVector result;

		materials = m_scene->get_scene_resources(
			).get_mesh_data_cache()->get_mesh_materials(name);

		BOOST_FOREACH(const MaterialDescription &material, materials)
		{
			result.push_back(material.get_name());
		}

		return result;
	}

	void EditorMapData::get_heights(const Uint16Array2 &vertex,
		const float radius, HeightVector &heights) const
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
		temp = m_height_image->get_width() - 1.0f;
		max_x = boost::numeric_cast<Uint32>(std::min(temp, tmp));

		tmp = static_cast<float>(vertex[1]) + radius;
		temp = m_height_image->get_height() - 1.0f;
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
					Height value(x, y);

					value.set_value(
						m_blend_image->get_pixel(x, y,
							0, 0, 0).r);

					heights.push_back(value);
				}
			}
		}
	}

	void EditorMapData::change_heights(const Uint16Array2 &vertex,
		const float strength, const float radius,
		const EditorBrushType brush_type, HeightVector &heights) const
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

	float EditorMapData::calc_brush_effect(const glm::vec2 &centre,
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

	void EditorMapData::get_blend_values(const Uint16Array2 &vertex,
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

	void EditorMapData::change_blend_values(const Uint16Array2 &position,
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

	float EditorMapData::get_blend_value(const glm::vec4 &blend,
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

	glm::vec4 EditorMapData::get_blend_values(const glm::vec4 &blend)
	{
		glm::vec4 result;
		Uint32 i;

		for (i = 0; i < 4; i++)
		{
			result[i] = get_blend_value(blend, i);
		}

		return result;
	}

	void EditorMapData::set_blend_value(const float value,
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

	EditorBrushType EditorMapData::get_brush_type(const int brush_type)
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

}
