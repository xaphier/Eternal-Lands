/****************************************************************************
 *            scene.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a27e8c66_665a_4294_afa3_cca7b7b31054
#define	UUID_a27e8c66_665a_4294_afa3_cca7b7b31054

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "rstartreelightvisitor.hpp"
#include "rstartreeobjectvisitor.hpp"
#include "objectdata.hpp"
#include "sceneresources.hpp"
#include "statemanager.hpp"
#include "sceneview.hpp"

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

			GlobalVarsSharedPtr m_global_vars;
			SceneResources m_scene_resources;
			StateManager m_state_manager;
			RStarTreeObjectVisitor m_visible_objects;
			RStarTreeObjectVisitor m_shadow_objects;
			RStarTreeLightVisitor m_visible_lights;
			boost::scoped_ptr<RStarTree> m_object_tree;
			boost::scoped_ptr<RStarTree> m_light_tree;
			AbstractFrameBufferSharedPtr m_shadow_frame_buffer;
			AbstractFrameBufferSharedPtr
				m_shadow_filter_frame_buffer;
			Uint32ActorSharedPtrMap m_actors;
			Uint32ObjectSharedPtrMap m_objects;
			Uint32LightSharedPtrMap m_lights;
			Vec4Vector m_light_position_array;
			Vec4Vector m_light_color_array;
			SceneView m_scene_view;
			glm::vec4 m_ambient;
			glm::vec4 m_main_light_direction;
			glm::vec4 m_main_light_color;
			glm::vec4 m_main_light_ambient;
			glm::vec4 m_fog;
			String m_name;
			Uint64 m_frame_id;
			Uint64 m_program_vars_id;
			Uint32 m_id;
			float m_time;
			SubFrustumsMask m_shadow_objects_mask;
			bool m_dungeon;
			bool m_night;
			bool m_shadow_map_change;

			void get_lights(const BoundingBox &bounding_box,
				Uint32 &light_count);
			void set_lights(const GlslProgramSharedPtr &program,
				const glm::ivec3 &dynamic_light_count,
				const glm::vec4 &color);
			void draw_object(const ObjectSharedPtr &object);
			void draw_object_depth(const ObjectSharedPtr &object);
			void draw_sub_object_depth(
				const ObjectSharedPtr &object,
				const SubObject &sub_object);
			void draw_object_shadow(const ObjectSharedPtr &object);
			void pick_object(const ObjectSharedPtr &object,
				PairUint32SelectionTypeVector &ids,
				const glm::vec2 &min, const glm::vec2 &max);
			bool switch_program(
				const GlslProgramSharedPtr &program);
			void cull_shadows(const glm::vec3 &camera,
				const Uint16 index);
			void cull_all_shadows();
			void draw_shadows(const Uint16 index);
			void draw_shadows_array(const Uint16 index);
			void draw_all_shadows();
			void draw_all_shadows_array();
			void update_shadow_map();

			inline GlobalVarsSharedPtr get_global_vars() const
			{
				return m_global_vars;
			}

		public:
			/**
			 * Default constructor.
			 */
			Scene(const GlobalVarsSharedPtr &global_vars,
				const FileSystemWeakPtr &file_system);

			/**
			 * Default destructor.
			 */
			~Scene() throw();

			ActorSharedPtr add_actor(const Uint32 type_id,
				const Uint32 id, const String &name,
				const SelectionType selection,
				const bool enhanced_actor);
			void remove_actor(const Uint32 id);
			void remove_all_actors();
			void add_object(const ObjectData &object_data);
			void add_object(const InstanceData &instance_data);
			void remove_object(const Uint32 id);
			bool get_object_position(const Uint32 id,
				glm::vec3 &position);
			void add_light(const glm::vec3 &position,
				const glm::vec3 &color, const float ambient,
				const float radius, const Uint32 id);
			void remove_light(const Uint32 id);
			void set_fog(const glm::vec3 &color,
				const float density);
			void clear();
			void draw();
			void init();
			void cull();
			void load(const String &name, const glm::vec3 &ambient,
				const bool dungeon);
			Uint32 pick(const glm::vec2 &offset,
				const glm::vec2 &size,
				SelectionType &selection);

			inline void shadow_map_change()
			{
				m_shadow_map_change = true;
			}

			inline bool unbind_all()
			{
				return m_state_manager.unbind_all();
			}

			inline void set_perspective(const float fov,
				const float aspect, const float z_near,
				const float z_far)
			{
				m_scene_view.set_perspective(fov, aspect,
					z_near);
			}

			inline float get_fov() const
			{
				return m_scene_view.get_fov();
			}

			inline float get_aspect() const
			{
				return m_scene_view.get_aspect();
			}

			inline float get_z_near() const
			{
				return m_scene_view.get_z_near();
			}

			inline void set_main_light_ambient(
				const glm::vec3 &main_light_ambient)
			{
				m_main_light_ambient =
					glm::vec4(main_light_ambient, 0.0f);
			}

			inline const glm::vec4 &get_main_light_ambient() const
			{
				return m_main_light_ambient;
			}

			inline void set_main_light_color(
				const glm::vec3 &main_light_color)
			{
				m_main_light_color =
					glm::vec4(main_light_color, 0.0f);
			}

			inline const glm::vec4 &get_main_light_color() const
			{
				return m_main_light_color;
			}

			inline void set_main_light_direction(
				const glm::vec3 &main_light_direction)
			{
				m_main_light_direction = glm::vec4(
					glm::normalize(main_light_direction),
						0.0f);
			}

			inline const glm::vec4 &get_main_light_direction()
				const
			{
				return m_main_light_direction;
			}

			inline void set_focus(const glm::vec3 &focus)
			{
				m_scene_view.set_focus(focus);
			}

			inline void set_ambient(const glm::vec3 &ambient)
			{
				m_ambient = glm::vec4(ambient, 0.0f);
			}

			inline const glm::vec4 &get_ambient() const
			{
				return m_ambient;
			}

			inline void set_dungeon(const bool dungeon)
			{
				m_dungeon = dungeon;
			}

			inline bool get_dungeon() const
			{
				return m_dungeon;
			}

			inline void set_night(const bool night)
			{
				m_night = night;
			}

			inline bool get_night() const
			{
				return m_night;
			}

			inline const String &get_name() const
			{
				return m_name;
			}

			inline Uint32 get_id() const
			{
				return m_id;
			}

			inline SceneResources &get_scene_resources()
			{
				return m_scene_resources;
			}

	};

}

#endif	/* UUID_a27e8c66_665a_4294_afa3_cca7b7b31054 */
