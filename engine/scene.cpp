/****************************************************************************
 *            scene.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "scene.hpp"
#include "map.hpp"
#include "light.hpp"
#include "object.hpp"
#include "exceptions.hpp"
#include "rstartree.hpp"
#include "meshcache.hpp"
#include "actordatacache.hpp"
#include "frustum.hpp"
#include "instancedata.hpp"
#include "objectdescription.hpp"
#include "glslprogram.hpp"
#include "actor.hpp"
#include "effect.hpp"
#include "texturecache.hpp"
#include "renderobjectdata.hpp"
#include "meshbuilder.hpp"
#include "abstractmesh.hpp"
#include "shader/autoparameterutil.hpp"
#include "abstractframebuffer.hpp"
#include "shader/shadertextureutil.hpp"
#include "globalvars.hpp"
#include "subobject.hpp"
#include "materialdescription.hpp"
#include "framebufferbuilder.hpp"
#include "filter.hpp"
#include "texture.hpp"
#include "convexbody.hpp"
#include "materialeffect.hpp"
#include "loader/maploader.hpp"
#include "freeidsmanager.hpp"

#include "../client_serv.h"

namespace eternal_lands
{

	namespace
	{

		class StateManagerUtil
		{
			private:
				StateManager &m_manager;

			public:
				inline StateManagerUtil(StateManager &manager):
					m_manager(manager)
				{
				}

				inline ~StateManagerUtil() throw()
				{
					m_manager.unbind_all();
				}

		};

	}

	Scene::Scene(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system):
		m_global_vars(global_vars), m_file_system(file_system),
		m_scene_resources(global_vars, file_system),
		m_scene_view(global_vars), m_frame_id(0), m_program_vars_id(0),
		m_shadow_map_change(true)
	{
		m_light_position_array.resize(8);
		m_light_color_array.resize(8);

		set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		set_main_light_color(glm::vec3(0.2f));
		set_main_light_ambient(glm::vec3(0.2f));

		m_free_ids = boost::make_shared<FreeIdsManager>();
	}

	Scene::~Scene() throw()
	{
	}

	ActorSharedPtr Scene::add_actor(const Uint32 type_id, const Uint32 id,
		const Uint32 index, const String &name,
		const SelectionType selection, const bool enhanced_actor)
	{
		std::pair<Uint32ActorSharedPtrMap::iterator, bool> result;
		ActorSharedPtr actor;

		actor = get_scene_resources().get_actor_data_cache()->get_actor(
			type_id, id, name, selection, enhanced_actor);

		result = m_actors.insert(Uint32ActorSharedPtrMap::value_type(
			index, actor));

		assert(result.second);

		return actor;
	}

	void Scene::remove_actor(const Uint32 index)
	{
		m_actors.erase(index);
	}

	void Scene::remove_all_actors()
	{
		m_actors.clear();
	}

	void Scene::add_object(const ObjectData &object_data)
	{
		m_map->add_object(object_data);
	}

	void Scene::add_object(const ObjectData &object_data,
		const MaterialEffectDescriptionVector &materials)
	{
		m_map->add_object(object_data, materials);
	}

	void Scene::add_object(const InstanceData &instance_data)
	{
		m_map->add_object(instance_data);
	}

	void Scene::remove_object(const Uint32 id)
	{
		m_map->remove_object(id);
	}

	bool Scene::get_object_position(const Uint32 id, glm::vec3 &position)
	{
		return m_map->get_object_position(id, position);
	}

	void Scene::add_light(const LightData &light_data)
	{
		m_map->add_light(light_data);
	}

	void Scene::remove_light(const Uint32 id)
	{
		m_map->remove_light(id);
	}

	void Scene::set_fog(const glm::vec3 &color, const float density)
	{
		/* 1.442695 = 1 / ln(2) */
		m_fog = glm::vec4(color, -density * density * 1.442695f);
	}

	void Scene::init(const FileSystemSharedPtr &file_system)
	{
		glGenQueries(m_querie_ids.size(), m_querie_ids.data());

		m_scene_resources.init(file_system);
	}

	void Scene::update_shadow_map()
	{
		Uint32 shadow_map_width, shadow_map_height, shadow_map_size;
		Uint16 mipmaps, samples, shadow_map_count;

		shadow_map_width = m_scene_view.get_shadow_map_size().x;
		shadow_map_height = m_scene_view.get_shadow_map_size().y;

		shadow_map_size = std::max(shadow_map_width, shadow_map_height);

		if (m_scene_view.get_shadow_map_count() == 0)
		{
			m_shadow_frame_buffer.reset();
			return;
		}

		mipmaps = 0;

		if (!m_scene_view.get_exponential_shadow_maps())
		{
			m_shadow_frame_buffer = get_scene_resources(
				).get_framebuffer_builder()->build(
					String(UTF8("Shadow")),
					shadow_map_width, shadow_map_height, 0,
					tft_depth32);

			return;
		}

		while ((1 << mipmaps) < shadow_map_size)
		{
			mipmaps++;
		}

		shadow_map_count = m_scene_view.get_shadow_map_count();
		samples = 0;

		m_shadow_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
				String(UTF8("Shadow")),	shadow_map_width,
				shadow_map_height, shadow_map_count,
				mipmaps, samples, tft_r32f,
				get_global_vars()->get_use_layered_rendering());
	}

	void Scene::load(const String &name, const glm::vec3 &ambient,
		const bool dungeon)
	{
		m_map.reset(new Map(m_scene_resources.get_codec_manager(),
			m_file_system, m_global_vars,
			m_scene_resources.get_mesh_builder(),
			m_scene_resources.get_mesh_cache(),
			m_scene_resources.get_effect_cache(),
			m_scene_resources.get_texture_cache(),
			m_scene_resources.get_material_description_cache(),
			name));

		m_map->load(ambient, dungeon);
	}

	void Scene::clear()
	{
		m_map.reset();
		m_actors.clear();
		m_free_ids->clear();
	}

	void Scene::get_lights(const BoundingBox &bounding_box,
		Uint32 &light_count)
	{
		Uint32 i, size;

		light_count = 1;

		if (m_map->get_dungeon() || m_night)
		{
			BOOST_FOREACH(const LightSharedPtr &light,
				m_visible_lights.get_lights())
			{
				if (light->intersect(bounding_box))
				{
					m_light_position_array[light_count] =
						glm::vec4(light->get_position(),
						light->get_inv_sqr_radius());
					m_light_color_array[light_count] =
						glm::vec4(light->get_color(),
						1.0);
					light_count++;

					if (light_count >=
						m_light_position_array.size())
					{
						break;
					}
				}
			}
		}

		size = m_light_color_array.size();

		for (i = light_count; i < size; ++i)
		{
			m_light_color_array[i] = glm::vec4(0.0f);
			m_light_position_array[i] = glm::vec4(0.0f);
		}
	}

	void Scene::set_lights(const GlslProgramSharedPtr &program,
		const glm::ivec3 &dynamic_light_count)
	{
		program->set_parameter(apt_dynamic_light_count,
			dynamic_light_count);
		program->set_parameter(apt_light_positions,
			m_light_position_array);
		program->set_parameter(apt_light_colors, m_light_color_array);
	}

	void Scene::cull()
	{
		Frustum frustum;
		Uint32ActorSharedPtrMap::iterator it, end;
		SubFrustumsMask mask;

		m_scene_view.update();

		assert(m_scene_view.get_projection_view_matrix().size() > 0);
		frustum = Frustum(m_scene_view.get_projection_view_matrix()[0]);

		m_visible_lights.get_lights().clear();

		if (m_map->get_dungeon())
		{
			m_light_position_array[0] =
				glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
			m_light_color_array[0] =
				glm::vec4(glm::vec3(0.2f), 0.0f);
		}
		else
		{
			m_light_position_array[0] = m_main_light_direction;

			if (m_night)
			{
				m_light_color_array[0] = m_main_light_color +
					glm::vec4(glm::vec3(0.3f), 0.0f);
			}
			else
			{
				m_light_color_array[0] = m_main_light_color +
					glm::vec4(glm::vec3(0.1f), 0.0f);
			}
		}

		m_time = SDL_GetTicks() * 0.001f;

		m_visible_objects.next_frame();

#ifdef	DEBUG
		if (get_global_vars()->get_draw_objects())
		{
			m_map->intersect(frustum, m_visible_objects);
		}
#else	/* DEBUG */
		m_map->intersect(frustum, m_visible_objects);
#endif	/* DEBUG */

		end = m_actors.end();
#ifdef	DEBUG
		if (!get_global_vars()->get_draw_actors())
		{
			end = m_actors.begin();
		}
#endif	/* DEBUG */

		for (it = m_actors.begin(); it != end; ++it)
		{
			mask = frustum.intersect_sub_frustums(
				it->second->get_bounding_box());

			if (mask.any())
			{
				if ((it->second->get_buffs().to_ulong() &
					BUFF_INVISIBILITY) != 0)
				{
					m_visible_objects.add(it->second, 0.25f,
						true, mask);
				}
				else
				{
					m_visible_objects.add(it->second, mask);
				}
			}
		}

		m_visible_objects.sort(glm::vec3(m_scene_view.get_camera()));

		if (m_scene_view.get_shadow_map_count() > 0)
		{
			cull_all_shadows();
		}

		m_map->intersect(frustum, m_visible_lights);
		m_visible_lights.sort(glm::vec3(m_scene_view.get_focus()));
	}

	void Scene::cull_all_shadows()
	{
		SubFrustumsConvexBodys convex_bodys;
		SubFrustumsBoundingBoxes receiver_boxes;
		SubFrustumsBoundingBoxes caster_boxes;
		Frustum frustum;
		glm::vec4 camera;
		Uint32ActorSharedPtrMap::iterator it, end;
		SubFrustumsMask mask;
		Uint32 i, count;

		if (m_shadow_map_change)
		{
			update_shadow_map();
			m_shadow_map_change = false;
		}

		frustum = Frustum(
			m_scene_view.get_split_projection_view_matrix());

		BOOST_FOREACH(RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			mask = frustum.intersect_sub_frustums(
				object.get_object()->get_bounding_box());

			for (i = 0; i < mask.size(); ++i)
			{
				if (mask[i])
				{
					receiver_boxes[i].merge(
						object.get_object(
						)->get_bounding_box());
				}
			}

			object.set_sub_frustums_mask(mask);
		}

		count = convex_bodys.size();

		for (i = 0; i < count; ++i)
		{
			convex_bodys[i] = ConvexBody(frustum, i);
			convex_bodys[i].clip(receiver_boxes[i]);
		}

		m_scene_view.build_shadow_matrices(
			glm::vec3(get_main_light_direction()), convex_bodys, 
			m_map->get_bounding_box().get_max().z);

		camera = m_scene_view.get_camera();

		frustum = Frustum(
			m_scene_view.get_shadow_projection_view_matrix());

		m_shadow_objects.next_frame();
#ifdef	DEBUG
		if (get_global_vars()->get_draw_objects())
		{
			m_map->intersect(frustum, m_shadow_objects);
		}
#else	/* DEBUG */
		m_map->intersect(frustum, m_shadow_objects);
#endif	/* DEBUG */

		end = m_actors.end();
#ifdef	DEBUG
		if (!get_global_vars()->get_draw_actors())
		{
			end = m_actors.begin();
		}
#endif	/* DEBUG */

		for (it = m_actors.begin(); it != end; ++it)
		{
			mask = frustum.intersect_sub_frustums(
				it->second->get_bounding_box());

			if (mask.any())
			{
				if ((it->second->get_buffs().to_ulong() &
					BUFF_INVISIBILITY) == 0)
				{
					m_shadow_objects.add(it->second, mask);
				}
			}
		}

		m_shadow_objects.sort(glm::vec3(camera));

		m_shadow_objects_mask.reset();

		count = mask.size();

		BOOST_FOREACH(const RenderObjectData &object,
			m_shadow_objects.get_objects())
		{
			mask = object.get_sub_frustums_mask();

			for (i = 0; i < count; ++i)
			{
				if (mask[i])
				{
					caster_boxes[i].merge(
						object.get_object(
							)->get_bounding_box());
				}
			}

			m_shadow_objects_mask |= mask;
		}

		for (i = 0; i < count; ++i)
		{
			convex_bodys[i] = ConvexBody(frustum, i);
			convex_bodys[i].clip(caster_boxes[i]);
		}

		m_scene_view.update_shadow_matrices(convex_bodys);
	}

	bool Scene::switch_program(const GlslProgramSharedPtr &program,
		const Uint16 layer)
	{
		bool result;

		result = m_state_manager.switch_program(program);

		if (program.get() == 0)
		{
			return result;
		}

		if (m_program_vars_id != program->get_last_used())
		{
			program->set_parameter(apt_view_matrix,
				m_scene_view.get_current_view_matrix());
			program->set_parameter(apt_view_rotation_matrix,
				m_scene_view.get_current_view_rotation_matrix()
				);
			program->set_parameter(apt_projection_matrix,
				m_scene_view.get_current_projection_matrix(),
				layer);
			program->set_parameter(apt_projection_view_matrix,
				m_scene_view.get_current_projection_view_matrix(
					), layer);
			program->set_parameter(apt_time, m_time);
			program->set_parameter(apt_fog_data, m_fog);
			program->set_parameter(apt_camera,
				m_scene_view.get_camera());
			program->set_parameter(apt_shadow_camera,
				m_scene_view.get_shadow_camera());
			program->set_parameter(apt_shadow_texture_matrix,
				m_scene_view.get_shadow_texture_matrix());
			program->set_parameter(apt_split_distances,
				m_scene_view.get_split_distances());
			program->set_parameter(apt_terrain_height_scale,
				0.1f / 257.0f);

			if (m_map->get_dungeon())
			{
				program->set_parameter(apt_ambient,
					m_map->get_ambient());
			}
			else
			{
				if (m_night)
				{
					program->set_parameter(apt_ambient,
						m_main_light_ambient);
				}
				else
				{
					program->set_parameter(apt_ambient,
						m_main_light_ambient);
				}
			}

			program->set_last_used(m_program_vars_id);
		}

		return result;
	}

	void Scene::draw_object(const ObjectSharedPtr &object,
		const Uint16 distance)
	{
		glm::ivec3 dynamic_light_count;
		Uint32 light_count;
		Uint16 count, material, mesh, i, lod;
		bool object_data_set;

		m_state_manager.switch_mesh(object->get_mesh());

		lod = object->get_lod(distance);
		count = object->get_lods_count(lod);

		get_lights(object->get_bounding_box(), light_count);

		object_data_set = false;

		for (i = 0; i < count; ++i)
		{
			mesh = object->get_mesh_index(lod, i);
			material = object->get_material_effects_index(lod, i);

			if (switch_program(object->get_material_effects(
				)[material].get_effect(
				)->get_default_program(light_count,
					dynamic_light_count)))
			{
				object_data_set = false;
			}

			if (!object_data_set)
			{
				m_state_manager.get_program()->set_parameter(
					apt_world_transformation,
					object->get_world_transformation(
						).get_data());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object->get_bones());
				set_lights(m_state_manager.get_program(),
					dynamic_light_count);
				object_data_set = true;
			}

			object->get_material_effects()[material].bind(
				m_state_manager);

			m_state_manager.get_program()->set_parameter(
				apt_texture_matrices,
				object->get_material_effects(
					)[material].get_texture_matrices());

			m_state_manager.get_program()->set_parameter(
				apt_albedo_scale_offsets,
				object->get_material_effects(
					)[material].get_albedo_scale_offsets());

			m_state_manager.get_program()->set_parameter(
				apt_emission_scale_offset,
				object->get_material_effects(
					)[material].get_emission_scale_offset());

			m_state_manager.get_program()->set_parameter(
				apt_specular_scale_offset,
				object->get_material_effects(
					)[material].get_specular_scale_offset());

			m_state_manager.draw(mesh, 1);
		}
	}

	void Scene::draw_object_shadow(const ObjectSharedPtr &object,
		const Uint16 layer, const Uint16 distance)
	{
		Uint16 count, material, mesh, i, lod;
		bool object_data_set;

		m_state_manager.switch_mesh(object->get_mesh());

		lod = object->get_lod(distance);
		count = object->get_lods_count(lod);

		object_data_set = false;

		for (i = 0; i < count; ++i)
		{
			mesh = object->get_mesh_index(lod, i);
			material = object->get_material_effects_index(lod, i);

			if (!object->get_material_effects(
				)[material].get_cast_shadows())
			{
				continue;
			}

			if (switch_program(object->get_material_effects(
				)[material].get_effect()->get_shadow_program(),
				layer))
			{
				object_data_set = false;
			}

			if (!object_data_set)
			{
				m_state_manager.get_program()->set_parameter(
					apt_world_transformation,
					object->get_world_transformation(
						).get_data());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object->get_bones());
				object_data_set = true;
			}

			m_state_manager.get_program()->set_parameter(
				apt_texture_matrices,
				object->get_material_effects(
					)[material].get_texture_matrices());

			object->get_material_effects()[material].bind(
				m_state_manager);
			m_state_manager.draw(mesh, 1);
		}
	}

	void Scene::draw_object_depth(const ObjectSharedPtr &object,
		const Uint16 distance)
	{
		Uint16 count, material, mesh, i, lod;
		bool object_data_set;

		m_state_manager.switch_mesh(object->get_mesh());

		lod = object->get_lod(distance);
		count = object->get_lods_count(lod);

		object_data_set = false;

		for (i = 0; i < count; ++i)
		{
			mesh = object->get_mesh_index(lod, i);
			material = object->get_material_effects_index(lod, i);

			if (switch_program(object->get_material_effects(
				)[material].get_effect()->get_depth_program()))
			{
				object_data_set = false;
			}

			if (!object_data_set)
			{
				m_state_manager.get_program()->set_parameter(
					apt_world_transformation,
					object->get_world_transformation(
						).get_data());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object->get_bones());
				object_data_set = true;
			}

			m_state_manager.get_program()->set_parameter(
				apt_texture_matrices,
				object->get_material_effects(
					)[material].get_texture_matrices());

			object->get_material_effects()[material].bind(
				m_state_manager);
			m_state_manager.draw(mesh, 1);
		}
	}

	void Scene::draw_shadows(const Uint16 index)
	{
		DEBUG_CHECK_GL_ERROR();

		m_state_manager.switch_polygon_offset_fill(
			!m_scene_view.get_exponential_shadow_maps());
		m_state_manager.switch_color_mask(glm::bvec4(
			m_scene_view.get_exponential_shadow_maps()));

		m_scene_view.set_shadow_view();

		BOOST_FOREACH(const RenderObjectData &object,
			m_shadow_objects.get_objects())
		{
			if (object.get_sub_frustums_mask(index))
			{
				draw_object_shadow(object.get_object(), index,
					object.get_distance());
			}
		}

		m_program_vars_id++;

		DEBUG_CHECK_GL_ERROR();

		unbind_all();
	}

	void Scene::draw_shadows_array(const Uint16 index)
	{
		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->bind(index);
		m_shadow_frame_buffer->clear(glm::vec4(1e38f));

		draw_shadows(index);

		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->blit();

		DEBUG_CHECK_GL_ERROR();
	}

	void Scene::draw_all_shadows_array()
	{
		Uint32 width, height;
		Uint16 i;

		DEBUG_CHECK_GL_ERROR();

		if (!m_scene_view.get_exponential_shadow_maps())
		{
			glPolygonOffset(1.25f, 32.0f);
		}

		DEBUG_CHECK_GL_ERROR();

		width = m_shadow_frame_buffer->get_width();
		height = m_shadow_frame_buffer->get_height();

		glViewport(0, 0, width, height);

		for (i = 0; i < m_scene_view.get_shadow_map_count(); ++i)
		{
			if (m_shadow_objects_mask[i])
			{
				draw_shadows_array(i);
			}
			else
			{
				m_shadow_frame_buffer->bind_texture(i);
				m_shadow_frame_buffer->clear(glm::vec4(1e38f));
			}
		}

		unbind_all();

		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->unbind();

		DEBUG_CHECK_GL_ERROR();

		m_state_manager.switch_texture(stt_shadow,
			m_shadow_frame_buffer->get_texture());

		DEBUG_CHECK_GL_ERROR();

		if (m_scene_view.get_exponential_shadow_maps())
		{
			glGenerateMipmap(ttt_2d_texture_array);
		}

		DEBUG_CHECK_GL_ERROR();

		glViewport(m_scene_view.get_view_port()[0],
			m_scene_view.get_view_port()[1],
			m_scene_view.get_view_port()[2],
			m_scene_view.get_view_port()[3]);

		DEBUG_CHECK_GL_ERROR();
	}

	void Scene::draw_all_shadows()
	{
		Uint32 width, height;

		DEBUG_CHECK_GL_ERROR();

		if (!m_scene_view.get_exponential_shadow_maps())
		{
			glPolygonOffset(1.25f, 32.0f);
		}

		DEBUG_CHECK_GL_ERROR();

		width = m_shadow_frame_buffer->get_width();
		height = m_shadow_frame_buffer->get_height();

		m_shadow_frame_buffer->bind(0);
		m_shadow_frame_buffer->clear(glm::vec4(1e38f));

		glViewport(0, 0, width, height);
		DEBUG_CHECK_GL_ERROR();

		m_state_manager.switch_polygon_offset_fill(
			!m_scene_view.get_exponential_shadow_maps());
		m_state_manager.switch_color_mask(glm::bvec4(
			m_scene_view.get_exponential_shadow_maps()));

		m_scene_view.set_shadow_view();

		BOOST_FOREACH(const RenderObjectData &object,
			m_shadow_objects.get_objects())
		{
			draw_object_shadow(object.get_object(), 0,
				object.get_distance());
		}

		m_program_vars_id++;

		DEBUG_CHECK_GL_ERROR();

		unbind_all();

		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->unbind();

		DEBUG_CHECK_GL_ERROR();

		m_state_manager.switch_texture(stt_shadow,
			m_shadow_frame_buffer->get_texture());

		DEBUG_CHECK_GL_ERROR();

		if (m_scene_view.get_exponential_shadow_maps())
		{
			glGenerateMipmap(ttt_2d_texture_array);
		}

		DEBUG_CHECK_GL_ERROR();

		glViewport(m_scene_view.get_view_port()[0],
			m_scene_view.get_view_port()[1],
			m_scene_view.get_view_port()[2],
			m_scene_view.get_view_port()[3]);

		DEBUG_CHECK_GL_ERROR();
	}

	void Scene::draw()
	{
		StateManagerUtil state(m_state_manager);
		CHECK_GL_ERROR();

		DEBUG_CHECK_GL_ERROR();

		m_state_manager.init();

		if (m_scene_view.get_shadow_map_count() > 0)
		{
			if (get_global_vars()->get_use_layered_rendering() &&
				(m_scene_view.get_shadow_map_count() > 1))
			{
				draw_all_shadows();
			}
			else
			{
				draw_all_shadows_array();
			}
		}

		m_scene_view.set_default_view();

		m_state_manager.switch_multisample(true);

		DEBUG_CHECK_GL_ERROR();

		glBlendFunc(GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);

		BOOST_FOREACH(const RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			if (m_state_manager.switch_blend(object.get_blend()))
			{
				if (object.get_blend())
				{
					glBlendColor(1.0f, 1.0f, 1.0f,
						object.get_transparency());
				}
			}

			draw_object(object.get_object(), object.get_distance());
		}

		DEBUG_CHECK_GL_ERROR();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		CHECK_GL_ERROR();

		m_program_vars_id++;
		m_frame_id++;
	}

	void Scene::pick_object(const ObjectSharedPtr &object,
		PairUint32SelectionTypeVector &ids)
	{
		PairUint32SelectionType data;
		Uint32 i, sub_objects, material;
		bool object_data_set;

		if (object->get_selection() == st_none)
		{
			return;
		}

		if (object->get_sub_objects().size() == 0)
		{
			data.first = object->get_id();
			data.second = object->get_selection();

			glBeginQuery(GL_SAMPLES_PASSED,
				m_querie_ids[ids.size()]);

			draw_object_depth(object, 0);

			glEndQuery(GL_SAMPLES_PASSED);

			ids.push_back(data);

			return;
		}

		m_state_manager.switch_mesh(object->get_mesh());

		sub_objects = object->get_sub_objects().size();

		object_data_set = false;

		for (i = 0; i < sub_objects; ++i)
		{
			if (object->get_sub_objects()[i].get_selection()
				== st_none)
			{
				continue;
			}

			data.first = object->get_sub_objects()[i].get_id();
			data.second = object->get_sub_objects(
				)[i].get_selection();

			glBeginQuery(GL_SAMPLES_PASSED,
				m_querie_ids[ids.size()]);

			material = object->get_sub_objects()[i].get_material();

			if (switch_program(object->get_material_effects(
				)[material].get_effect()->get_depth_program()))
			{
				object_data_set = false;
			}

			if (!object_data_set)
			{
				m_state_manager.get_program()->set_parameter(
					apt_world_transformation,
					object->get_world_transformation(
						).get_data());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object->get_bones());
				object_data_set = true;
			}

			object->get_material_effects()[material].bind(
				m_state_manager);
			m_state_manager.get_program()->set_parameter(
				apt_texture_matrices,
				object->get_material_effects(
					)[material].get_texture_matrices());
			m_state_manager.draw(object->get_sub_objects()[i], 1);

			glEndQuery(GL_SAMPLES_PASSED);

			ids.push_back(data);
		}
	}

	Uint32 Scene::pick(const glm::vec2 &offset, const glm::vec2 &size,
		SelectionType &selection)
	{
		PairUint32SelectionTypeVector ids;
		Uint32 i, max_count, count, id;
		StateManagerUtil state(m_state_manager);

		m_state_manager.unbind_all();
		m_state_manager.switch_scissor_test(true);
		glScissor(offset.x - size.x, offset.y - size.y, 2 * size.x,
			2 * size.y);
		m_state_manager.switch_color_mask(glm::bvec4(false));
		m_state_manager.switch_multisample(false);

		glDepthFunc(GL_LEQUAL);

		m_scene_view.set_default_view();

		m_state_manager.switch_depth_mask(false);

		BOOST_FOREACH(const RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			pick_object(object.get_object(), ids);
		}

		m_program_vars_id++;

		max_count = 0;
		id = 0xFFFFFF;
		selection = st_none;

		for (i = 0; i < ids.size(); ++i)
		{
			count = 0;

			glGetQueryObjectuiv(m_querie_ids[i], GL_QUERY_RESULT,
				&count);

			if (count > max_count)
			{
				id = ids[i].first;
				selection = ids[i].second;

				max_count = count;
			}
		}

		CHECK_GL_ERROR();

		return id;
	}

	void Scene::load(const String &name)
	{
		boost::scoped_ptr<MapLoader> map_loader;

		clear();

		map_loader.reset(new MapLoader(
			m_scene_resources.get_codec_manager(),
			m_file_system, m_global_vars,
			m_scene_resources.get_mesh_builder(),
			m_scene_resources.get_mesh_cache(),
			m_scene_resources.get_mesh_data_cache(),
			m_scene_resources.get_effect_cache(),
			m_scene_resources.get_texture_cache(),
			m_scene_resources.get_material_description_cache(),
			m_free_ids));

		m_map = map_loader->load(name);
	}

	const ParticleDataVector &Scene::get_particles() const
	{
		return m_map->get_particles();
	}

	Uint16 Scene::get_height(const Uint16 x, const Uint16 y) const
	{
		return m_map->get_height(x, y);
	}

	Uint16 Scene::get_tile(const Uint16 x, const Uint16 y) const
	{
		return m_map->get_tile(x, y);
	}

	glm::uvec2 Scene::get_height_map_size() const
	{
		return m_map->get_height_map_size();
	}

	glm::uvec2 Scene::get_tile_map_size() const
	{
		return m_map->get_tile_map_size();
	}

}
