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

#include "../client_serv.h"
#include "font/text.hpp"
#include "font/textattribute.hpp"

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
		m_global_vars(global_vars), m_scene_resources(global_vars,
			file_system), m_scene_view(global_vars),
			m_frame_id(0), m_program_vars_id(0),
			m_shadow_map_filter(ft_gauss_5_tap),
			m_shadow_map_change(true)
	{
		m_light_position_array.resize(8);
		m_light_color_array.resize(8);

		set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		set_main_light_color(glm::vec3(0.2f));
		set_main_light_ambient(glm::vec3(0.2f));

		m_map.reset(new Map(m_scene_resources.get_mesh_builder(),
			m_scene_resources.get_mesh_cache(),
			m_scene_resources.get_effect_cache(),
			m_scene_resources.get_texture_cache()));

		m_fonts.reset(new TextureFontCache(file_system,
			m_scene_resources.get_mesh_builder(), 512, 512,
				4096));
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

	void Scene::add_light(const glm::vec3 &position, const glm::vec3 &color,
		const float ambient, const float radius, const Uint32 id)
	{
		m_map->add_light(position, color, ambient, radius, id);
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

		m_fonts->load_xml(file_system, String(UTF8("fonts.xml")));
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
			m_shadow_filter_frame_buffer.reset();
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

		if (get_global_vars()->get_msaa_shadows())
		{
			samples = 4;
		}
		else
		{
			samples = 0;
		}

		m_shadow_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
				String(UTF8("Shadow")),	shadow_map_width,
				shadow_map_height, shadow_map_count,
				mipmaps, samples, tft_r32f);

		if (get_global_vars()->get_filter_shadow_map())
		{
			m_shadow_filter_frame_buffer = get_scene_resources(
				).get_framebuffer_builder()->build_filter(
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
		m_map->load(name, ambient, dungeon);
	}

	void Scene::clear()
	{
		m_map->clear();
		m_actors.clear();
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
		const glm::ivec3 &dynamic_light_count, const glm::vec4 &color)
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

		frustum = Frustum(m_scene_view.get_projection_view_matrix());

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
			m_light_color_array[0] = m_main_light_color +
				glm::vec4(glm::vec3(0.1f), 0.0f);
		}

		m_visible_objects.next_frame();
		m_map->get_object_tree().intersect(frustum, m_visible_objects);

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

		m_map->get_light_tree().intersect(frustum, m_visible_lights);
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

		frustum = Frustum(m_scene_view.get_projection_view_matrices());

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
			m_map->get_object_tree().get_bounding_box().get_max().z);

		camera = glm::vec4(0.0, 0.0, 0.0f, 1.0f);
		camera = glm::inverse(
			m_scene_view.get_shadow_view_matrix()) * camera;

		frustum = Frustum(
			m_scene_view.get_shadow_projection_view_matrices());

		m_shadow_objects.next_frame();
		m_map->get_object_tree().intersect(frustum, m_shadow_objects);

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
			program->set_parameter(apt_shadow_texture_matrix,
				m_scene_view.get_shadow_texture_matrices());
			program->set_parameter(apt_split_distances,
				m_scene_view.get_split_distances());

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
						m_main_light_ambient + 0.2f);
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
		const bool shadow_receiver)
	{
		glm::ivec3 dynamic_light_count;
		Uint32 light_count, materials, i;
		bool object_data_set;

#ifndef	NDEBUG
		m_state_manager.unbind_mesh();
		m_state_manager.unbind_program();
		m_state_manager.unbind_textures();
#endif /* NDEBUG */
		m_state_manager.switch_mesh(object->get_mesh());

		materials = object->get_materials().size();

		get_lights(object->get_bounding_box(), light_count);

		object_data_set = false;

		for (i = 0; i < materials; ++i)
		{
			if (switch_program(object->get_materials(
				)[i].get_effect()->get_default_program(
					light_count, shadow_receiver,
					dynamic_light_count)))
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

			m_state_manager.draw(i);
		}
	}

	void Scene::draw_object_shadow(const ObjectSharedPtr &object)
	{
		Uint32 materials, i;
		bool object_data_set;

#ifndef	NDEBUG
		m_state_manager.unbind_mesh();
		m_state_manager.unbind_program();
		m_state_manager.unbind_textures();
#endif /* NDEBUG */
		m_state_manager.switch_mesh(object->get_mesh());

		materials = object->get_materials().size();

		object_data_set = false;

		for (i = 0; i < materials; ++i)
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
			m_state_manager.draw(i);
		}
	}

	void Scene::draw_object_depth(const ObjectSharedPtr &object)
	{
		Uint32 materials, i;
		bool object_data_set;

#ifndef	NDEBUG
		m_state_manager.unbind_mesh();
		m_state_manager.unbind_program();
		m_state_manager.unbind_textures();
#endif /* NDEBUG */
		m_state_manager.switch_mesh(object->get_mesh());

		materials = object->get_materials().size();

		object_data_set = false;

		for (i = 0; i < materials; ++i)
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
			m_state_manager.draw(i);
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

		if (m_scene_view.get_exponential_shadow_maps() &&
			get_global_vars()->get_filter_shadow_map())
		{
			tmp = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);

			width = m_shadow_frame_buffer->get_width();
			height = m_shadow_frame_buffer->get_height();

			m_state_manager.switch_depth_mask(false);
			m_state_manager.switch_depth_test(false);

			m_shadow_filter_frame_buffer->bind(0);
			m_shadow_filter_frame_buffer->clear(glm::vec4(1e38f));
			m_state_manager.switch_texture(stt_diffuse_0,
				m_shadow_frame_buffer->get_texture());
			m_scene_resources.get_filter().bind(tmp, tmp, width,
				height, index, 1, get_shadow_map_filter(),
				false, m_state_manager);

			m_shadow_frame_buffer->bind_texture(index);
			m_shadow_frame_buffer->clear(glm::vec4(1e38f));
			m_state_manager.switch_texture(stt_diffuse_0,
				m_shadow_filter_frame_buffer->get_texture());
			m_scene_resources.get_filter().bind(tmp, tmp, width,
				height, 0, 1, get_shadow_map_filter(), true,
				m_state_manager);

			unbind_all();
		}

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

	void Scene::draw()
	{
		StateManagerUtil state(m_state_manager);
		CHECK_GL_ERROR();

		DEBUG_CHECK_GL_ERROR();

		m_state_manager.init();

		if (m_scene_view.get_shadow_map_count() > 0)
		{
			draw_all_shadows_array();
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

			draw_object(object.get_object(),
				object.get_sub_frustums_mask().any());
		}

		DEBUG_CHECK_GL_ERROR();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		CHECK_GL_ERROR();

		m_program_vars_id++;
		m_frame_id++;
	}

	void Scene::add_font(const FileSystemSharedPtr &file_system,
		const String &file_name, const String &index, const float size)
	{
		m_fonts->add_font(file_system, index, file_name, size);
	}

	struct TextCacheItem
	{
		Text text;
		AbstractMeshSharedPtr mesh;
		Uint64 frame_id;
		Uint32 count;
		Uint32 lines;

		TextCacheItem()
		{
			lines = 0;
			count = 0;
			frame_id = 0;
		}
	};

	boost::array<TextCacheItem, 256> text_cache;

	void Scene::draw_text(const Text &text,
		const glm::mat4x3 &world_matrix)
	{
		StateManagerUtil state(m_state_manager);

		m_state_manager.switch_depth_mask(false);
		m_state_manager.switch_blend(true);

		m_state_manager.switch_program(m_fonts->get_program());

		m_state_manager.get_program()->set_parameter(apt_view_matrix,
			m_scene_view.get_current_view_matrix());
		m_state_manager.get_program()->set_parameter(
			apt_projection_matrix,
			m_scene_view.get_current_projection_matrix());
		m_state_manager.get_program()->set_parameter(
			apt_projection_view_matrix,
			m_scene_view.get_current_projection_view_matrix());
		m_state_manager.get_program()->set_parameter(apt_time, m_time);
		m_state_manager.get_program()->set_parameter(apt_world_matrix,
			world_matrix);

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		m_fonts->draw(m_state_manager, text, glm::vec2(0.0f), 0, 1,
			std::numeric_limits<float>::max(),
			std::numeric_limits<float>::max());
	}

	Uint32 Scene::draw_2d_text(const Text &text, const glm::vec2 &position,
		const glm::mat4x3 &world_matrix, const Uint32 min_line,
		const Uint32 max_line, const float max_width,
		const float max_height, const WrapModeType wrap)
	{
		StateManagerUtil state(m_state_manager);
		glm::mat4 matrix, project, view;

		if (text.get_length() == 0)
		{
			return 0;
		}

		glGetFloatv(GL_PROJECTION_MATRIX, glm::value_ptr(project));
		glGetFloatv(GL_MODELVIEW_MATRIX, glm::value_ptr(view));

		matrix = project * view;

		m_state_manager.switch_depth_mask(false);
		m_state_manager.switch_blend(true);

		m_state_manager.switch_program(m_fonts->get_program());

		m_state_manager.get_program()->set_parameter(
			apt_projection_view_matrix,
			matrix);//m_scene_view.get_ortho_projection_matrix());
		m_state_manager.get_program()->set_parameter(apt_time, m_time);
		m_state_manager.get_program()->set_parameter(apt_world_matrix,
			world_matrix);

		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		Uint64 frame_id;
		Uint32 i, index;

		index = 0;
		frame_id = m_frame_id;

		for (i = 0; i < text_cache.size(); ++i)
		{
			if (text_cache[i].text == text)
			{
				m_fonts->draw(m_state_manager,
					text_cache[i].mesh,
					text_cache[i].count);
				text_cache[i].frame_id = frame_id;

				return text_cache[i].lines;
			}

			if (text_cache[i].frame_id < frame_id)
			{
				index = i;
				frame_id = text_cache[i].frame_id;
			}
		}

		text_cache[index].text = text;
		text_cache[index].frame_id = m_frame_id;
		text_cache[index].count = 0;
		text_cache[index].mesh.reset();

		text_cache[index].lines = m_fonts->build_mesh(text, position,
			min_line, max_line, max_width, max_height, wrap,
			text_cache[index].mesh, text_cache[index].count);

		m_fonts->draw(m_state_manager, text_cache[index].mesh,
			text_cache[index].count);

		return text_cache[index].lines;
	}

	float Scene::get_text_width(const Text &text) const
	{
		return m_fonts->get_width(text);
	}

	float Scene::get_font_height(const String &font) const
	{
		return m_fonts->get_height(font);
	}

	void Scene::build_mesh(const VertexBuffersSharedPtr &buffers,
		const Uint32 count, AbstractMeshSharedPtr &mesh) const
	{
		m_fonts->build_mesh(buffers, count, mesh);
	}

	Uint32 Scene::write_to_stream(const Text &text,
		const VertexStreamsSharedPtr &streams,
		const glm::vec2 &start_position, const Uint32 min_line,
		const Uint32 max_line, const float max_width,
		const float max_height, const WrapModeType wrap, Uint32 &line)
		const
	{
		return m_fonts->write_to_stream(text, streams, start_position,
			min_line, max_line, max_width, max_height, wrap, line);
	}

	void Scene::draw(const AbstractMeshSharedPtr &mesh, const Uint32 count)
	{
		m_fonts->draw(m_state_manager, mesh, count);
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

			draw_object_depth(object);

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
			m_state_manager.draw(object->get_sub_objects()[i]);

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

}
