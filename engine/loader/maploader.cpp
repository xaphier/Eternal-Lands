/****************************************************************************
 *            maploader.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "maploader.hpp"
#include "instancedata.hpp"
#include "objectdescription.hpp"
#include "filesystem.hpp"
#include "particledata.hpp"
#include "reader.hpp"
#include "logging.hpp"
#include "map.hpp"

namespace eternal_lands
{

	MapLoader::MapLoader(const ReaderSharedPtr &reader): m_reader(reader)
	{
	}

	MapLoader::~MapLoader() throw()
	{
	}

	void MapLoader::load(Map &map)
	{
	}

	bool MapLoader::check_format(const Uint8Array8 &id)
	{
		return true;
	}

	void MapLoader::read_3d_objects(const Uint32 obj_3d_size,
		const Uint32 obj_3d_count, const Uint32 obj_3d_offset)
	{
		glm::vec3 translation, rotation_angles, color;
		String name;
		StringVector material_names;
		float transparency, scale;
		Uint32 i, blended, material_index, material_count;
		SelectionType selection;
		bool self_lit;

		LOG_DEBUG("Loading %1% 3d objects.", obj_3d_count);

		for (i = 0; i < obj_3d_count; ++i)
		{
			get_reader()->set_position(obj_3d_offset +
				i * obj_3d_size);

			name = get_reader()->read_utf8_string(80);

			translation.x = get_reader()->read_float_le();
			translation.y = get_reader()->read_float_le();
			translation.z = get_reader()->read_float_le();

			rotation_angles.x = get_reader()->read_float_le();
			rotation_angles.y = get_reader()->read_float_le();
			rotation_angles.z = get_reader()->read_float_le();

			self_lit = get_reader()->read_u8() != 0;
			blended = get_reader()->read_u8();
			selection = static_cast<SelectionType>(
				get_reader()->read_u8());
			get_reader()->skip(sizeof(Uint8));

			color.r = get_reader()->read_float_le();
			color.g = get_reader()->read_float_le();
			color.b = get_reader()->read_float_le();

			scale = get_reader()->read_float_le();
			material_count = get_reader()->read_u32_le();
			material_index = get_reader()->read_u32_le();

			if (blended == 20)
			{
				continue;
			}

			if (blended == 1)
			{
				transparency = 0.3f;
			}
			else
			{
				transparency = 0.0f;
			}

			if (true)
			{
				scale = 1.0f;
				material_count = 0;
				material_index = 0;
			}

			add_object(name, translation, rotation_angles, scale,
				transparency, i, blended == 1, selection,
				material_names);
		}
	}

	void MapLoader::read_2d_objects(const Uint32 obj_2d_size,
		const Uint32 obj_2d_count, const Uint32 obj_2d_offset)
	{
		glm::vec3 translation, rotation_angles;
		String name;
		StringVector material_names;
		Uint32 i;

		LOG_DEBUG("Loading %1% 2d objects.", obj_2d_count);

		for (i = 0; i < obj_2d_count; ++i)
		{
			get_reader()->set_position(obj_2d_offset +
				i * obj_2d_size);

			name = get_reader()->read_utf8_string(80);

			translation.x = get_reader()->read_float_le();
			translation.y = get_reader()->read_float_le();
			translation.z = get_reader()->read_float_le();

			rotation_angles.x = get_reader()->read_float_le();
			rotation_angles.y = get_reader()->read_float_le();
			rotation_angles.z = get_reader()->read_float_le();

			add_object(name, translation, rotation_angles, 1.0f,
				0.0f, i, false, st_none, material_names);
		}
	}

	void MapLoader::read_lights(const Uint32 light_size,
		const Uint32 light_count, const Uint32 light_offset)
	{
		glm::vec3 color, position;
		float radius;
		Uint32 i;

		LOG_DEBUG("Loading %1% lights.", light_count);

		for (i = 0; i < light_count; ++i)
		{
			get_reader()->set_position(light_offset +
				i * light_size);

			position.x = get_reader()->read_float_le();
			position.y = get_reader()->read_float_le();
			position.z = get_reader()->read_float_le();

			color.r = get_reader()->read_float_le();
			color.g = get_reader()->read_float_le();
			color.b = get_reader()->read_float_le();

			radius = get_reader()->read_float_le();

			add_light(position, color, radius, i);
		}
	}

	void MapLoader::read_particles(const Uint32 particle_size,
		const Uint32 particle_count, const Uint32 particle_offset)
	{
		glm::vec3 position;
		String name;
		Uint32 i;

		LOG_DEBUG("Loading %1% particles.", particle_count);

		for (i = 0; i < particle_count; ++i)
		{
			get_reader()->set_position(particle_offset +
				i * particle_size);

			name = get_reader()->read_utf8_string(80);

			position.x = get_reader()->read_float_le();
			position.y = get_reader()->read_float_le();
			position.z = get_reader()->read_float_le();
		}
	}

	void MapLoader::read_height_map(const Uint32 height_map_width,
		const Uint32 height_map_height, const Uint32 height_map_offset)
	{
		Uint32 x, y;

		LOG_DEBUG("Height size <%1%, %2%>.", height_map_width %
			height_map_height);

		get_reader()->set_position(height_map_offset);

		for (y = 0; y < height_map_height; y++)
		{
			for (x = 0; x < height_map_width; x++)
			{
				set_height(x, y, get_reader()->read_u8());
			}
		}
	}

	void MapLoader::read_tile_map(const Uint32 tile_map_width,
		const Uint32 tile_map_height, const Uint32 tile_map_offset)
	{
		Uint32 x, y;

		LOG_DEBUG("Tile size <%1%, %2%>.", tile_map_width %
			tile_map_height);

		get_reader()->set_position(tile_map_offset);

		for (y = 0; y < tile_map_height; y++)
		{
			for (x = 0; x < tile_map_width; x++)
			{
				set_tile(x, y, get_reader()->read_u8());
			}
		}
	}

	void MapLoader::read_terrain(const Uint32 terrain_offset)
	{
		StringArray4 albedo;
		String blend, height, dudv;
		glm::vec3 translation, rotation_angles;
		float scale, height_scale;

		get_reader()->set_position(terrain_offset);

		albedo[0] = get_reader()->read_utf8_string(128);
		albedo[1] = get_reader()->read_utf8_string(128);
		albedo[2] = get_reader()->read_utf8_string(128);
		albedo[3] = get_reader()->read_utf8_string(128);
		blend = get_reader()->read_utf8_string(128);
		height = get_reader()->read_utf8_string(128);
		dudv = get_reader()->read_utf8_string(128);

		translation.x = get_reader()->read_float_le();
		translation.y = get_reader()->read_float_le();
		translation.z = get_reader()->read_float_le();
		rotation_angles.x = get_reader()->read_float_le();
		rotation_angles.y = get_reader()->read_float_le();
		rotation_angles.z = get_reader()->read_float_le();
		scale = get_reader()->read_float_le();
		height_scale = get_reader()->read_float_le();

		set_terrain(albedo, blend, height, dudv, translation,
			rotation_angles, scale, height_scale);
	}

	void MapLoader::read_material_names(const Uint32 material_name_size,
		const Uint32 material_name_count,
		const Uint32 material_name_offset)
	{
		StringVector material_names;
		Uint32 i;

		LOG_DEBUG("Loading %1% material names.", material_name_count);

		material_names.resize(material_name_count);

		for (i = 0; i < material_name_count; ++i)
		{
			get_reader()->set_position(material_name_offset +
				i * material_name_size);
			material_names[i] =
				get_reader()->read_utf8_string(128);
		}
	}

	void MapLoader::add_object(const String &name,
		const glm::vec3 &position, const glm::vec3 &rotation_angles,
		const float scale, const float transparency,
		const Uint32 id, const bool blended,
		const SelectionType selection,
		const StringVector &material_names)
	{
	}

	void MapLoader::add_light(const glm::vec3 &position,
		const glm::vec3 &color, const float radius, const Uint32 id)
	{
		m_map->add_light(position, color, radius, id);
	}

	void MapLoader::add_particle(const glm::vec3 &position,
		const String &name)
	{
	}

	void MapLoader::set_terrain(const StringArray4 &albedo,
		const String &blend, const String &height, const String &dudv,
		const glm::vec3 &translation, const glm::vec3 &rotation_angles,
		const float scale, const float height_scale)
	{
	}

	void MapLoader::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
	}

	void MapLoader::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
	}

}
