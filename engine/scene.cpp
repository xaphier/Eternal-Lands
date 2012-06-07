/****************************************************************************
 *            scene.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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

#include "materialcache.hpp"

#include "../client_serv.h"

#include "terrainvisitor.hpp"
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

		m_light_position_array.resize(8);
		m_light_color_array.resize(8);
		m_shadow_update_mask = 0x55;

		set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		set_main_light_color(glm::vec3(0.2f));
		set_main_light_ambient(glm::vec3(0.2f));

		m_free_ids = boost::make_shared<FreeIdsManager>();

		glGenQueries(m_querie_ids.size(), m_querie_ids.data());

		m_scene_resources.init(get_file_system());

		if (get_global_vars()->get_opengl_3_2())
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
			String(UTF8("empty"))));
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
		Uint16 mipmaps, samples, shadow_map_count;

		m_shadow_update_mask = 0xFF;

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
					0, ttt_texture_2d, tft_depth32, true);

			return;
		}

		while ((1 << mipmaps) < shadow_map_size)
		{
			mipmaps++;
		}

		shadow_map_count = m_scene_view.get_shadow_map_count();

		samples = 0;

		if (get_global_vars()->get_use_multisample_shadows())
		{
			samples = 4;
		}

		m_shadow_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
				String(UTF8("Shadow")),	shadow_map_width,
				shadow_map_height, shadow_map_count,
				mipmaps, samples, ttt_texture_2d_array,
				tft_r32f, false, true);
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

			format = tft_rgb8;
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
				m_clipmap.get_slices(), mipmaps, target,
				format, false);

		if (m_map.get() != nullptr)
		{
			m_map->set_clipmap_texture(
				m_clipmap_frame_buffer->get_texture());
		}
	}

	void Scene::clear()
	{
		m_actors.clear();
		m_free_ids->clear();
	}

	void Scene::get_lights(const BoundingBox &bounding_box,
		Uint16 &light_count)
	{
		Uint32 i, size;

		light_count = 1;

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

			if (get_lights())
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

		if (m_map->get_dungeon() || get_lights())
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
			m_light_position_array[i] = glm::vec4(0.0f, 0.0f, 1.0f,
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

			if (get_lights())
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

		intersect(frustum, false, m_visible_objects);

		DEBUG_CHECK_GL_ERROR();

		if (get_global_vars()->get_opengl_3_2())
		{
			TerrainVisitor terrain_visitor(
				m_visible_terrain.get_uniform_buffer(
					)->get_mapped_uniform_buffer());

			DEBUG_CHECK_GL_ERROR();

			intersect_terrain(frustum, glm::vec3(
				m_scene_view.get_camera()), terrain_visitor);

			m_visible_terrain.set_mesh(terrain_visitor.get_mesh());
			m_visible_terrain.set_material(
				terrain_visitor.get_material());
			m_visible_terrain.set_instances(
				terrain_visitor.get_instances());
		}

		DEBUG_CHECK_GL_ERROR();

		m_visible_objects.sort(glm::vec3(m_scene_view.get_camera()));

		if (m_scene_view.get_shadow_map_count() > 0)
		{
			cull_all_shadows();
		}

		intersect(frustum, m_visible_lights);

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
			m_scene_view.get_split_projection_view_matrices());

		if (get_global_vars()->get_opengl_3_2())
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
//			convex_bodys[i].clip(m_map->get_bounding_box());
			convex_bodys[i].clip(receiver_boxes[i]);
		}

		m_scene_view.build_shadow_matrices(
			glm::vec3(get_main_light_direction()), convex_bodys, 
			m_map->get_bounding_box().get_max().z);

		camera = glm::vec3(m_scene_view.get_shadow_camera());

		frustum = Frustum(
			m_scene_view.get_shadow_projection_view_matrices());

		m_shadow_objects.next_frame();

		intersect(frustum, true, m_shadow_objects);

		m_shadow_objects.sort(camera);

		m_shadow_objects_mask.reset();

		if (get_global_vars()->get_opengl_3_2())
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
			frustum = Frustum(
				m_scene_view.get_split_projection_view_matrices());

			for (i = 0; i < count; ++i)
			{
				convex_bodys[i] = ConvexBody(frustum, i);
				convex_bodys[i].clip(caster_boxes[i]);
				convex_bodys[i].clip(receiver_boxes[i]);
			}
		}

		m_scene_view.update_shadow_matrices(convex_bodys,
			m_shadow_update_mask);
	}

	bool Scene::switch_program(const GlslProgramSharedPtr &program)
	{
		bool result;

		result = m_state_manager.switch_program(program);

		if (program.get() == nullptr)
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
				m_scene_view.get_current_projection_matrix());
			program->set_parameter(apt_projection_view_matrix,
				m_scene_view.get_current_projection_view_matrix(
					));
			program->set_parameter(apt_time, m_time);
			program->set_parameter(apt_fog_data, m_fog);
			program->set_parameter(apt_camera,
				m_scene_view.get_camera());
			program->set_parameter(apt_shadow_distance_transform,
				m_scene_view.get_shadow_distance_transform());
			program->set_parameter(apt_shadow_texture_matrices,
				m_scene_view.get_shadow_texture_matrices());
			program->set_parameter(apt_split_distances,
				m_scene_view.get_split_distances());
			program->set_parameter(apt_terrain_scale,
				m_map->get_terrain_size_data());
			program->set_parameter(apt_terrain_texture_size,
				m_clipmap.get_terrain_texture_size());
			program->set_parameter(apt_z_params,
				m_scene_view.get_z_params());
			m_state_manager.get_program()->set_parameter(
				apt_clipmap_matrices,
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

			program->set_last_used(m_program_vars_id);
		}

		return result;
	}

	void Scene::draw_terrain(const TerrainRenderingData &terrain_data,
		const EffectProgramType type, const bool lights)
	{
		float tmp;
		Uint16 light_count;

		if (terrain_data.get_instances() == 0)
		{
			return;
		}

		try
		{
			STRING_MARKER(UTF8("terrain instance(s) %1%"),
				terrain_data.get_instances());

			DEBUG_CHECK_GL_ERROR();

			m_state_manager.switch_mesh(terrain_data.get_mesh());

			MaterialLock material(terrain_data.get_material());

			DEBUG_CHECK_GL_ERROR();

			terrain_data.get_uniform_buffer()->bind();

			DEBUG_CHECK_GL_ERROR();

			if (lights)
			{
				tmp = std::numeric_limits<float>::max();

				get_lights(BoundingBox(-glm::vec3(tmp),
					glm::vec3(tmp)), light_count);
			}

			switch_program(material->get_effect()->get_program(
				type));

			m_state_manager.switch_sample_alpha_to_coverage(
				material->get_effect()->get_description(
					).get_transparent() &&
				(type == ept_shadow));

			light_count = std::min(light_count,
				material->get_effect()->get_max_light_count());

			m_state_manager.get_program()->set_parameter(
				apt_dynamic_light_count, light_count);
			m_state_manager.get_program()->set_parameter(
				apt_light_positions, m_light_position_array);
			m_state_manager.get_program()->set_parameter(
				apt_light_colors, m_light_color_array);

			DEBUG_CHECK_GL_ERROR();

			material->bind(m_state_manager);

			DEBUG_CHECK_GL_ERROR();

			m_state_manager.draw(0, terrain_data.get_instances());

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
		const Uint16 distance, const bool lights)
	{
		Uint16 count, i, light_count;
		bool object_data_set;

		m_state_manager.switch_mesh(object->get_mesh());

		DEBUG_CHECK_GL_ERROR();

		count = object->get_materials().size();

		if (lights)
		{
			get_lights(object->get_bounding_box(), light_count);
		}

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

			m_state_manager.switch_sample_alpha_to_coverage(
				material->get_effect()->get_description(
					).get_transparent());

			if (!object_data_set)
			{
				light_count = std::min(light_count,
					material->get_effect(
						)->get_max_light_count());
				m_state_manager.get_program()->set_parameter(
					apt_world_transformation,
					object->get_world_transformation(
						).get_data());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object->get_bones());
				m_state_manager.get_program()->set_parameter(
					apt_dynamic_light_count, light_count);
				m_state_manager.get_program()->set_parameter(
					apt_light_positions,
					m_light_position_array);
				m_state_manager.get_program()->set_parameter(
					apt_light_colors, m_light_color_array);

				object_data_set = true;
			}

			DEBUG_CHECK_GL_ERROR();

			material->bind(m_state_manager);

			DEBUG_CHECK_GL_ERROR();

			m_state_manager.draw(i, instances);

			DEBUG_CHECK_GL_ERROR();
		}
	}

	void Scene::draw_object(const ObjectSharedPtr &object,
		const EffectProgramType type, const Uint16 instances,
		const Uint16 distance, const bool lights)
	{
		try
		{
			STRING_MARKER(UTF8("object name '%1%', mesh name "
				"'%2%', instances %3%"), object->get_name() %
				object->get_mesh()->get_name() % instances);

			do_draw_object(object, type, instances, distance,
				lights);
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

	void Scene::draw_shadows(const Uint16 index)
	{
		DEBUG_CHECK_GL_ERROR();

		m_state_manager.init();

		STRING_MARKER(UTF8("drawing shadows %1%"), index);

		m_state_manager.switch_polygon_offset_fill(
			!m_scene_view.get_exponential_shadow_maps());
		m_state_manager.switch_color_mask(glm::bvec4(
			m_scene_view.get_exponential_shadow_maps()));

		if (!m_scene_view.get_exponential_shadow_maps())
		{
			glPolygonOffset(1.05f, 8.0f);
			glCullFace(GL_FRONT);
		}

		DEBUG_CHECK_GL_ERROR();

		m_scene_view.set_shadow_view(index);

		draw_terrain(m_shadow_terrain[index], ept_shadow, false);

		BOOST_FOREACH(const RenderObjectData &object,
			m_shadow_objects.get_objects())
		{
			if (object.get_sub_frustums_mask(index))
			{
				draw_object(object.get_object(), ept_shadow,
					1, object.get_distance(), false);
			}
		}

		m_program_vars_id++;

		DEBUG_CHECK_GL_ERROR();

		m_state_manager.unbind_all();
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

		width = m_shadow_frame_buffer->get_width();
		height = m_shadow_frame_buffer->get_height();

		glViewport(0, 0, width, height);

		if (get_global_vars()->get_opengl_3_2())
		{
			glEnable(GL_DEPTH_CLAMP);
		}

		for (i = 0; i < m_scene_view.get_shadow_map_count(); ++i)
		{
			if (!m_shadow_update_mask[i])
			{
				continue;
			}

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

		if (get_global_vars()->get_opengl_3_2())
		{
			glDisable(GL_DEPTH_CLAMP);
		}

		DEBUG_CHECK_GL_ERROR();

		m_shadow_frame_buffer->unbind();

		DEBUG_CHECK_GL_ERROR();

		m_state_manager.switch_texture(spt_shadow,
			m_shadow_frame_buffer->get_texture());

		DEBUG_CHECK_GL_ERROR();

		if (m_scene_view.get_exponential_shadow_maps())
		{
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		}

		DEBUG_CHECK_GL_ERROR();

		glViewport(m_scene_view.get_view_port()[0],
			m_scene_view.get_view_port()[1],
			m_scene_view.get_view_port()[2],
			m_scene_view.get_view_port()[3]);

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

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("depth"));

		DEBUG_CHECK_GL_ERROR();

		m_scene_view.set_default_view();

		m_state_manager.switch_color_mask(glm::bvec4(false));

		index = 0;

		draw_terrain(m_visible_terrain, ept_depth, false);

		BOOST_FOREACH(RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			if (object.get_blend() != bt_disabled)
			{
				continue;
			}

			object.set_occlusion_culling(index);

			glBeginQuery(GL_SAMPLES_PASSED, m_querie_ids[index]);

			draw_object(object.get_object(), ept_depth,
				1, object.get_distance(), false);

			glEndQuery(GL_SAMPLES_PASSED);

			index++;
		}

		unbind_all();

		DEBUG_CHECK_GL_ERROR();

		m_program_vars_id++;
	}

	void Scene::draw_default()
	{
		Uint32 index, count;
		BlendType blend;

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("default"));

		DEBUG_CHECK_GL_ERROR();

		m_scene_view.set_default_view();

		m_state_manager.switch_multisample(true);

		DEBUG_CHECK_GL_ERROR();

		blend = bt_disabled;

		draw_terrain(m_visible_terrain, ept_default, true);

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

				m_state_manager.switch_blend(blend !=
					bt_disabled);
			}

			if (blend == bt_alpha_transparency_value)
			{
				glBlendColor(1.0f, 1.0f, 1.0f,
					object.get_transparency());
			}

			DEBUG_CHECK_GL_ERROR();

			draw_object(object.get_object(), ept_default,
				1, object.get_distance(), true);
		}

		DEBUG_CHECK_GL_ERROR();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		CHECK_GL_ERROR();

		m_program_vars_id++;
		m_frame_id++;
	}

	void Scene::update_terrain_texture(
		const MaterialSharedPtrVector &materials,
		const Mat2x3Array2 &texture_matrices, const Uint16 index)
	{
		Uint32 i, count;

		m_clipmap_frame_buffer->bind(index);
		m_clipmap_frame_buffer->clear(glm::vec4(0.0f));

		count = materials.size();

		glBlendFunc(GL_ONE, GL_ONE);

		for (i = 0; i < count; ++i)
		{
			MaterialLock material(materials[i]);

			DEBUG_CHECK_GL_ERROR();

			switch_program(material->get_effect(
				)->get_program(ept_default));

			DEBUG_CHECK_GL_ERROR();

			material->bind(m_state_manager);

			DEBUG_CHECK_GL_ERROR();

			m_state_manager.get_program()->set_parameter(
				apt_texture_matrices, texture_matrices);
			m_state_manager.get_program()->set_parameter(
				apt_albedo_scale_offsets,
				material->get_albedo_scale_offsets());
			m_state_manager.get_program()->set_parameter(
				apt_emission_scale_offset,
				material->get_emission_scale_offset());
			m_state_manager.get_program()->set_parameter(
				apt_specular_scale_offset,
				material->get_specular_scale_offset());

			DEBUG_CHECK_GL_ERROR();

			m_state_manager.draw(0, 1);

			DEBUG_CHECK_GL_ERROR();
			break;
		}
/*
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (i = 0; i < count; ++i)
		{
			MaterialLock material(materials[i]);

			DEBUG_CHECK_GL_ERROR();

			switch_program(material->get_effect(
				)->get_default_program(), 0);

			DEBUG_CHECK_GL_ERROR();

			material->bind(m_state_manager);

			DEBUG_CHECK_GL_ERROR();

			m_state_manager.get_program()->set_parameter(
				apt_texture_matrices,
				material->get_texture_matrices());
			m_state_manager.get_program()->set_parameter(
				apt_albedo_scale_offsets,
				material->get_albedo_scale_offsets());

			DEBUG_CHECK_GL_ERROR();

			m_state_manager.draw(0, 1);

			DEBUG_CHECK_GL_ERROR();
		}

		m_clipmap_frame_buffer->blit();
*/
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

			update_terrain_texture(m_clipmap.get_materials(),
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
		Uint32 i, width, height, count;

		width = m_clipmap_frame_buffer->get_width();
		height = m_clipmap_frame_buffer->get_height();

		glViewport(0, 0, width, height);
		DEBUG_CHECK_GL_ERROR();

		m_scene_view.set_ortho_view();
		m_state_manager.switch_depth_mask(false);
		m_state_manager.switch_depth_test(false);
		m_state_manager.switch_blend(true);
		m_state_manager.switch_multisample(false);
		m_state_manager.switch_mesh(m_screen_quad);

		count = m_clipmap.get_slices();

		for (i = 0; i < count; ++i)
		{
			update_terrain_texture(i);
		}

		m_program_vars_id++;

		DEBUG_CHECK_GL_ERROR();

		unbind_all();

		DEBUG_CHECK_GL_ERROR();

		m_clipmap_frame_buffer->unbind();

		DEBUG_CHECK_GL_ERROR();

		m_state_manager.switch_texture(spt_albedo_0,
			m_clipmap_frame_buffer->get_texture());

		if (get_global_vars()->get_opengl_3_0())
		{
			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
		}

		DEBUG_CHECK_GL_ERROR();

		glViewport(m_scene_view.get_view_port()[0],
			m_scene_view.get_view_port()[1],
			m_scene_view.get_view_port()[2],
			m_scene_view.get_view_port()[3]);
	}

	void Scene::draw()
	{
		StateManagerUtil state(m_state_manager);
		CHECK_GL_ERROR();

		DEBUG_CHECK_GL_ERROR();

		m_state_manager.init();

		m_scene_view.set_default_view();
		glDepthFunc(GL_LEQUAL);
		draw_depth();

		if (m_scene_view.get_shadow_map_count() > 0)
		{
			STRING_MARKER(UTF8("drawing mode '%1%'"),
				UTF8("shadows"));
			draw_all_shadows_array();
		}

		if (m_clipmap.update(glm::vec3(m_scene_view.get_camera()),
			glm::vec3(m_scene_view.get_view_dir()),
			glm::vec2(m_scene_view.get_focus())))
		{
			update_terrain_texture();
		}

		if (m_scene_view.get_shadow_map_count() > 0)
		{
			m_state_manager.switch_texture(spt_shadow,
				m_shadow_frame_buffer->get_texture());
		}

		m_state_manager.switch_depth_mask(false);
		draw_default();
	}

	void Scene::pick_object(const RenderObjectData &object,
		PairUint32SelectionTypeVector &ids)
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
				m_querie_ids[ids.size()]);

			draw_object(object.get_object(), ept_depth,
				1, object.get_distance(), false);

			glEndQuery(GL_SAMPLES_PASSED);

			ids.push_back(data);

			return;
		}

		m_state_manager.switch_mesh(object.get_object()->get_mesh());

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
				m_querie_ids[ids.size()]);

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
				m_state_manager.get_program()->set_parameter(
					apt_world_transformation,
					object.get_object(
						)->get_world_transformation(
						).get_data());
				m_state_manager.get_program()->set_parameter(
					apt_bones, object.get_object(
						)->get_bones());
				object_data_set = true;
			}

			material->bind(m_state_manager);

			m_state_manager.get_program()->set_parameter(
				apt_texture_matrices,
				material->get_texture_matrices());

			m_state_manager.draw(object.get_object(
				)->get_sub_objects()[i], 1);

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

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("picking"));

		m_state_manager.unbind_all();
		m_state_manager.switch_scissor_test(true);
		glScissor(offset.x - size.x, offset.y - size.y, 2 * size.x,
			2 * size.y);
		m_state_manager.switch_color_mask(glm::bvec4(false));
		m_state_manager.switch_multisample(false);
//		m_state_manager.switch_polygon_offset_fill(true);
//		glPolygonOffset(0.99f, -4.0f);

		glDepthFunc(GL_LEQUAL);

		m_scene_view.set_default_view();

		m_state_manager.switch_depth_mask(false);

		BOOST_FOREACH(const RenderObjectData &object,
			m_visible_objects.get_objects())
		{
			pick_object(object, ids);
		}

		m_program_vars_id++;

		max_count = 0;
		id = 0xFFFFFFFF;
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
			m_scene_resources.get_material_cache(),
			m_scene_resources.get_material_description_cache(),
			m_free_ids));

		set_map(map_loader->load(name));
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

}
