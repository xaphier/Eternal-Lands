/****************************************************************************
 *            editormapdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_dd6047da_2207_47c6_8553_f0c2b368da93
#define	UUID_dd6047da_2207_47c6_8553_f0c2b368da93

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "editorobjectdescription.hpp"
#include "imagevalue.hpp"
#include "height.hpp"

/**
 * @file
 * @brief The @c class EditorMapData.
 * This file contains the @c class EditorMapData.
 */
namespace eternal_lands
{

	class EditorScene;

	enum RenderableType
	{
		rt_none,
		rt_object,
		rt_light,
		rt_particle
	};

	enum EditorBrushType
	{
		ebt_set = 0,
		ebt_const = 1,
		ebt_linear = 2,
		ebt_quadratic = 3,
		ebt_linear_smooth = 4,
		ebt_quadratic_smooth = 5
	};

	/**
	 * @brief @c class for maps.
	 *
	 * @c class for maps.
	 */
	class EditorMapData
	{
		private:
			std::map<Uint32, EditorObjectDescription> m_objects;
			std::map<Uint32, LightData> m_lights;
			std::map<Uint32, ParticleData> m_particles;
			boost::scoped_ptr<EditorScene> m_scene;
			ImageSharedPtr m_height_image;
			ImageSharedPtr m_blend_image;
			Uint16MultiArray2 m_height_map;
			Uint8MultiArray2 m_tile_map;
			Uint32 m_id;
			RenderableType m_renderable;

			void get_heights(const Uint16Array2 &vertex,
				const float radius, HeightVector &heights)
				const;
			void change_heights(const Uint16Array2 &vertex,
				const float strength, const float radius,
				const EditorBrushType brush_type,
				HeightVector &heights) const;
			void get_blend_values(const Uint16Array2 &vertex,
				const float radius,
				ImageValueVector &blend_values) const;
			static void change_blend_values(
				const Uint16Array2 &position,
				const Uint32 index, const float strength,
				const float radius,
				const EditorBrushType brush_type,
				ImageValueVector &blend_values);
			static float calc_brush_effect(const glm::vec2 &centre,
				const glm::vec2 &point, const float value,
				const float average, const float strength,
				const float radius,
				const EditorBrushType brush_type);
			static float get_blend_value(const glm::vec4 &blend,
				const Uint32 index);
			static void set_blend_value(const float value,
				const Uint32 index, glm::vec4 &blend);
			static glm::vec4 get_blend_values(
				const glm::vec4 &blend);
			static EditorBrushType get_brush_type(
				const int brush_type);

		public:
			EditorMapData(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);
			~EditorMapData() throw();
			void add_object(const EditorObjectDescription &object);
			void add_light(const LightData &light);
			void add_particle(const ParticleData &particle);
			void modify_object(const EditorObjectDescription &object);
			void modify_light(const LightData &light);
			void modify_particle(const ParticleData &particle);
			bool remove_object(const Uint32 id);
			bool remove_light(const Uint32 id);
			bool remove_particle(const Uint32 id);
			bool get_object(const Uint32 id,
				EditorObjectDescription &object) const;
			bool get_light(const Uint32 id, LightData &light) const;
			bool get_particle(const Uint32 id,
				ParticleData &particle) const;
			void set_ambient(const glm::vec3 &color);
			const glm::vec3 &get_ambient() const;
			void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile);
			Uint16 get_tile(const Uint16 x, const Uint16 y) const;
			Uint16Array2 get_tile_offset(const glm::vec2 &point)
				const; 
			void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height);
			void set_heights(const HeightVector &heights);
			void set_terrain_heights(const HeightVector &heights,
				const Uint16 id);
			void set_terrain_albedo_map(const String &name,
				const Uint16 index, const Uint16 id);
			void set_terrain_blend_map(const String &name,
				const Uint16 id);
			void set_terrain_height_map(const String &name,
				const Uint16 id);
			void set_terrain_dudv_map(const String &name,
				const Uint16 id);
			const String &get_terrain_albedo_map(const Uint16 index,
				const Uint16 id) const;
			const String &get_terrain_blend_map(const Uint16 id)
				const;
			const String &get_terrain_height_map(const Uint16 id)
				const;
			const String &get_terrain_dudv_map(const Uint16 id)
				const;
			Uint32 get_free_object_id() const;
			Uint32 get_free_light_id() const;
			void init();
			void set_view_matrix(const glm::mat4 &view_matrix);
			void set_perspective(const float fov,
				const float aspect, const float z_near,
				const float z_far);
			void set_view_port(const glm::uvec4 &view_port);
			const glm::mat4 &get_projection_matrix() const;
			void load_map(const String &name);
			void draw();
			void select(const Uint16Array2 &position,
				const Uint16Array2 &half_size);
			void set_draw_lights(const bool draw_lights);
			void set_draw_light_spheres(
				const bool draw_light_spheres);
			void set_lights_enabled(const bool enabled);
			StringVector get_materials() const;
			StringVector get_default_materials(const String &name)
				const;
			ImageSharedPtr get_image(const String &name) const;

			void set_height_map_size(const Uint16 width,
				const Uint16 height)
			{
				m_height_map.resize(
					boost::extents[width][height]);
			}

			void set_tile_map_size(const Uint16 width,
				const Uint16 height)
			{
				m_tile_map.resize(
					boost::extents[width][height]);
			}

			inline Uint32 get_id() const
			{
				return m_id;
			}

			inline RenderableType get_renderable() const
			{
				return m_renderable;
			}

	};

}

#endif	/* UUID_dd6047da_2207_47c6_8553_f0c2b368da93 */
