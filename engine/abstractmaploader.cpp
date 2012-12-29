/****************************************************************************
 *            abstractmaploader.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "globalvars.hpp"
#include "lightdata.hpp"
#include "exceptions.hpp"
#include "freeidsmanager.hpp"
#include "glmutil.hpp"
#include "simplexnoise.hpp"
#include "codec/codecmanager.hpp"
#include "terrain/terrainmaterialdata.hpp"
#include "image.hpp"
#include "tilebuilder.hpp"

namespace eternal_lands
{

	StringSet AbstractMapLoader::load_harvestables(
		const FileSystemSharedPtr &file_system)
	{
		std::vector<std::string> lines, line;
		std::vector<std::string>::iterator it;
		std::string str;
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
				result.insert(
					FileSystem::get_strip_relative_path(
						String(str)));
			}

			it++;
		}

		return result;
	}

	StringSet AbstractMapLoader::load_entrables(
		const FileSystemSharedPtr &file_system)
	{
		std::vector<std::string> lines, line;
		std::vector<std::string>::iterator it;
		std::string str;
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
				str = UTF8("3dobjects/");
				str += *it;
				result.insert(
					FileSystem::get_strip_relative_path(
						String(str)));
			}

			it++;
		}

		return result;
	}

	AbstractMapLoader::AbstractMapLoader(
		const FileSystemSharedPtr &file_system,
		const FreeIdsManagerSharedPtr &free_ids,
		const GlobalVarsSharedPtr &global_vars):
		m_file_system(file_system), m_free_ids(free_ids),
		m_global_vars(global_vars)
	{
		assert(m_global_vars.get() != nullptr);
		assert(m_free_ids.get() != nullptr);
		assert(m_file_system.get() != nullptr);

		m_harvestables = load_harvestables(file_system);
		m_entrables = load_entrables(file_system);
	}

	AbstractMapLoader::~AbstractMapLoader() noexcept
	{
	}

	bool AbstractMapLoader::check_format(const Uint8Array8 &id) noexcept
	{
		return true;
	}

	SelectionType AbstractMapLoader::get_selection(const String &name) const
		noexcept
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
		const Uint32 offset, const MapVersionType version)
	{
		glm::vec3 translation, rotation_angles, scale;
		String name;
		BitSet64 blend_mask;
		BitSet8 options;
		float transparency, glow;
		Uint32 id, blended, material_index, material_count;
		SelectionType selection;
		BlendType blend;

		get_reader()->set_position(offset);

		name = get_reader()->read_utf8_string(80);

		translation.x = get_reader()->read_float_le();
		translation.y = get_reader()->read_float_le();
		translation.z = get_reader()->read_float_le();

		rotation_angles.x = get_reader()->read_float_le();
		rotation_angles.y = get_reader()->read_float_le();
		rotation_angles.z = get_reader()->read_float_le();

		options = get_reader()->read_u8();
		blended = get_reader()->read_u8();

		blend = static_cast<BlendType>(blended);
		selection = static_cast<SelectionType>(
			get_reader()->read_u8());
		transparency = get_reader()->read_u8() / 255.0f;

		material_index = get_reader()->read_u32_le();
		material_count = get_reader()->read_u32_le();
		id = get_reader()->read_u32_le();

		scale.x = get_reader()->read_half_le();
		scale.y = get_reader()->read_half_le();
		scale.z = get_reader()->read_half_le();
		glow = get_reader()->read_half_le();

		if (options[0])
		{
			blend_mask.set();
		}
		else
		{
			blend_mask.reset();
		}

		if (version == mvt_1_0)
		{
			if (blended == 20)
			{
				return;
			}

			blend = bt_alpha_transparency_source_value;
			transparency = 0.7f;
			scale = glm::vec3(1.0f);
			material_count = 0;
			material_index = 0;
			name = FileSystem::get_strip_relative_path(name);
			selection = get_selection(name);
			id = index;
			glow = 0.0f;

			if (blended != 0)
			{
				blend_mask.set();
			}
			else
			{
				blend_mask.reset();
			}
		}

		LOG_DEBUG(lt_map_loader, UTF8("Adding 3d object (%1%) '%2%' at"
			" %3% with rotation %4%, scale %5%, blend mask %6%, "
			"blended %7%."), index % name %
			glm::to_string(translation) %
			glm::to_string(rotation_angles) % glm::to_string(scale)
			% blend_mask % blended);

		if ((material_index + material_count) > m_names.size())
		{
			LOG_ERROR(lt_map_loader, UTF8("File '%1%' has wrong "
				"material name index [%2%] and count [%3%] for"
				" object %4% [%5%]."), get_reader()->get_name()
				% material_index % material_count % name %
				index);

			material_count = 0;
			material_index = 0;
		}

		id = get_free_ids().use_typeless_object_id(id, it_3d_object);

		add_object(translation, rotation_angles, scale, name,
			blend_mask, transparency, glow, id, selection, blend,
			get_names(material_index, material_count));
	}

	void AbstractMapLoader::read_2d_object(const Uint32 index,
		const Uint32 offset, const MapVersionType version)
	{
		glm::vec3 translation, rotation_angles;
		String name;
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

		LOG_DEBUG(lt_map_loader, UTF8("Adding 2d object (%1%) '%2%' at"
			"%3% with rotation %4%."), index % name %
			glm::to_string(translation) %
			glm::to_string(rotation_angles));

		add_object(translation, rotation_angles, glm::vec3(1.0f), name,
			0, 0.0f, 0.0f, id, st_none,
			bt_alpha_transparency_source_value, StringVector());
	}

	void AbstractMapLoader::read_light(const Uint32 index,
		const Uint32 offset, const MapVersionType version)
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

		radius = get_reader()->read_half_le();

		if (version == mvt_1_0)
		{
			scale = std::max(std::max(color.r, color.g),
				std::max(color.b, 1.0f));
			color /= scale;
			radius = 2.5f * scale;
		}

		LOG_DEBUG(lt_map_loader, UTF8("Adding light (%1%) at %2% "
			"with color %3% and radius %4%."), index %
			glm::to_string(position) %
			glm::to_string(color) % radius);

		get_free_ids().use_light_id(index);

		add_light(position, color, radius, index);
	}

	void AbstractMapLoader::read_particle(const Uint32 index,
		const Uint32 offset, const MapVersionType version)
	{
		glm::vec3 position;
		String name;

		get_reader()->set_position(offset);

		name = get_reader()->read_utf8_string(80);

		position.x = get_reader()->read_float_le();
		position.y = get_reader()->read_float_le();
		position.z = get_reader()->read_float_le();

		LOG_DEBUG(lt_map_loader, UTF8("Adding particle (%1%) at %2% "
			"with name '%3%'."), index % glm::to_string(position) %
			name);

		get_free_ids().use_particle_id(index);

		add_particle(position, name, index);
	}

	void AbstractMapLoader::read_decal(const Uint32 index,
		const Uint32 offset, const MapVersionType version)
	{
		String texture;
		glm::vec2 position, scale;
		float rotation;

		get_reader()->set_position(offset);

		texture = get_reader()->read_utf8_string(80);

		position.x = get_reader()->read_float_le();
		position.y = get_reader()->read_float_le();
		scale.x = get_reader()->read_float_le();
		scale.y = get_reader()->read_float_le();
		rotation = get_reader()->read_float_le();

		LOG_DEBUG(lt_map_loader, UTF8("Adding decal (%1%) at %2% "
			"with scale %3%, rotation %4% and texture '%5%'."),
			index % glm::to_string(position) %
			glm::to_string(scale) % rotation % texture);

		get_free_ids().use_decal_id(index);

		add_decal(position, scale, rotation, texture, index);
	}

	void AbstractMapLoader::read_terrain(const Uint32 offset,
		const MapVersionType version)
	{
		TerrainMaterialData material_data;
		ImageCompressionTypeSet compressions;
		ImageSharedPtr displacement_map, normal_tangent_map, dudv_map;
		ImageSharedPtr blend_map;
		Vec3Vector default_speculars;
		FloatVector default_glosses, default_heights, blend_sizes;
		StringVector albedo_maps, specular_maps, gloss_maps;
		StringVector height_maps;
		String displacement_map_name, normal_tangent_map_name;
		String dudv_map_name, blend_map_name, albedo_map, height_map;
		String specular_map, gloss_map, dir_name;
		glm::vec4 dudv_scale_offset;
		glm::vec3 default_specular, translation;
		glm::uvec3 image_size;
		glm::uvec2 size;
		BitSet64 use_blend_size_textures, use_height_maps, mask;
		BitSet64 use_blend_sizes, use_specular_maps, use_gloss_maps;
		float blend_size, default_height, default_gloss;
		Uint32 i, count;

		if (version == mvt_1_0)
		{
			return;
		}

		try
		{
			get_reader()->set_position(offset);

			dir_name = FileSystem::get_dir_name(
				m_reader->get_name());

			if (!dir_name.get().empty())
			{
				dir_name = String(dir_name.get() + UTF8("/"));
			}

			displacement_map_name = String(dir_name.get() + 
				get_reader()->read_utf8_string(128).get());
			normal_tangent_map_name = String(dir_name.get() + 
				get_reader()->read_utf8_string(128).get());
			dudv_map_name = String(dir_name.get() + 
				get_reader()->read_utf8_string(128).get());
			blend_map_name = String(dir_name.get() + 
				get_reader()->read_utf8_string(128).get());

			dudv_scale_offset.x = get_reader()->read_float_le();
			dudv_scale_offset.y = get_reader()->read_float_le();
			dudv_scale_offset.z = get_reader()->read_float_le();
			dudv_scale_offset.w = get_reader()->read_float_le();

			translation.x = get_reader()->read_float_le();
			translation.y = get_reader()->read_float_le();
			translation.z = get_reader()->read_float_le();

			size.x = get_reader()->read_u32_le();
			size.y = get_reader()->read_u32_le();

			count = get_reader()->read_u32_le();

			use_blend_size_textures = get_reader()->read_u64_le();
			use_specular_maps = get_reader()->read_u64_le();
			use_gloss_maps = get_reader()->read_u64_le();
			use_height_maps = get_reader()->read_u64_le();

			for (i = 0; i < count; ++i)
			{
				mask[i] = true;

				default_specular.r =
					get_reader()->read_float_le();
				default_specular.g =
					get_reader()->read_float_le();
				default_specular.b =
					get_reader()->read_float_le();
				default_gloss = get_reader()->read_float_le();
				default_height = get_reader()->read_float_le();
				blend_size = get_reader()->read_float_le();

				albedo_map = get_reader()->read_utf8_string(
					128);
				specular_map = get_reader(
					)->read_utf8_string(128);
				gloss_map = get_reader(
					)->read_utf8_string(128);
				height_map = get_reader()->read_utf8_string(
					128);

				default_speculars.push_back(default_specular);
				default_glosses.push_back(default_gloss);
				default_heights.push_back(default_height);
				blend_sizes.push_back(blend_size);
				albedo_maps.push_back(albedo_map);
				specular_maps.push_back(specular_map);
				gloss_maps.push_back(gloss_map);
				height_maps.push_back(height_map);
			}

			use_blend_size_textures &= mask;
			use_specular_maps &= mask;
			use_gloss_maps &= mask;
			use_height_maps &= mask;

			material_data.set_use_blend_size_textures(
				use_blend_size_textures);
			material_data.set_use_specular_maps(use_specular_maps);
			material_data.set_use_gloss_maps(use_gloss_maps);
			material_data.set_use_height_maps(use_height_maps);
			material_data.set_blend_sizes(blend_sizes);
			material_data.set_default_speculars(default_speculars);
			material_data.set_default_glosses(default_glosses);
			material_data.set_default_heights(default_heights);

			LOG_DEBUG(lt_map_loader, UTF8("Adding terrain with "
				"displacement '%1%', normal & tangengt '%2%', "
				"dudv '%3%' and blend '%4%', dudv scale offset "
				"%5%, size %6%, material '%7%', albedo maps "
				"'%8%', specular maps '%9%', gloss maps '%10%',"
				" height maps '%11%' and translation %12%."),
				displacement_map_name % normal_tangent_map_name
				% dudv_map_name % blend_map_name %
				glm::to_string(dudv_scale_offset) %
				glm::to_string(size) %
				material_data.save_xml_string() %
				to_string(albedo_maps) %
				to_string(specular_maps) %
				to_string(gloss_maps) % to_string(height_maps)
				% glm::to_string(translation));

			if (get_global_vars()->get_opengl_3_0())
			{
				compressions.insert(ict_rgtc);
			}

			displacement_map = CodecManager::load_image(
				displacement_map_name, get_file_system(),
				compressions, true, false, false);

			normal_tangent_map = CodecManager::load_image(
				normal_tangent_map_name, get_file_system(),
				compressions, true, false, false);

			dudv_map = CodecManager::load_image(
				dudv_map_name, get_file_system(), compressions,
				true, false, false);

			blend_map = CodecManager::load_image(blend_map_name,
				get_file_system(), compressions, true, false,
				false);

			image_size = displacement_map->get_size();

			if ((image_size.x != size.x) &&
				((image_size.y) != size.y) &&
				((image_size.z) != 0))
			{
				EL_THROW_EXCEPTION(SizeErrorException()
					<< errinfo_message(UTF8(
						"Image has wrong size"))
					<< errinfo_width(image_size.x)
					<< errinfo_height(image_size.y)
					<< errinfo_depth(image_size.z)
					<< errinfo_expected_width(size.x)
					<< errinfo_expected_height(size.y)
					<< errinfo_expected_depth(0)
					<< boost::errinfo_file_name(
						displacement_map_name));
			}

			image_size = normal_tangent_map->get_size();

			if ((image_size.x != size.x) &&
				((image_size.y) != size.y) &&
				((image_size.z) != 0))
			{
				EL_THROW_EXCEPTION(SizeErrorException()
					<< errinfo_message(UTF8(
						"Image has wrong size"))
					<< errinfo_width(image_size.x)
					<< errinfo_height(image_size.y)
					<< errinfo_depth(image_size.z)
					<< errinfo_expected_width(size.x)
					<< errinfo_expected_height(size.y)
					<< errinfo_expected_depth(0)
					<< boost::errinfo_file_name(
						normal_tangent_map_name));
			}

			image_size = dudv_map->get_size();

			if ((image_size.x != size.x) &&
				((image_size.y) != size.y) &&
				((image_size.z) != 0))
			{
				EL_THROW_EXCEPTION(SizeErrorException()
					<< errinfo_message(UTF8(
						"Image has wrong size"))
					<< errinfo_width(image_size.x)
					<< errinfo_height(image_size.y)
					<< errinfo_depth(image_size.z)
					<< errinfo_expected_width(size.x)
					<< errinfo_expected_height(size.y)
					<< errinfo_expected_depth(0)
					<< boost::errinfo_file_name(
						dudv_map_name));
			}

			set_terrain(displacement_map, normal_tangent_map,
				dudv_map, blend_map, albedo_maps,
				specular_maps, gloss_maps, height_maps,
				material_data, dudv_scale_offset,
				translation, size);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}
		catch (const std::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}
	}

	void AbstractMapLoader::read_name(const Uint32 index,
		const MapVersionType version)
	{
		m_names[index] = get_reader()->read_dynamic_utf8_string();
	}

	void AbstractMapLoader::read_3d_objects(const Uint32 obj_3d_size,
		const Uint32 obj_3d_count, const Uint32 obj_3d_offset,
		const MapVersionType version)
	{
		Uint32 i;

		LOG_DEBUG(lt_map_loader, UTF8("Loading %1% 3d objects."),
			obj_3d_count);

		for (i = 0; i < obj_3d_count; ++i)
		{
			try
			{
				read_3d_object(i, obj_3d_offset +
					i * obj_3d_size, version);
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
		const Uint32 obj_2d_count, const Uint32 obj_2d_offset,
		const MapVersionType version)
	{
		Uint32 i;

		LOG_DEBUG(lt_map_loader, UTF8("Loading %1% 2d objects."),
			obj_2d_count);

		for (i = 0; i < obj_2d_count; ++i)
		{
			try
			{
				read_2d_object(i, obj_2d_offset +
					i * obj_2d_size, version);
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
		const Uint32 light_count, const Uint32 light_offset,
		const MapVersionType version)
	{
		Uint32 i;

		LOG_DEBUG(lt_map_loader, UTF8("Loading %1% lights."),
			light_count);

		for (i = 0; i < light_count; ++i)
		{
			try
			{
				read_light(i, light_offset + i * light_size,
					version);
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
		const Uint32 particle_count, const Uint32 particle_offset,
		const MapVersionType version)
	{
		Uint32 i;

		LOG_DEBUG(lt_map_loader, UTF8("Loading %1% particles."),
			particle_count);

		for (i = 0; i < particle_count; ++i)
		{
			try
			{
				read_particle(i, particle_offset +
					i * particle_size, version);
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

	void AbstractMapLoader::read_decals(const Uint32 decal_size,
		const Uint32 decal_count, const Uint32 decal_offset,
		const MapVersionType version)
	{
		Uint32 i;

		LOG_DEBUG(lt_map_loader, UTF8("Loading %1% decals."),
			decal_count);

		for (i = 0; i < decal_count; ++i)
		{
			try
			{
				read_decal(i, decal_offset + i * decal_size,
					version);
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
		const Uint32 height_map_height, const Uint32 height_map_offset,
		const MapVersionType version)
	{
		Uint32 x, y;

		LOG_DEBUG(lt_map_loader, UTF8("Height size <%1%, %2%>."),
			height_map_width % height_map_height);

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

	void AbstractMapLoader::read_tile_map(const Uint32 width,
		const Uint32 height, const Uint32 index, const Uint32 offset,
		const float z_position, const MapVersionType version)
	{
		Uint8MultiArray2 tile_map;
		Uint32 x, y, tile;

		LOG_DEBUG(lt_map_loader, UTF8("Tile layer %1% size <%2%, "
			"%3%>, %4%."), index % width % height % z_position);

		get_reader()->set_position(offset);

		tile_map.resize(boost::extents[width][height]);

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				try
				{
					tile = get_reader()->read_u8();

					tile_map[x][y] = tile;
				}
				catch (boost::exception &exception)
				{
					exception << errinfo_array_index_0(x);
					exception << errinfo_array_index_1(y);
					exception << errinfo_array_index_2(
						index);
					LOG_EXCEPTION(exception);
				}
				catch (const std::exception &exception)
				{
					LOG_EXCEPTION(exception);
				}
			}
		}

		set_tile_layer(tile_map, z_position, index);
	}

	void AbstractMapLoader::read_old_tile_map(const Uint32 tile_map_width,
		const Uint32 tile_map_height, const Uint32 tile_map_offset,
		const MapVersionType version)
	{
		Uint8MultiArray2 tile_map, water_map;
		Uint32 i, j, x, y, tile, water, data, no_tile;
		bool tile_used, water_used;

		LOG_DEBUG(lt_map_loader, UTF8("Tile map size <%1%, %2%>."),
			tile_map_width % tile_map_height);

		get_reader()->set_position(tile_map_offset);

		no_tile = std::numeric_limits<Uint16>::max();

		tile_map.resize(boost::extents[tile_map_width * 6]
			[tile_map_height * 6]);
		water_map.resize(boost::extents[tile_map_width * 6]
			[tile_map_height * 6]);

		for (y = 0; y < tile_map_height; y++)
		{
			for (x = 0; x < tile_map_width; x++)
			{
				try
				{
					data = get_reader()->read_u8();

					tile = no_tile;
					water = no_tile;

					if (data != 0xFF)
					{
						if ((data == 0) || (data > 230))
						{
							water = data;
							tile = no_tile;
						}
						else
						{
							water = no_tile;
							tile = data;
						}
					}

					tile_used |= tile != no_tile;
					water_used |= water != no_tile;

					for (j = 0; j < 6; ++j)
					{
						for (i = 0; i < 6; ++i)
						{
							tile_map[x * 6 + i]
								[y * 6 + j] =
									tile;
							water_map[x * 6 + i]
								[y * 6 + j] =
									water;
						}
					}
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

		set_tile_layer_heights(glm::vec4(-0.001f, -0.25f, 0.0f, 0.0f));

		set_tile_layer(tile_map, -0.001f, 0);
		set_tile_layer(water_map, -0.25f, 1);
	}

	void AbstractMapLoader::read_tile_maps(const Uint32 tile_map_width,
		const Uint32 tile_map_height, const Uint32 tile_map_offset,
		const MapVersionType version)
	{
		glm::vec4 tile_layer_heights;
		Uint32 i, count, size;

		LOG_DEBUG(lt_map_loader, UTF8("Tile size <%1%, %2%>."),
			tile_map_width % tile_map_height);

		get_reader()->set_position(tile_map_offset);

		count = get_reader()->read_u16_le();
		count = std::min(count, 4u);

		for (i = 0; i < count; ++i)
		{
			tile_layer_heights[i] = get_reader()->read_half_le();
		}

		LOG_DEBUG(lt_map_loader, UTF8("Tile maps %1%, heights %2%."),
			count % glm::to_string(tile_layer_heights));

		set_tile_layer_heights(tile_layer_heights);

		size = tile_map_width * tile_map_height;

		for (i = 0; i < count; ++i)
		{
			read_tile_map(tile_map_width, tile_map_height, i,
				tile_map_offset + 2 + 2 * count + size * i,
				tile_layer_heights[i], version);
		}
	}

	void AbstractMapLoader::read_names(const Uint32 name_count,
		const Uint32 name_offset, const MapVersionType version)
	{
		Uint32 i;

		LOG_DEBUG(lt_map_loader, UTF8("Loading %1% names."),
			name_count);

		m_names.resize(name_count);

		for (i = 0; i < name_count; ++i)
		{
			try
			{
				read_name(i, version);
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

	void AbstractMapLoader::read(const String &name,
		MapItemsTypeSet skip_items)
	{
		glm::vec4 ground_hemisphere;
		glm::uvec2 height_map_size, map_size, tile_map_size;
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
//		Uint32 clusters_offset;
		Uint32 terrain_offset;
		Uint32 name_count;
		Uint32 name_offset; 
		Sint8Array4 magic_number;
		Uint16 version_number, version_major, version_minor;
		MapVersionType version;
		bool dungeon;

		m_reader = get_file_system()->get_file(name);

		LOG_DEBUG(lt_map_loader, UTF8("Loading map '%1%'."),
			get_reader()->get_name());

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

		tile_map_size.x = get_reader()->read_u32_le();
		tile_map_size.y = get_reader()->read_u32_le();
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

		dungeon = get_reader()->read_u8() != 0;

		LOG_DEBUG(lt_map_loader, UTF8("dungeon %1%."), dungeon);

		set_dungeon(dungeon);

		get_reader()->skip(sizeof(Uint8));

		version_major = get_reader()->read_u8();
		version_minor = get_reader()->read_u8();

		version_number = version_major << 8;
		version_number |= version_minor;

		ground_hemisphere.r = get_reader()->read_float_le();
		ground_hemisphere.g = get_reader()->read_float_le();
		ground_hemisphere.b = get_reader()->read_float_le();
		ground_hemisphere.a = 0.0f;

		LOG_DEBUG(lt_map_loader, UTF8("ground hemisphere color %1%."),
			glm::to_string(ground_hemisphere));

		set_ground_hemisphere(ground_hemisphere);

		particle_size = get_reader()->read_u32_le();
		particle_count = get_reader()->read_u32_le();
		particle_offset = get_reader()->read_u32_le();
//		clusters_offset = get_reader()->read_u32_le();
		get_reader()->skip(sizeof(Uint32));

		name_count = get_reader()->read_u32_le();
		name_offset = get_reader()->read_u32_le();
		terrain_offset = get_reader()->read_u32_le();

		switch (version_number)
		{
			case 0x0000:
			case 0x0100:
				version = mvt_1_0;
				version_major = 1;
				version_minor = 0;
				break;
			case 0x0101:
				version = mvt_1_1;
				version_major = 1;
				version_minor = 1;
				break;
			default:
				LOG_ERROR(lt_map_loader, UTF8("File '%1%' has "
					"invalid version number %2%.%3%"),
					get_reader()->get_name() %
					version_major % version_minor);
				version = mvt_1_0;
				break;
		}

		LOG_DEBUG(lt_map_loader, UTF8("Map version %1%.%2%."),
			version_major % version_minor);

		if (version == 0)
		{
			terrain_offset = 0;
			name_count = 0;
			name_offset = 0;

			map_size = tile_map_size * 6u;
			height_map_size = tile_map_size * 6u;
		}
		else
		{
			map_size = tile_map_size;
			height_map_size = tile_map_size;
		}

		LOG_DEBUG(lt_map_loader, UTF8("map size <%1%, %2%>."),
			map_size.x % map_size.y);

		set_map_size(map_size);
		set_tile_map_size(tile_map_size);
		set_height_map_size(height_map_size);

		if (obj_3d_size != get_3d_object_size())
		{
			LOG_ERROR(lt_map_loader, UTF8("File '%1%' has wrong 3d"
				" object size of %2% instead of %3%."),
				get_reader()->get_name() %
				obj_3d_size % get_3d_object_size());
			obj_3d_count = 0;
			obj_3d_offset = 0;

			skip_items.insert(mit_3d_objects);
		}

		if (obj_2d_size != get_2d_object_size())
		{
			LOG_ERROR(lt_map_loader, UTF8("File '%1%' has wrong 2d"
				" object size of %2% instead of %3%."),
				get_reader()->get_name() %
				obj_2d_size % get_2d_object_size());
			obj_2d_count = 0;
			obj_2d_offset = 0;

			skip_items.insert(mit_2d_objects);
		}

		if (light_size != get_light_size())
		{
			LOG_ERROR(lt_map_loader, UTF8("File '%1%' has wrong "
				"light size of %2% instead of %3%."),
				get_reader()->get_name() %
				light_size % get_light_size());
			light_count = 0;
			light_offset = 0;

			skip_items.insert(mit_lights);
		}

		if (particle_size != get_particle_size())
		{
			LOG_ERROR(lt_map_loader, UTF8("File '%1%' has wrong "
				"particle size of %2% instead of %3%."),
				get_reader()->get_name() %
				particle_size % get_particle_size());
			particle_count = 0;
			particle_offset = 0;

			skip_items.insert(mit_particles);
		}

		if (terrain_offset == 0)
		{
			skip_items.insert(mit_terrain);
		}

		read_names(name_count, name_offset, version);

		if (skip_items.count(mit_3d_objects) == 0)
		{
			read_3d_objects(obj_3d_size, obj_3d_count,
				obj_3d_offset, version);
		}

		if (skip_items.count(mit_2d_objects) == 0)
		{
			read_2d_objects(obj_2d_size, obj_2d_count,
				obj_2d_offset, version);
		}

		if (skip_items.count(mit_lights) == 0)
		{
			read_lights(light_size, light_count, light_offset,
				version);
		}

		if (skip_items.count(mit_particles) == 0)
		{
			read_particles(particle_size, particle_count,
				particle_offset, version);
		}

		if (skip_items.count(mit_height_map) == 0)
		{
			read_height_map(height_map_size.x, height_map_size.y,
				height_map_offset, version);
		}

		if (skip_items.count(mit_tile_map) == 0)
		{
			if (version == mvt_1_0)
			{
				read_old_tile_map(tile_map_size.x,
					tile_map_size.y, tile_map_offset,
					version);
			}
			else
			{
				read_tile_maps(tile_map_size.x, tile_map_size.y,
					tile_map_offset, version);
			}
		}

		if (skip_items.count(mit_terrain) == 0)
		{
			read_terrain(terrain_offset, version);
		}

		LOG_DEBUG(lt_map_loader, UTF8("Done loading map '%1%'."),
			get_reader()->get_name());

		instance();
	}

	void AbstractMapLoader::instance()
	{
	}

	ObjectDescription AbstractMapLoader::get_object_description(
		const glm::vec3 &translation,
		const glm::vec3 &rotation_angles,
		const glm::vec3 &scale, const StringVector &material_names,
		const String &name, const BitSet64 blend_mask,
		const float transparency, const float glow, const Uint32 id,
		const SelectionType selection, const BlendType blend)
	{
		Transformation transformation;

		assert(glm::all(glm::lessThanEqual(glm::abs(translation),
			glm::vec3(1e7f))));

		transformation.set_translation(translation);
		transformation.set_rotation_angles(rotation_angles);
		transformation.set_scale(scale);

		return ObjectDescription(transformation, material_names, name,
			blend_mask, 0.7f, 0.0f, id, selection, blend);
	}

	StringVector AbstractMapLoader::get_names(const Uint32 index,
		const Uint32 count) const
	{
		StringVector result;
		Uint32 i;

		if (index > m_names.size())
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_parameter_name(UTF8("index"))
				<< errinfo_range_index(index)
				<< errinfo_range_max(m_names.size()));
		}

		if ((index + count) > m_names.size())
		{
			EL_THROW_EXCEPTION(RangeErrorException()
				<< errinfo_message(UTF8("index plus count are "
					"too big"))
				<< errinfo_range_index(index + count)
				<< errinfo_range_max(m_names.size()));
		}

		for (i = 0; i < count; ++i)
		{
			result.push_back(m_names[i + index]);
		}

		return result;
	}

}
