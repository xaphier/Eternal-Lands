/****************************************************************************
 *            editor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e8300c87_0a6d_44dd_b202_82af4a234947
#define	UUID_e8300c87_0a6d_44dd_b202_82af4a234947

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "textureformatutil.hpp"
#include "undostack.hpp"
#include "editormapdata.hpp"
#include "editorobjectdescription.hpp"
#include "blenddata.hpp"
#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace eternal_lands
{

	class Editor
	{
		private:
			EditorMapData m_data;
			UndoStack m_undo;
			boost::mt19937 m_gen;
			boost::uniform_real<> m_uni_dist;
			boost::variate_generator<boost::mt19937&,
				boost::uniform_real<> > m_random;
			glm::vec3 m_random_translation_min;
			glm::vec3 m_random_translation_max;
			glm::vec3 m_random_rotation_min;
			glm::vec3 m_random_rotation_max;
			float m_random_scale_min;
			float m_random_scale_max;
			glm::bvec3 m_random_translation;
			glm::bvec3 m_random_rotation;
			Uint32 m_edit_id;
			bool m_random_scale;

			void change_object(const ModificationType type,
				const EditorObjectDescription
					&object_description);
			void change_objects(const ModificationType type,
				const EditorObjectDescriptionVector
					&object_descriptions);
			void change_light(const ModificationType type,
				const LightData &light_data);
			void change_lights(const ModificationType type,
				const LightDataVector &light_datas);
			bool do_set_terrain_albedo_map(const String &str,
				const Uint16 index, const Uint16 id);
			void remove_ground_tile(const Uint32 id);
			static StringVector get_ground_tile_materials(
				const Uint8 index);
			bool add_needed(const Uint32 id,
				const ModificationType type);

		public:
			Editor(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);
			bool undo();
			void set_terrain_material(const String &albedo_map,
				const String &extra_map,
				const float blend_size,
				const bool use_blend_size_sampler,
				const bool use_blend_size,
				const bool use_extra_map,
				const Uint16 index);
			void set_ground_tile(const glm::vec2 &point,
				const Uint16 tile);
			void add_3d_object(const glm::vec3 &position,
				const String &mesh,
				const SelectionType selection);
			void add_light(const glm::vec3 &position,
				const float radius);
			void add_ground_tile(const Uint32 x, const Uint32 y,
				const Uint8 texture);
			void set_dungeon(const bool dungeon);
			bool get_dungeon() const;
			void set_ambient(const glm::vec3 &color);
			const glm::vec3 &get_ambient() const;
			void ground_tile_edit(const glm::vec3 &point,
				const Uint8 tile);
			void water_tile_edit(const glm::vec3 &start,
				const glm::vec3 &point, const Uint16 water);
			void height_edit(const glm::vec3 &point,
				const Uint8 height);
			void remove_object(const Uint32 id);
			void remove_objects(const Uint32 id);
			void save(const String &name) const;
			void load_map(const String &name,
				const bool load_2d_objects,
				const bool load_3d_objects,
				const bool load_lights,
				const bool load_particles,
				const bool load_materials,
				const bool load_height_map,
				const bool load_tile_map,
				const bool load_walk_map,
				const bool load_terrain,
				const bool load_water);
			void set_object_transparency(const Uint32 id,
				const float transparency);
			void set_object_translation(const Uint32 id,
				const glm::vec3 &translation);
			void set_object_rotation(const Uint32 id,
				const glm::vec3 &rotation);
			void set_object_scale(const Uint32 id,
				const glm::vec3 &scale);
			void set_object_blend(const Uint32 id,
				const BlendType blend);
			void set_object_selection(const Uint32 id,
				const SelectionType selection);
			void set_object_walkable(const Uint32 id,
				const bool walkable);
			void set_object_materials(const Uint32 id,
				const StringVector &materials);
			void set_objects_transparency(const Uint32Set &ids,
				const float transparency);
			void set_objects_translation(const Uint32Set &ids,
				const glm::vec3 &translation);
			void set_objects_rotation(const Uint32Set &ids,
				const glm::vec3 &rotation);
			void set_objects_scale(const Uint32Set &ids,
				const glm::vec3 &scale);
			void set_objects_blend(const Uint32Set &ids,
				const BlendType blend);
			void set_objects_selection(const Uint32Set &ids,
				const SelectionType selection);
			void set_objects_walkable(const Uint32Set &ids,
				const bool walkable);
			void set_objects_materials(const Uint32Set &ids,
				const StringVector &materials);
			void remove_light(const Uint32 id);
			void set_light_position(const Uint32 id,
				const glm::vec3 &position);
			void set_light_radius(const Uint32 id,
				const float radius);
			void set_light_color(const Uint32 id,
				const glm::vec3 &color);
			void set_lights_position(const Uint32Set &ids,
				const glm::vec3 &position);
			void set_lights_radius(const Uint32Set &ids,
				const float radius);
			void set_lights_color(const Uint32Set &ids,
				const glm::vec3 &color);
			void get_object_description(const Uint32 id,
				EditorObjectDescription &object_description)
				const;
			void get_light_data(const Uint32 id,
				LightData &light_data) const;
			void change_terrain_displacement_values(
				const glm::vec3 &position,
				const glm::vec3 &data, const glm::bvec3 &mask,
				const glm::vec2 &size,
				const float attenuation_size,
				const int attenuation, const int shape,
				const int effect);
			void change_terrain_blend_values(
				const glm::vec3 &position,
				const glm::vec2 &size,
				const float attenuation_size, const float data,
				const int attenuation, const int shape,
				const int effect, const int layer);
			bool check_default_materials(const String &name,
				const StringVector &materials) const;
			void import_terrain_height_map(const String &name);
			void import_terrain_blend_map(const String &name);
			void relax_terrain_uv(
				const AbstractProgressSharedPtr &progress,
				const Uint16 count);
			void update_terrain_dudv();

			static inline const glm::vec3 &get_terrain_offset_min()
			{
				return EditorMapData::get_terrain_offset_min();
			}

			static inline const glm::vec3 &get_terrain_offset_max()
			{
				return EditorMapData::get_terrain_offset_max();
			}

			void get_terrain_material(String &albedo_map,
				String &extra_map, float &blend_size,
				bool &use_blend_size_sampler,
				bool &use_blend_size, bool &use_extra_map,
				const Uint16 index) const
			{
				m_data.get_terrain_material(albedo_map,
					extra_map, blend_size,
					use_blend_size_sampler, use_blend_size,
					use_extra_map, index);
			}

			inline StringVector get_debug_modes() const
			{
				return m_data.get_debug_modes();
			}

			inline void set_debug_mode(const int value)
			{
				m_data.set_debug_mode(value);
			}

			inline void init_terrain(const glm::uvec2 &size,
				const String &albedo_map,
				const String &extra_map,
				const bool use_blend_size_sampler,
				const bool use_extra_map)
			{
				m_data.init_terrain(size, albedo_map,
					extra_map, use_blend_size_sampler,
					use_extra_map);
			}

			inline void init_terrain(const String &height_map_name,
				const glm::uvec2 &size,
				const String &albedo_map,
				const String &extra_map,
				const bool use_blend_size_sampler,
				const bool use_extra_map)
			{
				m_data.init_terrain(height_map_name, size,
					albedo_map, extra_map,
					use_blend_size_sampler, use_extra_map);
			}

			inline ImageSharedPtr get_image(const String &name,
				TextureFormatType &format) const
			{
				return m_data.get_image(name, format);
			}

			inline void get_image_data(const String &name,
				TextureFormatType &format, glm::uvec3 &size,
				Uint16 &mipmaps, bool &cube_map, bool &array)
				const
			{
				return m_data.get_image_data(name, format,
					size, mipmaps, cube_map, array);
			}

			inline void set_draw_objects(const bool draw_objects)
			{
				m_data.set_draw_objects(draw_objects);
			}

			inline void set_draw_terrain(const bool draw_terrain)
			{
				m_data.set_draw_terrain(draw_terrain);
			}

			inline void set_draw_lights(const bool draw_lights)
			{
				m_data.set_draw_lights(draw_lights);
			}

			inline void set_draw_light_spheres(
				const bool draw_light_spheres)
			{
				m_data.set_draw_light_spheres(
					draw_light_spheres);
			}

			inline void set_draw_heights(const bool draw_heights)
			{
				m_data.set_draw_heights(draw_heights);
			}

			inline void set_lights_enabled(const bool enabled)
			{
				m_data.set_lights_enabled(enabled);
			}

			inline void set_view_matrix(
				const glm::mat4 &view_matrix)
			{
				m_data.set_view_matrix(view_matrix);
			}

			inline void set_z_near(const float z_near) noexcept
			{
				m_data.set_z_near(z_near);
			}

			inline void set_perspective(const float fov,
				const float aspect) noexcept
			{
				m_data.set_perspective(fov, aspect);
			}

			inline void set_ortho(const glm::vec4 &ortho)
			{
				m_data.set_ortho(ortho);
			}

			inline void set_view_port(const glm::uvec4 &view_port)
			{
				m_data.set_view_port(view_port);
			}

			inline void set_focus(const glm::vec3 &focus) noexcept
			{
				m_data.set_focus(focus);
			}

			inline const glm::mat4 &get_projection_matrix() const
			{
				return m_data.get_projection_matrix();
			}

			inline void get_object_description(
				EditorObjectDescription &object_description)
				const
			{
				assert(get_object_selected());
				get_object_description(get_id(),
					object_description);
			}

			inline void get_light_data(LightData &light_data) const
			{
				assert(get_light_selected());
				get_light_data(get_id(), light_data);
			}

			inline bool get_object_selected() const
			{
				return get_renderable() == rt_object;
			}

			inline bool get_light_selected() const
			{
				return get_renderable() == rt_light;
			}

			inline bool get_objects_selected() const
			{
				return get_renderable() == rt_objects;
			}

			inline bool get_lights_selected() const
			{
				return get_renderable() == rt_lights;
			}

			inline bool get_object_selected(const Uint32 id) const
			{
				if (get_object_selected())
				{
					return get_id() == id;
				}
				else
				{
					return false;
				}
			}

			inline bool get_light_selected(const Uint32 id) const
			{
				if (get_light_selected())
				{
					return get_id() == id;
				}
				else
				{
					return false;
				}
			}

			inline bool get_can_undo() const
			{
				return m_undo.can_undo();
			}

			inline void remove_object()
			{
				assert(get_object_selected());
				remove_object(get_id());
			}

			inline void remove_objects()
			{
				assert(get_object_selected());
				remove_objects(get_id());
			}

			inline void set_object_blend(const BlendType blend)
			{
				assert(get_object_selected());
				set_object_blend(get_id(), blend);
			}

			inline void set_object_walkable(const bool walkable)
			{
				assert(get_object_selected());
				set_object_walkable(get_id(), walkable);
			}

			inline void set_object_transparency(
				const float transparency)
			{
				assert(get_object_selected());
				set_object_transparency(get_id(),
					transparency);
			}

			inline void set_object_translation(
				const glm::vec3 &translation)
			{
				assert(get_object_selected());
				set_object_translation(get_id(), translation);
			}

			inline void set_object_rotation(
				const glm::vec3 &rotation)
			{
				assert(get_object_selected());
				set_object_rotation(get_id(), rotation);
			}

			inline void set_object_scale(const glm::vec3 &scale)
			{
				assert(get_object_selected());
				set_object_scale(get_id(), scale);
			}

			inline void set_object_selection(
				const SelectionType selection)
			{
				assert(get_object_selected());
				set_object_selection(get_id(), selection);
			}

			inline void set_object_materials(
				const StringVector &materials)
			{
				assert(get_object_selected());
				set_object_materials(get_id(), materials);
			}

			inline void set_objects_blend(const BlendType blend)
			{
				assert(get_objects_selected());
				set_objects_blend(get_ids(), blend);
			}

			inline void set_objects_walkable(const bool walkable)
			{
				assert(get_objects_selected());
				set_objects_walkable(get_ids(), walkable);
			}

			inline void set_objects_transparency(
				const float transparency)
			{
				assert(get_objects_selected());
				set_objects_transparency(get_ids(),
					transparency);
			}

			inline void set_objects_translation(
				const glm::vec3 &translation)
			{
				assert(get_objects_selected());
				set_objects_translation(get_ids(), translation);
			}

			inline void set_objects_rotation(
				const glm::vec3 &rotation)
			{
				assert(get_objects_selected());
				set_objects_rotation(get_ids(), rotation);
			}

			inline void set_objects_scale(const glm::vec3 &scale)
			{
				assert(get_objects_selected());
				set_objects_scale(get_ids(), scale);
			}

			inline void set_objects_selection(
				const SelectionType selection)
			{
				assert(get_objects_selected());
				set_objects_selection(get_ids(), selection);
			}

			inline void set_objects_materials(
				const StringVector &materials)
			{
				assert(get_objects_selected());
				set_objects_materials(get_ids(), materials);
			}

			inline void remove_light()
			{
				assert(get_light_selected());
				remove_light(get_id());
			}

			inline void set_light_position(
				const glm::vec3 &position)
			{
				assert(get_light_selected());
				set_light_position(get_id(), position);
			}

			inline void set_light_radius(const float radius)
			{
				assert(get_light_selected());
				set_light_radius(get_id(), radius);
			}

			inline void set_light_color(const glm::vec3 &color)
			{
				assert(get_light_selected());
				set_light_color(get_id(), color);
			}

			inline void set_lights_position(
				const glm::vec3 &position)
			{
				assert(get_lights_selected());
				set_lights_position(get_ids(), position);
			}

			inline void set_lights_radius(const float radius)
			{
				assert(get_lights_selected());
				set_lights_radius(get_ids(), radius);
			}

			inline void set_lights_color(const glm::vec3 &color)
			{
				assert(get_lights_selected());
				set_lights_color(get_ids(), color);
			}

			inline void set_random_translation(const bool value,
				const Uint16 index)
			{
				m_random_translation[index] = value;
			}

			inline void set_random_rotation(const bool value,
				const Uint16 index)
			{
				m_random_rotation[index] = value;
			}

			inline void set_random_scale(const bool value)
			{
				m_random_scale = value;
			}

			inline void set_random_translation_min(
				const float value, const Uint16 index)
			{
				m_random_translation_min[index] = value;
			}

			inline void set_random_rotation_min(
				const float value, const Uint16 index)
			{
				m_random_rotation_min[index] = value;
			}

			inline void set_random_scale_min(const float value)
			{
				m_random_scale_min = value;
			}

			inline void set_random_translation_max(
				const float value, const Uint16 index)
			{
				m_random_translation_max[index] = value;
			}

			inline void set_random_rotation_max(
				const float value, const Uint16 index)
			{
				m_random_rotation_max[index] = value;
			}

			inline void set_random_scale_max(const float value)
			{
				m_random_scale_max = value;
			}

			inline void draw()
			{
				m_data.draw();
			}

			inline void draw_selection(
				const glm::uvec4 &selection_rect)
			{
				m_data.draw_selection(selection_rect);
			}

			inline void select(const glm::uvec2 &position,
				const glm::uvec2 &half_size)
			{
				m_data.select(position, half_size);
			}

			inline void set_depth_selection(
				const glm::uvec2 &position)
			{
				m_data.set_depth_selection(position);
			}

			inline double get_depth() const
			{
				return m_data.get_depth();
			}

			inline Uint32 get_id() const
			{
				return m_data.get_id();
			}

			inline Uint32Set get_ids() const
			{
				return m_data.get_ids();
			}

			inline RenderableType get_renderable() const
			{
				return m_data.get_renderable();
			}

			inline StringVector get_materials() const
			{
				return m_data.get_materials();
			}

			inline StringVector get_default_materials(
				const String &name) const
			{
				return m_data.get_default_materials(name);
			}

			inline Uint32 get_edit_id() const noexcept
			{
				return m_edit_id;
			}

			inline void update_edit_id() noexcept
			{
				m_edit_id++;
			}

			inline void save(const String &file_name)
			{
				m_data.save(file_name);
			}

			inline bool get_terrain() const
			{
				return m_data.get_terrain();
			}

			inline Uint16 get_terrain_layer_count() const
			{
				return m_data.get_terrain_layer_count();
			}

	};

}

#endif	/* UUID_e8300c87_0a6d_44dd_b202_82af4a234947 */
