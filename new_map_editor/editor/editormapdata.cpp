/****************************************************************************
 *            editormapdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editormapdata.hpp"
#include "lightdata.hpp"
#include "objectdescription.hpp"
#include "exceptions.hpp"
#include "particledata.hpp"
#include "image.hpp"
#include "editorscene.hpp"
#include "freeidsmanager.hpp"
#include "materialdescription.hpp"
#include "materialdescriptioncache.hpp"
#include "meshdatacache.hpp"
#include "abstractmaploader.hpp"
#include "codec/codecmanager.hpp"
#include "shader/shaderbuildutil.hpp"
#include "effectcache.hpp"
#include "abstractterrainmanager.hpp"

namespace eternal_lands
{

	EditorMapData::EditorMapData(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system)
	{
		glm::uvec2 value;
		Uint32 i, j, index;

		m_scene.reset(new EditorScene(global_vars, file_system));
		m_scene->set_main_light_ambient(glm::vec3(0.1f));
		m_scene->set_main_light_color(glm::vec3(0.8f));
		m_scene->set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		m_scene->set_lights(false);

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

	EditorMapData::~EditorMapData() throw()
	{
	}

	void EditorMapData::set_draw_objects(const bool draw_objects)
	{
		m_scene->set_draw_objects(draw_objects);
	}

	void EditorMapData::set_draw_terrain(const bool draw_terrain)
	{
		m_scene->set_draw_terrain(draw_terrain);
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

	void EditorMapData::add_object(
		const EditorObjectDescription &object_description)
	{
		m_objects.insert(std::pair<Uint32,
			EditorObjectDescription>(object_description.get_id(),
			object_description));
		m_scene->add_object(object_description);
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

	void EditorMapData::modify_object(
		const EditorObjectDescription &object_description)
	{
		m_objects.erase(object_description.get_id());
		m_scene->remove_object(object_description.get_id());

		m_objects.insert(std::pair<Uint32,
			EditorObjectDescription>(object_description.get_id(),
				object_description));
		m_scene->add_object(object_description);
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
		EditorObjectDescription &object_description) const
	{
		std::map<Uint32, EditorObjectDescription>::const_iterator found;

		found = m_objects.find(id);

		if (found == m_objects.end())
		{
			return false;
		}

		object_description = found->second;
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
		StringVector materials;
		StringStream str;
		String file_name;
		Transformation transformation;
		glm::vec3 offset;
		Uint32 id;

		RANGE_CECK_MAX(x, m_tile_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK_MAX(y, m_tile_map.shape()[1],
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

		materials.push_back(String(str.str()));

		m_scene->add_object(ObjectDescription(transformation, materials,
			String(UTF8("plane_4")), 0.0f, id, st_none,
			bt_disabled));
	}

	Uint16 EditorMapData::get_tile(const Uint16 x, const Uint16 y) const
	{
		RANGE_CECK_MAX(x, m_tile_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK_MAX(y, m_tile_map.shape()[1],
			UTF8("index value too big"));

		return m_tile_map[x][y];
	}

	glm::uvec2 EditorMapData::get_tile_offset(const glm::vec2 &point)
		const
	{
		return glm::uvec2(glm::clamp(point / 3.0f, glm::vec2(0.0f),
			glm::vec2(get_tile_map_size())));
	}

	void EditorMapData::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		RANGE_CECK_MAX(x, m_height_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK_MAX(y, m_height_map.shape()[1],
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

	void EditorMapData::set_terrain_values(
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
			m_terrain_vector_image->set_pixel_uint(
				terrain_value.get_x(), terrain_value.get_y(),
				0, 0, 0, terrain_value.get_value());

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

		m_scene->set_terrain(m_terrain_vector_image,
			m_terrain_normal_image, m_terrain_dudv_image);
	}

	void EditorMapData::set_terrain_albedo_map(const String &name,
		const Uint16 index)
	{
	}

	void EditorMapData::set_terrain_blend_map(const String &name,
		const Uint16 index)
	{
	}

	void EditorMapData::set_terrain_vector_map(const String &name)
	{
	}

	void EditorMapData::set_terrain_dudv_map(const String &name)
	{
	}

	String tmp;

	const String &EditorMapData::get_terrain_albedo_map(const Uint16 index)
		const
	{
		return tmp;
	}

	const String &EditorMapData::get_terrain_blend_map(const Uint16 index)
		const
	{
		return tmp;
	}

	const String &EditorMapData::get_terrain_vector_map() const
	{
		return tmp;
	}

	const String &EditorMapData::get_terrain_dudv_map() const
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
		m_renderable = rt_none;

		m_scene->load_map(name, *this);
	}

	void EditorMapData::draw()
	{
		m_scene->cull();
		m_scene->draw();
	}

	void EditorMapData::select(const glm::uvec2 &position,
		const glm::uvec2 &half_size)
	{
		Uint32 typeless_id;
		SelectionType selection;
		IdType type;
		bool selected;

		m_id = m_scene->pick(glm::vec2(position), glm::vec2(half_size),
			selection);

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

	void EditorMapData::select_depth(const glm::uvec2 &position)
	{
		m_depth = m_scene->get_depth(position);
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
		return m_scene->get_scene_resources(
			).get_mesh_data_cache()->get_mesh_materials(name);
	}

	void EditorMapData::get_terrain_values(const glm::uvec2 &vertex,
		const float radius, TerrainValueVector &terrain_values) const
	{
		glm::vec2 centre, point;
		Sint64 temp;
		Uint32 x, y;
		Uint32 min_x, min_y, max_x, max_y;
		float tmp, sqr_radius;

		tmp = static_cast<float>(vertex.x) - radius;
		min_x = std::max(static_cast<Sint64>(tmp), 0l);

		tmp = static_cast<float>(vertex.y) - radius;
		min_y = std::max(static_cast<Sint64>(tmp), 0l);

		tmp = static_cast<float>(vertex.x) + radius;
		temp = m_terrain_vector_image->get_width();
		max_x = std::min(static_cast<Sint64>(tmp), temp - 1);

		tmp = static_cast<float>(vertex.y) + radius;
		temp = m_terrain_vector_image->get_height();
		max_y = std::min(static_cast<Sint64>(tmp), temp - 1);

		terrain_values.clear();

		centre = glm::vec2(vertex[0], vertex[1]);

		sqr_radius = radius * radius;

		for (x = min_x; x <= max_x; x++)
		{
			for (y = min_y; y <= max_y; y++)
			{
				point = glm::vec2(x, y);

				if (glm::distance2(centre, point) <= sqr_radius)
				{
					TerrainValue value(x, y);

					value.set_value(
						m_terrain_vector_image->
							get_pixel_uint(x, y,
								0, 0, 0));

					terrain_values.push_back(value);
				}
			}
		}
	}

	void EditorMapData::change_terrain_values_add_normal(
		const glm::uvec2 &vertex, const float scale,
		const float radius, const AddBrushType brush_type,
		TerrainValueVector &terrain_values) const
	{
		glm::vec3 value, normal;
		glm::vec2 centre, point;
		glm::ivec2 index;
		Uint32 i;

		centre = glm::vec2(vertex);

		normal = scale * get_normal(glm::ivec2(vertex));

		for (i = 0; i < terrain_values.size(); i++)
		{
			value = AbstractTerrainManager::get_offset_scaled_rgb10_a2(
				terrain_values[i].get_value());

			index.x = terrain_values[i].get_x();
			index.y = terrain_values[i].get_y();
			point = index;

			value += normal * calc_brush_effect_add(centre, point,
				radius, brush_type);

			terrain_values[i].set_value(
				AbstractTerrainManager::get_value_scaled_rgb10_a2(
					value));
		}
	}

	void EditorMapData::change_terrain_values_add(const glm::uvec2 &vertex,
		const glm::vec3 &add_value, const float radius,
		const AddBrushType brush_type,
		TerrainValueVector &terrain_values) const
	{
		glm::vec3 value;
		glm::vec2 centre, point;
		Uint32 i;

		centre = glm::vec2(vertex);

		for (i = 0; i < terrain_values.size(); i++)
		{
			value = AbstractTerrainManager::get_offset_scaled_rgb10_a2(
				terrain_values[i].get_value());

			point.x = terrain_values[i].get_x();
			point.y = terrain_values[i].get_y();

			value += add_value * calc_brush_effect_add(centre,
				point, radius, brush_type);

			terrain_values[i].set_value(
				AbstractTerrainManager::get_value_scaled_rgb10_a2(
					value));
		}
	}

	void EditorMapData::change_terrain_values_smooth(
		const glm::uvec2 &vertex, const float strength,
		const float radius, const SmoothBrushType brush_type,
		TerrainValueVector &terrain_values) const
	{
		glm::vec3 average, value;
		glm::vec2 centre, point;
		Uint32 i;

		centre = glm::vec2(vertex);

		if (terrain_values.size() < 2)
		{
			return;
		}

		average = glm::vec3(0.0f);

		BOOST_FOREACH(const TerrainValue &terrain_value, terrain_values)
		{
			average += AbstractTerrainManager::get_offset_scaled_rgb10_a2(
				terrain_value.get_value());
		}

		average /= terrain_values.size();

		for (i = 0; i < terrain_values.size(); i++)
		{
			value = AbstractTerrainManager::get_offset_scaled_rgb10_a2(
				terrain_values[i].get_value());

			point.x = terrain_values[i].get_x();
			point.y = terrain_values[i].get_y();

			value = calc_brush_effect_smooth(centre, point,
				value, average, strength, radius, brush_type);

			terrain_values[i].set_value(
				AbstractTerrainManager::get_value_scaled_rgb10_a2(
					value));
		}
	}

	void EditorMapData::change_terrain_values_set(const glm::uvec2 &vertex,
		const glm::vec3 &set_value, const glm::bvec3 &mask,
		const float radius, const AddBrushType brush_type,
		TerrainValueVector &terrain_values) const
	{
		glm::vec3 value, mix;
		glm::vec2 centre, point;
		float tmp;
		Uint32 i;

		centre = glm::vec2(vertex);
		mix = glm::vec3(mask);

		for (i = 0; i < terrain_values.size(); i++)
		{
			value = AbstractTerrainManager::get_offset_scaled_rgb10_a2(
				terrain_values[i].get_value());

			point.x = terrain_values[i].get_x();
			point.y = terrain_values[i].get_y();

			tmp = calc_brush_effect_add(centre, point, radius,
				brush_type);

			value = glm::mix(value, set_value, mix * tmp);

			terrain_values[i].set_value(
				AbstractTerrainManager::get_value_scaled_rgb10_a2(
					value));
		}
	}

	float EditorMapData::calc_brush_effect_add(const glm::vec2 &centre,
		const glm::vec2 &point, const float radius,
		const AddBrushType brush_type)
	{
		float dist;

		switch (brush_type)
		{
			case abt_const:
				return 1.0f;
			case abt_linear:
				dist = glm::distance(centre, point);
				return std::max(1.0f - dist / radius, 0.0f);
			case abt_quadratic:
				dist = glm::distance2(centre, point);
				return std::max(1.0f - dist / (radius * radius),
					0.0f);
		};

		return 0.0f;
	}

	glm::vec3 EditorMapData::calc_brush_effect_smooth(
		const glm::vec2 &centre, const glm::vec2 &point,
		const glm::vec3 &value, const glm::vec3 &average,
		const float strength, const float radius,
		const SmoothBrushType brush_type)
	{
		glm::vec3 result;
		float dist, tmp;

		result = value;

		switch (brush_type)
		{
			case sbt_linear:
				dist = glm::distance(centre, point);
				tmp = std::max(1.0f - dist / radius, 0.0f);
				result -= (value - average) * strength * tmp;
				break;
			case sbt_quadratic:
				dist = glm::distance2(centre, point);
				tmp = std::max(1.0f - dist / (radius * radius),
					0.0f);
				result -= (value - average) * strength * tmp;
				break;
		};

		return result;
	}

	void EditorMapData::get_blend_values(const glm::uvec2 &vertex,
		const float radius, ImageValueVector &blend_values) const
	{
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

					value.set_value(
						m_blend_images[0]->get_pixel(x,
							y, 0, 0, 0));
					blend_values.push_back(value);
				}
			}
		}
	}

	glm::vec4 EditorMapData::get_blend_values(const glm::vec4 &blend)
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
/*
		assert(std::abs(value - get_blend_value(blend, index)) <
			epsilon);
*/
	}

	ImageSharedPtr EditorMapData::get_image(const String &name) const
	{
		return m_scene->get_scene_resources().get_codec_manager(
			)->load_image(name, m_scene->get_file_system(),
			ImageCompressionTypeSet(), false, false);
	}

	void EditorMapData::init_terrain(const glm::uvec2 &size)
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

		m_scene->set_terrain(m_terrain_vector_image,
			m_terrain_normal_image, m_terrain_dudv_image);
		m_scene->rebuild_terrain_map();
	}

	void EditorMapData::set_focus(const glm::vec3 &focus) noexcept
	{
		m_scene->set_focus(focus);
	}

	void EditorMapData::set_debug_mode(const int value)
	{
		ShaderBuildType shader_debug;

		shader_debug = static_cast<ShaderBuildType>(
			sbt_debug_uv + value);

		m_scene->get_scene_resources().get_effect_cache(
			)->set_debug_shader(shader_debug);
		m_scene->get_scene_resources().get_effect_cache()->reload();
	}

	StringVector EditorMapData::get_debug_modes() const
	{
		StringVector result;
		Uint32 i, count;

		count = ShaderBuildUtil::get_shader_build_count();

		for (i = 0; i < count; ++i)
		{
			if (i < sbt_debug_uv)
			{
				continue;
			}

			result.push_back(ShaderBuildUtil::get_str(
				static_cast<ShaderBuildType>(i)));
		}

		return result;
	}

	glm::uvec2 EditorMapData::get_best_normal(const glm::vec3 &normal) const
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

	glm::vec3 EditorMapData::get_position(const glm::ivec2 &index) const
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

	glm::vec3 EditorMapData::get_direction(const glm::vec3 &centre,
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

	glm::vec3 EditorMapData::get_normal(const glm::ivec2 &index) const
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

	void EditorMapData::update_normal(const glm::ivec2 &index)
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

	void EditorMapData::update_normals(const Ivec2Set &indices)
	{
		BOOST_FOREACH(const glm::ivec2 &index, indices)
		{
			update_normal(index);
		}
	}

	const glm::vec3 &EditorMapData::get_terrain_offset()
	{
		return AbstractTerrainManager::get_vector_scale();
	}

	const glm::vec3 &EditorMapData::get_terrain_offset_min()
	{
		return AbstractTerrainManager::get_vector_min();
	}

	const glm::vec3 &EditorMapData::get_terrain_offset_max()
	{
		return AbstractTerrainManager::get_vector_max();
	}

	glm::uvec2 EditorMapData::get_vertex(const glm::vec3 &world_position)
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
