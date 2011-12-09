/****************************************************************************
 *            scene.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "scene.hpp"
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
					m_manager.init();
				}

				inline ~StateManagerUtil() throw()
				{
					m_manager.unbind_all();
				}

		};

	}

	Scene::Scene(const GlobalVarsSharedPtr &global_vars,
		const FileSystemWeakPtr &file_system):
		m_global_vars(global_vars), m_scene_resources(global_vars,
			file_system), m_scene_view(global_vars),
			m_name(UTF8("")), m_frame_id(0), m_program_vars_id(0),
			m_id(0), m_dungeon(false), m_shadow_map_change(true)
	{
		m_light_tree.reset(new RStarTree());
		m_object_tree.reset(new RStarTree());

		m_light_position_array.resize(8);
		m_light_color_array.resize(8);

		set_ambient(glm::vec3(0.2f));
		set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		set_main_light_color(glm::vec3(0.2f));
		set_main_light_ambient(glm::vec3(0.2f));
	}

	Scene::~Scene() throw()
	{
	}

	ActorSharedPtr Scene::add_actor(const Uint32 type_id, const Uint32 id,
		const String &name, const SelectionType selection,
		const bool enhanced_actor)
	{
		std::pair<Uint32ActorSharedPtrMap::iterator, bool> result;
		ActorSharedPtr actor;

		actor = get_scene_resources().get_actor_data_cache().get_actor(
			type_id, id, name, selection, enhanced_actor);

		result = m_actors.insert(Uint32ActorSharedPtrMap::value_type(id,
			actor));

		assert(result.second);

		return actor;
	}

	void Scene::remove_actor(const Uint32 id)
	{
		m_actors.erase(id);
	}

	void Scene::remove_all_actors()
	{
		m_actors.clear();
	}

	void Scene::add_object(const ObjectData &object_data)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;
		MaterialDescriptionVector materials;

		get_scene_resources().get_mesh_cache().get_mesh(
			object_data.get_name(), mesh, materials);

		object = boost::make_shared<Object>(object_data, mesh,
			materials, get_scene_resources().get_effect_cache_ptr(),
			get_scene_resources().get_texture_cache_ptr());

		temp = m_objects.insert(Uint32ObjectSharedPtrMap::value_type(
			object_data.get_id(), object));

		assert(temp.second);

		m_object_tree->add(object);
	}

	void Scene::add_object(const InstanceData &instance_data)
	{
		std::pair<Uint32ObjectSharedPtrMap::iterator, bool> temp;
		ObjectSharedPtr object;
		AbstractMeshSharedPtr mesh;

		mesh = get_scene_resources().get_mesh_builder().get_mesh(
			vft_instanced_mesh, instance_data.get_mesh_data_tool());

		object = boost::make_shared<Object>(instance_data, mesh,
			instance_data.get_materials(),
			get_scene_resources().get_effect_cache_ptr(),
			get_scene_resources().get_texture_cache_ptr(),
			instance_data.get_instanced_objects());
		temp = m_objects.insert(Uint32ObjectSharedPtrMap::value_type(
			instance_data.get_id(), object));

		assert(temp.second);

		m_object_tree->add(object);
	}

	void Scene::remove_object(const Uint32 id)
	{
		Uint32ObjectSharedPtrMap::iterator found;

		found = m_objects.find(id);

		if (found != m_objects.end())
		{
			m_object_tree->remove(found->second);

			m_objects.erase(found);
		}
	}

	bool Scene::get_object_position(const Uint32 id, glm::vec3 &position)
	{
		Uint32ObjectSharedPtrMap::iterator found;

		found = m_objects.find(id);

		if (found == m_objects.end())
		{
			return false;
		}

		position = found->second->get_world_matrix()[3];

		return true;
	}

	void Scene::add_light(const glm::vec3 &position, const glm::vec3 &color,
		const float ambient, const float radius, const Uint32 id)
	{
		std::pair<Uint32LightSharedPtrMap::iterator, bool> temp;
		LightSharedPtr light;

		light = boost::make_shared<Light>(position, color, ambient,
			radius, id);
		assert(light->get_radius() == radius);
		temp = m_lights.insert(Uint32LightSharedPtrMap::value_type(id,
			light));

		assert(temp.second);

		m_light_tree->add(light);
	}

	void Scene::remove_light(const Uint32 id)
	{
		Uint32LightSharedPtrMap::iterator found;

		found = m_lights.find(id);

		if (found != m_lights.end())
		{
			m_light_tree->remove(found->second);

			m_lights.erase(found);
		}
	}

	void Scene::set_fog(const glm::vec3 &color, const float density)
	{
		/* 1.442695 = 1 / ln(2) */
		m_fog = glm::vec4(color, -density * density * 1.442695f);
	}

	namespace
	{

		boost::array<GLuint, 0x10000> querie_ids;

	}

	void Scene::init()
	{
		Uint32 i;

		for (i = 0; i < querie_ids.size(); i++)
		{
			querie_ids[i] = i + 1;
		}

		glGenQueries(querie_ids.size(), querie_ids.data());

		m_scene_resources.init();
	}

	void Scene::update_shadow_map()
	{
		Uint32 shadow_map_width, shadow_map_height, shadow_map_size;
		Uint16 mipmaps, samples, shadow_map_count;

		shadow_map_width = m_scene_view.get_shadow_map_width();

		shadow_map_height = m_scene_view.get_shadow_map_height();

		shadow_map_size = std::max(shadow_map_width, shadow_map_height);

		if (m_scene_view.get_shadow_map_count() == 0)
		{
			m_shadow_frame_buffer.reset();
			m_shadow_filter_frame_buffer.reset();
			return;
		}

		mipmaps = 0;

		if (!m_scene_view.get_exponential_shadow_maps())
		{
			m_shadow_frame_buffer = get_scene_resources(
				).get_framebuffer_builder().build(
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

		if (get_global_vars()->get_msaa_shadows())
		{
			samples = 4;
		}
		else
		{
			samples = 0;
		}

		if (shadow_map_count == 3)
		{
			shadow_map_count = 1;
			shadow_map_width = (shadow_map_width * 3) / 2;
		}

		m_shadow_frame_buffer = get_scene_resources(
			).get_framebuffer_builder().build(
				String(UTF8("Shadow")),	shadow_map_width,
				shadow_map_height, shadow_map_count,
				mipmaps, samples, tft_r32f);

		if (get_global_vars()->get_filter_shadow_map())
		{
			m_shadow_filter_frame_buffer = get_scene_resources(
				).get_framebuffer_builder().build_filter(
					String(UTF8("ShadowFilter")),
					shadow_map_size, shadow_map_size,
					tft_r32f);
		}
		else
		{
			m_shadow_filter_frame_buffer.reset();
		}
	}

	void Scene::load(const String &name, const glm::vec3 &ambient,
		const bool dungeon)
	{
		m_name = name;

		set_dungeon(dungeon);
		set_ambient(ambient);

		clear();
	}

	void Scene::clear()
	{
		m_light_tree->clear();
		m_object_tree->clear();
		m_actors.clear();
		m_objects.clear();
		m_lights.clear();
	}

	void Scene::get_lights(const BoundingBox &bounding_box,
		Uint32 &light_count)
	{
		Uint32 i, size;

		light_count = 1;

		if (m_dungeon || m_night)
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

		for (i = light_count; i < size; i++)
		{
			m_light_color_array[i] = glm::vec4(0.0f);
			m_light_position_array[i] = glm::vec4(0.0f);
		}
	}

	void Scene::set_lights(const GlslProgramSharedPtr &program,
		const glm::ivec3 &dynamic_light_count, const glm::vec4 &color)
	{
		if (m_dungeon)
		{
			program->set_parameter(apt_ambient, m_ambient + color);
		}
		else
		{
			if (m_night)
			{
				program->set_parameter(apt_ambient,
					m_ambient + m_main_light_ambient +
					color);
			}
			else
			{
				program->set_parameter(apt_ambient,
					m_ambient + m_main_light_ambient);
			}
		}

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

		frustum = Frustum(m_scene_view.get_projection_view_matrix());

		m_visible_lights.get_lights().clear();

		if (m_dungeon)
		{
			m_light_position_array[0] =
				glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
			m_light_color_array[0] =
				glm::vec4(glm::vec3(0.2f), 0.0f);
		}
		else
		{
			m_light_position_array[0] = m_main_light_direction;
			m_light_color_array[0] = m_main_light_color +
				glm::vec4(glm::vec3(0.1f), 0.0f);
		}

		m_visible_objects.next_frame();
		m_object_tree->intersect(frustum, m_visible_objects);

		m_time = SDL_GetTicks() * 0.001f;

		end = m_actors.end();

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

		cull_all_shadows();

		m_light_tree->intersect(frustum, m_visible_lights);
		m_visible_lights.sort(glm::vec3(m_scene_view.get_focus()));
	}

	void Scene::cull_all_shadows()
	{
		SubFrustumsBoundingBoxes split_boxes, receiver_boxes;
		SubFrustumsBoundingBoxes caster_boxes, shadow_boxes;
		Frustum frustum;
		glm::vec4 camera;
		Uint32ActorSharedPtrMap::iterator it, end;
		SubFrustumsMask mask;
		Uint32 i;

		if (m_shadow_map_change)
		{
			update_shadow_map();
			m_shadow_map_change = false;
		}

		frustum = Frustum(m_scene_view.get_projection_view_matrices());

		BOOST_FOREACH(const RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			mask = frustum.intersect_sub_frustums(
				object.get_object()->get_bounding_box());

			for (i = 0; i < mask.size(); i++)
			{
				if (mask[i])
				{
					receiver_boxes[i].merge(
						object.get_object(
						)->get_bounding_box());
				}
			}
		}

		split_boxes = frustum.get_bounding_boxes();

		m_scene_view.build_shadow_matrices(
			glm::vec3(get_main_light_direction()),
			split_boxes, receiver_boxes,
			m_object_tree->get_bounding_box().get_max().z);

		camera = glm::vec4(0.0, 0.0, 0.0f, 1.0f);
		camera = glm::inverse(
			m_scene_view.get_shadow_view_matrix()) * camera;

		frustum = Frustum(
			m_scene_view.get_shadow_projection_view_matrices());

		m_shadow_objects.next_frame();
		m_object_tree->intersect(frustum, m_shadow_objects);

		end = m_actors.end();

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

		BOOST_FOREACH(const RenderObjectData &object,
			m_shadow_objects.get_objects())
		{
			mask = frustum.intersect_sub_frustums(
				object.get_object()->get_bounding_box());

			for (i = 0; i < mask.size(); i++)
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

		shadow_boxes = frustum.get_bounding_boxes();

		m_scene_view.update_shadow_matrices(shadow_boxes,
			receiver_boxes, caster_boxes);
	}

	bool Scene::switch_program(const GlslProgramSharedPtr &program)
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
			program->set_parameter(apt_projection_matrix,
				m_scene_view.get_current_projection_matrix());
			program->set_parameter(apt_projection_view_matrix,
				m_scene_view.get_current_projection_view_matrix(
					));
			program->set_parameter(apt_time, m_time);
			program->set_parameter(apt_fog_data, m_fog);
			program->set_parameter(apt_camera,
				m_scene_view.get_camera());
			program->set_parameter(apt_shadow_camera,
				m_scene_view.get_shadow_camera());
			program->set_parameter(apt_shadow_view_matrix,
				m_scene_view.get_shadow_view_matrix());
			program->set_parameter(apt_shadow_projection_matrix,
				m_scene_view.get_shadow_projection_matrix());
			program->set_parameter(
				apt_shadow_projection_view_matrix,
				m_scene_view.get_shadow_projection_view_matrix(
				));
			program->set_parameter(apt_shadow_texture_matrix,
				m_scene_view.get_shadow_texture_matrices());
			program->set_parameter(apt_split_distances,
				m_scene_view.get_split_distances());

			program->set_last_used(m_program_vars_id);
		}

		return result;
	}

	void Scene::draw_object(const ObjectSharedPtr &object)
	{
		glm::ivec3 dynamic_light_count;
		Uint32 light_count, materials, i;
		bool object_data_set;

		m_state_manager.switch_mesh(object->get_mesh());

		materials = object->get_materials().size();

		get_lights(object->get_bounding_box(), light_count);

		object_data_set = false;

		for (i = 0; i < materials; i++)
		{
			if (switch_program(object->get_materials(
				)[i].get_effect()->get_default_program(
					light_count, dynamic_light_count)))
			{
				object_data_set = false;
			}

			if (!object_data_set)
			{
				m_state_manager.get_program()->set_parameter(
					apt_world_matrix,
					object->get_world_matrix());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object->get_bones());
				set_lights(m_state_manager.get_program(),
					dynamic_light_count,
					object->get_color());
				object_data_set = true;
			}

			object->get_materials()[i].bind(m_state_manager);

			m_state_manager.get_mesh()->draw(i);
		}
	}

	void Scene::draw_object_shadow(const ObjectSharedPtr &object)
	{
		Uint32 materials, i;
		bool object_data_set;

		m_state_manager.switch_mesh(object->get_mesh());

		materials = object->get_materials().size();

		object_data_set = false;

		for (i = 0; i < materials; i++)
		{
			if (!object->get_materials()[i].get_shadow())
			{
				continue;
			}

			if (switch_program(object->get_materials(
				)[i].get_effect()->get_shadow_program()))
			{
				object_data_set = false;
			}

			if (!object_data_set)
			{
				m_state_manager.get_program()->set_parameter(
					apt_world_matrix,
					object->get_world_matrix());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object->get_bones());
				object_data_set = true;
			}

			object->get_materials()[i].bind(m_state_manager);
			m_state_manager.get_mesh()->draw(i);
		}
	}

	void Scene::draw_object_depth(const ObjectSharedPtr &object)
	{
		Uint32 materials, i;
		bool object_data_set;

		m_state_manager.switch_mesh(object->get_mesh());

		materials = object->get_materials().size();

		object_data_set = false;

		for (i = 0; i < materials; i++)
		{
			if (switch_program(object->get_materials(
				)[i].get_effect()->get_depth_program()))
			{
				object_data_set = false;
			}

			if (!object_data_set)
			{
				m_state_manager.get_program()->set_parameter(
					apt_world_matrix,
					object->get_world_matrix());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object->get_bones());
				object_data_set = true;
			}

			object->get_materials()[i].bind(m_state_manager);
			m_state_manager.get_mesh()->draw(i);
		}
	}

	void Scene::draw_shadows(const Uint16 index)
	{
		DEBUG_CHECK_GL_ERROR();

		m_state_manager.switch_polygon_offset_fill(
			!m_scene_view.get_exponential_shadow_maps());
		m_state_manager.switch_color_mask(glm::bvec4(
			m_scene_view.get_exponential_shadow_maps()));
		m_state_manager.switch_multisample(
			get_global_vars()->get_msaa_shadows());
		m_state_manager.switch_sample_alpha_to_coverage(
			get_global_vars()->get_msaa_shadows());

		m_scene_view.set_shadow_view(index);

		BOOST_FOREACH(const RenderObjectData &object,
			m_shadow_objects.get_objects())
		{
			if (object.get_sub_frustums_mask(index))
			{
				draw_object_shadow(object.get_object());
			}
		}

		m_program_vars_id++;

		DEBUG_CHECK_GL_ERROR();

		unbind_all();
	}

	void Scene::draw_shadows_array(const Uint16 index)
	{
		glm::vec4 tmp;
		Uint32 width, height;

		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->bind(index);
		m_shadow_frame_buffer->clear(glm::vec4(1e38f));

		draw_shadows(index);

		m_shadow_frame_buffer->blit();

		DEBUG_CHECK_GL_ERROR();

		unbind_all();

		if (m_scene_view.get_exponential_shadow_maps() &&
			get_global_vars()->get_filter_shadow_map())
		{
			tmp = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);

			width = m_shadow_frame_buffer->get_width();
			height = m_shadow_frame_buffer->get_height();

			m_state_manager.switch_depth_mask(false);
			m_state_manager.switch_depth_test(false);

			m_shadow_filter_frame_buffer->bind(0);
			m_shadow_filter_frame_buffer->clear(glm::vec4(0));
			m_state_manager.switch_texture(stt_diffuse_0,
				m_shadow_frame_buffer->get_texture());
			m_scene_resources.get_filter().bind(tmp, tmp, width,
				height, index, 1, ft_gauss_5_tap, false,
				m_state_manager);

			m_shadow_frame_buffer->bind_texture(index);
			m_shadow_frame_buffer->clear(glm::vec4(0));
			m_state_manager.switch_texture(stt_diffuse_0,
				m_shadow_filter_frame_buffer->get_texture());
			m_scene_resources.get_filter().bind(tmp, tmp, width,
				height, 1, ft_gauss_5_tap, true,
				m_state_manager);
		}

		unbind_all();

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

		for (i = 0; i < m_scene_view.get_shadow_map_count(); i++)
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
		glm::vec4 source, dest;
		Uint32 width, height, size;
		Uint16 i;

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

		size = m_shadow_frame_buffer->get_height() / 2;

		for (i = 0; i < m_scene_view.get_shadow_map_count(); i++)
		{
			switch (i)
			{
				case 0:
					glViewport(0, 0, size * 2, size * 2);
					break;
				case 1:
					glViewport(size * 2, 0, size, size);
					break;
				case 2:
					glViewport(size * 2, size, size, size);
					break;
			}

			if (m_shadow_objects_mask[i])
			{
				draw_shadows(i);
			}
		}

		DEBUG_CHECK_GL_ERROR();

		glViewport(0, 0, width, height);

		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->blit();

		DEBUG_CHECK_GL_ERROR();

		unbind_all();

		if (m_scene_view.get_exponential_shadow_maps() &&
			get_global_vars()->get_filter_shadow_map())
		{
			dest = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
			source = glm::vec4(2.0f / 3.0f, 1.0f, 0.0f, 0.0f);

			width = m_shadow_filter_frame_buffer->get_width();
			height = m_shadow_filter_frame_buffer->get_height();

			glViewport(0, 0, width, height);

			m_state_manager.switch_depth_mask(false);
			m_state_manager.switch_depth_test(false);

			m_shadow_filter_frame_buffer->bind(0);
			m_shadow_filter_frame_buffer->clear(glm::vec4(0));

			width = m_shadow_frame_buffer->get_width();
			height = m_shadow_frame_buffer->get_height();

			m_state_manager.switch_texture(stt_diffuse_0,
				m_shadow_frame_buffer->get_texture());
			m_scene_resources.get_filter().bind(source, dest, width,
				height, 0, 1, ft_gauss_5_tap, false,
				m_state_manager);

			dest = glm::vec4(2.0f / 3.0f, 1.0f, 0.0f, 0.0f);
			source = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);

			glViewport(0, 0, width, height);

			width = m_shadow_filter_frame_buffer->get_width();
			height = m_shadow_filter_frame_buffer->get_height();

			m_shadow_frame_buffer->bind_texture(0);
