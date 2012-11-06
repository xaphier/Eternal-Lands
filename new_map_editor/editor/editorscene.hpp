/****************************************************************************
 *            editorscene.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "abstractmaploader.hpp"

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
			Uint32ObjectSharedPtrMap m_heights;
			boost::scoped_ptr<RStarTree> m_height_tree;
			MaterialSharedPtr m_selection_material;
			TerrainRenderingData m_top_down_terrain;
			ObjectVisitor m_top_down_objects;
			glm::uvec2 m_depth_selection;
			double m_depth;
			Uint32 m_selected_object;
			bool m_draw_objects;
			bool m_draw_terrain;
			bool m_draw_lights;
			bool m_draw_light_spheres;
			bool m_draw_heights;
			GLuint m_querie_id;

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
			void load_map(const String &name, EditorMapData &data,
				const MapItemsTypeSet &skip_items);
			void set_terrain_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map);
			void set_terrain_blend_map(
				const ImageSharedPtr &blend_map);
			void set_terrain_material(
				const StringVector &albedo_maps,
				const StringVector &extra_maps,
				const TerrainMaterialData &material_data);
			void set_terrain_dudv_scale_offset(
				const glm::vec4 &dudv_scale_offset);
			void update_terrain_geometry_maps(
				const ImageUpdate &displacement_map,
				const ImageUpdate &normal_map,
				const ImageUpdate &dudv_map);
			void update_terrain_blend_map(
				const ImageUpdate &blend_map);
			void draw_selection(const glm::uvec4 &selection_rect);
			void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height);
			void cull_map();
			void draw_scale_view(const bool depth_only);
			void set_terrain(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const ImageSharedPtr &blend_map,
				const StringVector &albedo_maps,
				const StringVector &extra_maps,
				const TerrainMaterialData &material_data,
				const glm::vec4 &dudv_scale_offset);
			glm::vec3 get_map_center() const;

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

			inline bool get_draw_heights() const noexcept
			{
				return m_draw_heights;
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

			inline void set_draw_heights(const bool draw_heights)
				noexcept
			{
				m_draw_heights = draw_heights;
			}

	};

}

#endif	/* UUID_bd1cf1bf_f1c7_4ef9_914f_843f4df019e8 */
