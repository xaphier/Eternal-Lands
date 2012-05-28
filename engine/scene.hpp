/****************************************************************************
 *            scene.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
#include "clipmap.hpp"
#include "effectprogramutil.hpp"

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
			Clipmap m_clipmap;
			StateManager m_state_manager;
			ObjectVisitor m_visible_objects;
			ObjectVisitor m_shadow_objects;
			LightVisitor m_visible_lights;
			MapSharedPtr m_map;
			AbstractFrameBufferSharedPtr m_shadow_frame_buffer;
			AbstractMeshSharedPtr m_screen_quad;
			AbstractFrameBufferSharedPtr m_clipmap_frame_buffer;
			Uint32ActorSharedPtrMap m_actors;
			Vec4Vector m_light_position_array;
			Vec4Vector m_light_color_array;
			SceneView m_scene_view;
			glm::vec4 m_main_light_direction;
			glm::vec4 m_main_light_color;
			glm::vec4 m_main_light_ambient;
			glm::vec4 m_fog;
			FreeIdsManagerSharedPtr m_free_ids;
			Uint64 m_frame_id;
			Uint64 m_program_vars_id;
			float m_time;
			SubFrustumsMask m_shadow_objects_mask;
			SubFrustumsMask m_shadow_update_mask;
			bool m_lights;
			bool m_shadow_map_change;

			void get_lights(const BoundingBox &bounding_box,
				Uint16 &light_count);
			void do_draw_object(const ObjectSharedPtr &object,
				const EffectProgramType type,
				const Uint16 instances, const Uint16 distance,
				const bool lights);
			void draw_object(const ObjectSharedPtr &object,
				const EffectProgramType type,
				const Uint16 instances, const Uint16 distance,
				const bool lights);
			void pick_object(const RenderObjectData &object,
				PairUint32SelectionTypeVector &ids);
			bool switch_program(
				const GlslProgramSharedPtr &program);
			void cull_shadows(const glm::vec3 &camera,
				const Uint16 index);
			void cull_all_shadows();
			void draw_shadows(const Uint16 index);
			void draw_shadows_array(const Uint16 index);
			void draw_all_shadows_array();
			void draw_depth();
			void draw_default();
			void update_shadow_map();
			void update_terrain_map();
			void update_terrain_texture(
				const MaterialSharedPtrVector &materials,
				const Mat2x3Array2 &texture_matrices,
				const Uint16 index);
			void update_terrain_texture(const Uint16 slice);
			void update_terrain_texture();

		protected:
			virtual void intersect(const Frustum &frustum,
				ObjectVisitor &visitor) const;
			virtual void intersect(const Frustum &frustum,
				LightVisitor &visitor) const;
			virtual void intersect_shadow(const Frustum &frustum,
				ObjectVisitor &visitor) const;

			inline void set_map(const MapSharedPtr &map) noexcept
			{
				m_map = map;
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
			void init();
			void cull();
			Uint32 pick(const glm::vec2 &offset,
				const glm::vec2 &size,
				SelectionType &selection);
			void load(const String &name);
			const ParticleDataVector &get_particles() const;
			Uint16 get_height(const Uint16 x, const Uint16 y) const;
			Uint16 get_tile(const Uint16 x, const Uint16 y) const;
			glm::uvec2 get_height_map_size() const;
			glm::uvec2 get_tile_map_size() const;
			bool get_dungeon() const;
			void set_ambient(const glm::vec3 &color);
			const glm::vec3 &get_ambient() const;

			inline void terrain_change()
			{
				update_terrain_map();
			}

			inline void shadow_map_change() noexcept
			{
				m_shadow_map_change = true;
			}

			inline bool unbind_all()
			{
				return m_state_manager.unbind_all();
			}

			inline void set_perspective(const float fov,
				const float aspect, const float z_near,
				const float z_far) noexcept
			{
				m_scene_view.set_perspective(fov, aspect,
					z_near);
			}

			inline void set_view_port(const glm::uvec4 &view_port)
				noexcept
			{
				m_scene_view.set_view_port(view_port);
			}

			inline void set_window_size(
				const glm::uvec2 &window_size) noexcept
			{
				m_scene_view.set_window_size(window_size);
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