//			m_shadow_frame_buffer->clear(glm::vec4(0));
			m_state_manager.switch_texture(stt_diffuse_0,
				m_shadow_filter_frame_buffer->get_texture());
			m_scene_resources.get_filter().bind(source, dest, width,
				height, 1, ft_gauss_5_tap, true,
				m_state_manager);
		}

		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->unbind();

		unbind_all();

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
			if (m_scene_view.get_shadow_map_count() == 3)
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

		m_state_manager.switch_sample_alpha_to_coverage(
			get_global_vars()->get_alpha_to_coverage());

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

			draw_object(object.get_object());
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

			glBeginQuery(GL_SAMPLES_PASSED, querie_ids[ids.size()]);

			draw_object_depth(object);

			glEndQuery(GL_SAMPLES_PASSED);

			ids.push_back(data);

			return;
		}

		m_state_manager.switch_mesh(object->get_mesh());

		sub_objects = object->get_sub_objects().size();

		object_data_set = false;

		for (i = 0; i < sub_objects; i++)
		{
			if (object->get_sub_objects()[i].get_selection()
				== st_none)
			{
				continue;
			}

			data.first = object->get_sub_objects()[i].get_id();
			data.second = object->get_sub_objects(
				)[i].get_selection();

			glBeginQuery(GL_SAMPLES_PASSED, querie_ids[ids.size()]);

			material = object->get_sub_objects()[i].get_material();

			if (switch_program(object->get_materials(
				)[material].get_effect()->get_depth_program()))
			{
				object_data_set = false;
			}

			if (!object_data_set)
			{
				m_state_manager.get_program()->set_parameter(
					apt_world_matrix,
					object->get_world_matrix());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object->get_bones());
				object_data_set = true;
			}

			object->get_materials()[material].bind(m_state_manager);
			m_state_manager.get_mesh()->draw(
				object->get_sub_objects()[i]);

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

		for (i = 0; i < ids.size(); i++)
		{
			count = 0;

			glGetQueryObjectuiv(querie_ids[i], GL_QUERY_RESULT,
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

}
