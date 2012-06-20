/****************************************************************************
 *            editorscene.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_bd1cf1bf_f1c7_4ef9_914f_843f4df019e8
#define	UUID_bd1cf1bf_f1c7_4ef9_914f_843f4df019e8

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "scene.hpp"
#include "renderobjectdata.hpp"

/**
 * @file
 * @brief The @c class EditorScene.
 * This file contains the @c class EditorScene.
 */
namespace eternal_lands
{

	class EditorMapData;

	/**
	 * @brief @c class for scenes used in the editor.
	 *
	 * @c class for scenes used in the editor.
	 */
	class EditorScene: public Scene
	{
		private:
			Uint32ObjectSharedPtrMap m_light_objects;
			Uint32ObjectSharedPtrMap m_light_sphere_objects;
			glm::uvec2 m_depth_selection;
			double m_depth;
			Uint32 m_selected_object;
			bool m_draw_objects;
			bool m_draw_terrain;
			bool m_draw_lights;
			bool m_draw_light_spheres;

		protected:
			virtual void depth_read();
			virtual void intersect_terrain(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const;
			virtual void intersect_terrain(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const;
			virtual void intersect(const Frustum &frustum,
				const bool shadow, ObjectVisitor &visitor)
				const;

		public:
			/**
			 * Default constructor.
			 */
			EditorScene(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);

			/**
			 * Default destructor.
			 */
			virtual ~EditorScene() throw();
			virtual void add_light(const LightData &light_data);
			virtual void remove_light(const Uint32 id);
			void load_map(const String &name, EditorMapData &data);
			void set_terrain(const ImageSharedPtr &vector_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map);

			inline void deselect_object() noexcept
			{
				set_selected_object(
					std::numeric_limits<Uint32>::max());
			}

			inline const glm::uvec2 &get_depth_selection() const
				noexcept
			{
				return m_depth_selection;
			}

			inline double get_depth() const noexcept
			{
				return m_depth;
			}

			inline Uint32 get_selected_object() const noexcept
			{
				return m_selected_object;
			}

			inline void set_depth_selection(
				const glm::uvec2 &depth_selection) noexcept
			{
				m_depth_selection = depth_selection;
			}

			inline void set_selected_object(
				const Uint32 selected_object) noexcept
			{
				m_selected_object = selected_object;
			}

			inline bool get_draw_objects() const noexcept
			{
				return m_draw_objects;
			}

			inline bool get_draw_terrain() const noexcept
			{
				return m_draw_terrain;
			}

			inline bool get_draw_lights() const noexcept
			{
				return m_draw_lights;
			}

			inline bool get_draw_light_spheres() const noexcept
			{
				return m_draw_light_spheres;
			}

			inline void set_draw_objects(const bool draw_objects)
				noexcept
			{
				m_draw_objects = draw_objects;
			}

			inline void set_draw_terrain(const bool draw_terrain)
				noexcept
			{
				m_draw_terrain = draw_terrain;
			}

			inline void set_draw_lights(const bool draw_lights)
				noexcept
			{
				m_draw_lights = draw_lights;
			}

			inline void set_draw_light_spheres(
				const bool draw_light_spheres) noexcept
			{
				m_draw_light_spheres = draw_light_spheres;
			}

	};

}

#endif	/* UUID_bd1cf1bf_f1c7_4ef9_914f_843f4df019e8 */
