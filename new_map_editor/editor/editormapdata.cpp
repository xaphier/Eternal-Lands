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
#include "effect/effectcache.hpp"
#include "writer.hpp"
#include "rstartree.hpp"
#include "filesystem.hpp"
#include "imageupdate.hpp"
#include "tilebuilder.hpp"

namespace eternal_lands
{

	namespace
	{

		typedef std::multimap<String, Uint32> StringUint32MultiMap;

		void update_names_list(const StringVector &materials,
			StringVector &names, StringUint32MultiMap &index_map,
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
					names.size())
				{
					continue;
				}

				found = true;

				for (i = 0; i < material_count; ++i)
				{
					if (names[i + material_index]
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
				material_index = names.size();

				for (i = 0; i < material_count; ++i)
				{
					index_map.insert(
						std::pair<String, Uint32>(
							materials[i],
							names.size()));
					names.push_back(materials[i]);
				}
			}
		}

	}

	EditorMapData::EditorMapData(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system):
		m_id(std::numeric_limits<Uint32>::max()), m_renderable(rt_none)
	{
		m_scene.reset(new EditorScene(global_vars, file_system));
		m_scene->set_sky_hemisphere(glm::vec4(0.2f));
		m_scene->set_main_light_color(glm::vec3(0.8f));
		m_scene->set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		m_scene->set_lights(false);
		clear_tile_layers();
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

	void EditorMapData::set_draw_heights(const bool draw_heights)
	{
		m_scene->set_draw_heights(draw_heights);
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

	void EditorMapData::set_ground_hemisphere(
		const glm::vec4 &ground_hemisphere)
	{
		m_scene->set_ground_hemisphere(ground_hemisphere);
	}

	const glm::vec4 &EditorMapData::get_ground_hemisphere() const
	{
		return m_scene->get_ground_hemisphere();
	}

	void EditorMapData::set_dungeon(const bool dungeon)
	{
		m_scene->set_dungeon(dungeon);
	}

	bool EditorMapData::get_dungeon() const
	{
		return m_scene->get_dungeon();
	}

	void EditorMapData::change_selection(const Uint32 id,
		const RenderableType renderable,
		const SelectionChangeType selection)
	{
		switch (selection)
		{
			case sct_no:
				break;
			case sct_unselect:
				m_renderable = rt_none;
				break;
			case sct_select:
				if (m_renderable != rt_none)
				{
					break;
				}
			case sct_force_select:
				m_id = id;
				m_renderable = renderable;
				break;
		}
	}

	void EditorMapData::add_object(
		const EditorObjectDescription &object_description,
		const SelectionChangeType selection)
	{
		m_objects.insert(std::pair<Uint32,
			EditorObjectDescription>(object_description.get_id(),
			object_description));
		m_scene->add_object(object_description);

		change_selection(object_description.get_id(), rt_object,
			selection);
	}

	void EditorMapData::add_light(const LightData &light,
		const SelectionChangeType selection)
	{
		m_lights.insert(std::pair<Uint32,
			LightData>(light.get_id(), light));
		m_scene->add_light(light);

		change_selection(light.get_id(), rt_light, selection);
	}

	void EditorMapData::add_particle(const ParticleData &particle,
		const SelectionChangeType selection)
	{
		m_particles.insert(std::pair<Uint32,
			ParticleData>(particle.get_id(), particle));
//		m_scene->add_particle(particle);

		change_selection(particle.get_id(), rt_particle, selection);
	}

	void EditorMapData::modify_object(
		const EditorObjectDescription &object_description)
	{
		m_objects.erase(object_description.get_id());
		m_scene->remove_object(object_description.get_id());

		m_scene->get_free_ids()->use_object_id(
			object_description.get_id());

		m_objects.insert(std::pair<Uint32,
			EditorObjectDescription>(object_description.get_id(),
				object_description));
		m_scene->add_object(object_description);
	}

	void EditorMapData::modify_object(
		const EditorObjectDescription &object_description,
		const Uint32 id)
	{
		m_objects.erase(id);
		m_scene->remove_object(id);

		m_scene->get_free_ids()->use_object_id(
			object_description.get_id());

		m_objects.insert(std::pair<Uint32,
			EditorObjectDescription>(object_description.get_id(),
				object_description));
		m_scene->add_object(object_description);
		m_id = object_description.get_id();
	}

	void EditorMapData::modify_light(const LightData &light)
	{
		m_lights.erase(light.get_id());
		m_scene->remove_light(light.get_id());

		m_scene->get_free_ids()->use_light_id(light.get_id());

		m_lights.insert(std::pair<Uint32,
			LightData>(light.get_id(), light));
		m_scene->add_light(light);
	}

	void EditorMapData::modify_particle(const ParticleData &particle)
	{
		m_particles.erase(particle.get_id());
//		m_scene->remove_particle(particle.get_id());

//		m_scene->get_free_ids()->use_particle_id(particle.get_id());

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

	bool EditorMapData::get_objects(const Uint32 id,
		EditorObjectDescriptionVector &object_descriptions) const
	{
		std::map<Uint32, EditorObjectDescription>::const_iterator found;
		std::map<Uint32, EditorObjectDescription>::const_iterator it;
		std::map<Uint32, EditorObjectDescription>::const_iterator end;

		found = m_objects.find(id);
		end = m_objects.end();

		object_descriptions.clear();

		if (found == end)
		{
			return false;
		}

		for (it = m_objects.begin(); it != end; ++it)
		{
			if (found->second.get_name() ==
				it->second.get_name())
			{
				object_descriptions.push_back(it->second);
			}
		}

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

	void EditorMapData::set_tile_values(const ImageValueVector &tile_values,
		const Uint16 layer)
	{
		std::set<Uint16Uint16Pair> values;
		glm::uvec2 offset;
		Uint32 x, y, tile_size;

		RANGE_CECK_MAX(layer, 3, UTF8("layer value too big"));

		tile_size = TileBuilder::get_tile_size();

		BOOST_FOREACH(const ImageValue &tile_value, tile_values)
		{
			x = tile_value.get_x();
			y = tile_value.get_y();

			RANGE_CECK_MAX(x, m_tile_maps.shape()[0] - 1,
				UTF8("x value too big"));
			RANGE_CECK_MAX(y, m_tile_maps.shape()[1] - 1,
				UTF8("y value too big"));

			m_tile_maps[x][y][layer] = tile_value.get_value();

			x = x / tile_size;
			y = y / tile_size;

			values.insert(std::pair<Uint16, Uint16>(x, y));
		}

		BOOST_FOREACH(const Uint16Uint16Pair &value, values)
		{
			offset.x = value.first;
			offset.y = value.second;

			update_tile_page(offset, layer);
		}
	}

	void EditorMapData::update_tile_layer(const Uint16 layer)
	{
		glm::uvec2 offset;
		Uint32 x, y, width, height, tile_size;

		RANGE_CECK_MAX(layer, 3, UTF8("layer value too big"));

		width = m_tile_maps.shape()[0];
		height = m_tile_maps.shape()[1];

		tile_size = TileBuilder::get_tile_size();

		width = (width + tile_size - 1) / tile_size;
		height = (height + tile_size - 1) / tile_size;

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				offset.x = x;
				offset.y = y;

				update_tile_page(offset, layer);
			}
		}
	}

	void EditorMapData::get_tile_values(const glm::uvec3 &offset,
		const Uint16 size, ImageValueVector &tile_values)
	{
		Uint32 x, y, min_x, min_y, max_x, max_y, width, height;

		RANGE_CECK_MAX(offset.z, 3, UTF8("layer value too big"));

		tile_values.clear();

		width = m_tile_maps.shape()[0];
		height = m_tile_maps.shape()[1];

		min_x = 0;
		max_x = std::min(offset.x + size, width - 1);
		min_y = 0;
		max_y = std::min(offset.y + size, height - 1);

		if (offset.x > size)
		{
			min_x = offset.x - size;
		}

		if (offset.y > size)
		{
			min_y = offset.y - size;
		}

		for (y = min_y; y <= max_y; ++y)
		{
			for (x = min_x; x <= max_x; ++x)
			{
				ImageValue tile_value(x, y);

				tile_value.set_value(
					m_tile_maps[x][y][offset.z]);

				tile_values.push_back(tile_value);
			}
		}
	}

	void EditorMapData::update_tile_page(const glm::uvec2 &offset,
		const Uint16 layer)
	{
		Uint16MultiArray2 tile_page;
		Uint32 x, y, xx, yy, width, height, tile_size, tile;

		width = m_tile_maps.shape()[0];
		height = m_tile_maps.shape()[1];

		tile_size = TileBuilder::get_tile_size();

		tile_page.resize(boost::extents[tile_size][tile_size]);

		std::set<Uint16> tiles;

		for (y = 0; y < tile_size; y++)
		{
			for (x = 0; x < tile_size; x++)
			{
				xx = x + offset.x * tile_size;
				yy = y + offset.y * tile_size;

				if ((xx < width) && (yy < height))
				{
					tile = m_tile_maps[xx][yy][layer];

					if (tile == 0xFF)
					{
						tile = std::numeric_limits<
							Uint16>::max();
					}
				}
				else
				{
					tile = std::numeric_limits<
						Uint16>::max();
				}

				tile_page[x][y] = tile;
				tiles.insert(tile);
			}
		}

		set_tile_page(tile_page, offset, get_tile_layer_height(layer),
			layer);
	}

	void EditorMapData::set_tile_page(const Uint16MultiArray2 &tile_page,
		const glm::uvec2 &offset, const float z_position,
		const Uint16 layer)
	{
		Transformation transformation;
		AbstractMeshSharedPtr mesh;
		MaterialSharedPtrVector materials;
		StringStream str;
		BitSet64 blend_mask;
		Uint32 x, y, id, tile, tile_size;
		bool use_tiles;

		id = m_scene->get_free_ids()->get_tile_id(
			offset.x, offset.y, layer);
		tile_size = TileBuilder::get_tile_size();
		use_tiles = false;

		for (y = 0; y < tile_size; y++)
		{
			for (x = 0; x < tile_size; x++)
			{
				tile = tile_page[x][y];
				use_tiles |= tile !=
					std::numeric_limits<Uint16>::max();
			}
		}

		if (!m_scene->get_free_ids()->get_is_object_id_free(id))
		{
			m_scene->remove_object(id);
		}

		if (!use_tiles)
		{
			return;
		}

		m_scene->get_free_ids()->use_object_id(id);

		str << UTF8("tile_") << offset.x << UTF8("x") << offset.y;
		str << UTF8("x") << layer;

		m_scene->get_scene_resources().get_tile_builder()->get_tile(
			tile_page, mesh, materials, blend_mask);

		transformation.set_translation(glm::vec3(glm::vec2(offset *
			tile_size) * TileBuilder::get_tile_world_scale(),
			z_position));

		m_scene->add_object(ObjectData(transformation,
			String(str.str()), blend_mask, 0.0f, 0.0f, id, st_none,
			bt_alpha_transparency_source_value), mesh, materials);
	}

	void EditorMapData::set_tile_layer(const Uint8MultiArray2 &tile_map,
		const Uint16 layer)
	{
		Uint32 x, y, height, width;

		RANGE_CECK_MAX(layer, 3, UTF8("layer value too big"));

		assert(m_tile_maps.shape()[0] == tile_map.shape()[0]);
		assert(m_tile_maps.shape()[1] == tile_map.shape()[1]);

		width = m_tile_maps.shape()[0];
		height = m_tile_maps.shape()[1];

		for (y = 0; y < height; ++y)
		{
			for (x = 0; x < width; ++x)
			{
				m_tile_maps[x][y][layer] = tile_map[x][y];
			}
		}

		update_tile_layer(layer);
	}

	void EditorMapData::clear_tile_layers()
	{
		Uint32 x, y, height, width;

		width = m_tile_maps.shape()[0];
		height = m_tile_maps.shape()[1];

		for (y = 0; y < height; y++)
		{
			for (x = 0; x < width; x++)
			{
				m_tile_maps[x][y][0] = 0xFF;
				m_tile_maps[x][y][1] = 0xFF;
				m_tile_maps[x][y][2] = 0xFF;
				m_tile_maps[x][y][3] = 0xFF;
			}
		}
	}

	Uint16 EditorMapData::get_tile(const Uint16 x, const Uint16 y,
		const Uint16 layer) const
	{
		RANGE_CECK_MAX(x, m_tile_maps.shape()[0] - 1,
			UTF8("x value too big"));
		RANGE_CECK_MAX(y, m_tile_maps.shape()[1] - 1,
			UTF8("y value too big"));
		RANGE_CECK_MAX(layer, 3, UTF8("layer value too big"));

		return m_tile_maps[x][y][layer];
	}

	glm::uvec2 EditorMapData::get_tile_offset(const glm::vec2 &point)
		const
	{
		return glm::uvec2(glm::clamp(point /
			TileBuilder::get_tile_world_scale(), glm::vec2(0.0f),
			glm::vec2(get_tile_map_size())));
	}

	void EditorMapData::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		RANGE_CECK_MAX(x, m_height_map.shape()[0] - 1,
			UTF8("index value too big"));
		RANGE_CECK_MAX(y, m_height_map.shape()[1] - 1,
			UTF8("index value too big"));

		m_scene->set_height(x, y, height);

		m_height_map[x][y] = height;
	}

	Uint16 EditorMapData::get_height(const Uint16 x, const Uint16 y) const
	{
		RANGE_CECK_MAX(x, m_height_map.shape()[0] - 1,
			UTF8("index value too big"));
		RANGE_CECK_MAX(y, m_height_map.shape()[1] - 1,
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

	void EditorMapData::set_terrain_displacement_values(
		const DisplacementValueVector &displacement_values)
	{
		m_terrain_editor.set_displacement_values(displacement_values);

		m_scene->update_terrain_geometry_maps(
			m_terrain_editor.get_displacement_map(),
			m_terrain_editor.get_normal_tangent_map(),
			m_terrain_editor.get_dudv_map(),
			m_terrain_editor.get_translation());
	}

	void EditorMapData::set_terrain_blend_values(
		const ImageValueVector &blend_values, const Uint16 layer)
	{
		m_terrain_editor.set_blend_values(blend_values, layer);

		m_scene->update_terrain_blend_map(
			m_terrain_editor.get_blend_map());
	}

	void EditorMapData::set_terrain_blend_values(
		const ImageValuesVector &blend_values)
	{
		m_terrain_editor.set_blend_values(blend_values);

		m_scene->update_terrain_blend_map(
			m_terrain_editor.get_blend_map());
	}

	void EditorMapData::swap_terrain_blend_layers(const Uint16 idx0,
		const Uint16 idx1)
	{
		m_terrain_editor.swap_blend_layers(idx0, idx1);

		m_scene->update_terrain_blend_map(
			m_terrain_editor.get_blend_map());

		m_scene->set_terrain_material(
			m_terrain_editor.get_albedo_maps(),
			m_terrain_editor.get_specular_maps(),
			m_terrain_editor.get_gloss_maps(),
			m_terrain_editor.get_height_maps(),
			m_terrain_editor.get_material_data());
	}

	void EditorMapData::move_terrain_blend_layer(const Uint16 idx0,
		const Uint16 idx1)
	{
		m_terrain_editor.move_blend_layer(idx0, idx1);

		m_scene->update_terrain_blend_map(
			m_terrain_editor.get_blend_map());

		m_scene->set_terrain_material(
			m_terrain_editor.get_albedo_maps(),
			m_terrain_editor.get_specular_maps(),
			m_terrain_editor.get_gloss_maps(),
			m_terrain_editor.get_height_maps(),
			m_terrain_editor.get_material_data());
	}

	void EditorMapData::set_terrain_material(const String &albedo_map,
		const String &specular_map, const String &gloss_map,
		const String &height_map, const glm::vec3 &default_specular,
		const float default_gloss, const float default_height,
		const float blend_size, const bool use_blend_size_texture,
		const bool use_specular_map, const bool use_gloss_map,
		const bool use_height_map, const Uint16 index)
	{
		m_terrain_editor.set_material(albedo_map, specular_map,
			gloss_map, height_map, default_specular, default_gloss,
			default_height, blend_size, use_blend_size_texture,
			use_specular_map, use_gloss_map, use_height_map, index);

		m_scene->set_terrain_material(
			m_terrain_editor.get_albedo_maps(),
			m_terrain_editor.get_specular_maps(),
			m_terrain_editor.get_gloss_maps(),
			m_terrain_editor.get_height_maps(),
			m_terrain_editor.get_material_data());
	}

	Uint32 EditorMapData::get_free_object_id() const
	{
		return m_scene->get_free_ids()->get_next_free_object_id(
			it_3d_object);
	}

	Uint32Set EditorMapData::get_free_object_ids() const
	{
		Uint32Set result;

		result = m_scene->get_free_ids()->get_free_object_ids(
			it_3d_object).get_free_typeless_ids();

		result.insert(m_scene->get_free_ids()->get_free_object_ids(
			it_3d_object).get_next_free_typeless_id());

		return result;
	}

	Uint32 EditorMapData::get_free_light_id() const
	{
		return m_scene->get_free_ids()->get_next_free_light_id();
	}

	void EditorMapData::use_object_id(const Uint32 id)
	{
		m_scene->get_free_ids()->use_object_id(id);
	}

	void EditorMapData::use_light_id(const Uint32 id)
	{
		m_scene->get_free_ids()->use_light_id(id);
	}

	void EditorMapData::load_map(const String &name,
		const MapItemsTypeSet &skip_items)
	{
		m_renderable = rt_none;

		m_scene->load_map(name, *this, skip_items);
	}

	glm::vec3 EditorMapData::get_map_min() const
	{
		return m_scene->get_map_min();
	}

	glm::vec3 EditorMapData::get_map_max() const
	{
		return m_scene->get_map_max();
	}

	void EditorMapData::draw()
	{
//		m_scene->cull_map();
		m_scene->cull();

		m_scene->draw();
		m_scene->blit_to_back_buffer();
/*
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		m_scene->draw_scale_view(false);
*/
	}

	void EditorMapData::draw_selection(const glm::uvec4 &selection_rect)
	{
		m_scene->draw_selection(selection_rect);
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
			case it_tile_layer_0_object:
			case it_tile_layer_1_object:
			case it_tile_layer_2_object:
			case it_tile_layer_3_object:
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

	ImageSharedPtr EditorMapData::get_image(const String &name,
		TextureFormatType &format) const
	{
		glm::uvec3 size;
		Uint16 mipmaps;
		bool cube_map, array;

		CodecManager::get_image_information(name,
			m_scene->get_file_system(), true, true, format,
			size, mipmaps, cube_map, array);

		return CodecManager::load_image(name,
			m_scene->get_file_system(), ImageCompressionTypeSet(),
			false, false, false);
	}

	void EditorMapData::get_image_data(const String &name,
		TextureFormatType &format, glm::uvec3 &size, Uint16 &mipmaps,
		bool &cube_map, bool &array) const
	{
		CodecManager::get_image_information(name,
			m_scene->get_file_system(), true, true, format, size,
			mipmaps, cube_map, array);
	}

	void EditorMapData::init_terrain(const glm::vec3 &translation,
		const glm::uvec2 &size, const String &albedo_map,
		const String &specular_map, const String &gloss_map,
		const String &height_map, const glm::vec3 &default_specular,
		const float default_gloss, const float default_height,
		const float blend_size, const bool use_blend_size_texture,
		const bool use_specular_map, const bool use_gloss_map,
		const bool use_height_map)
	{
		m_terrain_editor.init(translation, size, albedo_map,
			specular_map, gloss_map, height_map, default_specular,
			default_gloss, default_height, blend_size,
			use_blend_size_texture, use_specular_map,
			use_gloss_map, use_height_map);

		m_scene->set_terrain(m_terrain_editor.get_displacement_map(),
			m_terrain_editor.get_normal_tangent_map(),
			m_terrain_editor.get_dudv_map(),
			m_terrain_editor.get_blend_map(),
			m_terrain_editor.get_albedo_maps(),
			m_terrain_editor.get_specular_maps(),
			m_terrain_editor.get_gloss_maps(),
			m_terrain_editor.get_height_maps(),
			m_terrain_editor.get_material_data(),
			m_terrain_editor.get_dudv_scale_offset(),
			m_terrain_editor.get_translation());
	}

	void EditorMapData::init_terrain(const String &displacement_map_name,
		const glm::vec3 &translation, const glm::uvec2 &size,
		const String &albedo_map, const String &specular_map,
		const String &gloss_map, const String &height_map,
		const glm::vec3 &default_specular, const float default_gloss,
		const float default_height, const float blend_size,
		const bool use_blend_size_texture, const bool use_specular_map,
		const bool use_gloss_map, const bool use_height_map)
	{
		ImageSharedPtr displacement_map;

		displacement_map = CodecManager::load_image(
			displacement_map_name, m_scene->get_file_system(),
			ImageCompressionTypeSet(), false, false, false);

		m_terrain_editor.init(displacement_map, translation, size,
			albedo_map, specular_map, gloss_map, height_map,
			default_specular, default_gloss, default_height,
			blend_size, use_blend_size_texture, use_specular_map,
			use_gloss_map, use_height_map);

		m_scene->set_terrain(m_terrain_editor.get_displacement_map(),
			m_terrain_editor.get_normal_tangent_map(),
			m_terrain_editor.get_dudv_map(),
			m_terrain_editor.get_blend_map(),
			m_terrain_editor.get_albedo_maps(),
			m_terrain_editor.get_specular_maps(),
			m_terrain_editor.get_gloss_maps(),
			m_terrain_editor.get_height_maps(),
			m_terrain_editor.get_material_data(),
			m_terrain_editor.get_dudv_scale_offset(),
			m_terrain_editor.get_translation());
	}

	void EditorMapData::set_terrain_enabled(const bool enabled)
	{
		m_terrain_editor.set_enabled(enabled);
	}

	void EditorMapData::set_terrain_translation(
		const glm::vec3 &translation)
	{
		m_terrain_editor.set_translation(translation);

		m_scene->update_terrain_geometry_maps(
			m_terrain_editor.get_displacement_map(),
			m_terrain_editor.get_normal_tangent_map(),
			m_terrain_editor.get_dudv_map(),
			m_terrain_editor.get_translation());
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

	bool EditorMapData::get_water_vertex(const glm::vec3 &start_position,
		const glm::vec3 &world_position, glm::uvec2 &result) const
	{
		return true;
	}

	void EditorMapData::save(const AbstractProgressSharedPtr &progress,
		const String &file_name) const
	{
		WriterSharedPtr writer;

		writer = boost::make_shared<Writer>(file_name);

		save(progress, writer, mvt_1_1);
	}

	void EditorMapData::import_terrain_height_map(const String &name)
	{
		ImageSharedPtr height_map;

		height_map = CodecManager::load_image(name,
			m_scene->get_file_system(), ImageCompressionTypeSet(),
			false, false, false);

		m_terrain_editor.import_height_map(height_map);

		m_scene->update_terrain_geometry_maps(
			m_terrain_editor.get_displacement_map(),
			m_terrain_editor.get_normal_tangent_map(),
			m_terrain_editor.get_dudv_map(),
			m_terrain_editor.get_translation());
	}

	void EditorMapData::import_terrain_blend_map(const String &name)
	{
		ImageSharedPtr blend_map;

		blend_map = CodecManager::load_image(name,
			m_scene->get_file_system(), ImageCompressionTypeSet(),
			false, false, false);

		m_terrain_editor.import_blend_map(blend_map);

		m_scene->update_terrain_blend_map(
			m_terrain_editor.get_blend_map());
	}

	void EditorMapData::set_terrain(const ImageSharedPtr &displacement_map,
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
		m_terrain_editor.set(displacement_map, normal_tangent_map,
			dudv_map, blend_map, albedo_maps, specular_maps,
			gloss_maps, height_maps, material_data,
			dudv_scale_offset, translation, size);

		m_scene->set_terrain(m_terrain_editor.get_displacement_map(),
			m_terrain_editor.get_normal_tangent_map(),
			m_terrain_editor.get_dudv_map(),
			m_terrain_editor.get_blend_map(),
			m_terrain_editor.get_albedo_maps(),
			m_terrain_editor.get_specular_maps(),
			m_terrain_editor.get_gloss_maps(),
			m_terrain_editor.get_height_maps(),
			m_terrain_editor.get_material_data(),
			m_terrain_editor.get_dudv_scale_offset(),
			m_terrain_editor.get_translation());
	}

	void EditorMapData::update_terrain_dudv()
	{
		m_scene->update_terrain_geometry_maps(
			m_terrain_editor.get_displacement_map(),
			m_terrain_editor.get_normal_tangent_map(),
			m_terrain_editor.get_dudv_map(),
			m_terrain_editor.get_translation());

		m_scene->set_terrain_dudv_scale_offset(
			m_terrain_editor.get_dudv_scale_offset());
	}

	void EditorMapData::clear_invisible_terrain_layers()
	{
		m_terrain_editor.clear_invisible_layers();

		m_scene->update_terrain_blend_map(
			m_terrain_editor.get_blend_map());

		m_scene->set_terrain_material(
			m_terrain_editor.get_albedo_maps(),
			m_terrain_editor.get_specular_maps(),
			m_terrain_editor.get_gloss_maps(),
			m_terrain_editor.get_height_maps(),
			m_terrain_editor.get_material_data());
	}

	void EditorMapData::pack_terrain_layers()
	{
		m_terrain_editor.pack_layers();

		m_scene->update_terrain_blend_map(
			m_terrain_editor.get_blend_map());

		m_scene->set_terrain_material(
			m_terrain_editor.get_albedo_maps(),
			m_terrain_editor.get_specular_maps(),
			m_terrain_editor.get_gloss_maps(),
			m_terrain_editor.get_height_maps(),
			m_terrain_editor.get_material_data());
	}

	const glm::uvec2 &EditorMapData::get_map_size() const
	{
		return m_scene->get_map_size();
	}

	void EditorMapData::set_map_size(const glm::uvec2 &size)
	{
		m_scene->set_map_size(size);
	}

	void EditorMapData::clear()
	{
		m_terrain_editor.clear();
		m_water_image.reset();
		m_objects.clear();
		m_lights.clear();
		m_particles.clear();
		clear_tile_layers();
	}

	void EditorMapData::fill_terrain_blend_layer(const float strength,
		const BlendEffectType effect, const Uint16 layer)
	{
		m_terrain_editor.fill_blend_layer(strength, effect, layer);

		m_scene->update_terrain_blend_map(
			m_terrain_editor.get_blend_map());
	}

	void EditorMapData::save(const AbstractProgressSharedPtr &progress,
		const WriterSharedPtr &writer, const MapVersionType version)
		const
	{
		std::map<Uint32, EditorObjectDescription>::const_iterator
			object_it, object_end;
		std::map<Uint32, LightData>::const_iterator light_it;
		std::map<Uint32, LightData>::const_iterator light_end;
		std::map<Uint32, ParticleData>::const_iterator particle_it;
		std::map<Uint32, ParticleData>::const_iterator particle_end;
		StringVector names;
		StringUint32MultiMap index_map;
		String map_name, displacement_map_name, normal_tangent_map_name;
		String dudv_map_name, blend_map_name;
		glm::vec4 ground_hemisphere;
		glm::uvec2 size;
		BitSet64 mask;
		BitSet8 options;
		Uint32 x, y, i, id, count, position;
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
		Uint32 name_count;
		Uint32 name_offset; 
		Uint32 tile_map_width;
		Uint32 tile_map_height;
		Uint32 terrain_offset;
		Uint16 dungeon;
		IdType type;

		map_name = FileSystem::get_name_without_extension(
			writer->get_name());

		displacement_map_name = String(map_name.get() +
			UTF8("_displacement.dds"));
		normal_tangent_map_name = String(map_name.get() +
			UTF8("_normal_tangent.dds"));
		dudv_map_name = String(map_name.get() + UTF8("_dudv.dds"));
		blend_map_name = String(map_name.get() + UTF8("_blend.dds"));

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

		ground_hemisphere = get_ground_hemisphere();

		size = get_tile_map_size();

		tile_map_width = size.x;
		tile_map_height = size.y;

		tile_map_offset = writer->get_position();

		count = 4;

		writer->write_u16_le(count);

		for (i = 0; i < count; ++i)
		{
			writer->write_half_le(get_tile_layer_height(i));
		}

		for (i = 0; i < count; ++i)
		{
			for (y = 0; y < size.y; ++y)
			{
				for (x = 0; x < size.x; ++x)
				{
					writer->write_u8(get_tile(x, y, i));
				}
			}
		}

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

			position = writer->get_position();

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

				if (object_it->second.get_blend_mask().none())
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
				options[0] = object_it->second.get_blend_mask(
					).any();

				writer->write_u8(options.to_ulong());
				writer->write_u8(
					object_it->second.get_blend());
				writer->write_u8(
					object_it->second.get_selection());
				writer->write_u8(
					object_it->second.get_transparency() *
					255.5f);
			}

			update_names_list(
				object_it->second.get_material_names(),
				names, index_map, material_index,
				material_count);

			writer->write_u32_le(material_index);
			writer->write_u32_le(material_count);
			writer->write_u32_le(id);

			writer->write_half_le(object_it->second.get_scale().x);
			writer->write_half_le(object_it->second.get_scale().y);
			writer->write_half_le(object_it->second.get_scale().z);
			writer->write_half_le(object_it->second.get_glow());

			writer->write_u32_le(0);
			writer->write_u32_le(0);
			writer->write_u32_le(0);
			writer->write_u32_le(0);

			assert(writer->get_position() ==
				(position + obj_3d_size));
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

			position = writer->get_position();

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

			assert(writer->get_position() ==
				(position + obj_2d_size));
		}

		light_size = AbstractMapLoader::get_light_size();
		light_count = 0;
		light_offset = writer->get_position();

		light_end = m_lights.end();

		for (light_it = m_lights.begin(); light_it != light_end;
			++light_it)
		{
			light_count++;

			position = writer->get_position();

			writer->write_float_le(
				light_it->second.get_position().x);
			writer->write_float_le(
				light_it->second.get_position().y);
			writer->write_float_le(
				light_it->second.get_position().z);

			writer->write_float_le(light_it->second.get_color().r);
			writer->write_float_le(light_it->second.get_color().g);
			writer->write_float_le(light_it->second.get_color().b);

			writer->write_half_le(light_it->second.get_radius());

			for (i = 0; i < 14; ++i)
			{
				writer->write_u8(0);
			}

			assert(writer->get_position() ==
				(position + light_size));
		}

		particle_size = AbstractMapLoader::get_particle_size();
		particle_count = 0;
		particle_offset = writer->get_position();

		particle_end = m_particles.end();

		for (particle_it = m_particles.begin();
			particle_it != particle_end; ++particle_it)
		{
			particle_count++;

			position = writer->get_position();

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

			assert(writer->get_position() ==
				(position + particle_size));
		}

		name_count = 0;
		name_offset = writer->get_position();

		BOOST_FOREACH(const String &name, names)
		{
			name_count++;

			writer->write_utf8_string(name, 128);
		}

		terrain_offset = 0;

		if (m_terrain_editor.get_enabled())
		{
			terrain_offset = writer->get_position();

			count = m_terrain_editor.get_used_layer_count();

			CodecManager::save_image_as_dds_dxt10(
				m_terrain_editor.get_displacement_map(),
				displacement_map_name);
			CodecManager::save_image_as_dds_dxt10(
				m_terrain_editor.get_normal_tangent_map(),
				normal_tangent_map_name);
			CodecManager::save_image_as_dds_dxt10(
				m_terrain_editor.get_dudv_map(),
				dudv_map_name);
			CodecManager::save_image_as_dds_dxt10(
				m_terrain_editor.get_partial_blend_map(count),
				blend_map_name);

			displacement_map_name = FileSystem::get_file_name(
				displacement_map_name);
			normal_tangent_map_name = FileSystem::get_file_name(
				normal_tangent_map_name);
			dudv_map_name = FileSystem::get_file_name(
				dudv_map_name);
			blend_map_name = FileSystem::get_file_name(
				blend_map_name);

			writer->write_utf8_string(displacement_map_name, 128);
			writer->write_utf8_string(normal_tangent_map_name, 128);
			writer->write_utf8_string(dudv_map_name, 128);
			writer->write_utf8_string(blend_map_name, 128);

			writer->write_float_le(
				m_terrain_editor.get_dudv_scale_offset().x);
			writer->write_float_le(
				m_terrain_editor.get_dudv_scale_offset().y);
			writer->write_float_le(
				m_terrain_editor.get_dudv_scale_offset().z);
			writer->write_float_le(
				m_terrain_editor.get_dudv_scale_offset().w);

			writer->write_float_le(
				m_terrain_editor.get_translation().x);
			writer->write_float_le(
				m_terrain_editor.get_translation().y);
			writer->write_float_le(
				m_terrain_editor.get_translation().z);

			writer->write_u32_le(m_terrain_editor.get_size().x);
			writer->write_u32_le(m_terrain_editor.get_size().y);

			writer->write_u32_le(count);

			for (i = 0; i < count; ++i)
			{
				mask[i] = true;
			}

			writer->write_u64_le((
				m_terrain_editor.get_material_data(
					).get_use_blend_size_textures() &
						mask).to_ulong());
			writer->write_u64_le((
				m_terrain_editor.get_material_data(
					).get_use_specular_maps() &
						mask).to_ulong());
			writer->write_u64_le((
				m_terrain_editor.get_material_data(
					).get_use_gloss_maps() &
						mask).to_ulong());
			writer->write_u64_le((
				m_terrain_editor.get_material_data(
					).get_use_height_maps() &
						mask).to_ulong());

			for (i = 0; i < count; ++i)
			{
				writer->write_float_le(
					m_terrain_editor.get_default_specular(
						i).r);
				writer->write_float_le(
					m_terrain_editor.get_default_specular(
						i).g);
				writer->write_float_le(
					m_terrain_editor.get_default_specular(
						i).b);
				writer->write_float_le(
					m_terrain_editor.get_default_gloss(i));
				writer->write_float_le(
					m_terrain_editor.get_default_height(
						i));
				writer->write_float_le(
					m_terrain_editor.get_blend_size(i));

				writer->write_utf8_string(
					m_terrain_editor.get_albedo_map(i),
					128);
				writer->write_utf8_string(
					m_terrain_editor.get_specular_map(i),
					128);
				writer->write_utf8_string(
					m_terrain_editor.get_gloss_map(i),
					128);
				writer->write_utf8_string(
					m_terrain_editor.get_height_map(i),
					128);
			}
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

		writer->write_float_le(ground_hemisphere.r);
		writer->write_float_le(ground_hemisphere.g);
		writer->write_float_le(ground_hemisphere.b);

		writer->write_u32_le(particle_size);
		writer->write_u32_le(particle_count);
		writer->write_u32_le(particle_offset);

		writer->write_u32_le(0);	// clusters_offset

		writer->write_u32_le(name_count);
		writer->write_u32_le(name_offset);

		writer->write_u32_le(terrain_offset);

		writer->write_u32_le(0);	// reserved_12
		writer->write_u32_le(0);	// reserved_13
		writer->write_u32_le(0);	// reserved_14
		writer->write_u32_le(0);	// reserved_15
		writer->write_u32_le(0);	// reserved_16
		writer->write_u32_le(0);	// reserved_17
	}

	void EditorMapData::export_tile_map(const String &file_name) const
	{
		ImageSharedPtr image;
		glm::uvec4 data;
		glm::uvec2 size;
		Uint32 x, y;

		size = get_tile_map_size();

		image = boost::make_shared<Image>(String(UTF8("tile map")),
			false, tft_rgba8, glm::uvec3(size, 0), 0, false);

		for (y = 0; y < size.y; ++y)
		{
			for (x = 0; x < size.x; ++x)
			{
				data.r = m_tile_maps[x][y][0];
				data.g = m_tile_maps[x][y][1];
				data.b = m_tile_maps[x][y][2];
				data.a = m_tile_maps[x][y][3];

				image->set_pixel_uint(x, y, 0, 0, 0, data);
			}
		}

		CodecManager::save_image_as_png(image, file_name);
	}

	void EditorMapData::set_tile_map_size(const glm::uvec2 &size)
	{
		glm::uvec2 min;
		Uint32 x, y, start_x;

		min = glm::min(get_tile_map_size(), size);

		m_tile_maps.resize(boost::extents[size.x][size.y]);

		for (y = 0; y < size.y; ++y)
		{
			if (min.y < y)
			{
				start_x = 0;
			}
			else
			{
				start_x = min.x;
			}

			for (x = start_x; x < size.x; ++x)
			{
				m_tile_maps[x][y][0] = 0xFF;
				m_tile_maps[x][y][1] = 0xFF;
				m_tile_maps[x][y][2] = 0xFF;
				m_tile_maps[x][y][3] = 0xFF;
			}
		}
	}

}
