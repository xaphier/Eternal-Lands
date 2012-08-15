/****************************************************************************
 *            scene.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "shader/glslprogram.hpp"
#include "actor.hpp"
#include "effect.hpp"
#include "texturecache.hpp"
#include "renderobjectdata.hpp"
#include "renderlightdata.hpp"
#include "meshbuilder.hpp"
#include "abstractmesh.hpp"
#include "shader/autoparameterutil.hpp"
#include "abstractframebuffer.hpp"
#include "shader/samplerparameterutil.hpp"
#include "globalvars.hpp"
#include "subobject.hpp"
#include "materialdescription.hpp"
#include "framebufferbuilder.hpp"
#include "filter.hpp"
#include "texture.hpp"
#include "convexbody.hpp"
#include "material.hpp"
#include "loader/maploader.hpp"
#include "freeidsmanager.hpp"
#include "thread/materiallock.hpp"
#include "logging.hpp"
#include "terrainvisitor.hpp"
#include "shader/uniformbuffer.hpp"

#include "materialcache.hpp"
#include "materialbuilder.hpp"
#include "image.hpp"

#include "../client_serv.h"

#include "colorcorrection.hpp"

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

				inline ~StateManagerUtil() noexcept
				{
					m_manager.unbind_all();
				}

		};

		class CullingTask
		{
			private:
			public:
				CullingTask();
				virtual ~CullingTask() noexcept;
				virtual void operator()();

		};

		CullingTask::CullingTask()
		{
		}

		CullingTask::~CullingTask() noexcept
		{
		}

		void CullingTask::operator()()
		{
		}

	}

	class Scene::TerrainVisitorTask
	{
		private:
			const Scene &m_scene;
			const Frustum m_frustum;
			const glm::vec3 m_camera;
			TerrainVisitor m_terrain_visitor;

		public:
			TerrainVisitorTask(const Scene &scene,
				const Frustum &frustum,
				const glm::vec3 &camera,
				const MappedUniformBufferSharedPtr
					&mapped_uniform_buffer);
			virtual ~TerrainVisitorTask() noexcept;
			virtual void operator()();

	};

	Scene::TerrainVisitorTask::TerrainVisitorTask(const Scene &scene,
		const Frustum &frustum, const glm::vec3 &camera,
		const MappedUniformBufferSharedPtr &mapped_uniform_buffer):
		m_scene(scene), m_frustum(frustum), m_camera(camera),
		m_terrain_visitor(mapped_uniform_buffer)
	{
	}

	Scene::TerrainVisitorTask::~TerrainVisitorTask() noexcept
	{
	}

	void Scene::TerrainVisitorTask::operator()()
	{
		m_scene.intersect_terrain(m_frustum, m_camera,
			m_terrain_visitor);
	}

	class Scene::ObjectVisitorTask
	{
		private:
			const Scene &m_scene;
			const Frustum m_frustum;
			const glm::vec3 m_camera;
			const bool m_shadow;
			ObjectVisitor &m_visitor;

		public:
			ObjectVisitorTask(const Scene &scene,
				const Frustum &frustum,
				const glm::vec3 &camera,
				const bool shadow,
				ObjectVisitor &visitor);
			virtual ~ObjectVisitorTask() noexcept;
			virtual void operator()();

	};

	Scene::ObjectVisitorTask::ObjectVisitorTask(const Scene &scene,
		const Frustum &frustum, const glm::vec3 &camera,
		const bool shadow, ObjectVisitor &visitor): m_scene(scene),
		m_frustum(frustum), m_camera(camera), m_shadow(shadow),
		m_visitor(visitor)
	{
	}

	Scene::ObjectVisitorTask::~ObjectVisitorTask() noexcept
	{
	}

	void Scene::ObjectVisitorTask::operator()()
	{
		m_scene.intersect(m_frustum, m_shadow, m_visitor);
		m_visitor.sort(m_camera);
	}

	class Scene::LightVisitorTask
	{
		private:
			const Scene &m_scene;
			const Frustum m_frustum;
			const glm::vec3 m_camera;
			LightVisitor &m_visitor;

		public:
			LightVisitorTask(const Scene &scene,
				const Frustum &frustum,
				const glm::vec3 &camera,
				LightVisitor &visitor);
			virtual ~LightVisitorTask() noexcept;
			virtual void operator()();

	};

	Scene::LightVisitorTask::LightVisitorTask(const Scene &scene,
		const Frustum &frustum, const glm::vec3 &camera,
		LightVisitor &visitor): m_scene(scene), m_frustum(frustum),
		m_camera(camera), m_visitor(visitor)
	{
	}

	Scene::LightVisitorTask::~LightVisitorTask() noexcept
	{
	}

	void Scene::LightVisitorTask::operator()()
	{
		m_scene.intersect(m_frustum, m_visitor);
		m_visitor.sort(m_camera);
	}

	Scene::Scene(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system):
		m_global_vars(global_vars), m_file_system(file_system),
		m_scene_resources(global_vars, file_system),
		m_clipmap(m_scene_resources.get_material_builder()),
		m_state_manager(global_vars), m_scene_view(global_vars),
		m_frame_id(0), m_program_vars_id(0),
		m_rebuild_terrain_map(true), m_rebuild_shadow_map(true)
	{
		Uint32 i, count;

		m_light_positions_array.resize(8);
		m_light_colors_array.resize(8);
		m_shadow_update_mask = 0x55;

		set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		set_main_light_color(glm::vec3(0.2f));
		set_main_light_ambient(glm::vec3(0.2f));

		m_free_ids = boost::make_shared<FreeIdsManager>();

		glGenQueries(m_querie_ids.size(), m_querie_ids.data());

		m_scene_resources.init(get_file_system());

		if (get_global_vars()->get_opengl_3_1())
		{
			DEBUG_CHECK_GL_ERROR();

			m_visible_terrain.set_uniform_buffer(boost::make_shared<
				UniformBuffer>(
				get_scene_resources(
					).get_hardware_buffer_mapper(),
				get_scene_resources(
					).get_uniform_buffer_description_cache(),
				ubt_terrain_instances));

			DEBUG_CHECK_GL_ERROR();

			count = m_shadow_terrain.size();

			for (i = 0; i < count; ++i)
			{
				m_shadow_terrain[i].set_uniform_buffer(
					boost::make_shared<UniformBuffer>(
						get_scene_resources(
							).get_hardware_buffer_mapper(),
						get_scene_resources(
							).get_uniform_buffer_description_cache(),
						ubt_terrain_instances));
			}

			DEBUG_CHECK_GL_ERROR();
		}

		get_scene_resources().get_mesh_cache()->get_mesh(
			String(UTF8("quad")), m_screen_quad);

		set_map(boost::make_shared<Map>(
			get_scene_resources().get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_scene_resources().get_mesh_builder(),
			get_scene_resources().get_mesh_cache(),
			get_scene_resources().get_material_cache(),
			get_scene_resources().get_material_builder(),
			get_scene_resources().get_texture_cache(),
			String(UTF8("empty"))));

		try
		{
			m_day_color_correction = get_scene_resources(
				).get_color_correction()->get_color_correction(
				String(UTF8("scripts/day.as")));
			m_night_color_correction = get_scene_resources(
				).get_color_correction()->get_color_correction(
				String(UTF8("scripts/night.as")));
		}
		catch (const boost::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}
		catch (const std::exception &exception)
		{
			LOG_EXCEPTION(exception);
		}

		init_light_indexed_deferred_rendering();
	}

	Scene::~Scene() noexcept
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

	void Scene::add_object(const ObjectDescription &object)
	{
		m_map->add_object(object);
	}

	void Scene::add_object(const ObjectData &object_data,
		const MaterialSharedPtrVector &materials)
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
		get_free_ids()->free_object_id(id);
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
		get_free_ids()->free_light_id(id);
	}

	void Scene::set_fog(const glm::vec3 &color, const float density)
	{
		/* 1.442695 = 1 / ln(2) */
		m_fog = glm::vec4(color, -density * density * 1.442695f);
	}

	void Scene::build_shadow_map()
	{
		Uint32 shadow_map_width, shadow_map_height, shadow_map_size;
		Uint16 mipmaps, shadow_map_count;
		TextureTargetType target;
		TextureFormatType format;
		bool depth_buffer;

		m_shadow_update_mask = 0xFF;

		shadow_map_width = get_scene_view().get_shadow_map_size().x;
		shadow_map_height = get_scene_view().get_shadow_map_size().y;

		shadow_map_size = std::max(shadow_map_width, shadow_map_height);

		if (get_scene_view().get_shadow_map_count() == 0)
		{
			m_shadow_frame_buffer.reset();
			return;
		}

		mipmaps = 0;

		while ((1 << mipmaps) < shadow_map_size)
		{
			mipmaps++;
		}

		shadow_map_count = get_scene_view().get_shadow_map_count();
		target = ttt_texture_2d_array;				
		format = tft_r32f;
		depth_buffer = true;

		if (!get_scene_view().get_exponential_shadow_maps())
		{
			shadow_map_count = 0;
			mipmaps = 0;
			target = ttt_texture_2d;
			format = tft_depth32;
			depth_buffer = false;
		}

		m_shadow_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
				String(UTF8("shadow")), shadow_map_width,
				shadow_map_height, depth_buffer);

		m_shadow_texture = boost::make_shared<Texture>(
			String(UTF8("shadow")));

		m_shadow_texture->set_target(target);
		m_shadow_texture->set_format(format);
		m_shadow_texture->set_wrap_s(twt_clamp);
		m_shadow_texture->set_wrap_t(twt_clamp);
		m_shadow_texture->set_wrap_r(twt_clamp);
		m_shadow_texture->set_mipmap_count(mipmaps);
		m_shadow_texture->init(shadow_map_width, shadow_map_height,
			shadow_map_count, mipmaps);

		m_shadow_frame_buffer->bind();

		if (depth_buffer)
		{
			m_shadow_frame_buffer->attach(m_shadow_texture,
				fbat_color_0, 0);
			m_shadow_frame_buffer->attach_depth_render_buffer();
			m_shadow_frame_buffer->set_draw_buffer(0, true);
		}
		else
		{
			m_shadow_frame_buffer->attach(m_shadow_texture,
				fbat_depth, 0);
			m_shadow_frame_buffer->set_draw_buffer(0, false);
		}

		m_shadow_frame_buffer->unbind();
	}

	void Scene::build_terrain_map()
	{
		Uint16 mipmaps;
		TextureTargetType target;
		TextureFormatType format;

		mipmaps = 0;

		if (get_global_vars()->get_opengl_3_0())
		{
			target = ttt_texture_2d_array;

			while ((1 << mipmaps) <
				get_global_vars()->get_clipmap_size())
			{
				mipmaps++;
			}

			format = tft_rgb8;//tft_r_rgtc1;
		}
		else
		{
			target = ttt_texture_3d;
			format = tft_r5g6b5;
		}

		m_clipmap.rebuild(m_map->get_terrain_size(),
			get_global_vars()->get_view_distance(),
			get_global_vars()->get_clipmap_world_size(),
			get_global_vars()->get_clipmap_size(),
			get_global_vars()->get_clipmap_slices());

		m_clipmap_frame_buffer.reset();
		m_clipmap_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
				String(UTF8("terrain")),
				get_global_vars()->get_clipmap_size(),
				get_global_vars()->get_clipmap_size(),
				false);

		m_clipmap_texture = boost::make_shared<Texture>(
			String(UTF8("terrain")));

		m_clipmap_texture->set_target(target);
		m_clipmap_texture->set_format(format);
		m_clipmap_texture->set_wrap_s(twt_clamp);
		m_clipmap_texture->set_wrap_t(twt_clamp);
		m_clipmap_texture->set_wrap_r(twt_clamp);
		m_clipmap_texture->set_mipmap_count(mipmaps);
		m_clipmap_texture->init(get_global_vars()->get_clipmap_size(),
			get_global_vars()->get_clipmap_size(),
			m_clipmap.get_slices(), mipmaps);

		m_clipmap_frame_buffer->bind();
		m_clipmap_frame_buffer->attach(m_clipmap_texture, fbat_color_0,
			0);
		m_clipmap_frame_buffer->set_draw_buffer(0, true);
		m_clipmap_frame_buffer->unbind();

		if (m_map.get() != nullptr)
		{
			m_map->set_clipmap_texture(m_clipmap_texture);
		}
	}

	void Scene::clear()
	{
		m_actors.clear();
		m_free_ids->clear();
	}

	void Scene::get_lights(const BoundingBox &bounding_box,
		Uint16 &lights_count)
	{
		Uint32 i, size;

		lights_count = 1;

		if (m_map->get_dungeon())
		{
			m_light_positions_array[0] =
				glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
			m_light_colors_array[0] =
				glm::vec4(glm::vec3(0.2f), 0.0f);
		}
		else
		{
			m_light_positions_array[0] = m_main_light_direction;

			if (get_lights())
			{
				m_light_colors_array[0] = m_main_light_color +
					glm::vec4(glm::vec3(0.3f), 0.0f);
			}
			else
			{
				m_light_colors_array[0] = m_main_light_color +
					glm::vec4(glm::vec3(0.1f), 0.0f);
			}
		}

		if (m_map->get_dungeon() || get_lights())
		{
			BOOST_FOREACH(const RenderLightData &light,
				m_visible_lights.get_lights())
			{
				if (light.get_light()->intersect(bounding_box))
				{
					m_light_positions_array[lights_count] =
						glm::vec4(light.get_light()->
							get_position(),
						light.get_light()->
							get_inv_sqr_radius());
					m_light_colors_array[lights_count] =
						glm::vec4(light.get_light()->
							get_color(), 1.0);
					lights_count++;

					if (lights_count >=
						m_light_positions_array.size())
					{
						break;
					}
				}
			}
		}

		size = m_light_colors_array.size();

		for (i = lights_count; i < size; ++i)
		{
			m_light_colors_array[i] = glm::vec4(0.0f);
			m_light_positions_array[i] = glm::vec4(0.0f, 0.0f, 1.0f,
				1.0f);
		}
	}

	void Scene::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, BoundingBox &bounding_box) const
	{
		m_map->intersect_terrain(frustum, camera, bounding_box);
	}

	void Scene::intersect_terrain(const Frustum &frustum,
		const glm::vec3 &camera, TerrainVisitor &terrain) const
	{
		m_map->intersect_terrain(frustum, camera, terrain);
	}

	void Scene::intersect(const Frustum &frustum, const bool shadow,
		ObjectVisitor &visitor) const
	{
		Uint32ActorSharedPtrMap::const_iterator it, end;
		SubFrustumsMask mask;

		m_map->intersect(frustum, visitor);

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
					visitor.add(it->second, mask);
				}

				if (shadow)
				{
					continue;
				}

				visitor.add(it->second, 0.25f,
					bt_alpha_transparency_value, mask);
			}
		}
	}

	void Scene::intersect(const Frustum &frustum, LightVisitor &visitor)
		const
	{
		m_map->intersect(frustum, visitor);
	}

	void Scene::cull()
	{
		Frustum frustum;

		get_scene_view().update();

		frustum = Frustum(
			get_scene_view().get_projection_view_matrix());

		m_visible_lights.get_lights().clear();

		BOOST_FOREACH(glm::vec4 &light_position,
			m_light_positions_array)
		{
			light_position = glm::vec4(0.0f);
		}

		BOOST_FOREACH(glm::vec4 &light_color, m_light_colors_array)
		{
			light_color = glm::vec4(0.0f);
		}

		if (m_map->get_dungeon())
		{
			m_light_positions_array[0] =
				glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
			m_light_colors_array[0] =
				glm::vec4(glm::vec3(0.3f), 0.0f);
		}
		else
		{
			m_light_positions_array[0] = m_main_light_direction;

			if (get_lights())
			{
				m_light_colors_array[0] = m_main_light_color +
					glm::vec4(glm::vec3(0.4f), 0.0f);
			}
			else
			{
				m_light_colors_array[0] = m_main_light_color +
					glm::vec4(glm::vec3(0.1f), 0.0f);
			}
		}

		m_time = SDL_GetTicks() * 0.001f;

		m_visible_objects.next_frame();

		intersect(frustum, false, m_visible_objects);

		DEBUG_CHECK_GL_ERROR();

		if (get_global_vars()->get_opengl_3_1())
		{
			TerrainVisitor terrain_visitor(
				m_visible_terrain.get_uniform_buffer(
					)->get_mapped_uniform_buffer());

			DEBUG_CHECK_GL_ERROR();

			intersect_terrain(frustum, glm::vec3(
				get_scene_view().get_camera()),
				terrain_visitor);

			m_visible_terrain.set_mesh(terrain_visitor.get_mesh());
			m_visible_terrain.set_material(
				terrain_visitor.get_material());
			m_visible_terrain.set_instances(
				terrain_visitor.get_instances());
		}

		DEBUG_CHECK_GL_ERROR();

		m_visible_objects.sort(glm::vec3(
			get_scene_view().get_camera()));

		if (get_scene_view().get_shadow_map_count() > 0)
		{
			cull_all_shadows();
		}

		intersect(frustum, m_visible_lights);

		m_visible_lights.update_camera(glm::vec3(
			get_scene_view().get_camera()));

		if (m_rebuild_terrain_map)
		{
			build_terrain_map();
			m_rebuild_terrain_map = false;
		}
	}

	void Scene::cull_all_shadows()
	{
		SubFrustumsConvexBodys convex_bodys;
		SubFrustumsBoundingBoxes receiver_boxes;
		SubFrustumsBoundingBoxes caster_boxes;
		Frustum frustum;
		glm::vec3 camera;
		SubFrustumsMask mask;
		Uint32 i, count;

		if (m_rebuild_shadow_map)
		{
			build_shadow_map();
			m_rebuild_shadow_map = false;
		}

		frustum = Frustum(
			get_scene_view().get_split_projection_view_matrices());

		if (get_global_vars()->get_opengl_3_1())
		{
			count = m_shadow_terrain.size();

			for (i = 0; i < count; ++i)
			{
				intersect_terrain(Frustum(frustum, i), camera,
					receiver_boxes[i]);
			}
		}

		count = mask.size();

		BOOST_FOREACH(const RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			mask = frustum.intersect_sub_frustums(object.get_object(
				)->get_bounding_box());

			for (i = 0; i < count; ++i)
			{
				if (mask[i])
				{
					receiver_boxes[i].merge(
						object.get_object(
							)->get_bounding_box());
				}
			}
		}

		count = convex_bodys.size();

		for (i = 0; i < count; ++i)
		{
			convex_bodys[i] = ConvexBody(frustum, i);
			convex_bodys[i].clip(receiver_boxes[i]);
		}

		get_scene_view().build_shadow_matrices(
			glm::vec3(get_main_light_direction()), convex_bodys, 
			m_map->get_bounding_box().get_max().z);

		camera = glm::vec3(get_scene_view().get_shadow_camera());

		frustum = Frustum(
			get_scene_view().get_shadow_projection_view_matrices());

		m_shadow_objects.next_frame();

		intersect(frustum, true, m_shadow_objects);

		m_shadow_objects.sort(camera);

		m_shadow_objects_mask.reset();

		if (get_global_vars()->get_opengl_3_1())
		{
			count = m_shadow_terrain.size();

			for (i = 0; i < count; ++i)
			{
				TerrainVisitor terrain_visitor(
					m_shadow_terrain[i].get_uniform_buffer(
						)->get_mapped_uniform_buffer());

				DEBUG_CHECK_GL_ERROR();

				intersect_terrain(Frustum(frustum, i), camera,
					terrain_visitor);

				m_shadow_terrain[i].set_mesh(
					terrain_visitor.get_mesh());
				m_shadow_terrain[i].set_material(
					terrain_visitor.get_material());
				m_shadow_terrain[i].set_instances(
					terrain_visitor.get_instances());

				caster_boxes[i].merge(
					terrain_visitor.get_bounding_box());

				m_shadow_objects_mask[i] =
					terrain_visitor.get_instances() > 0;
			}
		}

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

		if (get_global_vars()->get_opengl_3_2())
		{
			frustum = Frustum(get_scene_view(
				).get_split_projection_view_matrices());

			for (i = 0; i < count; ++i)
			{
				convex_bodys[i] = ConvexBody(frustum, i);
				convex_bodys[i].clip(caster_boxes[i]);
				convex_bodys[i].clip(receiver_boxes[i]);
			}
		}

		get_scene_view().update_shadow_matrices(convex_bodys,
			m_shadow_update_mask);
	}

	bool Scene::switch_program(const GlslProgramSharedPtr &program)
	{
		bool result;

		result = get_state_manager().switch_program(program);

		if (program.get() == nullptr)
		{
			return result;
		}

		if (m_program_vars_id != program->get_last_used())
		{
			program->set_parameter(apt_view_matrix,
				get_scene_view().get_current_view_matrix());
			program->set_parameter(apt_view_rotation_matrix,
				get_scene_view(
					).get_current_view_rotation_matrix());
			program->set_parameter(apt_projection_matrix,
				get_scene_view(
					).get_current_projection_matrix());
			program->set_parameter(apt_projection_view_matrix,
				get_scene_view(
					).get_current_projection_view_matrix());
			program->set_parameter(apt_time, m_time);
			program->set_parameter(apt_fog_data, m_fog);
			program->set_parameter(apt_camera,
				get_scene_view().get_camera());
			program->set_parameter(apt_shadow_distance_transform,
				get_scene_view(
					).get_shadow_distance_transform());
			program->set_parameter(apt_shadow_texture_matrices,
				get_scene_view().get_shadow_texture_matrices());
			program->set_parameter(apt_split_distances,
				get_scene_view().get_split_distances());
			program->set_parameter(apt_terrain_scale,
				m_map->get_terrain_size_data());
			program->set_parameter(apt_terrain_texture_size,
				m_clipmap.get_terrain_texture_size());
			program->set_parameter(apt_z_params,
				get_scene_view().get_z_params());
			program->set_parameter(apt_clipmap_matrices,
				m_clipmap.get_texture_matrices());

			if (m_map->get_dungeon())
			{
				program->set_parameter(apt_ambient,
					glm::vec4(m_map->get_ambient(), 1.0f));
			}
			else
			{
				if (get_lights())
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

			program->set_parameter(apt_dynamic_lights_count, 4);
			program->set_parameter(apt_light_positions,
				m_light_positions_array);
			program->set_parameter(apt_light_colors,
				m_light_colors_array);

			program->set_last_used(m_program_vars_id);
		}

		return result;
	}

	void Scene::draw_terrain(const TerrainRenderingData &terrain_data,
		const EffectProgramType type, const bool lights)
	{
		float tmp;
		Uint16 lights_count;

		if (terrain_data.get_instances() == 0)
		{
			return;
		}

		try
		{
			STRING_MARKER(UTF8("terrain instance(s) %1%"),
				terrain_data.get_instances());

			DEBUG_CHECK_GL_ERROR();

			get_state_manager().switch_mesh(
				terrain_data.get_mesh());

			MaterialLock material(terrain_data.get_material());

			DEBUG_CHECK_GL_ERROR();

			terrain_data.get_uniform_buffer()->bind();

			DEBUG_CHECK_GL_ERROR();

			if (lights)
			{
				tmp = std::numeric_limits<float>::max();

				get_lights(BoundingBox(-glm::vec3(tmp),
					glm::vec3(tmp)), lights_count);
			}

			switch_program(material->get_effect()->get_program(
				type));

			get_state_manager().switch_sample_alpha_to_coverage(
				material->get_effect()->get_description(
					).get_transparent() &&
				(type == ept_shadow));

			get_state_manager().get_program()->set_parameter(
				apt_dynamic_lights_count, lights_count);
			get_state_manager().get_program()->set_parameter(
				apt_light_positions, m_light_positions_array);
			get_state_manager().get_program()->set_parameter(
				apt_light_colors, m_light_colors_array);

			DEBUG_CHECK_GL_ERROR();

			material->bind(get_state_manager());

			DEBUG_CHECK_GL_ERROR();

			get_state_manager().draw(0,
				terrain_data.get_instances());

			DEBUG_CHECK_GL_ERROR();

		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% terrain "
				"instance(s) caught exception '%2%'"),
				terrain_data.get_instances() %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% terrain "
				"instance(s) caught exception '%2%'"),
				terrain_data.get_instances() %
				exception.what());
		}
	}

	void Scene::do_draw_object(const ObjectSharedPtr &object,
		const EffectProgramType type, const Uint16 instances,
		const Uint16 distance)
	{
		Uint16 count, i;
		bool object_data_set;

		get_state_manager().switch_mesh(object->get_mesh());

		DEBUG_CHECK_GL_ERROR();

		count = object->get_materials().size();

		object_data_set = false;

		DEBUG_CHECK_GL_ERROR();

		for (i = 0; i < count; ++i)
		{
			MaterialLock material(object->get_materials()[i]);

			if (switch_program(material->get_effect()->get_program(
				type)))
			{
				object_data_set = false;
			}

			get_state_manager().switch_sample_alpha_to_coverage(
				material->get_effect()->get_description(
					).get_transparent());

			if (!object_data_set)
			{
				get_state_manager().get_program()->set_parameter(
					apt_world_transformation,
					object->get_world_transformation(
						).get_data());
				get_state_manager().get_program()->set_parameter(
					apt_bones, object->get_bones());
				object_data_set = true;
			}

			DEBUG_CHECK_GL_ERROR();

			material->bind(get_state_manager());

			DEBUG_CHECK_GL_ERROR();

			get_state_manager().draw(i, instances);

			DEBUG_CHECK_GL_ERROR();
		}
	}

	void Scene::do_draw_object_old_lights(const ObjectSharedPtr &object,
		const EffectProgramType type, const Uint16 instances,
		const Uint16 distance)
	{
		Uint16 count, i, lights_count;
		bool object_data_set;

		get_state_manager().switch_mesh(object->get_mesh());

		DEBUG_CHECK_GL_ERROR();

		count = object->get_materials().size();

		get_lights(object->get_bounding_box(), lights_count);

		object_data_set = false;

		DEBUG_CHECK_GL_ERROR();

		for (i = 0; i < count; ++i)
		{
			MaterialLock material(object->get_materials()[i]);

			if (switch_program(material->get_effect()->get_program(
				type)))
			{
				object_data_set = false;
			}

			get_state_manager().switch_sample_alpha_to_coverage(
				material->get_effect()->get_description(
					).get_transparent());

			if (!object_data_set)
			{
				get_state_manager().get_program()->set_parameter(
					apt_world_transformation,
					object->get_world_transformation(
						).get_data());
				get_state_manager().get_program()->set_parameter(
					apt_bones, object->get_bones());
				get_state_manager().get_program()->set_parameter(
					apt_dynamic_lights_count, lights_count);
				get_state_manager().get_program()->set_parameter(
					apt_light_positions,
					m_light_positions_array);
				get_state_manager().get_program()->set_parameter(
					apt_light_colors, m_light_colors_array);

				object_data_set = true;
			}

			DEBUG_CHECK_GL_ERROR();

			material->bind(get_state_manager());

			DEBUG_CHECK_GL_ERROR();

			get_state_manager().draw(i, instances);

			DEBUG_CHECK_GL_ERROR();
		}
	}

	void Scene::draw_object_old_lights(const ObjectSharedPtr &object,
		const EffectProgramType type, const Uint16 instances,
		const Uint16 distance)
	{
		try
		{
			STRING_MARKER(UTF8("object name '%1%', mesh name "
				"'%2%', instances %3%"), object->get_name() %
				object->get_mesh()->get_name() % instances);

			do_draw_object_old_lights(object, type, instances,
				distance);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance(s)"
				"of object '%2%' caught exception '%3%'"),
				instances % object->get_name() %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance(s)"
				"of object '%2%' caught exception '%3%'"),
				instances % object->get_name() %
				exception.what());
		}
	}

	void Scene::draw_object(const ObjectSharedPtr &object,
		const EffectProgramType type, const Uint16 instances,
		const Uint16 distance)
	{
		try
		{
			STRING_MARKER(UTF8("object name '%1%', mesh name "
				"'%2%', instances %3%"), object->get_name() %
				object->get_mesh()->get_name() % instances);

			do_draw_object(object, type, instances, distance);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance(s)"
				"of object '%2%' caught exception '%3%'"),
				instances % object->get_name() %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance(s)"
				"of object '%2%' caught exception '%3%'"),
				instances % object->get_name() %
				exception.what());
		}
	}

	void Scene::draw_shadow(const Uint16 index)
	{
		DEBUG_CHECK_GL_ERROR();

		get_state_manager().init();

		if (get_scene_view().get_exponential_shadow_maps())
		{
			m_shadow_frame_buffer->attach(m_shadow_texture,
				fbat_color_0, index);
		}
		else
		{
			m_shadow_frame_buffer->attach(m_shadow_texture,
				fbat_depth, index);
		}

		m_shadow_frame_buffer->clear(glm::vec4(1e38f), 0);
		m_shadow_frame_buffer->clear(1.0f, 0);

		DEBUG_CHECK_GL_ERROR();

		STRING_MARKER(UTF8("drawing shadows %1%"), index);

		get_state_manager().switch_color_mask(glm::bvec4(
			get_scene_view().get_exponential_shadow_maps()));

		get_state_manager().switch_polygon_offset_fill(true);
		glPolygonOffset(1.0f, 32.0f);

		if (!get_scene_view().get_exponential_shadow_maps())
		{
			glCullFace(GL_FRONT);
		}

		DEBUG_CHECK_GL_ERROR();

		get_scene_view().set_shadow_view(index);

		draw_terrain(m_shadow_terrain[index], ept_shadow, false);

		BOOST_FOREACH(const RenderObjectData &object,
			m_shadow_objects.get_objects())
		{
			if (object.get_sub_frustums_mask(index))
			{
				draw_object(object.get_object(), ept_shadow,
					1, object.get_distance());
			}
		}

		update_program_vars_id();

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().unbind_all();
	}

	void Scene::draw_shadows()
	{
		Uint16 i;

		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->bind();

		m_shadow_frame_buffer->set_view_port();

		if (get_global_vars()->get_opengl_3_2())
		{
			glEnable(GL_DEPTH_CLAMP);
		}

		for (i = 0; i < get_scene_view().get_shadow_map_count(); ++i)
		{
			if (!m_shadow_update_mask[i])
			{
				continue;
			}

			if (m_shadow_objects_mask[i])
			{
				draw_shadow(i);
			}
			else
			{
				if (get_scene_view(
					).get_exponential_shadow_maps())
				{
					m_shadow_frame_buffer->attach(
						m_shadow_texture,
						fbat_color_0, i);
				}
				else
				{
					m_shadow_frame_buffer->attach(
						m_shadow_texture, fbat_depth,
						i);
				}

				m_shadow_frame_buffer->clear(glm::vec4(1e38f),
					0);
			}
		}

		m_shadow_frame_buffer->unbind();

		unbind_all();

		if (get_global_vars()->get_opengl_3_2())
		{
			glDisable(GL_DEPTH_CLAMP);
		}

		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->unbind();

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().switch_texture(spt_shadow,
			m_shadow_texture);

		DEBUG_CHECK_GL_ERROR();

		if (get_scene_view().get_exponential_shadow_maps())
		{
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		}

		DEBUG_CHECK_GL_ERROR();

		m_shadow_update_mask = 0x55;

		if ((m_frame_id % 2) == 0)
		{
			m_shadow_update_mask.flip();
		}

		m_shadow_update_mask[0] = true;
		m_shadow_update_mask = 0xFFFF;
	}

	void Scene::draw_depth()
	{
		Uint32 index;
		bool no_depth_read;

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("depth"));

		DEBUG_CHECK_GL_ERROR();

		get_scene_view().set_default_view();

		get_state_manager().switch_color_mask(glm::bvec4(false));

		index = 0;

		draw_terrain(m_visible_terrain, ept_depth, false);

		no_depth_read = false;

		BOOST_FOREACH(RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			if ((object.get_blend() != bt_disabled) ||
				!object.get_depth_read())
			{
				no_depth_read |= !object.get_depth_read();

				object.set_occlusion_culling(
					std::numeric_limits<Uint32>::max());
				continue;
			}

			object.set_occlusion_culling(index);

			glBeginQuery(GL_SAMPLES_PASSED, m_querie_ids[index]);

			draw_object(object.get_object(), ept_depth,
				1, object.get_distance());

			glEndQuery(GL_SAMPLES_PASSED);

			index++;
		}

		depth_read();

		if (no_depth_read)
		{
			BOOST_FOREACH(RenderObjectData &object,
				m_visible_objects.get_objects())
			{
				if ((object.get_blend() != bt_disabled) ||
					object.get_depth_read())
				{
					continue;
				}

				object.set_occlusion_culling(index);

				glBeginQuery(GL_SAMPLES_PASSED,
					m_querie_ids[index]);

				draw_object(object.get_object(), ept_depth,
					1, object.get_distance());

				glEndQuery(GL_SAMPLES_PASSED);

				index++;
			}
		}

		unbind_all();

		DEBUG_CHECK_GL_ERROR();

		update_program_vars_id();
	}

	void Scene::draw_default()
	{
		Uint32 index, count;
		EffectProgramType effect;
		BlendType blend;
		bool old_lights;

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("default"));

		DEBUG_CHECK_GL_ERROR();

		get_scene_view().set_default_view();

		get_state_manager().switch_multisample(true);

		DEBUG_CHECK_GL_ERROR();

		old_lights = get_global_vars()->get_light_system() ==
			lst_default;

		DEBUG_CHECK_GL_ERROR();

		effect = ept_default;

		if (get_global_vars()->get_effect_debug())
		{
			effect = ept_debug;
		}

		blend = bt_disabled;

		draw_terrain(m_visible_terrain, effect, true);

		BOOST_FOREACH(const RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			index = object.get_occlusion_culling();

			if (index < m_querie_ids.size())
			{
				count = 0;

				glGetQueryObjectuiv(m_querie_ids[index],
					GL_QUERY_RESULT, &count);

				if (count == 0)
				{
					continue;
				}
			}

			if (blend != object.get_blend())
			{
				blend = object.get_blend();

				switch (blend)
				{
					case bt_disabled:
						break;
					case bt_alpha_transparency_source_value:
						glBlendFunc(GL_SRC_ALPHA,
							GL_ONE_MINUS_SRC_ALPHA);
						break;
					case bt_alpha_transparency_value:
						glBlendFunc(GL_CONSTANT_ALPHA,
							GL_ONE_MINUS_CONSTANT_ALPHA);
						break;
					case bt_additive:
						glBlendFunc(GL_ONE,
							GL_ONE_MINUS_SRC_ALPHA);
						break;
				}

				get_state_manager().switch_blend(blend !=
					bt_disabled);
			}

			if (blend == bt_alpha_transparency_value)
			{
				glBlendColor(1.0f, 1.0f, 1.0f,
					object.get_transparency());
			}

			DEBUG_CHECK_GL_ERROR();

			if (old_lights)
			{
				draw_object_old_lights(object.get_object(),
					effect, 1, object.get_distance());
			}
			else
			{
				draw_object(object.get_object(), effect, 1,
					object.get_distance());
			}
		}

		DEBUG_CHECK_GL_ERROR();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		CHECK_GL_ERROR();

		update_program_vars_id();
		m_frame_id++;
	}

	void Scene::depth_read()
	{
	}

	void Scene::update_terrain_texture(const MaterialSharedPtr &material,
		const Mat2x3Array2 &texture_matrices, const Uint16 index)
	{
		m_clipmap_frame_buffer->bind();
		m_clipmap_frame_buffer->attach(m_clipmap_texture, fbat_color_0,
			index);
//		m_clipmap_frame_buffer->attach(m_clipmap_tmp_texture,
//			fbat_color_0, 0);
		m_clipmap_frame_buffer->clear(glm::vec4(0.0f), 0);

		MaterialLock material_lock(material);

		DEBUG_CHECK_GL_ERROR();

		switch_program(material_lock->get_effect(
			)->get_program(ept_default));

		DEBUG_CHECK_GL_ERROR();

		material_lock->bind(get_state_manager());

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().get_program()->set_parameter(
			apt_texture_matrices, texture_matrices);
		get_state_manager().get_program()->set_parameter(
			apt_albedo_scale_offsets,
			material->get_albedo_scale_offsets());
		get_state_manager().get_program()->set_parameter(
			apt_emission_scale_offset,
			material->get_emission_scale_offset());
		get_state_manager().get_program()->set_parameter(
			apt_specular_scale_offset,
			material->get_specular_scale_offset());

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().draw(0, 1);

		DEBUG_CHECK_GL_ERROR();
	}

	void Scene::update_terrain_texture(const Uint16 slice)
	{
		Mat2x3Array2 texture_matrices;
		glm::mat2x3 texture_matrix;
		glm::vec2 tile_scale;

		try
		{
			STRING_MARKER(UTF8("Updating terrain slice %1%"),
				slice);

			tile_scale = m_map->get_terrain_size() /
				glm::vec2(2.0f);

			m_clipmap.update_slice(slice);

			texture_matrices[0] = glm::mat2x3(glm::inverse(
				glm::mat3(m_clipmap.get_texture_matrices(
					)[slice])));

			texture_matrices[1] = texture_matrices[0];

			texture_matrices[1][0] *= tile_scale.x;
			texture_matrices[1][1] *= tile_scale.y;

			update_terrain_texture(m_map->get_clipmap_material(),
				texture_matrices, slice);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain slice "
				"%1% caught exception '%2%'"), slice %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain slice "
				"%1% caught exception '%2%'"), slice %
				exception.what());
		}
	}

	void Scene::update_terrain_texture()
	{
		Mat2x3Array2 texture_matrices;
		glm::mat2x3 texture_matrix;
		Uint32 i, count;

		DEBUG_CHECK_GL_ERROR();

		m_clipmap_frame_buffer->set_view_port();

		DEBUG_CHECK_GL_ERROR();

		get_scene_view().set_ortho_view();
		get_state_manager().switch_depth_mask(false);
		get_state_manager().switch_depth_test(false);
		get_state_manager().switch_blend(true);
		get_state_manager().switch_multisample(false);
		get_state_manager().switch_mesh(get_screen_quad());

		count = m_clipmap.get_slices();

		for (i = 0; i < count; ++i)
		{
			update_terrain_texture(i);
		}

		update_program_vars_id();

		DEBUG_CHECK_GL_ERROR();

		unbind_all();

		DEBUG_CHECK_GL_ERROR();

		m_clipmap_frame_buffer->unbind();

		DEBUG_CHECK_GL_ERROR();

		if (get_global_vars()->get_opengl_3_0())
		{
			get_state_manager().switch_texture(spt_effect_0,
				m_clipmap_texture);

			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		}

		DEBUG_CHECK_GL_ERROR();
	}

	void Scene::init_light_indexed_deferred_rendering()
	{
		AbstractMeshSharedPtr mesh;
		MaterialSharedPtrVector materials;
		MaterialDescription material_description;
		ObjectData object_data;
		glm::mat2x3 emission_scale_offset;

		object_data.set_name(String(UTF8("sphere")));
		object_data.set_id(0xFFFFFFFF);
		material_description.set_cast_shadows(false);

		get_scene_resources().get_mesh_cache()->get_mesh(
			object_data.get_name(), mesh);

		emission_scale_offset[1] = glm::vec3(1.0);

		material_description.set_emission_scale_offset(
			emission_scale_offset);
		material_description.set_name(String(UTF8("light")));
		material_description.set_effect(String(UTF8("light-index")));

		materials.push_back(get_scene_resources().get_material_builder(
			)->get_material(material_description));

		m_light_sphere = boost::make_shared<Object>(object_data, mesh,
			materials);
	}

	void Scene::draw_light(const glm::vec3 &position, const float size,
		const Uint8 light_index)
	{
		Transformation world_transformation;
		glm::mat2x3 emission_scale_offset;
		glm::uvec4 bits;
		glm::vec4 color;

		if (get_global_vars()->get_light_system() == lst_lidr_x5)
		{
			bits.r = (light_index & (0x03 << 0)) << 8;
			bits.g = (light_index & (0x03 << 2)) << 6;
			bits.b = (light_index & (0x03 << 4)) << 4;
			bits.a = 0;

			color = glm::vec4(bits) / 1023.0f;
		}
		else
		{
			bits.r = (light_index & (0x03 << 0)) << 6;
			bits.g = (light_index & (0x03 << 2)) << 4;
			bits.b = (light_index & (0x03 << 4)) << 2;
			bits.a = (light_index & (0x03 << 6)) << 0;

			color = glm::vec4(bits) / 255.0f;
		}

		world_transformation.set_scale(size);
		world_transformation.set_translation(position);
		world_transformation.set_rotation_angles(glm::vec3());

		m_light_sphere->set_world_transformation(world_transformation);

		m_light_sphere->get_materials()[0]->set_color(color);

		STRING_MARKER(UTF8("drawing light %1%, %2%"),
			static_cast<Uint16>(light_index) %
			glm::to_string(color));

		glCullFace(GL_FRONT);
		/************/
		glStencilFunc(GL_ALWAYS, light_index, 0xFFFFFFFF);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP); 

		// Disable color writes
		get_state_manager().switch_color_mask(glm::bvec4(false));
		get_state_manager().switch_blend(false);

		// Draw a sphere the radius of the light
		draw_object(m_light_sphere, ept_depth, 1, 0);

		// Set the stencil to only pass on equal value
		glStencilFunc(GL_EQUAL, light_index, 0xFFFFFFFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); 
		glCullFace(GL_BACK);
		/************/

		get_state_manager().switch_color_mask(glm::bvec4(true));
		get_state_manager().switch_blend(true);

		//TODO: Render lowers detail spheres when light is far away?
		// Draw a sphere the radius of the light
		draw_object(m_light_sphere, ept_default, 1, 0);
	}

	void Scene::draw_light_camera_inside(const glm::vec3 &position,
		const float size, const Uint8 light_index)
	{
		Transformation world_transformation;
		glm::mat2x3 emission_scale_offset;
		glm::uvec4 bits;
		glm::vec4 color;

		if (get_global_vars()->get_light_system() == lst_lidr_x5)
		{
			bits.r = (light_index & (0x03 << 0)) << 8;
			bits.g = (light_index & (0x03 << 2)) << 6;
			bits.b = (light_index & (0x03 << 4)) << 4;
			bits.a = 0;

			color = glm::vec4(bits) / 1023.0f;
		}
		else
		{
			bits.r = (light_index & (0x03 << 0)) << 6;
			bits.g = (light_index & (0x03 << 2)) << 4;
			bits.b = (light_index & (0x03 << 4)) << 2;
			bits.a = (light_index & (0x03 << 6)) << 0;

			color = glm::vec4(bits) / 255.0f;
		}

		world_transformation.set_scale(size);
		world_transformation.set_translation(position);
		world_transformation.set_rotation_angles(glm::vec3());

		m_light_sphere->set_world_transformation(world_transformation);

		m_light_sphere->get_materials()[0]->set_color(color);

		STRING_MARKER(UTF8("drawing light %1%, %2%"),
			static_cast<Uint16>(light_index) %
			glm::to_string(color));

		get_state_manager().switch_color_mask(glm::bvec4(true));
		get_state_manager().switch_depth_mask(false);
		get_state_manager().switch_blend(true);
		get_state_manager().switch_culling(true);

		draw_object(m_light_sphere, ept_default, 1, 0);
	}

	void Scene::draw_lights()
	{
		Uint32Uint32Map::const_iterator found;

		if (get_global_vars()->get_light_system() == lst_default)
		{
			return;
		}

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("lights"));
		m_scene_frame_buffer->bind();
		m_scene_frame_buffer->set_view_port();
		m_scene_frame_buffer->attach(m_light_index_texture,
			fbat_color_0, 0);
		m_scene_frame_buffer->clear(glm::vec4(0.0f), 0);

		glBlendFunc(GL_ONE, GL_CONSTANT_COLOR);

		// Set the constant blend color to bit shift 2 bits down on each call
		if (get_global_vars()->get_light_system() == lst_lidr_x5)
		{
			glBlendColor(0.25025f, 0.25025f, 0.25025f, 0.25025f);
		}
		else
		{
			glBlendColor(0.251f, 0.251f, 0.251f, 0.251f);
		}

		glEnable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_CLAMP);
		get_state_manager().switch_depth_mask(false);
		get_state_manager().switch_blend(true);
		get_state_manager().switch_culling(true);

		BOOST_FOREACH(const RenderLightData &light,
			m_visible_lights.get_lights())
		{
			if (light.get_camera_inside())
			{
				continue;
			}

			found = m_light_index.find(light.get_light()->get_id());

			if (found == m_light_index.end())
			{
				continue;
			}

			draw_light(light.get_light()->get_position(),
				light.get_light()->get_radius() + 1.2f,
				found->second);
		}

		glDisable(GL_STENCIL_TEST);

		glCullFace(GL_FRONT);
		glDepthFunc(GL_GREATER);

		get_state_manager().switch_color_mask(glm::bvec4(true));
		get_state_manager().switch_depth_mask(false);
		get_state_manager().switch_blend(true);
		get_state_manager().switch_culling(true);

		BOOST_FOREACH(const RenderLightData &light,
			m_visible_lights.get_lights())
		{
			if (!light.get_camera_inside())
			{
				continue;
			}

			found = m_light_index.find(light.get_light()->get_id());

			if (found == m_light_index.end())
			{
				continue;
			}

			draw_light_camera_inside(
				light.get_light()->get_position(),
				light.get_light()->get_radius() + 1.2f,
				found->second);
		}

		glDisable(GL_DEPTH_CLAMP);
		glCullFace(GL_FRONT);
		glDepthFunc(GL_LEQUAL);

		update_program_vars_id();

		DEBUG_CHECK_GL_ERROR();

		unbind_all();
	}

	void Scene::draw()
	{
		StateManagerUtil state(get_state_manager());

		CHECK_GL_ERROR();

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().init();

		get_scene_view().set_default_view();
		glDepthFunc(GL_LEQUAL);

		if (m_scene_frame_buffer.get() == nullptr)
		{
			set_view_port();
		}
		else
		{
			m_scene_frame_buffer->bind();
			m_scene_frame_buffer->attach(m_scene_texture,
				fbat_color_0, 0);
			m_scene_frame_buffer->clear(glm::vec4(0.0f), 0);
			m_scene_frame_buffer->clear(1.0f, 0);
			m_scene_frame_buffer->set_view_port();
		}

		draw_depth();

		if ((m_scene_frame_buffer.get() != nullptr) &&
			(get_global_vars()->get_opengl_3_2()))
		{
			draw_lights();
		}

		if (get_scene_view().get_shadow_map_count() > 0)
		{
			STRING_MARKER(UTF8("drawing mode '%1%'"),
				UTF8("shadows"));
			draw_shadows();
		}

		if (m_clipmap.update(glm::vec3(get_scene_view().get_camera()),
			glm::vec3(get_scene_view().get_view_dir()),
			glm::vec2(get_scene_view().get_focus())))
		{
			update_terrain_texture();
		}

		if (get_scene_view().get_shadow_map_count() > 0)
		{
			get_state_manager().switch_texture(spt_shadow,
				m_shadow_texture);
		}

		get_state_manager().switch_depth_mask(false);

		if (m_scene_frame_buffer.get() == nullptr)
		{
			set_view_port();
		}
		else
		{
			m_scene_frame_buffer->bind();
			m_scene_frame_buffer->set_view_port();
			m_scene_frame_buffer->attach(m_scene_texture,
				fbat_color_0, 0);

			if (get_global_vars()->get_opengl_3_2() &&
				(get_global_vars()->get_light_system() !=
					lst_default))
			{
				get_state_manager().switch_texture(
					spt_light_positions,
					m_light_position_texture);
				get_state_manager().switch_texture(
					spt_light_colors,
					m_light_color_texture);
				get_state_manager().switch_texture(
					spt_light_indices,
					m_light_index_texture);
			}
		}

		draw_default();
	}

	void Scene::pick_object(const RenderObjectData &object,
		PairUint32SelectionTypeVector &ids, Uint32 &query_index)
	{
		PairUint32SelectionType data;
		Uint32 i, sub_objects, index;
		bool object_data_set;

		if (object.get_object()->get_selection() == st_none)
		{
			return;
		}

		if (object.get_object()->get_sub_objects().size() == 0)
		{
			data.first = object.get_object()->get_id();
			data.second = object.get_object()->get_selection();

			glBeginQuery(GL_SAMPLES_PASSED,
				m_querie_ids[query_index]);

			draw_object(object.get_object(), ept_depth,
				1, object.get_distance());

			glEndQuery(GL_SAMPLES_PASSED);

			ids.push_back(data);
			query_index++;

			return;
		}

		get_state_manager().switch_mesh(
			object.get_object()->get_mesh());

		sub_objects = object.get_object()->get_sub_objects().size();

		object_data_set = false;

		for (i = 0; i < sub_objects; ++i)
		{
			if (object.get_object()->get_sub_objects(
				)[i].get_selection() == st_none)
			{
				continue;
			}

			STRING_MARKER(UTF8("sub object index '%1%'"), i);

			data.first = object.get_object()->get_sub_objects(
				)[i].get_id();
			data.second = object.get_object()->get_sub_objects(
				)[i].get_selection();

			glBeginQuery(GL_SAMPLES_PASSED,
				m_querie_ids[query_index]);

			index = object.get_object()->get_sub_objects(
				)[i].get_material();

			MaterialLock material(object.get_object(
				)->get_materials()[index]);

			if (switch_program(material->get_effect()->get_program(
				ept_depth)))
			{
				object_data_set = false;
			}

			if (!object_data_set)
			{
				get_state_manager().get_program()->set_parameter(
					apt_world_transformation,
					object.get_object(
						)->get_world_transformation(
						).get_data());
				get_state_manager().get_program()->set_parameter(
					apt_bones, object.get_object(
						)->get_bones());
				object_data_set = true;
			}

			material->bind(get_state_manager());

			get_state_manager().get_program()->set_parameter(
				apt_texture_matrices,
				material->get_texture_matrices());

			get_state_manager().draw(object.get_object(
				)->get_sub_objects()[i], 1);

			glEndQuery(GL_SAMPLES_PASSED);

			ids.push_back(data);
			query_index++;
		}
	}

	Uint32 Scene::pick(const glm::vec2 &offset, const glm::vec2 &size,
		SelectionType &selection)
	{
		PairUint32SelectionTypeVector ids;
		Uint32 i, max_count, count, id, query_offset, query_index;
		StateManagerUtil state(get_state_manager());

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("picking"));

		get_state_manager().unbind_all();
		get_state_manager().switch_scissor_test(true);
		glScissor(offset.x - size.x, offset.y - size.y, 2 * size.x,
			2 * size.y);
		get_state_manager().switch_multisample(true);
		get_state_manager().switch_depth_mask(false);
		get_state_manager().switch_color_mask(glm::bvec4(false));

		get_state_manager().switch_polygon_offset_fill(true);
		glPolygonOffset(0.0f, -1.0f);

		glDepthFunc(GL_LEQUAL);

		get_scene_view().set_default_view();

		query_offset = m_querie_ids.size() / 2;
		query_index = query_offset;

		BOOST_FOREACH(const RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			pick_object(object, ids, query_index);
		}

		update_program_vars_id();

		max_count = 0;
		id = 0xFFFFFFFF;
		selection = st_none;

		for (i = 0; i < ids.size(); ++i)
		{
			count = 0;
			assert(query_index > (i + query_offset));

			glGetQueryObjectuiv(m_querie_ids[i + query_offset],
				GL_QUERY_RESULT, &count);

			if ((count >= max_count) && (count > 0) &&
				(ids[i].second != st_none))
			{
				id = ids[i].first;
				selection = ids[i].second;

				max_count = count;
			}
		}

		CHECK_GL_ERROR();

		return id;
	}

	void Scene::select_rect(const glm::vec2 &min, const glm::vec2 &max,
		Uint32Set &selections)
	{
		PairUint32SelectionTypeVector ids;
		Uint32 i, count, query_offset, query_index;
		StateManagerUtil state(get_state_manager());

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("picking"));

		selections.clear();

		get_state_manager().unbind_all();
		get_state_manager().switch_scissor_test(true);
		glScissor(min.x, min.y, max.x - min.x, max.y - min.y);
		get_state_manager().switch_multisample(true);
		get_state_manager().switch_depth_mask(false);
		get_state_manager().switch_color_mask(glm::bvec4(false));

		get_state_manager().switch_polygon_offset_fill(true);
		glPolygonOffset(0.0f, -1.0f);

		glDepthFunc(GL_LEQUAL);

		get_scene_view().set_default_view();

		query_offset = m_querie_ids.size() / 2;
		query_index = query_offset;

		BOOST_FOREACH(const RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			pick_object(object, ids, query_index);
		}

		update_program_vars_id();

		for (i = 0; i < ids.size(); ++i)
		{
			count = 0;
			assert(query_index > (i + query_offset));

			glGetQueryObjectuiv(m_querie_ids[i + query_offset],
				GL_QUERY_RESULT, &count);

			if ((count > 0) && (ids[i].second != st_none))
			{
				selections.insert(ids[i].first);
			}
		}

		CHECK_GL_ERROR();
	}

	double Scene::get_depth(const glm::uvec2 &offset)
	{
		Uint32 depth;

		glReadPixels(offset.x, offset.y, 1, 1, GL_DEPTH_COMPONENT,
			GL_UNSIGNED_INT, &depth);

		return static_cast<double>(depth) /
			std::numeric_limits<Uint32>::max();
	}

	void Scene::load(const String &name)
	{
		boost::scoped_ptr<MapLoader> map_loader;

		clear();

		map_loader.reset(new MapLoader(
			get_scene_resources().get_codec_manager(),
			m_file_system, m_global_vars,
			get_scene_resources().get_mesh_builder(),
			get_scene_resources().get_mesh_cache(),
			get_scene_resources().get_mesh_data_cache(),
			get_scene_resources().get_effect_cache(),
			get_scene_resources().get_material_cache(),
			get_scene_resources().get_material_description_cache(),
			get_scene_resources().get_material_builder(),
			get_scene_resources().get_texture_cache(),
			m_free_ids));

		set_map(map_loader->load(name));
	}

	void Scene::map_changed()
	{
		rebuild_terrain_map();
		rebuild_shadow_map();

		update_light_system();
	}

	void Scene::update_light_system()
	{
		ImageSharedPtr light_position_image;
		ImageSharedPtr light_color_image;
		Uint32LightSharedPtrMap::const_iterator it, end;
		glm::vec4 position, color;
		Uint32 index, count;

		switch (get_global_vars()->get_light_system())
		{
			case lst_default:
				m_light_position_texture.reset();
				m_light_color_texture.reset();
				return;
			case lst_lidr_x4:
				count = 256;
				break;
			case lst_lidr_x5:
				count = 64;
				break;
		};

		m_light_position_texture = boost::make_shared<Texture>(
			String(UTF8("light position")));

		m_light_position_texture->set_target(ttt_texture_1d);
		m_light_position_texture->set_format(tft_rgba32f);
		m_light_position_texture->set_wrap_s(twt_clamp);
		m_light_position_texture->set_wrap_t(twt_clamp);
		m_light_position_texture->set_wrap_r(twt_clamp);
		m_light_position_texture->set_mipmap_count(0);
		m_light_position_texture->set_mag_filter(tft_nearest);
		m_light_position_texture->set_min_filter(tft_nearest);
		m_light_position_texture->set_mipmap_filter(tmt_none);

		m_light_color_texture = boost::make_shared<Texture>(
			String(UTF8("light color")));

		m_light_color_texture->set_target(ttt_texture_1d);
		m_light_color_texture->set_format(tft_rgb9_e5);
		m_light_color_texture->set_wrap_s(twt_clamp);
		m_light_color_texture->set_wrap_t(twt_clamp);
		m_light_color_texture->set_wrap_r(twt_clamp);
		m_light_color_texture->set_mipmap_count(0);
		m_light_color_texture->set_mag_filter(tft_nearest);
		m_light_color_texture->set_min_filter(tft_nearest);
		m_light_color_texture->set_mipmap_filter(tmt_none);

		light_position_image = boost::make_shared<Image>(
			String(UTF8("light position")), false, tft_rgba32f,
			glm::uvec3(count, 1, 1), 0);

		light_color_image = boost::make_shared<Image>(
			String(UTF8("light color")), false, tft_rgb9_e5,
			glm::uvec3(count, 1, 1), 0);

		light_position_image->set_pixel(0, 0, 0, 0, 0, glm::vec4());
		light_color_image->set_pixel(0, 0, 0, 0, 0, glm::vec4());

		end = get_map()->get_lights().end();

		index = 1;

		m_light_index.clear();

		for (it = get_map()->get_lights().begin(); it != end; ++it)
		{
			if (index == count)
			{
				break;
			}

			position = glm::vec4(it->second->get_position(),
				it->second->get_inv_sqr_radius());
			color = glm::vec4(it->second->get_color(), 1.0f);

			light_position_image->set_pixel(index, 0, 0, 0, 0,
				position);
			light_color_image->set_pixel(index, 0, 0, 0, 0, color);

			m_light_index[it->second->get_id()] = index;

			index++;
		}

		m_light_position_texture->set_image(light_position_image);
		m_light_color_texture->set_image(light_color_image);

		update_light_index_texture();
	}

	void Scene::update_light_index_texture()
	{
		if (get_global_vars()->get_light_system() == lst_default)
		{
			m_light_index_texture.reset();
			return;
		}

		m_light_index_texture = boost::make_shared<Texture>(
			String(UTF8("light index")));

		m_light_index_texture->set_target(ttt_texture_2d);

		if (get_global_vars()->get_light_system() == lst_lidr_x5)
		{
			m_light_index_texture->set_format(tft_rgb10_a2);
		}
		else
		{
			m_light_index_texture->set_format(tft_rgba8);
		}

		m_light_index_texture->set_wrap_s(twt_clamp);
		m_light_index_texture->set_wrap_t(twt_clamp);
		m_light_index_texture->set_wrap_r(twt_clamp);
		m_light_index_texture->set_mipmap_count(0);
		m_light_index_texture->set_mag_filter(tft_nearest);
		m_light_index_texture->set_min_filter(tft_nearest);
		m_light_index_texture->set_mipmap_filter(tmt_none);
		m_light_index_texture->init(get_view_port().z,
			get_view_port().w, 0, 0);
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

	glm::uvec2 Scene::get_walk_height_map_size() const
	{
		return m_map->get_walk_height_map_size();
	}

	glm::uvec2 Scene::get_height_map_size() const
	{
		return m_map->get_height_map_size();
	}

	glm::uvec2 Scene::get_tile_map_size() const
	{
		return m_map->get_tile_map_size();
	}

	void Scene::set_dungeon(const bool dungeon)
	{
		m_map->set_dungeon(dungeon);
	}

	bool Scene::get_dungeon() const
	{
		return m_map->get_dungeon();
	}

	const glm::vec3 &Scene::get_ambient() const
	{
		return m_map->get_ambient();
	}

	void Scene::set_ambient(const glm::vec3 &color)
	{
		m_map->set_ambient(color);
	}

	float Scene::get_walk_height(const Uint16 x, const Uint16 y) const
	{
		return m_map->get_walk_height(x, y);
	}

	bool Scene::get_terrain() const
	{
		return m_map->get_terrain();
	}

	void Scene::blit_to_back_buffer()
	{
		glm::uvec4 rect;

		if (m_scene_frame_buffer.get() == nullptr)
		{
			return;
		}

		rect = glm::uvec4(get_view_port());
		rect.z += rect.x;
		rect.w += rect.y;

		set_view_port();

		m_scene_frame_buffer->blit_to_back_buffer(rect, 0, true,
			true, false);
		m_scene_frame_buffer->unbind();
	}

	void Scene::set_view_port(const glm::uvec4 &view_port)
	{
		Uint16 mipmaps;

		if (!get_global_vars()->get_use_scene_fbo())
		{
			m_scene_frame_buffer.reset();

			get_scene_view().set_view_port(view_port);

			m_light_index_texture.reset();
			m_light_position_texture.reset();
			m_light_color_texture.reset();

			m_scene_texture.reset();

			return;
		}

		if ((view_port.z == get_view_port().z) &&
			(view_port.w == get_view_port().w)
			&& (m_scene_frame_buffer.get() != nullptr))
		{
			get_scene_view().set_view_port(view_port);

			return;
		}

		mipmaps = 0;

		while ((1 << mipmaps) <
			std::max(view_port.z, view_port.w))
		{
			mipmaps++;
		}

		m_scene_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
				String(UTF8("scene")), view_port.z,
				view_port.w, true);

		m_scene_texture = boost::make_shared<Texture>(
			String(UTF8("scene")));

		m_scene_texture->set_target(ttt_texture_2d);
		m_scene_texture->set_format(tft_rgba8);
		m_scene_texture->set_wrap_s(twt_clamp);
		m_scene_texture->set_wrap_t(twt_clamp);
		m_scene_texture->set_wrap_r(twt_clamp);
		m_scene_texture->set_mipmap_count(mipmaps);
		m_scene_texture->init(view_port.z, view_port.w, 0,
			mipmaps);

		m_scene_frame_buffer->bind();
		m_scene_frame_buffer->attach(m_scene_texture,
			fbat_color_0, 0);
		m_scene_frame_buffer->set_draw_buffer(0, true);
		m_scene_frame_buffer->unbind();

		update_light_index_texture();

		get_scene_view().set_view_port(view_port);
	}

	void Scene::set_view_port()
	{
		glViewport(get_view_port()[0], get_view_port()[1],
			get_view_port()[2], get_view_port()[3]);
	}

}
