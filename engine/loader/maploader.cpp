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
#include "materialeffectdescription.hpp"
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

	StringSet MapLoader::load_harvestables(
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

	StringSet MapLoader::load_entrables(
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

	MapLoader::MapLoader(const CodecManagerSharedPtr &codec_manager,
		const FileSystemSharedPtr &file_system,
		const GlobalVarsSharedPtr &global_vars,
		const MeshBuilderSharedPtr &mesh_builder,
		const MeshCacheSharedPtr &mesh_cache,
		const MeshDataCacheSharedPtr &mesh_data_cache,
		const EffectCacheSharedPtr &effect_cache,
		const TextureCacheSharedPtr &texture_cache,
		const MaterialDescriptionCacheSharedPtr
			&material_description_cache,
		const FreeIdsManagerSharedPtr &free_ids):
		m_codec_manager(codec_manager), m_file_system(file_system),
		m_global_vars(global_vars), m_mesh_builder(mesh_builder),
		m_mesh_cache(mesh_cache), m_mesh_data_cache(mesh_data_cache),
		m_effect_cache(effect_cache), m_texture_cache(texture_cache),
		m_material_description_cache(material_description_cache),
		m_free_ids(free_ids)
	{
		m_harvestables = load_harvestables(file_system);
		m_entrables = load_entrables(file_system);
	}

	MapLoader::~MapLoader() throw()
	{
	}

	MapSharedPtr MapLoader::load(const String &name)
	{
		m_instances_builder.reset(new InstancesBuilder(
			get_mesh_data_cache(), 8.0f,
			get_global_vars()->get_use_simd(),
			get_global_vars()->get_opengl_3_2()));

		m_reader = get_file_system()->get_file(name);

		m_map = boost::make_shared<Map>(get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_mesh_builder(), get_mesh_cache(),
			get_effect_cache(), get_texture_cache(),
			get_material_description_cache(), name);

		read();

		return m_map;
	}

	bool MapLoader::check_format(const Uint8Array8 &id)
	{
		return true;
	}

	SelectionType MapLoader::get_selection(const String &name) const
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

	void MapLoader::read_3d_object(const Uint32 index, const Uint32 offset)
	{
		glm::vec3 translation, rotation_angles, color;
		String name;
		StringVector material_names;
		float transparency, scale;
		Uint32 id, blended, material_index, material_count;
		SelectionType selection;
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

		LOG_DEBUG("Adding 3d object (%1%) '%2%' at <%3%> with "
			"rotation <%4%>, left lit %5%, blended %6% and "
			"color <%7%>.", index % name %
			glm::to_string(translation) %
			glm::to_string(rotation_angles) % self_lit % blended %
			glm::to_string(color));

		if (blended == 20)
		{
			return;
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
			name = FileSystem::get_strip_relative_path(name);
			selection = get_selection(name);
		}

		id = get_free_ids().use_typeless_id(index, it_3d_object);

		add_object(translation, rotation_angles, name, scale,
			transparency, id, blended == 1, selection,
			get_material_names(material_index, material_count));
	}

	void MapLoader::read_2d_object(const Uint32 index, const Uint32 offset)
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

		id = get_free_ids().use_typeless_id(index, it_2d_object);

		add_object(translation, rotation_angles, name, 1.0f,
			0.0f, id, false, st_none, material_names);
	}

	void MapLoader::read_light(const Uint32 index, const Uint32 offset)
	{
		glm::vec3 color, position;
		float radius;

		get_reader()->set_position(offset);

		position.x = get_reader()->read_float_le();
		position.y = get_reader()->read_float_le();
		position.z = get_reader()->read_float_le();

		color.r = get_reader()->read_float_le();
		color.g = get_reader()->read_float_le();
		color.b = get_reader()->read_float_le();

		radius = get_reader()->read_float_le();

		add_light(position, color, radius, index);
	}

	void MapLoader::read_particle(const Uint32 index, const Uint32 offset)
	{
		glm::vec3 position;
		String name;

		get_reader()->set_position(offset);

		name = get_reader()->read_utf8_string(80);

		position.x = get_reader()->read_float_le();
		position.y = get_reader()->read_float_le();
		position.z = get_reader()->read_float_le();

		add_particle(position, name, index);
	}

	void MapLoader::read_terrain(const Uint32 index, const Uint32 offset)
	{
		StringArray4 albedo;
		String blend, height, dudv;
		glm::vec3 translation, rotation_angles;
		float scale;

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
		rotation_angles.x = get_reader()->read_float_le();
		rotation_angles.y = get_reader()->read_float_le();
		rotation_angles.z = get_reader()->read_float_le();
		scale = get_reader()->read_float_le();

		add_terrain(albedo, blend, height, dudv, translation,
			rotation_angles, scale);
	}

	void MapLoader::read_material_name(const Uint32 index,
		const Uint32 offset)
	{
		get_reader()->set_position(offset);

		m_material_names[index] = get_reader()->read_utf8_string(128);
	}

	void MapLoader::read_3d_objects(const Uint32 obj_3d_size,
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

	void MapLoader::read_2d_objects(const Uint32 obj_2d_size,
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

	void MapLoader::read_lights(const Uint32 light_size,
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

	void MapLoader::read_particles(const Uint32 particle_size,
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

	void MapLoader::read_terrains(const Uint32 terrain_size,
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

	void MapLoader::read_material_names(const Uint32 material_name_size,
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

	void MapLoader::read()
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

	void MapLoader::instance()
	{
		InstanceDataVector instances;
		ObjectDataVector uninstanced;

		m_instances_builder->build(
			get_free_ids().get_free_ids(it_3d_object), instances,
			uninstanced);

		BOOST_FOREACH(const InstanceData &instance_data, instances)
		{
			m_map->add_object(instance_data);
		}

		BOOST_FOREACH(const ObjectData &object_data, uninstanced)
		{
			m_map->add_object(object_data);
		}
	}

	glm::quat MapLoader::get_rotation(const glm::vec3 &rotation_angles)
	{
		glm::quat rotation;

		rotation = glm::quat();
		rotation = glm::rotate(rotation, rotation_angles.z,
			glm::vec3(0.0f, 0.0f, 1.0f));
		rotation = glm::rotate(rotation, rotation_angles.x,
			glm::vec3(1.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, rotation_angles.y,
			glm::vec3(0.0f, 1.0f, 0.0f));

		return rotation;
	}

	Transformation MapLoader::get_transformation(
		const glm::vec3 &translation, const glm::vec3 &rotation_angles,
		const float scale)
	{
		Transformation transformation;

		transformation.set_rotation(get_rotation(rotation_angles));
		transformation.set_translation(translation);
		transformation.set_scale(scale);

		return transformation;
	}

	ObjectData MapLoader::get_object_data(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const String &name,
		const float scale, const bool transparent, const Uint32 id,
		const SelectionType selection) const
	{
		float transparency;

		assert(glm::all(glm::lessThanEqual(glm::abs(translation),
			glm::vec3(1e7f))));

		if (transparent)
		{
			transparency = 0.7f;
		}
		else
		{
			transparency = 1.0f;
		}

		return ObjectData(get_transformation(translation,
			rotation_angles, scale), name, transparency, id,
				selection, transparent);
	}

	StringVector MapLoader::get_material_names(const Uint32 index,
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

	void MapLoader::add_object(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const String &name,
		const float scale, const float transparency,
		const Uint32 id, const bool blended,
		const SelectionType selection,
		const StringVector &material_names)
	{
		if (blended)
		{
			m_map->add_object(get_object_data(translation,
				rotation_angles, name, scale, true, id,
				selection));
		}
		else
		{
			m_instances_builder->add(get_object_data(translation,
				rotation_angles, name, scale, false, id,
				selection));
		}
	}

	void MapLoader::add_light(const glm::vec3 &position,
		const glm::vec3 &color, const float radius, const Uint32 id)
	{
		m_map->add_light(LightData(position, color, radius, id));
	}

	void MapLoader::add_particle(const glm::vec3 &position,
		const String &name, const Uint32 id)
	{
		m_map->add_particle(ParticleData(position, name, id));
	}

	void MapLoader::add_terrain(const StringArray4 &albedo,
		const String &blend, const String &height, const String &dudv,
		const glm::vec3 &translation, const glm::vec3 &rotation_angles,
		const float scale)
	{
		AbstractTerrainManagerAutoPtr terrain;
		Transformation transformation;

		terrain.reset(new SimpleTerrainManager(get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_mesh_builder(), get_effect_cache(),
			get_texture_cache(), transformation, albedo, blend,
			height, dudv));

		m_map->add_terrain(terrain);
	}

	void MapLoader::set_tile(const Uint16 x, const Uint16 y,
		const Uint16 tile)
	{
		MaterialEffectDescriptionVector materials;
		MaterialEffectDescription material;
		StringStream str;
		String file_name;
		Transformation transformation;
		glm::vec3 offset;
		Uint32 id, i, j;

		m_map->set_tile(x, y, tile);

		if (tile == 255)
		{
			return;
		}

		offset.x = x * get_tile_size();
		offset.y = y * get_tile_size();

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

		switch (tile)
		{
			case 0:
				return;
			case 240:
				str << UTF8("lava");
				break;
			default:
				str << UTF8("tile") <<
					static_cast<Uint16>(tile);
				break;
		}

		material.set_material_descriptiont(
			get_material_description_cache(
				)->get_material_description(String(str.str())));

		material.set_world_transformation(String(UTF8("default")));

		materials.push_back(material);

		id = get_free_ids().use_typeless_id(x + (y << 10),
			it_tile_object);

		m_instances_builder->add(ObjectData(transformation,
			String(UTF8("plane_4")), 0.0f, id, st_none, false),
			materials);

		if (tile != 1)
		{
			return;
		}

		material.set_material_descriptiont(
			get_material_description_cache(
				)->get_material_description(String(UTF8("grass-billboard"))));

		material.set_world_transformation(String(UTF8("billboard")));
		material.set_culling(false);
		material.set_transparent(true);

		materials.clear();
		materials.push_back(material);

		transformation.set_scale(1.0f);

		for (i = 0; i < 12; ++i)
		{
			for (j = 0; j < 12; ++j)
			{
				offset.x = x * get_tile_size() + (i + (j % 2) * 0.5f) * 0.25f;
				offset.y = y * get_tile_size() + (j + (i % 2) * 0.5f) * 0.25f;
				offset.z = -0.0011f;

				offset.x += SimplexNoise::noise(offset * 0.24123542f + glm::vec3(0.0f, 0.0f, 0.0f)) * 0.1f - 0.05f;
				offset.y += SimplexNoise::noise(offset * 0.24123542f + glm::vec3(0.0f, 0.0f, 1.0f)) * 0.1f - 0.05f;

				transformation.set_translation(offset);
				
				id = get_free_ids().get_next_free_id(
					it_dynamic_object);

				m_instances_builder->add(ObjectData(transformation,
					String(UTF8("billboard")), 0.0f, id,
					st_none, false), materials);
			}
		}
	}

	void MapLoader::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		m_map->set_height(x, y, height);
	}

	void MapLoader::set_ambient(const glm::vec3 &ambient)
	{
		m_map->set_ambient(ambient);
	}

	void MapLoader::set_height_map_size(const Uint16 width,
		const Uint16 height)
	{
		m_map->set_height_map_size(width, height);
	}

	void MapLoader::set_tile_map_size(const Uint16 width,
		const Uint16 height)
	{
		m_map->set_tile_map_size(width, height);
	}

	void MapLoader::set_dungeon(const bool dungeon)
	{
		m_map->set_dungeon(dungeon);
	}

}
