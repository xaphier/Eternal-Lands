/****************************************************************************
 *            abstractmaploader.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractmaploader.hpp"
#include "instancedata.hpp"
#include "objectdescription.hpp"
#include "filesystem.hpp"
#include "particledata.hpp"
#include "reader.hpp"
#include "logging.hpp"
#include "map.hpp"
#include "materialdescription.hpp"
#include "instancesbuilder.hpp"
#include "materialdescriptioncache.hpp"
#include "terrain/simpleterrainmanager.hpp"
#include "globalvars.hpp"
#include "lightdata.hpp"
#include "exceptions.hpp"
#include "freeidsmanager.hpp"
#include "glmutil.hpp"
#include "simplexnoise.hpp"

namespace eternal_lands
{

	StringSet AbstractMapLoader::load_harvestables(
		const FileSystemSharedPtr &file_system)
	{
		std::vector<StringType> lines, line;
		std::vector<StringType>::iterator it;
		StringType str;
		String harvestables_str;
		StringSet result;

		harvestables_str = file_system->get_file_string(
			String(UTF8("harvestable.lst")));

		boost::split(lines, harvestables_str.get(),
			boost::is_any_of(UTF8("\n")),
			boost::token_compress_on);

		it = lines.begin();

		while (it != lines.end())
		{
			boost::algorithm::trim(*it);

			if (!it->empty())
			{
				str = UTF8("3dobjects/");
				str += *it;
				result.insert(String(str));
			}

			it++;
		}

		return result;
	}

	StringSet AbstractMapLoader::load_entrables(
		const FileSystemSharedPtr &file_system)
	{
		std::vector<StringType> lines, line;
		std::vector<StringType>::iterator it;
		StringType str;
		String entrable_str;
		StringSet result;

		entrable_str = file_system->get_file_string(
			String(UTF8("entrable.lst")));

		boost::split(lines, entrable_str.get(),
			boost::is_any_of(UTF8("\n")),
			boost::token_compress_on);

		it = lines.begin();

		while (it != lines.end())
		{
			boost::algorithm::trim(*it);

			if (!it->empty())
			{
				str = UTF8("3dobjects");
				str += *it;
				result.insert(String(str));
			}

			it++;
		}

		return result;
	}

	AbstractMapLoader::AbstractMapLoader(
		const FileSystemSharedPtr &file_system,
		const FreeIdsManagerSharedPtr &free_ids):
		m_file_system(file_system), m_free_ids(free_ids)
	{
		m_harvestables = load_harvestables(file_system);
		m_entrables = load_entrables(file_system);
	}

	AbstractMapLoader::~AbstractMapLoader() throw()
	{
	}

	bool AbstractMapLoader::check_format(const Uint8Array8 &id)
	{
		return true;
	}

	SelectionType AbstractMapLoader::get_selection(const String &name) const
	{
		if (m_harvestables.count(name) > 0)
		{
			return st_harvest;
		}

		if (m_entrables.count(name) > 0)
		{
			return st_enter;
		}

		return st_select;
	}

	void AbstractMapLoader::read_3d_object(const Uint32 index,
		const Uint32 offset)
	{
		glm::vec3 translation, rotation_angles, color;
		String name;
		StringVector material_names;
		float transparency, scale;
		Uint32 id, blended, material_index, material_count;
		SelectionType selection;
		BlendType blend;
		bool self_lit;

		get_reader()->set_position(offset);

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

		LOG_DEBUG(UTF8("Adding 3d object (%1%) '%2%' at <%3%> with "
			"rotation <%4%>, left lit %5%, blended %6% and "
			"color <%7%>."), index % name %
			glm::to_string(translation) %
			glm::to_string(rotation_angles) % self_lit % blended %
			glm::to_string(color));

		if (blended == 20)
		{
			return;
		}

		if (blended == 1)
		{
			transparency = 0.7f;
			blend = bt_alpha_transparency_source_value;
		}
		else
		{
			transparency = 1.0f;
			blend = bt_disabled;
		}

		if (true)
		{
			scale = 1.0f;
			material_count = 0;
			material_index = 0;
			name = FileSystem::get_strip_relative_path(name);
			selection = get_selection(name);
		}

		id = get_free_ids().use_typeless_object_id(index, it_3d_object);

		add_object(translation, rotation_angles, name, scale,
			transparency, id, selection, blend,
			get_material_names(material_index, material_count));
	}

	void AbstractMapLoader::read_2d_object(const Uint32 index,
		const Uint32 offset)
	{
		glm::vec3 translation, rotation_angles;
		String name;
		StringVector material_names;
		Uint32 id;

		get_reader()->set_position(offset);

		name = get_reader()->read_utf8_string(80);

		translation.x = get_reader()->read_float_le();
		translation.y = get_reader()->read_float_le();
		translation.z = get_reader()->read_float_le();

		rotation_angles.x = get_reader()->read_float_le();
		rotation_angles.y = get_reader()->read_float_le();
		rotation_angles.z = get_reader()->read_float_le();

		id = get_free_ids().use_typeless_object_id(index, it_2d_object);

		LOG_DEBUG(UTF8("Adding 2d object (%1%) '%2%' at <%3%> with "
			"rotation <%4%>."), index % name %
			glm::to_string(translation) %
			glm::to_string(rotation_angles));

		add_object(translation, rotation_angles, name, 1.0f,
			0.0f, id, st_none, bt_disabled, material_names);
	}

	void AbstractMapLoader::read_light(const Uint32 index,
		const Uint32 offset)
	{
		glm::vec3 color, position;
		float radius, scale;

		get_reader()->set_position(offset);

		position.x = get_reader()->read_float_le();
		position.y = get_reader()->read_float_le();
		position.z = get_reader()->read_float_le();

		color.r = get_reader()->read_float_le();
		color.g = get_reader()->read_float_le();
		color.b = get_reader()->read_float_le();

		radius = get_reader()->read_float_le();

		if (true)
		{
			scale = std::max(std::max(color.r, color.g),
				std::max(color.b, 1.0f));
			color /= scale;
			radius = 2.5f * scale;
		}

		LOG_DEBUG(UTF8("Adding light (%1%) at <%2%> with "
			"color <%3%> and radius %4%."), index %
			glm::to_string(position) %
			glm::to_string(color) % radius);

		get_free_ids().use_light_id(index);

		add_light(position, color, radius, index);
	}

	void AbstractMapLoader::read_particle(const Uint32 index,
		const Uint32 offset)
	{
		glm::vec3 position;
		String name;

		get_reader()->set_position(offset);

		name = get_reader()->read_utf8_string(80);

		position.x = get_reader()->read_float_le();
		position.y = get_reader()->read_float_le();
		position.z = get_reader()->read_float_le();

		LOG_DEBUG(UTF8("Adding particle (%1%) at <%2%> with "
			"date '%3%'."), index % glm::to_string(position) %
			name);

		get_free_ids().use_particle_id(index);

		add_particle(position, name, index);
	}

	void AbstractMapLoader::read_terrain(const Uint32 index,
		const Uint32 offset)
	{
		StringArray4 albedo;
		String blend, height, dudv;
		glm::vec3 translation;
		glm::vec2 dudv_scale;

		get_reader()->set_position(offset);

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
		dudv_scale.x = get_reader()->read_float_le();
		dudv_scale.y = get_reader()->read_float_le();

		add_terrain(albedo, blend, height, dudv, translation,
			dudv_scale);
	}

	void AbstractMapLoader::read_material_name(const Uint32 index,
		const Uint32 offset)
	{
		get_reader()->set_position(offset);

		m_material_names[index] = get_reader()->read_utf8_string(128);
	}

	void AbstractMapLoader::read_3d_objects(const Uint32 obj_3d_size,
		const Uint32 obj_3d_count, const Uint32 obj_3d_offset)
	{
		Uint32 i;

		LOG_DEBUG("Loading %1% 3d objects.", obj_3d_count);

		for (i = 0; i < obj_3d_count; ++i)
		{
			try
			{
				read_3d_object(i, obj_3d_offset +
					i * obj_3d_size);
			}
			catch (boost::exception &exception)
			{
				exception << errinfo_item_id(i);
				LOG_EXCEPTION(exception);
			}
			catch (const std::exception &exception)
			{
				LOG_EXCEPTION(exception);
			}
		}
	}

	void AbstractMapLoader::read_2d_objects(const Uint32 obj_2d_size,
		const Uint32 obj_2d_count, const Uint32 obj_2d_offset)
	{
		Uint32 i;

		LOG_DEBUG("Loading %1% 2d objects.", obj_2d_count);

		for (i = 0; i < obj_2d_count; ++i)
		{
			try
			{
				read_2d_object(i, obj_2d_offset +
					i * obj_2d_size);
			}
			catch (boost::exception &exception)
			{
				exception << errinfo_item_id(i);
				LOG_EXCEPTION(exception);
			}
			catch (const std::exception &exception)
			{
				LOG_EXCEPTION(exception);
			}
		}
	}

	void AbstractMapLoader::read_lights(const Uint32 light_size,
		const Uint32 light_count, const Uint32 light_offset)
	{
		Uint32 i;

		LOG_DEBUG("Loading %1% lights.", light_count);

		for (i = 0; i < light_count; ++i)
		{
			try
			{
				read_light(i, light_offset + i * light_size);
			}
			catch (boost::exception &exception)
			{
				exception << errinfo_item_id(i);
				LOG_EXCEPTION(exception);
			}
			catch (const std::exception &exception)
			{
				LOG_EXCEPTION(exception);
			}
		}
	}

	void AbstractMapLoader::read_particles(const Uint32 particle_size,
		const Uint32 particle_count, const Uint32 particle_offset)
	{
		Uint32 i;

		LOG_DEBUG("Loading %1% particles.", particle_count);

		for (i = 0; i < particle_count; ++i)
		{
			try
			{
				read_particle(i, particle_offset +
					i * particle_size);
			}
			catch (boost::exception &exception)
			{
				exception << errinfo_item_id(i);
				LOG_EXCEPTION(exception);
			}
			catch (const std::exception &exception)
			{
				LOG_EXCEPTION(exception);
			}
		}
	}

	void AbstractMapLoader::read_height_map(const Uint32 height_map_width,
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
				try
				{
					set_height(x, y,
						get_reader()->read_u8());
				}
				catch (boost::exception &exception)
				{
					exception << errinfo_array_index_0(x);
					exception << errinfo_array_index_1(y);
					LOG_EXCEPTION(exception);
				}
				catch (const std::exception &exception)
				{
					LOG_EXCEPTION(exception);
				}
			}
		}
	}

	void AbstractMapLoader::read_tile_map(const Uint32 tile_map_width,
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
				try
				{
					set_tile(x, y, get_reader()->read_u8());
				}
				catch (boost::exception &exception)
				{
					exception << errinfo_array_index_0(x);
					exception << errinfo_array_index_1(y);
					LOG_EXCEPTION(exception);
				}
				catch (const std::exception &exception)
				{
					LOG_EXCEPTION(exception);
				}
			}
		}
	}

	void AbstractMapLoader::read_terrains(const Uint32 terrain_size,
		const Uint32 terrain_count, const Uint32 terrain_offset)
	{
		Uint32 i;

		for (i = 0; i < terrain_count; ++i)
		{
			try
			{
				read_terrain(i, terrain_offset +
					i * terrain_size);
			}
			catch (boost::exception &exception)
			{
				exception << errinfo_item_id(i);
				LOG_EXCEPTION(exception);
			}
			catch (const std::exception &exception)
			{
				LOG_EXCEPTION(exception);
			}
		}
	}

	void AbstractMapLoader::read_material_names(const Uint32 material_name_size,
		const Uint32 material_name_count,
		const Uint32 material_name_offset)
	{
		Uint32 i;

		LOG_DEBUG("Loading %1% material names.", material_name_count);

		m_material_names.resize(material_name_count);

		for (i = 0; i < material_name_count; ++i)
		{
			try
			{
				read_material_name(i, material_name_offset +
					i * material_name_size);
			}
			catch (boost::exception &exception)
			{
				exception << errinfo_item_id(i);
				LOG_EXCEPTION(exception);
			}
			catch (const std::exception &exception)
			{
				LOG_EXCEPTION(exception);
			}
		}
	}

	void AbstractMapLoader::read(const String &name)
	{
		glm::vec3 ambient;
		Uint32 tile_map_offset;
		Uint32 height_map_offset;
		Uint32 obj_3d_size;
		Uint32 obj_3d_count;
		Uint32 obj_3d_offset;
		Uint32 obj_2d_size;
		Uint32 obj_2d_count;
		Uint32 obj_2d_offset;
		Uint32 light_size;
		Uint32 light_count;
		Uint32 light_offset;
		Uint32 particle_size;
		Uint32 particle_count;
		Uint32 particle_offset;
		Uint32 clusters_offset;
		Uint32 terrain_size;
		Uint32 terrain_count;
		Uint32 terrain_offset;
		Uint32 material_name_size;
		Uint32 material_name_count;
		Uint32 material_name_offset; 
		Uint32 height_map_width;
		Uint32 height_map_height;
		Uint32 tile_map_width;
		Uint32 tile_map_height;
		Sint8Array4 magic_number;
		Uint8Array2 version_number;
		Uint16 version;
		bool dungeon;

		m_reader = get_file_system()->get_file(name);

		LOG_DEBUG("Loading map '%1%'.", get_reader()->get_name());

		get_reader()->set_position(0);

		magic_number[0] = get_reader()->read_u8();
		magic_number[1] = get_reader()->read_u8();
		magic_number[2] = get_reader()->read_u8();
		magic_number[3] = get_reader()->read_u8();

		if (magic_number != get_magic_number())
		{
			EL_THROW_EXCEPTION(IoErrorException()
				<< boost::errinfo_file_name(
					m_reader->get_name()));
		}

		tile_map_width = get_reader()->read_u32_le();
		tile_map_height = get_reader()->read_u32_le();
		tile_map_offset = get_reader()->read_u32_le();
		height_map_offset = get_reader()->read_u32_le();
		obj_3d_size = get_reader()->read_u32_le();
		obj_3d_count = get_reader()->read_u32_le();
		obj_3d_offset = get_reader()->read_u32_le();
		obj_2d_size = get_reader()->read_u32_le();
		obj_2d_count = get_reader()->read_u32_le();
		obj_2d_offset = get_reader()->read_u32_le();
		light_size = get_reader()->read_u32_le();
		light_count = get_reader()->read_u32_le();
		light_offset = get_reader()->read_u32_le();

		LOG_DEBUG(UTF8("map size <%1%, %2%>."),
			(tile_map_width * get_tile_size())%
			(tile_map_height * get_tile_size()));

		dungeon = get_reader()->read_u8() != 0;

		LOG_DEBUG(UTF8("dungeon %1%."), dungeon);

		set_dungeon(dungeon);

		get_reader()->skip(sizeof(Uint8));

		version_number[0] = get_reader()->read_u8();
		version_number[1] = get_reader()->read_u8();

		ambient.r = get_reader()->read_float_le();
		ambient.g = get_reader()->read_float_le();
		ambient.b = get_reader()->read_float_le();

		LOG_DEBUG(UTF8("ambient color <%1%, %2%, %3%>."),
			ambient.r % ambient.g % ambient.b);

		set_ambient(ambient);

		particle_size = get_reader()->read_u32_le();
		particle_count = get_reader()->read_u32_le();
		particle_offset = get_reader()->read_u32_le();
		clusters_offset = get_reader()->read_u32_le();

		terrain_size = get_reader()->read_u32_le();
		terrain_count = get_reader()->read_u32_le();
		terrain_offset = get_reader()->read_u32_le();
		material_name_size = get_reader()->read_u32_le();
		material_name_count = get_reader()->read_u32_le();
		material_name_offset = get_reader()->read_u32_le();

		LOG_DEBUG("Map version %1%.%2%.",
			static_cast<Uint16>(version_number[0]) %
			static_cast<Uint16>(version_number[1]));

		version = static_cast<Uint16>(version_number[0]) << 8;
		version |= static_cast<Uint16>(version_number[1]) << 0;

		if (version == 0)
		{
			terrain_size = get_terrain_size();
			terrain_count = 0;
			terrain_offset = 0;
			material_name_size = get_material_name_size();
			material_name_count = 0;
			material_name_offset = 0;
		}

		set_tile_map_size(tile_map_width, tile_map_height);

		height_map_width = tile_map_width * 2 * get_tile_size();
		height_map_height = tile_map_height * 2 * get_tile_size();

		set_height_map_size(height_map_width, height_map_height);

		if (obj_3d_size != get_3d_object_size())
		{
			LOG_ERROR(UTF8("File '%1%' has wrong 3d object"
				" size of %2% instead of %3%."),
				get_reader()->get_name() %
				obj_3d_size % get_3d_object_size());
		}

		if (obj_2d_size != get_2d_object_size())
		{
			LOG_ERROR(UTF8("File '%1%' has wrong 2d object"
				" size of %2% instead of %3%."),
				get_reader()->get_name() %
				obj_2d_size % get_2d_object_size());
		}

		if (light_size != get_light_size())
		{
			LOG_ERROR(UTF8("File '%1%' has wrong light"
				" size of %2% instead of %3%."),
				get_reader()->get_name() %
				light_size % get_light_size());
		}

		if (particle_size != get_particle_size())
		{
			LOG_ERROR(UTF8("File '%1%' has wrong particle"
				" size of %2% instead of %3%."),
				get_reader()->get_name() %
				particle_size % get_particle_size());
		}

		if (terrain_size != get_terrain_size())
		{
			LOG_ERROR(UTF8("File '%1%' has wrong terrain"
				" size of %2% instead of %3%."),
				get_reader()->get_name() %
				terrain_size % get_terrain_size());
		}

		if (material_name_size != get_material_name_size())
		{
			LOG_ERROR(UTF8("File '%1%' has wrong material name"
				" size of %2% instead of %3%."),
				get_reader()->get_name() %
				material_name_size % get_material_name_size());
		}

		read_material_names(material_name_size, material_name_count,
			material_name_offset);
		read_3d_objects(obj_3d_size, obj_3d_count, obj_3d_offset);
		read_2d_objects(obj_2d_size, obj_2d_count, obj_2d_offset);
		read_lights(light_size, light_count, light_offset);
		read_particles(particle_size, particle_count, particle_offset);
		read_height_map(height_map_width, height_map_height,
			height_map_offset);
		read_tile_map(tile_map_width, tile_map_height, tile_map_offset);
		read_terrains(terrain_size, terrain_count, terrain_offset);

		LOG_DEBUG("Done loading map '%1%'.", get_reader()->get_name());

		instance();
	}

	void AbstractMapLoader::instance()
	{
	}

	ObjectDescription AbstractMapLoader::get_object_description(
		const glm::vec3 &translation,
		const glm::vec3 &rotation_angles,
		const StringVector &material_names, const String &name,
		const float scale, const Uint32 id,
		const SelectionType selection, const BlendType blend)
	{
		Transformation transformation;
		float transparency;

		assert(glm::all(glm::lessThanEqual(glm::abs(translation),
			glm::vec3(1e7f))));

		if (blend != bt_disabled)
		{
			transparency = 0.7f;
		}
		else
		{
			transparency = 1.0f;
		}

		transformation.set_translation(translation);
		transformation.set_rotation_angles(rotation_angles);
		transformation.set_scale(scale);

		return ObjectDescription(transformation, material_names, name,
			transparency, id, selection, blend);
	}

	StringVector AbstractMapLoader::get_material_names(const Uint32 index,
		const Uint32 count) const
	{
		StringVector result;
		Uint32 i;

		for (i = 0; i < count; ++i)
		{
			result.push_back(m_material_names[i + index]);
		}

		return result;
	}

}