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
#include "effect/effect.hpp"
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

#include "cpurasterizer.hpp"

#include "materialcache.hpp"
#include "materialbuilder.hpp"
#include "image.hpp"

#include "../client_serv.h"

#include "shader/uniformdescription.hpp"
#include "shader/uniformbufferdescription.hpp"

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

		class StateSetUtil
		{
			private:
				StateManager &m_manager;
				StateSet m_state_set;

			public:
				inline StateSetUtil(StateManager &manager):
					m_manager(manager)
				{
					m_state_set = m_manager.get_state_set();
				}

				inline ~StateSetUtil() noexcept
				{
					m_manager.set_state_set(m_state_set);
				}

		};

		class GlQuery
		{
			private:
				const GLenum m_query;

			public:
				inline GlQuery(const GLenum query,
					const GLuint id): m_query(query)
				{
					glBeginQuery(m_query, id);
				}

				inline ~GlQuery() noexcept
				{
					glEndQuery(m_query);
				}

		};
			
	}

	Scene::Scene(const GlobalVarsSharedPtr &global_vars,
		const FileSystemSharedPtr &file_system):
		m_global_vars(global_vars), m_file_system(file_system),
		m_scene_resources(global_vars, file_system),
		m_clipmap_terrain(m_scene_resources.get_material_builder()),
		m_state_manager(global_vars), m_scene_view(global_vars),
		m_frame_id(0), m_program_vars_id(0),
		m_rebuild_terrain_map(true), m_rebuild_shadow_map(true)
	{
		MapSharedPtr map;

		m_light_positions_array.resize(8);
		m_light_colors_array.resize(8);

		set_main_light_direction(glm::vec3(0.0f, 0.0f, 1.0f));
		set_main_light_color(glm::vec3(0.2f));
		set_main_light_ambient(glm::vec3(0.2f));

		m_free_ids = boost::make_shared<FreeIdsManager>();

		glGenQueries(m_querie_ids.size(), m_querie_ids.data());

		m_scene_resources.init(get_file_system());

		get_scene_resources().get_mesh_cache()->get_mesh(
			String(UTF8("quad")), m_screen_quad);

		map = boost::make_shared<Map>(
			get_scene_resources().get_codec_manager(),
			get_file_system(), get_global_vars(),
			get_scene_resources().get_mesh_builder(),
			get_scene_resources().get_mesh_cache(),
			get_scene_resources().get_material_cache(),
			get_scene_resources().get_terrain_builder(),
			get_scene_resources().get_texture_cache());

		map->set_name(String(UTF8("empty")));

		set_map(map);

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
		LOG_DEBUG(lt_rendering, UTF8("Building shadow map %1%"),
			UTF8("started"));

		try
		{
			do_build_shadow_map();
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While building shadow map "
				"caught exception '%1%'"),
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While building shadow map "
				"caught exception '%1%'"), exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While building shadow map "
				"caught '%1%'"), UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Building shadow map %1%"),
			UTF8("done"));
	}

	void Scene::do_build_shadow_map()
	{
		Uint32 shadow_map_width, shadow_map_height, shadow_map_size;
		Uint16 mipmaps, shadow_map_count, samples;
		TextureTargetType target;
		TextureFormatType format;
		bool depth_buffer;

		shadow_map_width = get_scene_view().get_shadow_map_size().x;
		shadow_map_height = get_scene_view().get_shadow_map_size().y;

		shadow_map_size = std::max(shadow_map_width, shadow_map_height);

		if (get_scene_view().get_shadow_map_count() == 0)
		{
			m_shadow_frame_buffer.reset();
			return;
		}

		mipmaps = 0;

		while ((1u << mipmaps) < shadow_map_size)
		{
			mipmaps++;
		}

		shadow_map_count = get_scene_view().get_shadow_map_count();
		samples = 0;

		if (get_global_vars()->get_use_multisample_shadows())
		{
			samples = 4;
		}

		target = ttt_texture_2d_array;				
		format = tft_r32f;
		depth_buffer = true;

		if (!get_scene_view().get_exponential_shadow_maps())
		{
			shadow_map_count = 0;
			mipmaps = 0;
			samples = 0;
			target = ttt_texture_2d;
			format = tft_depth32;
			depth_buffer = false;
		}

		m_shadow_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
				String(UTF8("shadow")), shadow_map_width,
				shadow_map_height, samples, depth_buffer);

		m_shadow_texture = boost::make_shared<Texture>(
			String(UTF8("shadow")), shadow_map_width,
			shadow_map_height, shadow_map_count, mipmaps, 0,
			format, target);

		m_shadow_texture->set_wrap_s(twt_clamp);
		m_shadow_texture->set_wrap_t(twt_clamp);
		m_shadow_texture->set_wrap_r(twt_clamp);
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

	void Scene::do_build_terrain_map()
	{
		Uint32 size;
		Uint16 mipmaps;
		TextureTargetType target;
		TextureFormatType format;

		size = get_global_vars()->get_clipmap_terrain_size();
		mipmaps = 0;

		if (get_global_vars()->get_opengl_3_0())
		{
			target = ttt_texture_2d_array;

			while ((1 << mipmaps) <
				get_global_vars()->get_clipmap_terrain_size())
			{
				mipmaps++;
			}

			if (get_global_vars()->get_use_linear_lighting())
			{
				format = tft_srgb8_a8;
			}
			else
			{
				format = tft_rgba8;
			}
		}
		else
		{
			target = ttt_texture_3d;

			if (get_global_vars()->get_use_linear_lighting())
			{
				format = tft_srgb8;
			}
			else
			{
				format = tft_rgb8;
			}
		}

		m_clipmap_terrain.rebuild(m_map->get_terrain_size(),
			get_global_vars()->get_view_distance(),
			get_global_vars()->get_clipmap_terrain_world_size(),
			size, get_global_vars()->get_clipmap_terrain_slices());

		m_clipmap_terrain_frame_buffer.reset();
		m_clipmap_terrain_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
				String(UTF8("terrain")), size, size, 0, false);

		m_clipmap_terrain_texture = boost::make_shared<Texture>(
			String(UTF8("terrain map")), size, size,
			m_clipmap_terrain.get_slices(), mipmaps, 0, format,
			target);

		m_clipmap_terrain_texture->set_wrap_s(twt_clamp);
		m_clipmap_terrain_texture->set_wrap_t(twt_clamp);
		m_clipmap_terrain_texture->set_wrap_r(twt_clamp);
		m_clipmap_terrain_texture->init(size, size,
			m_clipmap_terrain.get_slices(), mipmaps);

		if (get_global_vars()->get_opengl_3_0())
		{
			MaterialDescription material_description;

			material_description.set_name(
				String(UTF8("normal_map")));
			material_description.set_effect(
				String(UTF8("normal_map")));

			m_normal_map_material = get_scene_resources(
				).get_material_builder()->get_material(
					material_description);

			m_clipmap_terrain_normal_texture =
				boost::make_shared<Texture>(
					String(UTF8("terrain normal map")),
					size, size,
					m_clipmap_terrain.get_slices(),
					mipmaps, 0, tft_rg8, target);

			m_clipmap_terrain_normal_texture->set_wrap_s(twt_clamp);
			m_clipmap_terrain_normal_texture->set_wrap_t(twt_clamp);
			m_clipmap_terrain_normal_texture->set_wrap_r(twt_clamp);
			m_clipmap_terrain_normal_texture->init(size, size,
				m_clipmap_terrain.get_slices(), mipmaps);

			m_normal_map_material->set_texture(
				m_clipmap_terrain_texture, spt_effect_0);
		}

		m_clipmap_terrain_frame_buffer->bind();
		m_clipmap_terrain_frame_buffer->attach(
			m_clipmap_terrain_texture, fbat_color_0, 0);
		m_clipmap_terrain_frame_buffer->set_draw_buffer(0, true);
		m_clipmap_terrain_frame_buffer->unbind();

		if (m_map.get() != nullptr)
		{
			m_map->set_clipmap_terrain_texture(
				m_clipmap_terrain_texture);

			m_map->set_clipmap_terrain_normal_texture(
				m_clipmap_terrain_normal_texture);
		}
	}

	void Scene::build_terrain_map()
	{
		LOG_DEBUG(lt_rendering, UTF8("Building terrain map %1%"),
			UTF8("started"));

		try
		{
			do_build_terrain_map();
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While building terrain map "
				"caught exception '%1%'"),
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While building terrain map "
				"caught exception '%1%'"), exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While building terrain map "
				"caught '%1%'"), UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Building terrain map %1%"),
			UTF8("done"));
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

		assert(get_global_vars()->get_light_system() == lst_default);
