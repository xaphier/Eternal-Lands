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
#include "terraineditor.hpp"
#include "editorobjectdata.hpp"
#include "editor.hpp"
#include <boost/random.hpp>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>

namespace eternal_lands
{

	enum RenderableType
	{
		rt_mesh,
		rt_ground_tiles,
		rt_terrain,
		rt_light
	};

	class Editor: public TerrainEditor
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
			Uint32 m_id;
			RenderableType m_renderable;
			bool m_random_scale;
			bool m_selected;

			void change_object(const ModificationType type,
				const EditorObjectData &object_data);
			void change_light(const ModificationType type,
				const LightData &light_data);
			bool do_set_terrain_albedo_map(const String &str,
				const Uint16 index, const Uint16 id);
			void remove_ground_tile(const Uint32 id);
			static StringVector get_ground_tile_materials(
				const Uint8 index);
			void terrain_edit(const Uint16Array2 &vertex,
				const float strength, const float radius,
				const EditorBrushType brush_type,
				const Uint16 id);
			void blend_image_edit(const Uint16Array2 &vertex,
				const Uint32 index, const float strength,
				const float radius,
				const EditorBrushType brush_type,
				const Uint16 id);

			inline Uint32 get_id() const
			{
				return m_id;
			}

		public:
			Editor(const GlobalVarsSharedPtr &global_vars,
				const FileSystemSharedPtr &file_system);
			bool undo();
			void set_terrain_albedo_map(const String &texture,
				const Uint16 index, const Uint16 id);
			void set_ground_tile(const glm::vec2 &point,
				const Uint16 tile);
			void add_3d_object(const glm::vec3 &position,
				const String &mesh,
				const SelectionType selection);
			void add_light(const glm::vec3 &position);
			void add_ground_tile(const Uint32 x, const Uint32 y,
				const Uint8 texture);
			void set_ambient(const glm::vec3 &color);
			void terrain_height_edit(const Uint32 id,
				const glm::vec3 &p0, const glm::vec3 &p1,
				const float strength, const float radius,
				const int brush_type);
			void terrain_layer_edit(const Uint32 id,
				const glm::vec3 &p0, const glm::vec3 &p1,
				const Uint32 index, const float strength,
				const float radius, const int brush_type);
			void ground_tile_edit(const glm::vec3 &p0,
				const glm::vec3 &p1, const Uint8 tile);
			void water_tile_edit(const glm::vec3 &p0,
				const glm::vec3 &p1, const Uint8 water);
			void height_edit(const glm::vec3 &p0,
				const glm::vec3 &p1, const Uint8 height);
/*			void set_terrain(const MaterialData &terrain_material,
				const Uint16Array2 map_size, const Uint16Array2 blend_image_size);
			void set_terrain(const MaterialData &terrain_material,
				const String &image_name,
				const Uint16Array2 blend_image_size);
			void get_terrain_material_data(MaterialData &terrain_material) const;
*/			void remove_object(const Uint32 id);
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
			void remove_light(const Uint32 id);
			void set_light_position(const Uint32 id,
				const glm::vec3 &position);
			void set_light_radius(const Uint32 id,
				const float radius);
			void set_light_color(const Uint32 id,
				const glm::vec3 &color);
			void get_object_data(const Uint32 id,
				EditorObjectData &object_data) const;
			void get_light_data(const Uint32 id,
				LightData &light_data) const;
			const glm::vec3 &get_ambient() const;
			void export_blend_image(const String &file_name,
				const String &type) const;
			void export_terrain_map(const String &file_name,
				const String &type) const;
			void import_terrain_map(const String &file_name);
			const String &get_terrain_albedo_map(
				const Uint16 index, const Uint16 id) const;
			const String &get_terrain_height_map(const Uint16 id)
				const;
			const String &get_terrain_blend_map(const Uint16 id)
				const;
			const String &get_terrain_dudv_map(const Uint16 id)
				const;
			void draw();
			void select(const Uint16Array2 &position,
				const Uint16Array2 &half_size);

			inline void init()
			{
				m_data.init();
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

			inline bool get_selected() const
			{
				return m_selected;
			}

			inline void get_object_data(
				EditorObjectData &object_data) const
			{
				get_object_data(get_id(), object_data);
			}

			inline void get_light_data(LightData &light_data) const
			{
				get_light_data(get_id(), light_data);
			}

			inline RenderableType get_renderable() const
			{
				return m_renderable;
			}

			inline bool get_object_selected() const
			{
				return get_selected() &&
					(get_renderable() != rt_light);
			}

			inline bool get_light_selected() const
			{
				return get_selected() &&
					(get_renderable() == rt_light);
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
/*
			inline const Selection &get_selection() const
			{
				return m_scene.get_selection();
			}

			inline bool get_selected() const
			{
				return get_selection().get_valid();
			}

			inline EditorScene &get_scene()
			{
				return m_scene;
			}

			inline const EditorScene &get_scene() const
			{
				return m_scene;
			}

			inline void set_deselect()
			{
				m_scene.set_deselect();
			}
*/
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

			inline void set_terrain_albedo_map(
				const String &texture, const Uint16 index)
			{
				assert(get_object_selected());
				set_terrain_albedo_map(texture, index,
					get_id());
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

	};

}

#endif	/* UUID_e8300c87_0a6d_44dd_b202_82af4a234947 */
