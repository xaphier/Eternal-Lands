/****************************************************************************
 *            editormapdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "writer.hpp"

namespace eternal_lands
{

	namespace
	{

		typedef std::multimap<String, Uint32> StringUint32MultiMap;

		void update_materials_list(const StringVector &materials,
			StringVector &material_list,
			StringUint32MultiMap &index_map,
			Uint32 &material_index, Uint32 &material_count)
		{
			StringUint32MultiMap::const_iterator it, begin, end;
			Uint32 i;
			bool found;

			material_count = materials.size();

			if (material_count == 0)
			{
				material_index = 0;

				return; 
			}

			found = false;

			begin = index_map.lower_bound(materials[0]);
			end = index_map.upper_bound(materials[0]);

			for (it = begin; it != end; ++it)
			{
				material_index = it->second;

				if ((material_index + material_count) >=
					material_list.size())
				{
					continue;
				}

				found = true;

				for (i = 0; i < material_count; ++i)
				{
					if (material_list[i + material_index]
						!= materials[i])
					{
						found = false;
						break;
					}
				}

				if (found)
				{
					break;
				}
			}

			if (!found)
			{
				material_index = material_list.size();

				for (i = 0; i < material_count; ++i)
				{
					index_map.insert(
						std::pair<String, Uint32>(
							materials[i],
							material_list.size()));
					material_list.push_back(materials[i]);
				}
			}
		}

	}

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

	void EditorMapData::set_z_near(const float z_near)
	{
		m_scene->set_z_near(z_near);
	}

	void EditorMapData::set_perspective(const float fov,
		const float aspect)
	{
		m_scene->set_perspective(fov, aspect);
	}

	void EditorMapData::set_ortho(const glm::vec4 &ortho)
	{
		m_scene->set_ortho(ortho);
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

	void EditorMapData::set_dungeon(const bool dungeon)
	{
		m_scene->set_dungeon(dungeon);
	}

	bool EditorMapData::get_dungeon() const
	{
		return m_scene->get_dungeon();
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

	Uint16 EditorMapData::get_height(const Uint16 x, const Uint16 y) const
	{
		RANGE_CECK_MAX(x, m_height_map.shape()[0],
			UTF8("index value too big"));
		RANGE_CECK_MAX(y, m_height_map.shape()[1],
			UTF8("index value too big"));

		return m_height_map[x][y];
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
		m_terrain_editor.set_terrain_values(terrain_values);

		m_scene->set_terrain(
			m_terrain_editor.get_terrain_vector_image(),
			m_terrain_editor.get_terrain_normal_image(),
			m_terrain_editor.get_terrain_dudv_image());
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

		typeless_id = std::numeric_limits<Uint32>::max();

		m_id = m_scene->pick(glm::vec2(position), glm::vec2(half_size),
			selection);

		selected = FreeIdsManager::get_id_type(m_id, typeless_id, type);
		m_renderable = rt_none;

		m_scene->deselect_object();

		if (!selected)
		{
			return;
		}

		switch (type)
		{
			case it_3d_object:
			case it_2d_object:
				m_scene->set_selected_object(m_id);
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
		return m_scene->get_scene_resources(
			).get_mesh_data_cache()->get_mesh_materials(name);
	}

	ImageSharedPtr EditorMapData::get_image(const String &name) const
	{
		return m_scene->get_scene_resources().get_codec_manager(
			)->load_image(name, m_scene->get_file_system(),
			ImageCompressionTypeSet(), false, false);
	}

	void EditorMapData::init_terrain(const glm::uvec2 &size)
	{
		m_terrain_editor.init_terrain(size);

		m_scene->set_terrain(
			m_terrain_editor.get_terrain_vector_image(),
			m_terrain_editor.get_terrain_normal_image(),
			m_terrain_editor.get_terrain_dudv_image());
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

	double EditorMapData::get_depth() const
	{
		return m_scene->get_depth();
	}

	void EditorMapData::set_depth_selection(const glm::uvec2 &position)
	{
		m_scene->set_depth_selection(position);
	}

	void EditorMapData::save(const String &file_name) const
	{
		WriterSharedPtr writer;

		writer = boost::make_shared<Writer>(file_name);

		save(writer, mvt_1_1);
	}

	void EditorMapData::save(const WriterSharedPtr &writer,
		const MapVersionType version) const
	{
		std::map<Uint32, EditorObjectDescription>::const_iterator
			object_it, object_end;
		std::map<Uint32, LightData>::const_iterator light_it;
		std::map<Uint32, LightData>::const_iterator light_end;
		std::map<Uint32, ParticleData>::const_iterator particle_it;
		std::map<Uint32, ParticleData>::const_iterator particle_end;
		StringVector material_list;
		StringUint32MultiMap index_map;
		glm::vec3 ambient;
		glm::uvec2 size;
		Uint32 x, y, i, id, count;
		Uint32 material_count, material_index;
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
		Uint32 terrain_offset;
		Uint32 material_name_size;
		Uint32 material_name_count;
		Uint32 material_name_offset; 
		Uint32 tile_map_width;
		Uint32 tile_map_height;
		Uint16 dungeon;
		IdType type;

		writer->write_s8('e');
		writer->write_s8('l');
		writer->write_s8('m');
		writer->write_s8('f');

		writer->write_u32_le(0);	// tile map size x
		writer->write_u32_le(0);	// tile map size y
		writer->write_u32_le(0);	// tile map offset
		writer->write_u32_le(0);	// height map offset

		writer->write_u32_le(0);	// 3d object size
		writer->write_u32_le(0);	// 3d object count
		writer->write_u32_le(0);	// 3d object offset

		writer->write_u32_le(0);	// 2d object size
		writer->write_u32_le(0);	// 2d object count
		writer->write_u32_le(0);	// 2d object offset

		writer->write_u32_le(0);	// light size
		writer->write_u32_le(0);	// light count
		writer->write_u32_le(0);	// light offset

		writer->write_u8(0);		// dungeon
		writer->write_u8(0);		// res_2
		writer->write_u8(1);		// version_major
		writer->write_u8(0);		// version_minor

		writer->write_float_le(0.0f);	// ambient_r
		writer->write_float_le(0.0f);	// ambient_g
		writer->write_float_le(0.0f);	// ambient_b

		writer->write_u32_le(0);	// particle size
		writer->write_u32_le(0);	// particle count
		writer->write_u32_le(0);	// particle offset

		writer->write_u32_le(0);	// clusters_offset
		writer->write_u32_le(0);	// terrain_offset

		writer->write_u32_le(0);	// material_name_size
		writer->write_u32_le(0);	// material_name_count
		writer->write_u32_le(0);	// material_name_offset

		writer->write_u32_le(0);	// reserved_13
		writer->write_u32_le(0);	// reserved_14
		writer->write_u32_le(0);	// reserved_15
		writer->write_u32_le(0);	// reserved_16
		writer->write_u32_le(0);	// reserved_17

		if (get_dungeon())
		{
			dungeon = 1;
		}
		else
		{
			dungeon = 0;
		}

		ambient = get_ambient();

		size = get_tile_map_size();

		tile_map_width = size.x;
		tile_map_height = size.y;

		tile_map_offset = writer->get_position();

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				writer->write_u8(get_tile(x, y));
			}
		}

		size *= 6u;
		height_map_offset = writer->get_position();

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				writer->write_u8(get_height(x, y));
			}
		}

		obj_3d_size = AbstractMapLoader::get_3d_object_size();
		obj_3d_count = 0;
		obj_3d_offset = writer->get_position();

		object_end = m_objects.end();

		for (object_it = m_objects.begin(); object_it != object_end;
			++object_it)
		{
			if (!FreeIdsManager::get_id_type(
				object_it->second.get_id(), id, type))
			{
				continue;
			}

			if (type != it_3d_object)
			{
				continue;
			}

			if ((version == mvt_1_0) && (obj_3d_count < id))
			{
				count = id - obj_3d_count;

				obj_3d_count += count;

				count *= obj_3d_size;

				for (i = 0; i < count; ++i)
				{
					writer->write_u8(20);
				}
			}

			obj_3d_count++;

			writer->write_utf8_string(
				object_it->second.get_name(), 80);

			writer->write_float_le(
				object_it->second.get_translation().x);
			writer->write_float_le(
				object_it->second.get_translation().y);
			writer->write_float_le(
				object_it->second.get_translation().z);

			writer->write_float_le(
				object_it->second.get_rotation_angles().x);
			writer->write_float_le(
				object_it->second.get_rotation_angles().y);
			writer->write_float_le(
				object_it->second.get_rotation_angles().z);

			if (version == mvt_1_0)
			{
				writer->write_u8(0);

				if (object_it->second.get_blend() ==
					bt_disabled)
				{
					writer->write_u8(0);
				}
				else
				{
					writer->write_u8(1);
				}

				writer->write_u8(0);
				writer->write_u8(0);
			}
			else
			{
				writer->write_u8(0);
				writer->write_u8(
					object_it->second.get_blend());
				writer->write_u8(
					object_it->second.get_selection());
				writer->write_u8(
					object_it->second.get_transparency() *
					255.5f);
			}

			writer->write_float_le(0.0f);
			writer->write_float_le(0.0f);
			writer->write_float_le(0.0f);

			writer->write_float_le(object_it->second.get_scale());

			update_materials_list(
				object_it->second.get_material_names(),
				material_list, index_map, material_index,
				material_count);

			writer->write_u32_le(material_index);
			writer->write_u32_le(material_count);
			writer->write_u32_le(id);
			writer->write_u32_le(0);

			for (i = 0; i < 4; ++i)
			{
				writer->write_u8(0);
			}
		}

		obj_2d_size = AbstractMapLoader::get_2d_object_size();
		obj_2d_count = 0;
		obj_2d_offset = writer->get_position();

		object_end = m_objects.end();

		for (object_it = m_objects.begin(); object_it != object_end;
			++object_it)
		{
			if (!FreeIdsManager::get_id_type(
				object_it->second.get_id(), id, type))
			{
				continue;
			}

			if (type != it_2d_object)
			{
				continue;
			}

			obj_2d_count++;

			writer->write_utf8_string(
				object_it->second.get_name(), 80);

			writer->write_float_le(
				object_it->second.get_translation().x);
			writer->write_float_le(
				object_it->second.get_translation().y);
			writer->write_float_le(
				object_it->second.get_translation().z);

			writer->write_float_le(
				object_it->second.get_rotation_angles().x);
			writer->write_float_le(
				object_it->second.get_rotation_angles().y);
			writer->write_float_le(
				object_it->second.get_rotation_angles().z);

			for (i = 0; i < 24; ++i)
			{
				writer->write_u8(0);
			}
		}

		light_size = AbstractMapLoader::get_light_size();
		light_count = 0;
		light_offset = writer->get_position();

		light_end = m_lights.end();

		for (light_it = m_lights.begin(); light_it != light_end;
			++light_it)
		{
			light_count++;

			writer->write_float_le(
				light_it->second.get_position().x);
			writer->write_float_le(
				light_it->second.get_position().y);
			writer->write_float_le(
				light_it->second.get_position().z);

			writer->write_float_le(light_it->second.get_color().r);
			writer->write_float_le(light_it->second.get_color().g);
			writer->write_float_le(light_it->second.get_color().b);

			writer->write_float_le(light_it->second.get_radius());

			for (i = 0; i < 12; ++i)
			{
				writer->write_u8(0);
			}
		}

		particle_size = AbstractMapLoader::get_particle_size();
		particle_count = 0;
		particle_offset = writer->get_position();

		particle_end = m_particles.end();

		for (particle_it = m_particles.begin();
			particle_it != particle_end; ++particle_it)
		{
			particle_count++;

			writer->write_utf8_string(
				particle_it->second.get_name(), 80);
			writer->write_float_le(
				particle_it->second.get_position().x);
			writer->write_float_le(
				particle_it->second.get_position().y);
			writer->write_float_le(
				particle_it->second.get_position().z);

			for (i = 0; i < 12; ++i)
			{
				writer->write_u8(0);
			}
		}

		material_name_size =
			AbstractMapLoader::get_material_name_size();
		material_name_count = 0;
		material_name_offset = writer->get_position();

		BOOST_FOREACH(const String &material, material_list)
		{
			material_name_count++;

			writer->write_utf8_string(material, 128);
		}

		writer->set_position(0);

		writer->write_s8('e');
		writer->write_s8('l');
		writer->write_s8('m');
		writer->write_s8('f');

		writer->write_u32_le(tile_map_width);
		writer->write_u32_le(tile_map_height);
		writer->write_u32_le(tile_map_offset);
		writer->write_u32_le(height_map_offset);

		writer->write_u32_le(obj_3d_size);
		writer->write_u32_le(obj_3d_count);
		writer->write_u32_le(obj_3d_offset);

		writer->write_u32_le(obj_2d_size);
		writer->write_u32_le(obj_2d_count);
		writer->write_u32_le(obj_2d_offset);

		writer->write_u32_le(light_size);
		writer->write_u32_le(light_count);
		writer->write_u32_le(light_offset);

		writer->write_u8(dungeon);
		writer->write_u8(0);		// res_2
		writer->write_u8(1);		// version_major
		writer->write_u8(1);		// version_minor

		writer->write_float_le(ambient.r);
		writer->write_float_le(ambient.g);
		writer->write_float_le(ambient.b);

		writer->write_u32_le(particle_size);
		writer->write_u32_le(particle_count);
		writer->write_u32_le(particle_offset);

		writer->write_u32_le(0);	// clusters_offset
		writer->write_u32_le(0);	// terrain_offset

		writer->write_u32_le(material_name_size);
		writer->write_u32_le(material_name_count);
		writer->write_u32_le(material_name_offset);

		writer->write_u32_le(0);	// reserved_13
		writer->write_u32_le(0);	// reserved_14
		writer->write_u32_le(0);	// reserved_15
		writer->write_u32_le(0);	// reserved_16
		writer->write_u32_le(0);	// reserved_17
	}

}