/*
		if (!get_map()->get_dungeon() && !get_lights())
		{
			return;
		}
*/
		BOOST_FOREACH(const RenderLightData &light,
			m_visible_lights.get_lights())
		{
			if (!light.get_light()->intersect(bounding_box))
			{
				continue;
			}

			m_light_positions_array[lights_count] =
				glm::vec4(light.get_light()->get_position(),
					light.get_light()->get_inv_sqr_radius(
						));

			m_light_colors_array[lights_count] = glm::vec4(
				light.get_light()->get_color(), 1.0);

			lights_count++;

			if (lights_count >= m_light_positions_array.size())
			{
				break;
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

		m_map->intersect(frustum, visitor);

		end = m_actors.end();

		for (it = m_actors.begin(); it != end; ++it)
		{
			if (frustum.intersect(it->second->get_bounding_box())
				!= it_outside)
			{
				if ((it->second->get_buffs().to_ulong() &
					BUFF_INVISIBILITY) == 0)
				{
					visitor.add(it->second);
					continue;
				}

				if (shadow)
				{
					continue;
				}

				visitor.add(it->second, 0.25f,
					bt_alpha_transparency_value);
			}
		}
	}

	void Scene::intersect(const Frustum &frustum, LightVisitor &visitor)
		const
	{
		m_map->intersect(frustum, visitor);
	}

	void Scene::cull(const Frustum &frustum,
		const glm::mat4 &projection_view_matrix,
		const glm::vec3 &camera, const bool shadow,
		ObjectVisitor &objects) const
	{
		objects.next_frame();

		objects.set_projection_view_matrix(projection_view_matrix);

		intersect(frustum, shadow, objects);

		objects.sort(camera);
	}

	void Scene::cull_terrain(const Frustum &frustum,
		const AbstractWriteMemorySharedPtr &buffer,
		const glm::vec3 &camera, const Uint64 offset,
		const Uint16 max_instances, TerrainRenderingData &terrain_data)
		const
	{
		TerrainVisitor terrain_visitor(buffer, offset, max_instances);

		intersect_terrain(frustum, camera, terrain_visitor);

		terrain_data.set_material(terrain_visitor.get_material());
		terrain_data.set_instances(terrain_visitor.get_instances());
		terrain_data.set_terrain_lod_offset(
			terrain_visitor.get_terrain_lod_offset());
	}

	void Scene::cull()
	{
		Frustum frustum;
		AbstractWriteMemorySharedPtr visible_terrain_buffer;
		boost::array<AbstractWriteMemorySharedPtr, 4>
			shadow_terrain_buffers;
		boost::array<Frustum, 4> shadow_frutums;
		glm::vec3 camera;
		Uint16 offset;
		Uint32 i, count;
		Uint16 max_instances;
		bool use_terrain;

		use_terrain = get_global_vars()->get_opengl_3_3() &&
			get_terrain();

		get_scene_view().update();

		get_scene_view().set_scale_view(get_map()->get_bounding_box());

		if (m_rebuild_shadow_map)
		{
			build_shadow_map();
			m_rebuild_shadow_map = false;
		}

		count = get_scene_view().get_shadow_map_count();

		if (use_terrain)
		{
			visible_terrain_buffer = m_visible_terrain.get_mesh(
				)->get_vertex_buffer(1);

			for (i = 0; i < count; ++i)
			{
				shadow_terrain_buffers[i] = m_shadow_terrains[
					i].get_mesh()->get_vertex_buffer(1);
			}
		}

		frustum = Frustum(
			get_scene_view().get_scale_view_matrix());

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
			m_light_colors_array[0] = glm::vec4(glm::vec3(0.25f),
				0.0f);
		}
		else
		{
			m_light_positions_array[0] = m_main_light_direction;
			m_light_colors_array[0] = m_main_light_color;
		}

		for (i = 0; i < count; ++i)
		{
			get_scene_view().build_shadow_matrices(
				glm::vec3(get_main_light_direction()),
				m_map->get_bounding_box().get_max().z, i);
		}

		m_time = SDL_GetTicks() * 0.001f;

		if (get_global_vars()->get_use_cpu_rasterizer())
		{
			m_visible_objects.set_cpu_rasterizer(m_cpu_rasterizer);
		}
		else
		{
			m_visible_objects.set_cpu_rasterizer(
				CpuRasterizerSharedPtr());
		}

		frustum = Frustum(get_scene_view(
			).get_projection_view_matrix());

		for (i = 0; i < count; ++i)
		{
			shadow_frutums[i] = Frustum(get_scene_view(
				).get_shadow_projection_view_matrices()[i]);
		}

		#pragma omp parallel sections
		{
			#pragma omp section
			cull(frustum,
				get_scene_view().get_projection_view_matrix(),
				glm::vec3(get_scene_view().get_camera()),
				false, m_visible_objects);

			#pragma omp section
			if (use_terrain)
			{
				offset = m_visible_terrain.get_offset();
				max_instances =
					m_visible_terrain.get_max_instances();

				cull_terrain(frustum, visible_terrain_buffer,
					glm::vec3(get_scene_view(
						).get_camera()), offset,
					max_instances, m_visible_terrain);
			}

			#pragma omp section
			if (get_map()->get_dungeon() || get_lights())
			{
				intersect(frustum, m_visible_lights);

				m_visible_lights.update_camera(glm::vec3(
					get_scene_view().get_camera()));
			}

			#pragma omp section
			if (count > 0)
			{
				cull(shadow_frutums[0], get_scene_view(
					).get_shadow_projection_view_matrices(
						)[0],
					glm::vec3(get_scene_view(
						).get_shadow_cameras()[0]),
					true, m_shadow_objects[0]);
			}

			#pragma omp section
			if ((count > 0) && use_terrain)
			{
				offset = m_shadow_terrains[0].get_offset();
				max_instances = m_shadow_terrains[
					0].get_max_instances();

				cull_terrain(shadow_frutums[0],
					shadow_terrain_buffers[0],
					glm::vec3(get_scene_view(
						).get_shadow_cameras()[0]),
					offset, max_instances,
					m_shadow_terrains[0]);
			}

			#pragma omp section
			if (count > 1)
			{
				cull(shadow_frutums[1], get_scene_view(
					).get_shadow_projection_view_matrices(
						)[1],
					glm::vec3(get_scene_view(
						).get_shadow_cameras()[1]),
					true, m_shadow_objects[1]);
			}

			#pragma omp section
			if ((count > 1) && use_terrain)
			{
				offset = m_shadow_terrains[1].get_offset();
				max_instances = m_shadow_terrains[
					1].get_max_instances();

				cull_terrain(shadow_frutums[1],
					shadow_terrain_buffers[1],
					glm::vec3(get_scene_view(
						).get_shadow_cameras()[1]),
					offset, max_instances,
					m_shadow_terrains[1]);
			}

			#pragma omp section
			if (count > 2)
			{
				cull(shadow_frutums[2], get_scene_view(
					).get_shadow_projection_view_matrices(
						)[2],
					glm::vec3(get_scene_view(
						).get_shadow_cameras()[2]),
					true, m_shadow_objects[2]);
			}

			#pragma omp section
			if ((count > 2) && use_terrain)
			{
				offset = m_shadow_terrains[2].get_offset();
				max_instances = m_shadow_terrains[
					2].get_max_instances();

				cull_terrain(shadow_frutums[2],
					shadow_terrain_buffers[2],
					glm::vec3(get_scene_view(
						).get_shadow_cameras()[2]),
					offset, max_instances,
					m_shadow_terrains[2]);
			}

			#pragma omp section
			if (count > 3)
			{
				cull(shadow_frutums[3], get_scene_view(
					).get_shadow_projection_view_matrices(
						)[3],
					glm::vec3(get_scene_view(
						).get_shadow_cameras()[3]),
					true, m_shadow_objects[3]);
			}

			#pragma omp section
			if ((count > 3) && use_terrain)
			{
				offset = m_shadow_terrains[3].get_offset();
				max_instances = m_shadow_terrains[
					3].get_max_instances();

				cull_terrain(shadow_frutums[3],
					shadow_terrain_buffers[3],
					glm::vec3(get_scene_view(
						).get_shadow_cameras()[3]),
					offset, max_instances,
					m_shadow_terrains[3]);
			}
		}

		if (use_terrain)
		{
			visible_terrain_buffer.reset();

			for (i = 0; i < count; ++i)
			{
				shadow_terrain_buffers[i].reset();
			}
		}

		if (m_rebuild_terrain_map)
		{
			build_terrain_map();
			m_rebuild_terrain_map = false;
		}
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
			program->set_parameter(apt_shadow_distance_transforms,
				get_scene_view(
					).get_shadow_distance_transforms());
			program->set_parameter(
				apt_current_shadow_distance_transform,
				get_scene_view(
					).get_current_shadow_distance_transform(
						));
			program->set_parameter(apt_shadow_texture_matrices,
				get_scene_view().get_shadow_texture_matrices());
			program->set_parameter(apt_split_distances,
				get_scene_view().get_split_distances());
			program->set_parameter(apt_terrain_scale,
				m_map->get_terrain_size_data());
			program->set_parameter(apt_terrain_texture_size,
				m_clipmap_terrain.get_terrain_texture_size());
			program->set_parameter(apt_z_params,
				get_scene_view().get_z_params());
			program->set_parameter(apt_clipmap_terrain_matrices,
				m_clipmap_terrain.get_texture_matrices());

			if (get_map()->get_dungeon())
			{
				program->set_parameter(apt_ambient,
					glm::vec4(m_map->get_ambient(), 1.0f));
			}
			else
			{
				program->set_parameter(apt_ambient,
					m_main_light_ambient);
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

			lights_count = 1;

			if (lights && (get_global_vars()->get_light_system() ==
				lst_default))
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
			get_state_manager().get_program()->set_parameter(
				apt_terrain_lod_offset,
				terrain_data.get_terrain_lod_offset());

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
				"instance%2% caught exception '%3%'"),
				terrain_data.get_instances() %
				(terrain_data.get_instances() > 1 ? UTF8("s") :
					UTF8("")) %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% terrain "
				"instance%2% caught exception '%3%'"),
				terrain_data.get_instances() %
				(terrain_data.get_instances() > 1 ? UTF8("s") :
					UTF8("")) % exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% terrain "
				"instance%2% caught '%3%'"),
				terrain_data.get_instances() %
				(terrain_data.get_instances() > 1 ? UTF8("s") :
					UTF8("")) % UTF8("unknown error"));
		}
	}

	void Scene::do_draw_object(const ObjectSharedPtr &object,
		const BitSet64 visibility_mask, const EffectProgramType type,
		const Uint16 instances, const Uint16 distance,
		const bool flip_face_culling)
	{
		Uint16 count, i;
		bool object_data_set;

		get_state_manager().switch_mesh(object->get_mesh());

		get_state_manager().switch_flip_back_face_culling(
			flip_face_culling ^ object->get_world_transformation(
				).get_flip_back_face_culling());

		DEBUG_CHECK_GL_ERROR();

		count = object->get_materials().size();

		object_data_set = false;

		DEBUG_CHECK_GL_ERROR();

		for (i = 0; i < count; ++i)
		{
			if (i < visibility_mask.size())
			{
				if (!visibility_mask[i])
				{
					continue;
				}
			}

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
					glm::transpose(object->get_world_transformation(
						).get_matrix()));
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
		const BitSet64 visibility_mask, const EffectProgramType type,
		const Uint16 instances, const Uint16 distance,
		const bool flip_face_culling)
	{
		Uint16 count, i, lights_count;
		bool object_data_set;

		get_state_manager().switch_mesh(object->get_mesh());

		get_state_manager().switch_flip_back_face_culling(
			flip_face_culling ^ object->get_world_transformation(
				).get_flip_back_face_culling());

		DEBUG_CHECK_GL_ERROR();

		count = object->get_materials().size();

		get_lights(object->get_bounding_box(), lights_count);

		object_data_set = false;

		DEBUG_CHECK_GL_ERROR();

		for (i = 0; i < count; ++i)
		{
			if (i < visibility_mask.size())
			{
				if (!visibility_mask[i])
				{
					continue;
				}
			}

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
					glm::transpose(object->get_world_transformation(
						).get_matrix()));
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
		const BitSet64 visibility_mask, const EffectProgramType type,
		const Uint16 instances, const Uint16 distance,
		const bool flip_face_culling)
	{
		try
		{
			STRING_MARKER(UTF8("object name '%1%', mesh name "
				"'%2%', instances %3%, visibility mask %4%, "
				"flip face culling %5%"), object->get_name() %
				object->get_mesh()->get_name() % instances %
				visibility_mask % (flip_face_culling ?
					UTF8("yes") : UTF8("no")));

			do_draw_object_old_lights(object, visibility_mask,
				type, instances, distance, flip_face_culling);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance%2%"
				"of object '%3%' caught exception '%4%'"),
				instances % (instances > 1 ? UTF8("s") :
					UTF8("")) % object->get_name() %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance%2%"
				"of object '%3%' caught exception '%4%'"),
				instances % (instances > 1 ? UTF8("s") :
					UTF8("")) % object->get_name() %
				exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance%2%"
				"of object '%3%' caught '%4%'"), instances %
				(instances > 1 ? UTF8("s") : UTF8("")) %
				object->get_name() % UTF8("unknown error"));
		}
	}

	void Scene::draw_object(const ObjectSharedPtr &object,
		const BitSet64 visibility_mask, const EffectProgramType type,
		const Uint16 instances, const Uint16 distance,
		const bool flip_face_culling)
	{
		try
		{
			STRING_MARKER(UTF8("object name '%1%', mesh name "
				"'%2%', instances %3%, visibility mask %4%, "
				"flip face culling %5%"), object->get_name() %
				object->get_mesh()->get_name() % instances %
				visibility_mask % (flip_face_culling ?
					UTF8("yes") : UTF8("no")));

			do_draw_object(object, visibility_mask, type,
				instances, distance, flip_face_culling);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance%2%"
				"of object '%3%' caught exception '%4%'"),
				instances % (instances > 1 ? UTF8("s") :
					UTF8("")) % object->get_name() %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance%2%"
				"of object '%3%' caught exception '%4%'"),
				instances % (instances > 1 ? UTF8("s") :
					UTF8("")) % object->get_name() %
				exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While rendering %1% instance%2%"
				"of object '%3%' caught '%4%'"), instances %
				(instances > 1 ? UTF8("s") : UTF8("")) %
				object->get_name() % UTF8("unknown error"));
		}
	}

	void Scene::do_update_shadow_map(const Uint16 slice)
	{
		DEBUG_CHECK_GL_ERROR();

		get_state_manager().init();

		if (get_scene_view().get_exponential_shadow_maps())
		{
			m_shadow_frame_buffer->attach(m_shadow_texture,
				fbat_color_0, slice);
		}
		else
		{
			m_shadow_frame_buffer->attach(m_shadow_texture,
				fbat_depth, slice);
		}

		m_shadow_frame_buffer->clear(glm::vec4(1e38f), 0);
		m_shadow_frame_buffer->clear(1.0f, 0);

		DEBUG_CHECK_GL_ERROR();

		STRING_MARKER(UTF8("Updating shadow map slice %1%"), slice);

		get_state_manager().switch_color_mask(glm::bvec4(
			get_scene_view().get_exponential_shadow_maps()));

		get_state_manager().switch_polygon_offset_fill(true);
		glPolygonOffset(1.0f, 32.0f);

		if (!get_scene_view().get_exponential_shadow_maps())
		{
			get_state_manager().switch_flip_back_face_culling(
				false);
		}

		DEBUG_CHECK_GL_ERROR();

		get_scene_view().set_shadow_view(slice);

		if (get_terrain())
		{
			draw_terrain(m_shadow_terrains[slice], ept_shadow,
				false);
		}

		BOOST_FOREACH(const RenderObjectData &object,
			m_shadow_objects[slice].get_objects())
		{
			draw_object(object.get_object(),
				object.get_visibility_mask(), ept_shadow, 1,
				object.get_distance(), false);
		}

		update_program_vars_id();

		m_shadow_frame_buffer->blit_buffers();

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().unbind_all();
	}

	void Scene::update_shadow_map(const Uint16 slice)
	{
		LOG_DEBUG(lt_rendering, UTF8("Updating shadow map slice %1% "
			"%2%"), slice % UTF8("started"));

		try
		{
			do_update_shadow_map(slice);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating shadow map "
				"slice %1% caught exception '%2%'"), slice %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating shadow map "
				"slice %1% caught exception '%2%'"), slice %
				exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While updating shadow map "
				"slice %1% caught '%2%'"), slice %
				UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Updating shadow map slice %1% "
			"%2%"), slice % UTF8("done"));
	}

	void Scene::update_shadow_map()
	{
		StateSetUtil state_set(get_state_manager());
		Uint16 i;

		LOG_DEBUG(lt_rendering, UTF8("Updating shadow map %1%"),
			UTF8("started"));

		try
		{
			DEBUG_CHECK_GL_ERROR();

			m_shadow_frame_buffer->bind();

			m_shadow_frame_buffer->set_view_port();

			get_state_manager().switch_depth_clamp(true);

			for (i = 0; i < get_scene_view(
				).get_shadow_map_count(); ++i)
			{
				update_shadow_map(i);
			}

			unbind_all();

			DEBUG_CHECK_GL_ERROR();

			m_shadow_frame_buffer->unbind();

			DEBUG_CHECK_GL_ERROR();

			get_state_manager().switch_texture(spt_effect_15,
				m_shadow_texture);

			DEBUG_CHECK_GL_ERROR();

			if (get_scene_view().get_exponential_shadow_maps())
			{
				glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
			}

			DEBUG_CHECK_GL_ERROR();
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating shadow map "
				"caught exception '%1%'"),
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating shadow map "
				"caught exception '%1%'"), exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While updating shadow map "
				"caught '%1%'"), UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Updating shadow map %1%"),
			UTF8("done"));
	}

	void Scene::do_draw_depth()
	{
		StateSetUtil state_set(get_state_manager());
		Uint32 index;
		bool no_depth_read;

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("depth"));

		DEBUG_CHECK_GL_ERROR();

		get_scene_view().set_default_view();

		get_state_manager().switch_color_mask(glm::bvec4(false));

		index = 0;

		if (get_terrain())
		{
			draw_terrain(m_visible_terrain, ept_depth, false);
		}

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

			GlQuery query(GL_SAMPLES_PASSED, m_querie_ids[index]);

			draw_object(object.get_object(),
				object.get_visibility_mask(), ept_depth, 1,
				object.get_distance(), false);

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

				GlQuery query(GL_SAMPLES_PASSED,
					m_querie_ids[index]);

				draw_object(object.get_object(),
					object.get_visibility_mask(),
					ept_depth, 1, object.get_distance(),
					false);

				index++;
			}
		}

		unbind_all();

		DEBUG_CHECK_GL_ERROR();

		update_program_vars_id();
	}

	void Scene::draw_depth()
	{
		LOG_DEBUG(lt_rendering, UTF8("Drawing depth %1%"),
			UTF8("started"));

		try
		{
			do_draw_depth();
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While drawing depth "
				"caught exception '%1%'"),
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While drawing depth "
				"caught exception '%1%'"), exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While drawing depth "
				"caught '%1%'"), UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Drawing depth %1%"),
			UTF8("done"));
	}

	void Scene::do_draw_default()
	{
		StateSetUtil state_set(get_state_manager());
		Uint32 index, count;
		EffectProgramType effect;
		BlendType blend;
		bool old_lights;

		STRING_MARKER(UTF8("drawing mode '%1%'"), UTF8("default"));

		DEBUG_CHECK_GL_ERROR();

		get_scene_view().set_default_view();

		get_state_manager().switch_multisample(true);

		if (get_global_vars()->get_use_linear_lighting())
		{
			get_state_manager().switch_framebuffer_sRGB(true);
		}

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

		if (get_terrain())
		{
			draw_terrain(m_visible_terrain, effect, true);
		}

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
					object.get_visibility_mask(), effect,
					1, object.get_distance(), false);
			}
			else
			{
				draw_object(object.get_object(),
					object.get_visibility_mask(), effect,
					1, object.get_distance(), false);
			}
		}

		DEBUG_CHECK_GL_ERROR();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		CHECK_GL_ERROR();

		update_program_vars_id();
		m_frame_id++;
	}

	void Scene::draw_default()
	{
		LOG_DEBUG(lt_rendering, UTF8("Drawing default %1%"),
			UTF8("started"));

		try
		{
			do_draw_default();
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While default depth "
				"caught exception '%1%'"),
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While default depth "
				"caught exception '%1%'"), exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While default depth "
				"caught '%1%'"), UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Drawing default %1%"),
			UTF8("done"));
	}

	void Scene::depth_read()
	{
	}

	void Scene::update_terrain_map(const MaterialSharedPtr &material,
		const Mat2x3Array2 &texture_matrices, const Uint16 index)
	{
		StateSetUtil state_set(get_state_manager());

		m_clipmap_terrain_frame_buffer->bind();
		m_clipmap_terrain_frame_buffer->attach(
			m_clipmap_terrain_texture, fbat_color_0, index);
		m_clipmap_terrain_frame_buffer->clear(glm::vec4(0.0f), 0);

		if (get_global_vars()->get_use_linear_lighting())
		{
			get_state_manager().switch_framebuffer_sRGB(true);
		}

		MaterialLock material_lock(material);

		DEBUG_CHECK_GL_ERROR();

		switch_program(material_lock->get_effect(
			)->get_program(ept_default));

		DEBUG_CHECK_GL_ERROR();

		material_lock->bind(get_state_manager());

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().get_program()->set_parameter(
			apt_texture_matrices, texture_matrices);

		DEBUG_CHECK_GL_ERROR();

		get_state_manager().draw(0, 1);

		DEBUG_CHECK_GL_ERROR();
	}

	void Scene::update_terrain_map(const Uint16 slice)
	{
		Mat2x3Array2 texture_matrices;
		glm::mat2x3 texture_matrix;
		glm::vec2 tile_scale;

		LOG_DEBUG(lt_rendering, UTF8("Updating terrain map slice %1% "
			"%2%"), slice % UTF8("started"));

		try
		{
			STRING_MARKER(UTF8("Updating terrain map slice %1%"),
				slice);

			tile_scale = m_map->get_terrain_size() /
				glm::vec2(4.0f);

			m_clipmap_terrain.update_slice(slice);

			texture_matrices[0] = glm::mat2x3(glm::inverse(glm::mat3(
				m_clipmap_terrain.get_texture_matrices(
					)[slice])));

			texture_matrices[1] = texture_matrices[0];

			texture_matrices[1][0] *= tile_scale.x;
			texture_matrices[1][1] *= tile_scale.y;

			update_terrain_map(
				m_map->get_clipmap_terrain_material(),
				texture_matrices, slice);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain map "
				"slice %1% caught exception '%2%'"), slice %
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain map "
				"slice %1% caught exception '%2%'"), slice %
				exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain map "
				"slice %1% caught '%2%'"), slice %
				UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Updating terrain map slice %1% "
			"%2%"), slice % UTF8("done"));
	}

	void Scene::update_terrain_normal_map(const Uint16 slice)
	{
		StateSetUtil state_set(get_state_manager());

		LOG_DEBUG(lt_rendering, UTF8("Updating terrain normal map "
			"slice %1% %2%"), slice % UTF8("started"));

		try
		{
			STRING_MARKER(UTF8("Updating terrain normal map slice "
				"%1%"), slice);

			m_clipmap_terrain_frame_buffer->bind();
			m_clipmap_terrain_frame_buffer->attach(
				m_clipmap_terrain_normal_texture, fbat_color_0,
				slice);
			m_clipmap_terrain_frame_buffer->clear(glm::vec4(0.0f),
				0);

			MaterialLock material_lock(m_normal_map_material);

			DEBUG_CHECK_GL_ERROR();

			switch_program(material_lock->get_effect(
				)->get_program(ept_default));

			DEBUG_CHECK_GL_ERROR();

			material_lock->bind(get_state_manager());

			get_state_manager().get_program()->set_parameter(
				apt_layers, glm::ivec4(slice));

			DEBUG_CHECK_GL_ERROR();

			get_state_manager().draw(0, 1);

			DEBUG_CHECK_GL_ERROR();
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain normal "
				"map slice %1% caught exception '%2%'"), slice
				% boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain normal "
				"map slice %1% caught exception '%2%'"), slice
				% exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain normal "
				"map slice %1% caught '%2%'"), slice %
				UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Updating terrain normal map "
			"slice %1% %2%"), slice % UTF8("done"));
	}

	void Scene::update_terrain_map()
	{
		StateSetUtil state_set(get_state_manager());
		Mat2x3Array2 texture_matrices;
		glm::mat2x3 texture_matrix;
		Uint32 i, count;

		LOG_DEBUG(lt_rendering, UTF8("Updating terrain map %1%"),
			UTF8("started"));

		try
		{
			DEBUG_CHECK_GL_ERROR();

			m_clipmap_terrain_frame_buffer->set_view_port();

			DEBUG_CHECK_GL_ERROR();

			get_scene_view().set_ortho_view();
			get_state_manager().switch_depth_mask(false);
			get_state_manager().switch_depth_test(false);
			get_state_manager().switch_blend(true);
			get_state_manager().switch_multisample(false);
			get_state_manager().switch_mesh(get_screen_quad());

			count = m_clipmap_terrain.get_slices();

			for (i = 0; i < count; ++i)
			{
				update_terrain_map(i);
			}

			update_program_vars_id();

			DEBUG_CHECK_GL_ERROR();

			unbind_all();

			DEBUG_CHECK_GL_ERROR();

			m_clipmap_terrain_frame_buffer->blit_buffers();
			m_clipmap_terrain_frame_buffer->unbind();

			DEBUG_CHECK_GL_ERROR();

			if (!get_global_vars()->get_opengl_3_0())
			{
				return;
			}

			get_state_manager().switch_texture(spt_effect_0,
				m_clipmap_terrain_texture);

			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

			DEBUG_CHECK_GL_ERROR();

			/**
			 * Build normal map.
			 */
			m_clipmap_terrain_frame_buffer->set_view_port();

			DEBUG_CHECK_GL_ERROR();

			get_scene_view().set_ortho_view();
			get_state_manager().switch_depth_mask(false);
			get_state_manager().switch_depth_test(false);
			get_state_manager().switch_blend(true);
			get_state_manager().switch_multisample(false);
			get_state_manager().switch_mesh(get_screen_quad());

			count = m_clipmap_terrain.get_slices();

			for (i = 0; i < count; ++i)
			{
				update_terrain_normal_map(i);
			}

			update_program_vars_id();

			DEBUG_CHECK_GL_ERROR();

			unbind_all();

			DEBUG_CHECK_GL_ERROR();

			m_clipmap_terrain_frame_buffer->blit_buffers();
			m_clipmap_terrain_frame_buffer->unbind();

			DEBUG_CHECK_GL_ERROR();

			get_state_manager().switch_texture(spt_effect_0,
				m_clipmap_terrain_normal_texture);

			glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

			DEBUG_CHECK_GL_ERROR();
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain map "
				"caught exception '%1%'"),
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain map "
				"caught exception '%1%'"), exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While updating terrain map "
				"caught '%1%'"), UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Updating terrain map %1%"),
			UTF8("done"));
	}

	void Scene::init_light_indexed_deferred_rendering()
	{
		AbstractMeshSharedPtr mesh;
		MaterialSharedPtrVector materials;
		MaterialDescription material_description;
		ObjectData object_data;

		object_data.set_name(String(UTF8("sphere")));
		object_data.set_id(0xFFFFFFFF);
		material_description.set_cast_shadows(false);

		get_scene_resources().get_mesh_cache()->get_mesh(
			object_data.get_name(), mesh);

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

		switch (get_global_vars()->get_light_system())
		{
			case lst_default:
				return;
			case lst_lidr_x4:
				bits.r = (light_index & (0x03 << 0)) << 6;
				bits.g = (light_index & (0x03 << 2)) << 4;
				bits.b = (light_index & (0x03 << 4)) << 2;
				bits.a = (light_index & (0x03 << 6)) << 0;

				color = glm::vec4(bits) / 255.0f;
				break;
			case lst_lidr_x5:
				bits.r = (light_index & (0x03 << 0)) << 8;
				bits.g = (light_index & (0x03 << 2)) << 6;
				bits.b = (light_index & (0x03 << 4)) << 4;
				bits.a = 0;

				color = glm::vec4(bits) / 1023.0f;
				break;
			case lst_lidr_x8:
				bits.r = (light_index & (0x03 << 0)) << 14;
				bits.g = (light_index & (0x03 << 2)) << 12;
				bits.b = (light_index & (0x03 << 4)) << 10;
				bits.a = (light_index & (0x03 << 6)) << 8;

				color = glm::vec4(bits) / 65535.0f;
				break;
		};

		world_transformation.set_scale(glm::vec3(size));
		world_transformation.set_translation(position);
		world_transformation.set_rotation_angles(glm::vec3());

		m_light_sphere->set_world_transformation(world_transformation);

		m_light_sphere->get_materials()[0]->set_color(color);

		STRING_MARKER(UTF8("drawing light %1%, %2%"),
			static_cast<Uint16>(light_index) %
			glm::to_string(color));

		/************/
		glStencilFunc(GL_ALWAYS, light_index, 0xFFFFFFFF);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP); 

		// Disable color writes
		get_state_manager().switch_color_mask(glm::bvec4(false));
		get_state_manager().switch_blend(false);

		// Draw a sphere the radius of the light
		draw_object(m_light_sphere, 0xFFFFFFFF, ept_depth, 1, 0, true);

		// Set the stencil to only pass on equal value
		glStencilFunc(GL_EQUAL, light_index, 0xFFFFFFFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); 
		/************/

		get_state_manager().switch_color_mask(glm::bvec4(true));
		get_state_manager().switch_blend(true);

		//TODO: Render lowers detail spheres when light is far away?
		// Draw a sphere the radius of the light
		draw_object(m_light_sphere, 0xFFFFFFFF, ept_default, 1, 0,
			false);
	}

	void Scene::draw_light_camera_inside(const glm::vec3 &position,
		const float size, const Uint8 light_index)
	{
		Transformation world_transformation;
		glm::mat2x3 emission_scale_offset;
		glm::uvec4 bits;
		glm::vec4 color;

		switch (get_global_vars()->get_light_system())
		{
			case lst_default:
				return;
			case lst_lidr_x4:
				bits.r = (light_index & (0x03 << 0)) << 6;
				bits.g = (light_index & (0x03 << 2)) << 4;
				bits.b = (light_index & (0x03 << 4)) << 2;
				bits.a = (light_index & (0x03 << 6)) << 0;

				color = glm::vec4(bits) / 255.0f;
				break;
			case lst_lidr_x5:
				bits.r = (light_index & (0x03 << 0)) << 8;
				bits.g = (light_index & (0x03 << 2)) << 6;
				bits.b = (light_index & (0x03 << 4)) << 4;
				bits.a = 0;

				color = glm::vec4(bits) / 1023.0f;
				break;
			case lst_lidr_x8:
				bits.r = (light_index & (0x03 << 0)) << 14;
				bits.g = (light_index & (0x03 << 2)) << 12;
				bits.b = (light_index & (0x03 << 4)) << 10;
				bits.a = (light_index & (0x03 << 6)) << 8;

				color = glm::vec4(bits) / 65535.0f;
				break;
		};

		world_transformation.set_scale(glm::vec3(size));
		world_transformation.set_translation(position);
		world_transformation.set_rotation_angles(glm::vec3());

		m_light_sphere->set_world_transformation(world_transformation);

		m_light_sphere->get_materials()[0]->set_color(color);

		STRING_MARKER(UTF8("drawing light %1%, %2%"),
			static_cast<Uint16>(light_index) %
			glm::to_string(color));

		draw_object(m_light_sphere, 0xFFFFFFFF, ept_default, 1, 0,
			true);
	}

	void Scene::draw_lights()
	{
		StateSetUtil state_set(get_state_manager());
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

		switch (get_global_vars()->get_light_system())
		{
			case lst_default:
				return;
			case lst_lidr_x4:
				/**
				 * Set the constant blend color to bit shift
				 * 2 bits down on each call
				 */
				glBlendColor(0.251f, 0.251f, 0.251f, 0.251f);
				break;
			case lst_lidr_x5:
				/**
				 * Set the constant blend color to bit shift
				 * 2 bits down on each call
				 */
				glBlendColor(0.25025f, 0.25025f, 0.25025f,
					0.25025f);
				break;
			case lst_lidr_x8:
				/**
				 * Set the constant blend color to bit shift
				 * 2 bits down on each call
				 */
				glBlendColor(0.25f, 0.25f, 0.25f, 0.25f);
				break;
		};

		get_state_manager().switch_stencil_test(true);
		get_state_manager().switch_depth_clamp(true);
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

		get_state_manager().switch_stencil_test(false);
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

		if (m_scene_frame_buffer.get() != nullptr)
		{
			draw_lights();
		}

		if (get_scene_view().get_shadow_map_count() > 0)
		{
			update_shadow_map();
		}

		if (get_terrain())
		{
			if (m_clipmap_terrain.update(glm::vec3(
				get_scene_view().get_camera()),
				glm::vec3(get_scene_view().get_view_dir()),
				glm::vec2(get_scene_view().get_focus())))
			{
				update_terrain_map();
			}
		}

		if (get_scene_view().get_shadow_map_count() > 0)
		{
			get_state_manager().switch_texture(spt_effect_15,
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

			if (get_global_vars()->get_light_system() !=
				lst_default)
			{
				get_state_manager().switch_texture(
					spt_effect_12,
					m_light_position_texture);
				get_state_manager().switch_texture(
					spt_effect_13,
					m_light_color_texture);
				get_state_manager().switch_texture(
					spt_effect_14,
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

			GlQuery query(GL_SAMPLES_PASSED,
				m_querie_ids[query_index]);

			draw_object(object.get_object(),
				object.get_visibility_mask(), ept_depth, 1,
				object.get_distance(), false);

			ids.push_back(data);
			query_index++;

			return;
		}

		get_state_manager().switch_mesh(
			object.get_object()->get_mesh());

		get_state_manager().switch_flip_back_face_culling(
			object.get_object()->get_world_transformation(
				).get_flip_back_face_culling());

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

			GlQuery query(GL_SAMPLES_PASSED,
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
					glm::transpose(object.get_object(
						)->get_world_transformation(
						).get_matrix()));
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
			get_scene_resources().get_effect_cache(),
			get_scene_resources().get_mesh_builder(),
			get_scene_resources().get_mesh_cache(),
			get_scene_resources().get_mesh_data_cache(),
			get_scene_resources().get_material_cache(),
			get_scene_resources().get_material_description_cache(),
			get_scene_resources().get_terrain_builder(),
			get_scene_resources().get_texture_cache(),
			m_free_ids));

		set_map(map_loader->load(name));
	}

	void Scene::map_changed()
	{
		Uint32 i, count;

		rebuild_terrain_map();
		rebuild_shadow_map();

		update_light_system();

		get_map()->init_terrain_rendering_data(m_visible_terrain);

		count = m_shadow_terrains.size();

		for (i = 0; i < count; ++i)
		{
			get_map()->init_terrain_rendering_data(
				m_shadow_terrains[i]);
		}
	}

	void Scene::update_light_system()
	{
		ImageSharedPtr light_position_image;
		ImageSharedPtr light_color_image;
		Uint32LightSharedPtrMap::const_iterator it, end;
		glm::vec4 position, color;
		Uint32 index, count;
		TextureFormatType color_format;

		if (get_global_vars()->get_opengl_3_0())
		{
			color_format = tft_rgb9_e5;
		}
		else
		{
			color_format = tft_rgb8;
		}

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
			case lst_lidr_x8:
				count = 256;
				break;
		};

		m_light_position_texture = boost::make_shared<Texture>(
			String(UTF8("light position")), /* width */ count,
			/* height */ 0, /* depth */ 0, /* mipmaps */ 0,
			/* samples */ 0, tft_rgba32f, ttt_texture_1d);

		m_light_position_texture->set_wrap_s(twt_clamp);
		m_light_position_texture->set_wrap_t(twt_clamp);
		m_light_position_texture->set_wrap_r(twt_clamp);
		m_light_position_texture->set_mag_filter(tft_nearest);
		m_light_position_texture->set_min_filter(tft_nearest);
		m_light_position_texture->set_mipmap_filter(tmt_none);

		m_light_color_texture = boost::make_shared<Texture>(
			String(UTF8("light color")), /* width */ count,
			/* height */ 0, /* depth */ 0, /* mipmaps */ 0,
			/* samples */ 0, color_format, ttt_texture_1d);

		m_light_color_texture->set_wrap_s(twt_clamp);
		m_light_color_texture->set_wrap_t(twt_clamp);
		m_light_color_texture->set_wrap_r(twt_clamp);
		m_light_color_texture->set_mag_filter(tft_nearest);
		m_light_color_texture->set_min_filter(tft_nearest);
		m_light_color_texture->set_mipmap_filter(tmt_none);

		light_position_image = boost::make_shared<Image>(
			String(UTF8("light position")), false, tft_rgba32f,
			glm::uvec3(count, 0, 0), 0);

		light_color_image = boost::make_shared<Image>(
			String(UTF8("light color")), false, color_format,
			glm::uvec3(count, 0, 0), 0);

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

		build_light_index_map();
	}

	void Scene::build_light_index_map()
	{
		TextureFormatType format;

		LOG_DEBUG(lt_rendering, UTF8("Building light index map %1%"),
			UTF8("started"));

		try
		{
			switch (get_global_vars()->get_light_system())
			{
				case lst_default:
					m_light_index_texture.reset();
					return;
				case lst_lidr_x4:
					format = tft_rgba8;
					break;
				case lst_lidr_x5:
					format = tft_rgb10_a2;
					break;
				case lst_lidr_x8:
					format = tft_rgba16;
					break;
			};

			m_light_index_texture = boost::make_shared<Texture>(
				String(UTF8("light index")), get_view_port().z,
				get_view_port().w, 1, 0, 0, format,
				ttt_texture_2d);

			m_light_index_texture->set_wrap_s(twt_clamp);
			m_light_index_texture->set_wrap_t(twt_clamp);
			m_light_index_texture->set_wrap_r(twt_clamp);
			m_light_index_texture->set_mag_filter(tft_nearest);
			m_light_index_texture->set_min_filter(tft_nearest);
			m_light_index_texture->set_mipmap_filter(tmt_none);
			m_light_index_texture->init(get_view_port().z,
				get_view_port().w, 0, 0);
		}
		catch (boost::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While building light index map "
				"caught exception '%1%'"),
				boost::diagnostic_information(exception));
		}
		catch (std::exception &exception)
		{
			LOG_EXCEPTION_STR(UTF8("While building light index map "
				"caught exception '%1%'"), exception.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_STR(UTF8("While building light index map "
				"caught '%1%'"), UTF8("unknown error"));
		}

		LOG_DEBUG(lt_rendering, UTF8("Building light index map %1%"),
			UTF8("done"));
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

		m_scene_frame_buffer->blit_buffers();
		m_scene_frame_buffer->blit_to_back_buffer(rect, 0, true,
			true, false);
		m_scene_frame_buffer->unbind();
	}

	void Scene::set_view_port(const glm::uvec4 &view_port)
	{
		TextureFormatType texture_format;
		Uint16 mipmaps;

		m_cpu_rasterizer = boost::make_shared<CpuRasterizer>(
			view_port.y, view_port.w, 5.0f,
			get_global_vars()->get_use_simd());

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

		while ((1u << mipmaps) <
			std::max(view_port.z, view_port.w))
		{
			mipmaps++;
		}

		m_scene_frame_buffer = get_scene_resources(
			).get_framebuffer_builder()->build(
				String(UTF8("scene")), view_port.z,
				view_port.w, 0, true);

		if (get_global_vars()->get_use_linear_lighting())
		{
			texture_format = tft_srgb8_a8;
		}
		else
		{
			texture_format = tft_rgba8;
		}

		m_scene_texture = boost::make_shared<Texture>(
			String(UTF8("scene")), view_port.z, view_port.w, 1,
			mipmaps, 0, texture_format, ttt_texture_2d);

		m_scene_texture->set_wrap_s(twt_clamp);
		m_scene_texture->set_wrap_t(twt_clamp);
		m_scene_texture->set_wrap_r(twt_clamp);
		m_scene_texture->init(view_port.z, view_port.w, 0,
			mipmaps);

		m_scene_frame_buffer->bind();
		m_scene_frame_buffer->attach(m_scene_texture,
			fbat_color_0, 0);
		m_scene_frame_buffer->set_draw_buffer(0, true);
		m_scene_frame_buffer->unbind();

		build_light_index_map();

		get_scene_view().set_view_port(view_port);
	}

	void Scene::set_view_port()
	{
		glViewport(get_view_port()[0], get_view_port()[1],
			get_view_port()[2], get_view_port()[3]);
	}

	const glm::uvec2 &Scene::get_map_size() const
	{
		return get_map()->get_size();
	}

}
