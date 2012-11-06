/****************************************************************************
 *            editorscene.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editorscene.hpp"
#include "frustum.hpp"
#include "object.hpp"
#include "materialbuilder.hpp"
#include "materialdescription.hpp"
#include "lightdata.hpp"
#include "meshcache.hpp"
#include "editormapdata.hpp"
#include "editormaploader.hpp"
#include "freeidsmanager.hpp"
#include "map.hpp"
#include "renderobjectdata.hpp"
#include "thread/materiallock.hpp"
#include "effect/effect.hpp"
#include "rstartree.hpp"
#include "shader/uniformbuffer.hpp"

namespace eternal_lands
{

#ifndef	NDEBUG
#define	STRING_MARKER(description, arguments)	\
	do	\
	{	\
		if (GLEW_GREMEDY_string_marker)	\
		{	\
			BoostFormat format_string(description);	\
			\
			format_string % arguments;	\
			\
			glStringMarkerGREMEDY(0, format_string.str().c_str());	\
		}	\
	}	\
	while (false)
#else
#define	STRING_MARKER(description, arguments)	\
	do	\
	{	\
	}	\
	while (false)
#endif

	EditorScene::EditorScene(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system):
		Scene(global_vars, file_system), m_draw_objects(true),
		m_draw_terrain(true), m_draw_lights(false),
		m_draw_light_spheres(false)
	{
		MaterialDescription material_description;

		material_description.set_cast_shadows(false);

		material_description.set_color(
			glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
		material_description.set_name(String(UTF8("selection")));
		material_description.set_effect(
			String(UTF8("solid-color")));

		m_selection_material = get_scene_resources(
			).get_material_builder()->get_material(
				material_description);

		m_height_tree.reset(new RStarTree());

		init_terrain_rendering_data(m_top_down_terrain);

		glGenQueries(1, &m_querie_id);
	}

	EditorScene::~EditorScene() throw()
	{
	}

	void EditorScene::add_light(const LightData &light_data)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;
		MaterialSharedPtrVector materials;
		MaterialDescription material_description;
		ObjectData object_data;
		Transformation transformation;
		Uint32 object_id;

		object_id = get_free_ids()->use_typeless_object_id(
			light_data.get_id(), it_light_object);

		transformation.set_translation(light_data.get_position());
		object_data.set_name(String(UTF8("sphere")));
		object_data.set_selection(st_select);
		object_data.set_world_transformation(transformation);
		object_data.set_id(object_id);
		material_description.set_cast_shadows(false);

		get_scene_resources().get_mesh_cache()->get_mesh(
			object_data.get_name(), mesh);

		material_description.set_color(
			glm::vec4(light_data.get_color(), 1.0f));
		material_description.set_name(String(UTF8("light")));
		material_description.set_effect(String(UTF8("light-index")));

		materials.push_back(get_scene_resources().get_material_builder(
			)->get_material(material_description));

		object = boost::make_shared<Object>(object_data, mesh,
			materials);

		temp = m_light_objects.insert(
			Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

		assert(temp.second);

		materials.clear();
		materials.push_back(get_scene_resources().get_material_builder(
			)->get_material(material_description));

		transformation.set_scale(glm::vec3(light_data.get_radius()));
		object_data.set_world_transformation(transformation);
		object_data.set_selection(st_none);

		object = boost::make_shared<Object>(object_data, mesh,
			materials);

		temp = m_light_sphere_objects.insert(
			Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

		assert(temp.second);

		Scene::add_light(light_data);
	}

	void EditorScene::set_height(const Uint16 x, const Uint16 y,
		const Uint16 height)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		Uint32ObjectSharedPtrMap::iterator found;
		ObjectData object_data;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;
		MaterialSharedPtrVector materials;
		MaterialDescription material_description;
		Transformation transformation;
		glm::vec3 offset;
		Uint32 id;

		id = x + (y << 10);

		found = m_heights.find(id);

		if (found != m_heights.end())
		{
			m_height_tree->remove(found->second);

			m_heights.erase(found);
		}

		offset.x = x * 0.5f;
		offset.y = y * 0.5f;
		offset.z = height * 0.2f - 2.2f;

		assert(glm::all(glm::lessThanEqual(glm::abs(offset),
			glm::vec3(1e7f))));

		transformation.set_translation(offset);
		transformation.set_scale(glm::vec3(0.5f, 0.5f, 1.0f));

		object_data.set_name(String(UTF8("plane_2")));
		object_data.set_selection(st_none);
		object_data.set_world_transformation(transformation);
		object_data.set_transparency(0.5f);
		object_data.set_blend(bt_alpha_transparency_value);
		object_data.set_id(id);
		material_description.set_cast_shadows(false);

		get_scene_resources().get_mesh_cache()->get_mesh(
			object_data.get_name(), mesh);

		material_description.set_color(
			glm::vec4(0.0f, 1.0f, 0.3f, 1.0f));
		material_description.set_name(String(UTF8("height")));
		material_description.set_effect(String(UTF8("light-index")));

		materials.push_back(get_scene_resources().get_material_builder(
			)->get_material(material_description));

		object = boost::make_shared<Object>(object_data, mesh,
			materials);

		temp = m_heights.insert(Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

		assert(temp.second);

		m_height_tree->add(object);
	}

	void EditorScene::remove_light(const Uint32 id)
	{
		Uint32ObjectSharedPtrMap::iterator found;
		Uint32 object_id;

		object_id = get_free_ids()->get_object_id(id, it_light_object);

		found = m_light_objects.find(object_id);

		if (found != m_light_objects.end())
		{
			m_light_objects.erase(found);
		}

		found = m_light_sphere_objects.find(object_id);

		if (found != m_light_sphere_objects.end())
		{
			m_light_sphere_objects.erase(found);
		}

		get_free_ids()->free_object_id(object_id);

		Scene::remove_light(id);
	}

	void EditorScene::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, BoundingBox &bounding_box) const
	{
		if (get_draw_terrain())
		{
			Scene::intersect_terrain(frustum, camera, bounding_box);
		}
	}

	void EditorScene::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, TerrainVisitor &terrain) const
	{
		if (get_draw_terrain())
		{
			Scene::intersect_terrain(frustum, camera, terrain);
		}
	}

	void EditorScene::intersect(const Frustum &frustum,
		const bool shadow, ObjectVisitor &visitor) const
	{
		Uint32ObjectSharedPtrMap::const_iterator it, begin, end;
		float transparency;
		BlendType blend;

		if (get_draw_objects())
		{
			Scene::intersect(frustum, shadow, visitor);
		}

		if (get_draw_heights() && !shadow)
		{
			m_height_tree->intersect(frustum, visitor);
		}

		BOOST_FOREACH(RenderObjectData& object, visitor.get_objects())
		{
			if (object.get_object()->get_id() == m_selected_object)
			{
				object.set_depth_read(false);
			}
		}

		if (!get_draw_lights() || shadow)
		{
			return;
		}

		if (get_draw_light_spheres())
		{
			end = m_light_sphere_objects.end();
			begin = m_light_sphere_objects.begin();
			blend = bt_alpha_transparency_value;
			transparency = 0.5f;
		}
		else
		{
			end = m_light_objects.end();
			begin = m_light_objects.begin();
			blend = bt_disabled;
			transparency = 1.0f;
		}

		for (it = begin; it != end; ++it)
		{
			if (frustum.intersect(it->second->get_bounding_box())
				!= it_outside)
			{
				visitor.add(it->second, transparency, blend,
					false);
			}
		}
	}

	void EditorScene::load_map(const String &name, EditorMapData &data,
		const MapItemsTypeSet &skip_items)
	{
		boost::scoped_ptr<EditorMapLoader> map_loader;

		clear();
		m_light_objects.clear();
		m_light_sphere_objects.clear();
		m_heights.clear();
		m_height_tree->clear();

		map_loader.reset(new EditorMapLoader(
			get_scene_resources().get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_scene_resources().get_effect_cache(),
			get_scene_resources().get_mesh_builder(),
			get_scene_resources().get_mesh_cache(),
			get_scene_resources().get_mesh_data_cache(),
			get_scene_resources().get_material_cache(),
			get_scene_resources().get_terrain_builder(),
			get_scene_resources().get_texture_cache(),
			get_free_ids(), data));

		set_map(map_loader->get_map(name));

		map_loader->load(name, skip_items);
	}

	void EditorScene::set_terrain_geometry_maps(
		const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map)
	{
		get_map()->set_terrain_geometry_maps(displacement_map,
			normal_map, dudv_map);
	}

	void EditorScene::set_terrain_blend_map(
		const ImageSharedPtr &blend_map)
	{
		get_map()->set_terrain_blend_map(blend_map);
		rebuild_terrain_map();
	}

	void EditorScene::set_terrain_material(const StringVector &albedo_maps,
		const StringVector &extra_maps,
		const TerrainMaterialData &material_data)
	{
		get_map()->set_terrain_material(albedo_maps, extra_maps,
			material_data);
		rebuild_terrain_map();
	}

	void EditorScene::update_terrain_geometry_maps(
		const ImageUpdate &displacement_map,
		const ImageUpdate &normal_map, const ImageUpdate &dudv_map)
	{
		get_map()->update_terrain_geometry_maps(displacement_map,
			normal_map, dudv_map);
		rebuild_terrain_map();
	}

	void EditorScene::update_terrain_blend_map(
		const ImageUpdate &blend_map)
	{
		get_map()->update_terrain_blend_map(blend_map);
		rebuild_terrain_map();
	}

	void EditorScene::set_terrain_dudv_scale_offset(
		const glm::vec4 &dudv_scale_offset)
	{
		get_map()->set_terrain_dudv_scale_offset(dudv_scale_offset);
		rebuild_terrain_map();
	}

	void EditorScene::set_terrain(const ImageSharedPtr &displacement_map,
		const ImageSharedPtr &normal_map,
		const ImageSharedPtr &dudv_map,
		const ImageSharedPtr &blend_map,
		const StringVector &albedo_maps,
		const StringVector &extra_maps,
		const TerrainMaterialData &material_data,
		const glm::vec4 &dudv_scale_offset)
	{
		get_map()->set_terrain_geometry_maps(displacement_map,
			normal_map, dudv_map);

		get_map()->set_terrain_blend_map(blend_map);

		get_map()->set_terrain_material(albedo_maps, extra_maps,
			material_data);

		get_map()->set_terrain_dudv_scale_offset(dudv_scale_offset);

		rebuild_terrain_map();
	}

	void EditorScene::depth_read()
	{
		m_depth = Scene::get_depth(m_depth_selection);
	}

	void EditorScene::draw_selection(const glm::uvec4 &selection_rect)
	{
		update_program_vars_id();
		get_state_manager().unbind_all();
		get_scene_view().set_ortho_view();

		get_state_manager().switch_blend(true);
		get_state_manager().switch_scissor_test(true);
		get_state_manager().switch_depth_mask(false);
		get_state_manager().switch_depth_test(false);
		get_state_manager().switch_multisample(false);
		glScissor(selection_rect.x, selection_rect.y, selection_rect.z,
			selection_rect.w);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		MaterialLock material_lock(m_selection_material);

		DEBUG_CHECK_GL_ERROR();

		switch_program(material_lock->get_effect(
			)->get_program(ept_default));

		DEBUG_CHECK_GL_ERROR();

		material_lock->bind(get_state_manager());

		get_state_manager().switch_mesh(get_screen_quad());

		get_state_manager().draw(0, 1);

		get_state_manager().switch_blend(false);
		get_state_manager().switch_scissor_test(false);

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().unbind_all();

		Uint32Set selections;

		select_rect(glm::vec2(selection_rect.x, selection_rect.y),
			glm::vec2(selection_rect.x, selection_rect.y) +
			glm::vec2(selection_rect.z, selection_rect.w),
			selections);
	}

	void EditorScene::cull_map()
	{
		MappedUniformBufferSharedPtr top_down_terrain_buffer;

		DEBUG_CHECK_GL_ERROR();

		top_down_terrain_buffer = m_top_down_terrain.get_uniform_buffer(
			)->get_mapped_uniform_buffer();

		get_scene_view().set_scale_view(get_map()->get_bounding_box());

		get_scene_view().set_ortho_scale_view();

		cull(top_down_terrain_buffer,
			get_scene_view().get_current_projection_view_matrix(),
			glm::vec3(get_scene_view().get_camera()), false,
			m_top_down_terrain, m_top_down_objects);

		top_down_terrain_buffer.reset();
	}

	void EditorScene::draw_scale_view(const bool depth_only)
	{
		EffectProgramType type;

		STRING_MARKER(UTF8("drawing top down depth: %1%"), depth_only);

		CHECK_GL_ERROR();

		set_view_port();

		CHECK_GL_ERROR();

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().init();

		DEBUG_CHECK_GL_ERROR();

		glDepthFunc(GL_LEQUAL);

		if (depth_only)
		{
			type = ept_depth;
		}
		else
		{
			type = ept_default;
		}

		get_scene_view().set_ortho_scale_view();

		get_state_manager().switch_color_mask(glm::bvec4(!depth_only));
		get_state_manager().switch_depth_mask(true);
		get_state_manager().switch_culling(false);

		draw_terrain(m_top_down_terrain, type, true);

		glBeginQuery(GL_SAMPLES_PASSED, m_querie_id);

		BOOST_FOREACH(RenderObjectData &object,
			m_top_down_objects.get_objects())
		{
			draw_object_old_lights(object.get_object(),
				object.get_visibility_mask(), type, 1,
				object.get_distance());
		}

		glEndQuery(GL_SAMPLES_PASSED);

		GLuint count;

		count = 0;

		glGetQueryObjectuiv(m_querie_id, GL_QUERY_RESULT, &count);

		unbind_all();

		DEBUG_CHECK_GL_ERROR();

		update_program_vars_id();
	}

}
