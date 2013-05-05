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
			Uint32ObjectSharedPtrMap m_walk_heights;
			boost::scoped_ptr<RStarTree> m_walk_height_tree;
			MaterialSharedPtr m_selection_material;
			TerrainRenderingData m_top_down_terrain;
			ObjectVisitor m_top_down_objects;
			glm::uvec2 m_depth_selection;
			float m_terrain_depth;
			float m_object_depth;
			Uint32 m_selected_object;
			bool m_draw_objects;
			bool m_draw_terrain;
			bool m_draw_lights;
			bool m_draw_light_spheres;
			bool m_draw_walk_heights;
			GLuint m_querie_id;

		protected:
			virtual void terrain_depth_read() override;
			virtual void object_depth_read() override;
			virtual void intersect_terrain(const Frustum &frustum,
				const glm::vec3 &camera,
				BoundingBox &bounding_box) const override;
			virtual void intersect_terrain(const Frustum &frustum,
				const glm::vec3 &camera,
				TerrainVisitor &terrain) const override;
			virtual void intersect(const Frustum &frustum,
				const bool shadow, ObjectVisitor &visitor)
				const override;
			virtual void map_changed() override;

		public:
			/**
			 * Default constructor.
			 */
			EditorScene(const GlobalVarsConstSharedPtr &global_vars,
				const FileSystemConstSharedPtr &file_system);

			/**
			 * Default destructor.
			 */
			virtual ~EditorScene() noexcept;
			virtual void add_light(const LightData &light_data)
				override;
			virtual void remove_light(const Uint32 id) override;
			void load_map(const String &name, EditorMapData &data,
				const MapItemsTypeSet &skip_items);
			void set_terrain_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map,
				const glm::vec3 &translation);
			void set_terrain_blend_map(
				const ImageSharedPtr &blend_map);
			void set_terrain_material(
				const StringVector &albedo_maps,
				const StringVector &specular_maps,
				const StringVector &gloss_maps,
				const StringVector &height_maps,
				const TerrainMaterialData &material_data);
			void set_terrain_dudv_scale_offset(
				const glm::vec4 &dudv_scale_offset);
			void update_terrain_geometry_maps(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map,
				const glm::vec3 &translation);
			void update_terrain_blend_map(
				const ImageSharedPtr &blend_map);
			void draw_selection(const glm::uvec4 &selection_rect);
/*
			void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height);
*/
			void cull_map();
			void draw_scale_view(const bool depth_only);
			void set_terrain(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_tangent_map,
				const ImageSharedPtr &dudv_map,
				const ImageSharedPtr &blend_map,
				const StringVector &albedo_maps,
				const StringVector &specular_maps,
				const StringVector &gloss_maps,
				const StringVector &height_maps,
				const TerrainMaterialData &material_data,
				const glm::vec4 &dudv_scale_offset,
				const glm::vec3 &translation);
			glm::vec3 get_map_min() const;
			glm::vec3 get_map_max() const;
			void set_map_size(const glm::uvec2 &size);
			void set_walk_height_page(
				const Uint16MultiArray2 &walk_height_page,
				const glm::uvec2 &offset);

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

			inline double get_terrain_depth() const noexcept
			{
				return m_terrain_depth;
			}

			inline double get_object_depth() const noexcept
			{
				return m_object_depth;
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

			inline bool get_draw_walk_heights() const noexcept
			{
				return m_draw_walk_heights;
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

			inline void set_draw_walk_heights(
				const bool draw_walk_heights) noexcept
			{
				m_draw_walk_heights = draw_walk_heights;
			}

	};

}

#endif	/* UUID_bd1cf1bf_f1c7_4ef9_914f_843f4df019e8 */
