/****************************************************************************
 *            editor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e8300c87_0a6d_44dd_b202_82af4a234947
#define	UUID_e8300c87_0a6d_44dd_b202_82af4a234947

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
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
			bool m_random_scale;

			void change_object(const ModificationType type,
				const EditorObjectDescription
					&object_description);
			void change_light(const ModificationType type,
				const LightData &light_data);
			bool do_set_terrain_albedo_map(const String &str,
				const Uint16 index, const Uint16 id);
			void remove_ground_tile(const Uint32 id);
			static StringVector get_ground_tile_materials(
				const Uint8 index);
/*			void terrain_edit(const Uint16Array2 &vertex,
				const float strength, const float radius,
				const EditorBrushType brush_type);
			void blend_image_edit(const Uint16Array2 &vertex,
				const Uint32 index, const float strength,
				const float radius,
				const EditorBrushType brush_type,
				const Uint16 id);
*/
		public:
			Editor(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);
			bool undo();
			void set_terrain_albedo_map(const String &texture,
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
			void set_ambient(const glm::vec3 &color);
			void terrain_layer_edit(const glm::vec3 &point,
				const Uint32 index, const float strength,
				const float radius, const int brush_type);
			void ground_tile_edit(const glm::vec3 &point,
				const Uint8 tile);
			void water_tile_edit(const glm::vec3 &point,
				const Uint8 water);
			void height_edit(const glm::vec3 &point,
				const Uint8 height);
			void remove_object(const Uint32 id);
			void save(const String &name) const;
			void load_map(const String &name);
			void set_object_transparency(const Uint32 id,
				const BlendType blend);
			void set_object_translation(const Uint32 id,
				const glm::vec3 &translation);
			void set_object_rotation(const Uint32 id,
				const glm::vec3 &rotation);
			void set_object_scale(const Uint32 id,
				const float scale);
			void set_object_blend(const Uint32 id,
				const BlendType blend);
			void set_object_selection(const Uint32 id,
				const SelectionType selection);
			void set_object_materials(const Uint32 id,
				const StringVector &materials);
			void remove_light(const Uint32 id);
			void set_light_position(const Uint32 id,
				const glm::vec3 &position);
			void set_light_radius(const Uint32 id,
				const float radius);
			void set_light_color(const Uint32 id,
				const glm::vec3 &color);
			void get_object_description(const Uint32 id,
				EditorObjectDescription &object_description)
				const;
			void get_light_data(const Uint32 id,
				LightData &light_data) const;
			const glm::vec3 &get_ambient() const;
			void export_blend_image(const String &file_name,
				const String &type) const;
			void export_terrain_map(const String &file_name,
				const String &type) const;
			void import_terrain_map(const String &file_name);
			void terrain_vector_add_normal(
				const glm::vec3 &position, const float scale,
				const float radius, const Sint32 brush_type,
				const Uint32 id);
			void terrain_vector_add(const glm::vec3 &position,
				const glm::vec3 &add_value, const float radius,
				const Sint32 brush_type, const Uint32 id);
			void terrain_vector_smooth(const glm::vec3 &position,
				const float strength, const float radius,
				const Sint32 brush_type, const Uint32 id);
			void terrain_vector_set(const glm::vec3 &position,
				const glm::vec3 &set_value,
				const glm::bvec3 &mask, const float radius,
				const Sint32 brush_type, const Uint32 id);

			static inline const glm::vec3 &get_terrain_offset()
			{
				return EditorMapData::get_terrain_offset();
			}

			static inline const glm::vec3 &get_terrain_offset_min()
			{
				return EditorMapData::get_terrain_offset_min();
			}

			static inline const glm::vec3 &get_terrain_offset_max()
			{
				return EditorMapData::get_terrain_offset_max();
			}

			inline const String &get_terrain_albedo_map(
				const Uint16 index) const
			{
				return m_data.get_terrain_albedo_map(index);
			}

			inline const String &get_terrain_vector_map() const
			{
				return m_data.get_terrain_vector_map();
			}

			inline const String &get_terrain_blend_map(
				const Uint16 index) const
			{
				return m_data.get_terrain_blend_map(index);
			}

			inline const String &get_terrain_dudv_map() const
			{
				return m_data.get_terrain_dudv_map();
			}

			inline StringVector get_debug_modes() const
			{
				return m_data.get_debug_modes();
			}

			inline void set_debug_mode(const int value)
			{
				m_data.set_debug_mode(value);
			}

			inline void init_terrain(const glm::uvec2 &size)
			{
				m_data.init_terrain(size);
			}

			inline ImageSharedPtr get_image(const String &name)
				const
			{
				return m_data.get_image(name);
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

			inline void set_lights_enabled(const bool enabled)
			{
				m_data.set_lights_enabled(enabled);
			}

			inline void set_view_matrix(
				const glm::mat4 &view_matrix)
			{
				m_data.set_view_matrix(view_matrix);
			}

			inline void set_perspective(const float fov,
				const float aspect, const float z_near,
				const float z_far)
			{
				m_data.set_perspective(fov, aspect, z_near,
					z_far);
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
				get_object_description(get_id(),
					object_description);
			}

			inline void get_light_data(LightData &light_data) const
			{
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

			inline void set_object_blend(const BlendType blend)
			{
				assert(get_object_selected());
				set_object_blend(get_id(), blend);
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

			inline void set_object_scale(const float scale)
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

			inline void select(const glm::uvec2 &position,
				const glm::uvec2 &half_size)
			{
				m_data.select(position, half_size);
			}

			inline void select_depth(const glm::uvec2 &position)
			{
				m_data.select_depth(position);
			}

			inline float get_depth() const
			{
				return m_data.get_depth();
			}

			inline Uint32 get_id() const
			{
				return m_data.get_id();
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

	};

}

#endif	/* UUID_e8300c87_0a6d_44dd_b202_82af4a234947 */
