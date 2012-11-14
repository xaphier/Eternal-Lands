/****************************************************************************
 *            scene.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a27e8c66_665a_4294_afa3_cca7b7b31054
#define	UUID_a27e8c66_665a_4294_afa3_cca7b7b31054

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "lightvisitor.hpp"
#include "objectvisitor.hpp"
#include "objectdata.hpp"
#include "sceneresources.hpp"
#include "statemanager.hpp"
#include "sceneview.hpp"
#include "clipmapterrain.hpp"
#include "effect/effectprogramutil.hpp"
#include "terrainrenderingdata.hpp"

/**
 * @file
 * @brief The @c class Scene.
 * This file contains the @c class Scene.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for objects.
	 *
	 * @c class for objects.
	 */
	class Scene
	{
		private:
			typedef std::pair<Uint32, SelectionType>
				PairUint32SelectionType;
			typedef std::vector<PairUint32SelectionType>
				PairUint32SelectionTypeVector;

			boost::array<GLuint, 0x40000> m_querie_ids;
			GlobalVarsSharedPtr m_global_vars;
			FileSystemSharedPtr m_file_system;
			SceneResources m_scene_resources;
			ClipmapTerrain m_clipmap_terrain;
			StateManager m_state_manager;
			TerrainRenderingData m_visible_terrain;
			ObjectVisitor m_visible_objects;
			boost::array<TerrainRenderingData, 4> m_shadow_terrains;
			boost::array<ObjectVisitor, 4> m_shadow_objects;
			LightVisitor m_visible_lights;
			MapSharedPtr m_map;
			AbstractMeshSharedPtr m_screen_quad;
			AbstractFrameBufferSharedPtr m_shadow_frame_buffer;
			AbstractFrameBufferSharedPtr m_clipmap_terrain_frame_buffer;
			AbstractFrameBufferSharedPtr m_scene_frame_buffer;
			TextureSharedPtr m_shadow_texture;
			TextureSharedPtr m_clipmap_terrain_texture;
			TextureSharedPtr m_light_index_texture;
			TextureSharedPtr m_light_position_texture;
			TextureSharedPtr m_light_color_texture;
			TextureSharedPtr m_scene_texture;
			Uint32Uint32Map m_light_index;
			ObjectSharedPtr m_light_sphere;
			Uint32ActorSharedPtrMap m_actors;
			Vec4Vector m_light_positions_array;
			Vec4Vector m_light_colors_array;
			SceneView m_scene_view;
			CpuRasterizerSharedPtr m_cpu_rasterizer;
			glm::vec4 m_main_light_direction;
			glm::vec4 m_main_light_color;
			glm::vec4 m_main_light_ambient;
			glm::vec4 m_fog;
			FreeIdsManagerSharedPtr m_free_ids;
			Uint64 m_frame_id;
			Uint64 m_program_vars_id;
			float m_time;
			bool m_lights;
			bool m_rebuild_terrain_map;
			bool m_rebuild_shadow_map;

			void get_lights(const BoundingBox &bounding_box,
				Uint16 &lights_count);
			void do_draw_object_old_lights(
				const ObjectSharedPtr &object,
				const BitSet64 visibility_mask,
				const EffectProgramType type,
				const Uint16 instances, const Uint16 distance);
			void do_draw_object(const ObjectSharedPtr &object,
				const BitSet64 visibility_mask,
				const EffectProgramType type,
				const Uint16 instances, const Uint16 distance);
			void draw_light(const glm::vec3 &position,
				const float size, const Uint8 light_index);
			void draw_light_camera_inside(const glm::vec3 &position,
				const float size, const Uint8 light_index);
			void pick_object(const RenderObjectData &object,
				PairUint32SelectionTypeVector &ids,
				Uint32 &query_index);
			void update_terrain_texture(
				const MaterialSharedPtr &material,
				const Mat2x3Array2 &texture_matrices,
				const Uint16 index);
			void update_terrain_texture(const Uint16 slice);
			void update_terrain_texture();
			void build_terrain_map();
			void build_shadow_map();
			void draw_shadow(const Uint16 index);
			void draw_shadows();
			void draw_depth();
			void draw_lights();
			void init_light_indexed_deferred_rendering();
			void update_light_index_texture();

		protected:
			virtual void intersect_terrain(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const;
			virtual void intersect_terrain(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const;
			virtual void intersect(const Frustum &frustum,
				const bool shadow, ObjectVisitor &visitor)
				const;
			virtual void intersect(const Frustum &frustum,
				LightVisitor &visitor) const;
			virtual void depth_read();
			virtual void map_changed();
			bool switch_program(
				const GlslProgramSharedPtr &program);
			void cull(const Frustum &frustum,
				const glm::mat4 &projection_view_matrix,
				const glm::vec3 &camera, const bool shadow,
				ObjectVisitor &objects) const;
			void cull_terrain(const Frustum &frustum,
				const AbstractWriteMemorySharedPtr &buffer,
				const glm::vec3 &camera, const Uint64 offset,
				const Uint16 max_instances,
				TerrainRenderingData &terrain_data) const;
			void draw_terrain(
				const TerrainRenderingData &terrain_data,
				const EffectProgramType type,
				const bool lights);
			void draw_object(const ObjectSharedPtr &object,
				const BitSet64 visibility_mask,
				const EffectProgramType type,
				const Uint16 instances, const Uint16 distance);
			void draw_object_old_lights(
				const ObjectSharedPtr &object,
				const BitSet64 visibility_mask,
				const EffectProgramType type,
				const Uint16 instances, const Uint16 distance);
			void set_view_port();
			void draw_default();

			inline void update_program_vars_id() noexcept
			{
				m_program_vars_id++;
			}

			inline SceneView &get_scene_view() noexcept
			{
				return m_scene_view;
			}

			inline const SceneView &get_scene_view() const noexcept
			{
				return m_scene_view;
			}

			inline StateManager &get_state_manager() noexcept
			{
				return m_state_manager;
			}

			inline const StateManager &get_state_manager() const
				noexcept
			{
				return m_state_manager;
			}

			inline const AbstractMeshSharedPtr &get_screen_quad()
				const noexcept
			{
				return m_screen_quad;
			}

			inline const MapSharedPtr &get_map() const noexcept
			{
				return m_map;
			}

			inline void set_map(const MapSharedPtr &map)
			{
				m_map = map;
				map_changed();
			}

		public:
			/**
			 * Default constructor.
			 */
			Scene(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);

			/**
			 * Default destructor.
			 */
			virtual ~Scene() noexcept;

			ActorSharedPtr add_actor(const Uint32 type_id,
				const Uint32 id, const Uint32 index,
				const String &name,
				const SelectionType selection,
				const bool enhanced_actor);
			void remove_actor(const Uint32 id);
			void remove_all_actors();
			void add_object(
				const ObjectDescription &object_description);
			void add_object(const ObjectData &object_data,
				const MaterialSharedPtrVector &materials);
			void add_object(const InstanceData &instance_data);
			void remove_object(const Uint32 id);
			bool get_object_position(const Uint32 id,
				glm::vec3 &position);
			virtual void add_light(const LightData &light_data);
			virtual void remove_light(const Uint32 id);
			void set_fog(const glm::vec3 &color,
				const float density);
			void clear();
			void draw();
			void blit_to_back_buffer();
			void cull();
			Uint32 pick(const glm::vec2 &offset,
				const glm::vec2 &size,
				SelectionType &selection);
			void select_rect(const glm::vec2 &min,
				const glm::vec2 &max, Uint32Set &selections);
			double get_depth(const glm::uvec2 &offset);
			void load(const String &name);
			const ParticleDataVector &get_particles() const;
			Uint16 get_height(const Uint16 x, const Uint16 y) const;
			Uint16 get_tile(const Uint16 x, const Uint16 y) const;
			glm::uvec2 get_walk_height_map_size() const;
			glm::uvec2 get_height_map_size() const;
			glm::uvec2 get_tile_map_size() const;
			void set_dungeon(const bool dungeon);
			bool get_dungeon() const;
			void set_ambient(const glm::vec3 &color);
			const glm::vec3 &get_ambient() const;
			float get_walk_height(const Uint16 x, const Uint16 y)
				const;
			bool get_terrain() const;
			void set_view_port(const glm::uvec4 &view_port);
			void update_light_system();
			const glm::uvec2 &get_map_size() const;

			inline const glm::uvec4 &get_view_port() const noexcept
			{
				return m_scene_view.get_view_port();
			}

			inline void rebuild_terrain_map() noexcept
			{
				m_rebuild_terrain_map = true;
			}

			inline void rebuild_shadow_map() noexcept
			{
				m_rebuild_shadow_map = true;
			}

			inline bool unbind_all()
			{
				return m_state_manager.unbind_all();
			}

			inline void set_z_near(const float z_near) noexcept
			{
				m_scene_view.set_z_near(z_near);
			}

			inline void set_perspective(const float fov,
				const float aspect) noexcept
			{
				m_scene_view.set_perspective(fov, aspect);
			}

			inline void set_ortho(const glm::vec4 &ortho)
			{
				m_scene_view.set_ortho(ortho);
			}

			inline float get_fov() const noexcept
			{
				return m_scene_view.get_fov();
			}

			inline float get_aspect() const noexcept
			{
				return m_scene_view.get_aspect();
			}

			inline float get_z_near() const noexcept
			{
				return m_scene_view.get_z_near();
			}

			inline void set_main_light_ambient(
				const glm::vec3 &main_light_ambient) noexcept
			{
				m_main_light_ambient =
					glm::vec4(main_light_ambient, 0.0f);
			}

			inline const glm::vec4 &get_main_light_ambient() const
				noexcept
			{
				return m_main_light_ambient;
			}

			inline void set_main_light_color(
				const glm::vec3 &main_light_color) noexcept
			{
				m_main_light_color =
					glm::vec4(main_light_color, 0.0f);
			}

			inline const glm::vec4 &get_main_light_color() const
				noexcept
			{
				return m_main_light_color;
			}

			inline void set_main_light_direction(
				const glm::vec3 &main_light_direction) noexcept
			{
				m_main_light_direction = glm::vec4(
					glm::normalize(main_light_direction),
						0.0f);
			}

			inline const glm::vec4 &get_main_light_direction()
				const noexcept
			{
				return m_main_light_direction;
			}

			inline void set_focus(const glm::vec3 &focus) noexcept
			{
				m_scene_view.set_focus(focus);
			}

			inline void set_view_matrix(
				const glm::mat4 &view_matrix) noexcept
			{
				m_scene_view.set_view_matrix(view_matrix);
			}

			inline const glm::mat4 &get_projection_matrix() const
			{
				return m_scene_view.get_projection_matrix();
			}

			inline void set_lights(const bool lights) noexcept
			{
				m_lights = lights;
			}

			inline bool get_lights() const noexcept
			{
				return m_lights;
			}

			inline Uint64 get_frame_id() const noexcept
			{
				return m_frame_id;
			}

			inline SceneResources &get_scene_resources() noexcept
			{
				return m_scene_resources;
			}

			inline const FreeIdsManagerSharedPtr &get_free_ids()
				const noexcept
			{
				return m_free_ids;
			}

			inline const GlobalVarsSharedPtr &get_global_vars()
				const noexcept
			{
				return m_global_vars;
			}

			inline const FileSystemSharedPtr &get_file_system()
				const noexcept
			{
				return m_file_system;
			}

	};

}

#endif	/* UUID_a27e8c66_665a_4294_afa3_cca7b7b31054 */
