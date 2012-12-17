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
			bool add_needed(const Uint32 id,
				const ModificationType type);

		public:
			Editor(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);
			bool undo();
			void set_terrain_material(const String &albedo_map,
				const String &specular_map,
				const String &gloss_map,
				const String &height_map,
				const glm::vec3 &default_specular,
				const float default_gloss,
				const float default_height,
				const float blend_size,
				const bool use_blend_size_texture,
				const bool use_specular_map,
				const bool use_gloss_map,
				const bool use_height_map,
				const Uint16 index);
			void set_tile(const glm::uvec3 &offset,
				const Uint16 size, const Uint16 tile);
			void add_3d_object(const glm::vec3 &position,
				const String &name,
				const SelectionType selection);
			void add_light(const glm::vec3 &position,
				const float radius);
			void set_dungeon(const bool dungeon);
			bool get_dungeon() const;
			void set_ground_hemisphere(
				const glm::vec4 &ground_hemisphere);
			const glm::vec4 &get_ground_hemisphere() const;
			bool get_tile_edit(const glm::vec3 &min_position,
				const glm::vec3 &max_position,
				const Uint16 layer, glm::uvec3 &offset) const;
			void height_edit(const glm::vec3 &point,
				const Uint8 height);
			void remove_object(const Uint32 id);
			void remove_all_copies_of_object(const Uint32 id);
			void load_map(const String &name,
				const bool load_2d_objects,
				const bool load_3d_objects,
				const bool load_lights,
				const bool load_particles,
				const bool load_materials,
				const bool load_height_map,
				const bool load_tile_map,
				const bool load_walk_map,
				const bool load_terrain);
			void set_object_transparency(const Uint32 id,
				const float transparency);
			void set_object_glow(const Uint32 id,
				const float glow);
			void set_object_translation(const Uint32 id,
				const glm::vec3 &translation);
			void set_object_rotation(const Uint32 id,
				const glm::vec3 &rotation);
			void set_object_scale(const Uint32 id,
				const glm::vec3 &scale);
			void set_object_blend(const Uint32 id,
				const BlendType blend,
				const BitSet64 blend_mask);
			void set_object_blend_mask(const Uint32 id,
				const BitSet64 blend_mask);
			void set_object_selection(const Uint32 id,
				const SelectionType selection);
			void set_object_materials(const Uint32 id,
				const StringVector &materials);
			void set_object_name(const Uint32 id,
				const String &name);
			void set_object_description(const Uint32 id,
				const String &description);
			void set_all_copies_of_object_name(const Uint32 id,
				const String &name);
			void set_objects_transparency(const Uint32Set &ids,
				const float transparency);
			void set_objects_glow(const Uint32Set &ids,
				const float glow);
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
			void set_objects_materials(const Uint32Set &ids,
				const StringVector &materials);
			void set_objects_name(const Uint32Set &ids,
				const String &name);
			void set_objects_description(const Uint32Set &ids,
				const String &description);
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
				const float attenuation_size,
				const float strength, const int attenuation,
				const int shape, const int effect,
				const int layer);
			bool check_default_materials(const String &name,
				const StringVector &materials) const;
			void import_terrain_height_map(const String &name);
			void import_terrain_blend_map(const String &name);
			void relax_terrain_uv(
				const AbstractProgressSharedPtr &progress,
				const Uint16 count);
			void update_terrain_dudv();
			void get_terrain_layers_usage(
				Uint32Vector &use_layer_pixels, Uint32 &pixels)
				const;
			void clear_invisible_terrain_layers();
			void pack_terrain_layers();
			void deselect();
			void clear();
			bool get_terrain_displacement(
				const glm::vec3 &position,
				glm::vec3 &displacement) const;
			bool get_terrain_normal(
				const glm::vec3 &position, glm::vec3 &normal)
				const;
			void fill_terrain_blend_layer(const float strenght,
				const int mask, const int layer);
			void set_terrain_translation(
				const glm::vec3 &translation);
			float get_tile_layer_height(const Uint16 index) const;
			void set_tile_layer_height(const float value,
				const Uint16 layer);

			inline void export_tile_map(const String &file_name)
				const
			{
				m_data.export_tile_map(file_name);
			}

			static inline const glm::vec3 &get_terrain_offset_min()
			{
				return EditorMapData::get_terrain_offset_min();
			}

			static inline const glm::vec3 &get_terrain_offset_max()
			{
				return EditorMapData::get_terrain_offset_max();
			}

			inline glm::vec3 get_map_min() const
			{
				return m_data.get_map_min();
			}

			inline glm::vec3 get_map_max() const
			{
				return m_data.get_map_max();
			}

			inline const glm::uvec2 &get_map_size() const
			{
				return m_data.get_map_size();
			}

			inline void set_terrain_enabled(const bool enabled)
			{
				m_data.set_terrain_enabled(enabled);
			}

			void get_terrain_material(String &albedo_map,
				String &specular_map, String &gloss_map,
				String &height_map, glm::vec3 &default_specular,
				float &default_gloss, float &default_height,
				float &blend_size, bool &use_blend_size_texture,
				bool &use_specular_map, bool &use_gloss_map,
				bool &use_height_map, const Uint16 index) const
			{
				m_data.get_terrain_material(albedo_map,
					specular_map, gloss_map, height_map,
					default_specular, default_gloss,
					default_height, blend_size,
					use_blend_size_texture,
					use_specular_map, use_gloss_map,
					use_height_map, index);
			}

			inline StringVector get_debug_modes() const
			{
				return m_data.get_debug_modes();
			}

			inline void set_debug_mode(const int value)
			{
				m_data.set_debug_mode(value);
			}

			inline const glm::vec3 &get_terrain_translation() const
			{
				return m_data.get_terrain_translation();
			}

			inline void init_terrain(const glm::vec3 &translation,
				const glm::uvec2 &size,
				const String &albedo_map,
				const String &specular_map,
				const String &gloss_map,
				const String &height_map,
				const glm::vec3 &default_specular,
				const float default_gloss,
				const float default_height,
				const float blend_size,
				const bool use_blend_size_texture,
				const bool use_specular_map,
				const bool use_gloss_map,
				const bool use_height_map)
			{
				m_data.init_terrain(translation, size,
					albedo_map, specular_map, gloss_map,
					height_map, default_specular,
					default_gloss, default_height,
					blend_size, use_blend_size_texture,
					use_specular_map, use_gloss_map,
					use_height_map);
			}

			inline void init_terrain(const String &height_map_name,
				const glm::vec3 &translation,
				const glm::uvec2 &size,
				const String &albedo_map,
				const String &specular_map,
				const String &gloss_map,
				const String &height_map,
				const glm::vec3 &default_specular,
				const float default_gloss,
				const float default_height,
				const float blend_size,
				const bool use_blend_size_texture,
				const bool use_specular_map,
				const bool use_gloss_map,
				const bool use_height_map)
			{
				m_data.init_terrain(height_map_name,
					translation, size, albedo_map,
					specular_map, gloss_map, height_map,
					default_specular, default_gloss,
					default_height, blend_size,
					use_blend_size_texture,
					use_specular_map, use_gloss_map,
					use_height_map);
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

			inline bool get_object_description(
				EditorObjectDescription &object_description)
				const
			{
				if (!get_object_selected())
				{
					return false;
				}

				get_object_description(get_id(),
					object_description);

				return true;
			}

			inline bool get_light_data(LightData &light_data) const
			{
				if (!get_light_selected())
				{
					return false;
				}

				get_light_data(get_id(), light_data);

				return true;
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

			inline bool remove_object()
			{
				if (!get_object_selected())
				{
					return false;
				}

				remove_object(get_id());

				return true;
			}

			inline bool remove_all_copies_of_object()
			{
				if (!get_object_selected())
				{
					return false;
				}

				remove_all_copies_of_object(get_id());

				return true;
			}

			inline bool set_all_copies_of_object_name(
				const String &name)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_all_copies_of_object_name(get_id(), name);

				return true;
			}

			inline bool set_object_blend(const BlendType blend,
				const BitSet64 blend_mask)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_blend(get_id(), blend, blend_mask);

				return true;
			}

			inline bool set_object_blend_mask(
				const BitSet64 blend_mask)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_blend_mask(get_id(), blend_mask);

				return true;
			}

			inline bool set_object_transparency(
				const float transparency)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_transparency(get_id(),
					transparency);

				return true;
			}

			inline bool set_object_glow(const float glow)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_glow(get_id(), glow);

				return true;
			}

			inline bool set_object_translation(
				const glm::vec3 &translation)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_translation(get_id(), translation);

				return true;
			}

			inline bool set_object_rotation(
				const glm::vec3 &rotation)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_rotation(get_id(), rotation);

				return true;
			}

			inline bool set_object_scale(const glm::vec3 &scale)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_scale(get_id(), scale);

				return true;
			}

			inline bool set_object_selection(
				const SelectionType selection)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_selection(get_id(), selection);

				return true;
			}

			inline bool set_object_materials(
				const StringVector &materials)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_materials(get_id(), materials);

				return true;
			}

			inline bool set_object_description(
				const String &description)
			{
				if (!get_object_selected())
				{
					return false;
				}

				set_object_description(get_id(), description);

				return true;
			}

			inline bool set_objects_blend(const BlendType blend)
			{
				if (!get_objects_selected())
				{
					return false;
				}

				set_objects_blend(get_ids(), blend);

				return true;
			}

			inline bool set_objects_transparency(
				const float transparency)
			{
				if (!get_objects_selected())
				{
					return false;
				}

				set_objects_transparency(get_ids(),
					transparency);

				return true;
			}

			inline bool set_objects_glow(const float glow)
			{
				if (!get_objects_selected())
				{
					return false;
				}

				set_objects_glow(get_ids(), glow);

				return true;
			}

			inline bool set_objects_translation(
				const glm::vec3 &translation)
			{
				if (!get_objects_selected())
				{
					return false;
				}

				set_objects_translation(get_ids(), translation);

				return true;
			}

			inline bool set_objects_rotation(
				const glm::vec3 &rotation)
			{
				if (!get_objects_selected())
				{
					return false;
				}

				set_objects_rotation(get_ids(), rotation);

				return true;
			}

			inline bool set_objects_scale(const glm::vec3 &scale)
			{
				if (!get_objects_selected())
				{
					return false;
				}

				set_objects_scale(get_ids(), scale);

				return true;
			}

			inline bool set_objects_selection(
				const SelectionType selection)
			{
				if (!get_objects_selected())
				{
					return false;
				}

				set_objects_selection(get_ids(), selection);

				return true;
			}

			inline bool set_objects_materials(
				const StringVector &materials)
			{
				if (!get_objects_selected())
				{
					return false;
				}

				set_objects_materials(get_ids(), materials);

				return true;
			}

			inline bool set_objects_description(
				const String &description)
			{
				if (!get_objects_selected())
				{
					return false;
				}

				set_objects_description(get_ids(), description);

				return true;
			}

			inline bool remove_light()
			{
				if (!get_light_selected())
				{
					return false;
				}

				remove_light(get_id());

				return true;
			}

			inline bool set_light_position(
				const glm::vec3 &position)
			{
				if (!get_light_selected())
				{
					return false;
				}

				set_light_position(get_id(), position);

				return true;
			}

			inline bool set_light_radius(const float radius)
			{
				if (!get_light_selected())
				{
					return false;
				}

				set_light_radius(get_id(), radius);

				return true;
			}

			inline bool set_light_color(const glm::vec3 &color)
			{
				if (!get_light_selected())
				{
					return false;
				}

				set_light_color(get_id(), color);

				return true;
			}

			inline bool set_lights_position(
				const glm::vec3 &position)
			{
				if (!get_lights_selected())
				{
					return false;
				}

				set_lights_position(get_ids(), position);

				return true;
			}

			inline bool set_lights_radius(const float radius)
			{
				if (!get_lights_selected())
				{
					return false;
				}

				set_lights_radius(get_ids(), radius);

				return true;
			}

			inline bool set_lights_color(const glm::vec3 &color)
			{
				if (!get_lights_selected())
				{
					return false;
				}

				set_lights_color(get_ids(), color);

				return true;
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

			inline void save(
				const AbstractProgressSharedPtr &progress,
				const String &file_name)
			{
				m_data.save(progress, file_name);
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
