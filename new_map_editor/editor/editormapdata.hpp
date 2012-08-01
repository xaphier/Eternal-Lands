/****************************************************************************
 *            editormapdata.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
#include "terrainvalue.hpp"
#include "height.hpp"
#include "terraineditor.hpp"
#include "abstractmaploader.hpp"

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

	/**
	 * @brief @c class for maps.
	 *
	 * @c class for maps.
	 */
	class EditorMapData
	{
		private:
			TerrainEditor m_terrain_editor;
			std::map<Uint32, EditorObjectDescription> m_objects;
			std::map<Uint32, LightData> m_lights;
			std::map<Uint32, ParticleData> m_particles;
			boost::scoped_ptr<EditorScene> m_scene;
			Uint16MultiArray2 m_height_map;
			Uint8MultiArray2 m_tile_map;
			Uint32 m_id;
			RenderableType m_renderable;

			void save(const WriterSharedPtr &writer,
				const MapVersionType version) const;

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
			void set_dungeon(const bool dungeon);
			bool get_dungeon() const;
			void set_tile(const Uint16 x, const Uint16 y,
				const Uint16 tile);
			Uint16 get_tile(const Uint16 x, const Uint16 y) const;
			glm::uvec2 get_tile_offset(const glm::vec2 &point)
				const; 
			Uint16 get_height(const Uint16 x, const Uint16 y) const;
			void set_height(const Uint16 x, const Uint16 y,
				const Uint16 height);
			void set_heights(const HeightVector &heights);
			void set_terrain_values(
				const TerrainValueVector &terrain_values);
			void set_terrain_albedo_map(const String &name,
				const Uint16 index);
			void set_terrain_blend_map(const String &name,
				const Uint16 index);
			void set_terrain_vector_map(const String &name);
			void set_terrain_dudv_map(const String &name);
			const String &get_terrain_albedo_map(
				const Uint16 index) const;
			const String &get_terrain_blend_map(const Uint16 index)
				const;
			const String &get_terrain_vector_map() const;
			const String &get_terrain_dudv_map() const;
			Uint32 get_free_object_id() const;
			Uint32 get_free_light_id() const;
			void set_view_matrix(const glm::mat4 &view_matrix);
			void set_z_near(const float z_near);
			void set_perspective(const float fov,
				const float aspect);
			void set_ortho(const glm::vec4 &ortho);
			void set_view_port(const glm::uvec4 &view_port);
			const glm::mat4 &get_projection_matrix() const;
			void load_map(const String &name);
			void draw();
			void select(const glm::uvec2 &position,
				const glm::uvec2 &half_size);
			void set_depth_selection(const glm::uvec2 &position);
			void set_draw_objects(const bool draw_objects);
			void set_draw_terrain(const bool draw_terrain);
			void set_draw_lights(const bool draw_lights);
			void set_draw_light_spheres(
				const bool draw_light_spheres);
			void set_lights_enabled(const bool enabled);
			StringVector get_materials() const;
			StringVector get_default_materials(const String &name)
				const;
			ImageSharedPtr get_image(const String &name) const;
			void init_terrain(const glm::uvec2 &size);
			void set_focus(const glm::vec3 &focus) noexcept;
			void set_debug_mode(const int value);
			StringVector get_debug_modes() const;
			double get_depth() const;
			void save(const String &file_name) const;

			inline void get_terrain_values(const glm::uvec2 &vertex,
				const glm::vec2 &size,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				TerrainValueVector &terrain_values) const
			{
				m_terrain_editor.get_terrain_values(vertex,
					size, attenuation_size, attenuation,
					shape, terrain_values);
			}

			inline void change_terrain_values(const glm::vec3 &data,
				const glm::bvec3 &mask, const glm::vec2 &size,
				const glm::uvec2 &vertex,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BrushEffectType effect,
				TerrainValueVector &terrain_values) const
			{
				m_terrain_editor.change_terrain_values(data,
					mask, size, vertex, attenuation_size,
					attenuation, shape, effect,
					terrain_values);
			}

			inline glm::uvec2 get_vertex(
				const glm::vec3 &world_position) const
			{
				return m_terrain_editor.get_vertex(
					world_position);
			}

			static inline const glm::vec3 &get_terrain_offset()
			{
				return TerrainEditor::get_terrain_offset();
			}

			static inline const glm::vec3 &get_terrain_offset_min()
			{
				return TerrainEditor::get_terrain_offset_min();
			}

			static inline const glm::vec3 &get_terrain_offset_max()
			{
				return TerrainEditor::get_terrain_offset_max();
			}

			inline void set_height_map_size(const Uint16 width,
				const Uint16 height) noexcept
			{
				m_height_map.resize(
					boost::extents[width][height]);
			}

			inline void set_tile_map_size(const Uint16 width,
				const Uint16 height) noexcept
			{
				m_tile_map.resize(
					boost::extents[width][height]);
			}

			inline glm::uvec2 get_tile_map_size() const noexcept
			{
				return glm::uvec2(m_tile_map.shape()[0],
					m_tile_map.shape()[1]);
			}

			inline Uint32 get_id() const noexcept
			{
				return m_id;
			}

			inline RenderableType get_renderable() const noexcept
			{
				return m_renderable;
			}

	};

}

#endif	/* UUID_dd6047da_2207_47c6_8553_f0c2b368da93 */
