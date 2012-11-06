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
#include "textureformatutil.hpp"
#include "editorobjectdescription.hpp"
#include "imagevalue.hpp"
#include "displacementvalue.hpp"
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
		rt_particle,
		rt_decal,
		rt_objects,
		rt_lights,
		rt_particles,
		rt_decals
	};

	enum SelectionChangeType
	{
		sct_no,
		sct_unselect,
		sct_select,
		sct_force_select
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
			ImageSharedPtr m_water_image;
			Uint16MultiArray2 m_water_map;
			std::map<Uint32, EditorObjectDescription> m_objects;
			std::map<Uint32, LightData> m_lights;
			std::map<Uint32, ParticleData> m_particles;
			boost::scoped_ptr<EditorScene> m_scene;
			Uint16MultiArray2 m_height_map;
			Uint8MultiArray2 m_tile_map;
			Uint32Set m_ids;
			Uint32 m_id;
			RenderableType m_renderable;

			void save(const WriterSharedPtr &writer,
				const MapVersionType version) const;
			void change_selection(const Uint32 id,
				const RenderableType renderable,
				const SelectionChangeType selection);

		public:
			EditorMapData(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);
			~EditorMapData() throw();
			void add_object(const EditorObjectDescription &object,
				const SelectionChangeType select);
			void add_light(const LightData &light,
				const SelectionChangeType select);
			void add_particle(const ParticleData &particle,
				const SelectionChangeType select);
			void modify_object(
				const EditorObjectDescription &object);
			void modify_light(const LightData &light);
			void modify_particle(const ParticleData &particle);
			void modify_decal(const DecalDescription &decal);
			bool remove_object(const Uint32 id);
			bool remove_light(const Uint32 id);
			bool remove_particle(const Uint32 id);
			bool get_object(const Uint32 id,
				EditorObjectDescription &object_description)
				const;
			bool get_objects(const Uint32 id,
				EditorObjectDescriptionVector
					&object_descriptions) const;
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
			void set_terrain_displacement_values(
				const DisplacementValueVector
					&displacement_values);
			void set_terrain_blend_values(
				const ImageValueVector &blend_values);
			void set_terrain_material(const String &albedo_map,
				const String &extra_map, const float blend_size,
				const bool use_blend_size_sampler,
				const bool use_blend_size,
				const bool use_extra_map, const Uint16 index);
			Uint32 get_free_object_id() const;
			Uint32 get_free_light_id() const;
			void use_object_id(const Uint32 id);
			void use_light_id(const Uint32 id);
			void set_view_matrix(const glm::mat4 &view_matrix);
			void set_z_near(const float z_near);
			void set_perspective(const float fov,
				const float aspect);
			void set_ortho(const glm::vec4 &ortho);
			void set_view_port(const glm::uvec4 &view_port);
			const glm::mat4 &get_projection_matrix() const;
			void load_map(const String &name,
				const MapItemsTypeSet &skip_items);
			void draw();
			void select(const glm::uvec2 &position,
				const glm::uvec2 &half_size);
			void set_depth_selection(const glm::uvec2 &position);
			void set_draw_objects(const bool draw_objects);
			void set_draw_terrain(const bool draw_terrain);
			void set_draw_lights(const bool draw_lights);
			void set_draw_heights(const bool draw_heights);
			void set_draw_light_spheres(
				const bool draw_light_spheres);
			void set_lights_enabled(const bool enabled);
			StringVector get_materials() const;
			StringVector get_default_materials(const String &name)
				const;
			ImageSharedPtr get_image(const String &name,
				TextureFormatType &format) const;
			void get_image_data(const String &name,
				TextureFormatType &format, glm::uvec3 &sizes,
				Uint16 &mipmaps, bool &cube_map, bool &array)
				const;
			void init_terrain(const glm::uvec2 &size,
				const String &albedo_map,
				const String &extra_map,
				const bool use_blend_size_sampler,
				const bool use_extra_map);
			void init_terrain(const String &height_map_name,
				const glm::uvec2 &size,
				const String &albedo_map,
				const String &extra_map,
				const bool use_blend_size_sampler,
				const bool use_extra_map);
			void set_focus(const glm::vec3 &focus) noexcept;
			void set_debug_mode(const int value);
			StringVector get_debug_modes() const;
			double get_depth() const;
			void save(const String &file_name) const;
			void draw_selection(const glm::uvec4 &selection_rect);
			bool get_water_vertex(const glm::vec3 &start_position,
				const glm::vec3 &world_position,
				glm::uvec2 &result) const;
			void import_terrain_height_map(const String &name);
			void import_terrain_blend_map(const String &name);
			void set_terrain(
				const ImageSharedPtr &displacement_map,
				const ImageSharedPtr &normal_map,
				const ImageSharedPtr &dudv_map,
				const ImageSharedPtr &blend_map,
				const StringVector &albedo_maps,
				const StringVector &extra_maps,
				const TerrainMaterialData &material_data,
				const glm::vec4 &dudv_scale_offset,
				const glm::uvec2 &size);
			void update_terrain_dudv();

			inline void relax_terrain_uv(
				const AbstractProgressSharedPtr &progress,
				const Uint16 count, const bool use_simd)
			{
				m_terrain_editor.relax_uv(progress, count,
					use_simd);
			}

			inline void import_terrain_blend_map(
				const ImageSharedPtr &blend_map)
			{
				m_terrain_editor.import_blend_map(blend_map);
			}

			inline const glm::vec4 &get_terrain_dudv_scale_offset()
				const
			{
				return m_terrain_editor.get_dudv_scale_offset();
			}

			inline void get_terrain_material(String &albedo_map,
				String &extra_map, float &blend_size,
				bool &use_blend_size_sampler,
				bool &use_blend_size, bool &use_extra_map,
				const Uint16 index) const
			{
				m_terrain_editor.get_material(albedo_map,
					extra_map, blend_size,
					use_blend_size_sampler, use_blend_size,
					use_extra_map, index);
			}

			inline void get_terrain_displacement_values(
				const glm::uvec2 &vertex,
				const glm::vec2 &size,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				DisplacementValueVector &displacement_values)
				const
			{
				m_terrain_editor.get_displacement_values(vertex,
					size, attenuation_size, attenuation,
					shape, displacement_values);
			}

			inline void change_terrain_displacement_values(
				const glm::vec3 &data, const glm::bvec3 &mask,
				const glm::vec2 &size, const glm::uvec2 &vertex,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BrushEffectType effect,
				DisplacementValueVector &displacement_values)
				const
			{
				m_terrain_editor.change_displacement_values(
					data, mask, size, vertex,
					attenuation_size, attenuation, shape,
					effect, displacement_values);
			}

			inline void get_terrain_blend_values(
				const glm::uvec2 &vertex, const glm::vec2 &size,
				const float attenuation_size,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				ImageValueVector &blend_values) const
			{
				m_terrain_editor.get_blend_values(vertex,
					size, attenuation_size, attenuation,
					shape, blend_values);
			}

			inline void change_terrain_blend_values(
				const glm::vec2 &size, const glm::uvec2 &vertex,
				const float attenuation_size, const float data,
				const BrushAttenuationType attenuation,
				const BrushShapeType shape,
				const BrushEffectType effect,
				const int layer,
				ImageValueVector &blend_values) const
			{
				m_terrain_editor.change_blend_values(size,
					vertex, attenuation_size, data,
					attenuation, shape, effect, layer,
					blend_values);
			}

			inline bool get_terrain_vertex(
				const glm::vec3 &world_position,
				glm::uvec2 &result) const
			{
				return m_terrain_editor.get_vertex(
					world_position, result);
			}

			inline bool get_terrain() const
			{
				return m_terrain_editor.get_enabled();
			}

			inline Uint16 get_terrain_layer_count() const
			{
				return m_terrain_editor.get_layer_count();
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

			inline Uint32Set get_ids() const noexcept
			{
				return m_ids;
			}

			inline RenderableType get_renderable() const noexcept
			{
				return m_renderable;
			}

			inline void get_all_terrain_displacement_values(
				DisplacementValueVector &displacement_values)
				const
			{
				m_terrain_editor.get_all_displacement_values(
					displacement_values);
			}

			inline void get_all_terrain_blend_values(
				ImageValueVector &blend_values) const
			{
				m_terrain_editor.get_all_blend_values(
					blend_values);
			}

	};

}

#endif	/* UUID_dd6047da_2207_47c6_8553_f0c2b368da93 */
