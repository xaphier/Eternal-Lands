#ifndef	UUID_c7da8b8d_d0bd_44ad_a903_16889f710ce5
#define UUID_c7da8b8d_d0bd_44ad_a903_16889f710ce5

#include "prerequisites.hpp"
#include "editor/editor.hpp"
#include <QGLWidget>
#include <QKeyEvent>
#include <QBitArray>

using namespace eternal_lands;

enum KeyPressType
{
	kpt_nothing,
	kpt_rotate_x,
	kpt_rotate_y,
	kpt_rotate_z,
	kpt_scale
};

enum TerrainPickingType
{
	tpt_nothing,
	tpt_displacement,
	tpt_normal
};

class ELGLWidget: public QGLWidget
{
	Q_OBJECT

	private:
		boost::scoped_ptr<Editor> m_editor;
		GlobalVarsSharedPtr m_global_vars;
		FileSystemSharedPtr m_file_system;
		QStringList m_dirs;
		QTimer* m_timer;
		glm::mat3 m_camera_yaw_rotate;
		glm::vec3 m_pos;
		glm::vec3 m_dir;
		glm::vec3 m_start_position;
		glm::vec3 m_world_position;
		glm::vec3 m_grab_world_position;
		glm::vec3 m_move_offset;
		glm::vec3 m_rotation_offset;
		glm::vec3 m_scale_offset;
		glm::uvec4 m_selection_rect;
		glm::uvec2 m_select_pos, m_half_size;
		glm::uvec2 m_selected_screen_position;
		glm::uvec2 m_press_screen_position;
		glm::vec3 m_press_pos;
		glm::vec2 m_camera_yaw_roll;
		String m_object_name;
		float m_light_radius;
		float m_zoom;
		float m_aspect;
		int m_camera_roll;
		int m_camera_yaw;
		Uint32 m_terrain_type_index;
		Uint32 m_terrain_layer_index;
		bool m_select;
		bool m_select_depth;
		bool m_terrain_editing;
		bool m_object_adding;
		bool m_light_adding;
		bool m_mouse_click_action;
		bool m_mouse_move_action;
		bool m_grab_world_position_valid;
		TerrainPickingType m_terrain_picking;
		KeyPressType m_mouse_move;
		BlendType m_blend;
		Qt::MouseButton m_press_button;
		Qt::MouseButton m_select_rect_button;
		Qt::MouseButton m_click_button;
		Qt::MouseButton m_grab_button;
		Qt::MouseButton m_view_rotate_button;
		Qt::KeyboardModifier m_press_key_mod;
		Qt::KeyboardModifier m_select_rect_key_mod;
		Qt::KeyboardModifier m_click_key_mod;
		Qt::KeyboardModifier m_grab_key_mod;
		Qt::KeyboardModifier m_view_rotate_key_mod;
		Qt::KeyboardModifier m_wheel_zoom_x10;
		Qt::Key m_rotate_x_key;
		Qt::Key m_rotate_y_key;
		Qt::Key m_rotate_z_key;
		Qt::Key m_scale_key;
		bool m_swap_wheel_zoom;
		bool m_invert_z_rotation;

		inline bool get_terrain_editing() const
		{
			return m_terrain_editing;
		}

		inline bool get_object_adding() const
		{
			return m_object_adding;
		}

		inline bool get_light_adding() const
		{
			return m_light_adding;
		}

		inline Uint32 get_terrain_type_index() const
		{
			return m_terrain_type_index;
		}

		inline Uint32 get_terrain_layer_index() const
		{
			return m_terrain_layer_index;
		}

		bool get_select_rect_event(QMouseEvent *event) const
		{
			return event->buttons().testFlag(
				get_select_rect_button()) &&
				(event->modifiers().testFlag(
					get_select_rect_key_mod()) ||
				(get_select_rect_key_mod() == Qt::NoModifier));
		}

		bool get_click_event(QMouseEvent *event) const
		{
			return event->buttons().testFlag(get_click_button()) &&
				(event->modifiers().testFlag(
					get_click_key_mod()) ||
				(get_click_key_mod() == Qt::NoModifier));
		}

		bool get_grab_event(QMouseEvent *event) const
		{
			return event->buttons().testFlag(get_grab_button()) &&
				(event->modifiers().testFlag(
					get_grab_key_mod()) ||
				(get_grab_key_mod() == Qt::NoModifier));
		}

		bool get_view_rotate_event(QMouseEvent *event) const
		{
			return event->buttons().testFlag(
				get_view_rotate_button()) &&
				(event->modifiers().testFlag(
					get_view_rotate_key_mod()) ||
				(get_view_rotate_key_mod() == Qt::NoModifier));
		}

		void mouse_click_action();
		void mouse_move_action();
		void update_ortho();

	protected:
		virtual void initializeGL();
		virtual void resizeGL(int width, int height);
		virtual void paintGL();
		virtual void keyPressEvent(QKeyEvent *event);
		virtual void keyReleaseEvent(QKeyEvent *event);
		virtual void mouseReleaseEvent(QMouseEvent *event);
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void mouseMoveEvent(QMouseEvent *event);
		virtual void wheelEvent(QWheelEvent *event);

	public:
		ELGLWidget(QWidget *parent = 0);
		virtual ~ELGLWidget();
		void get_object_description(
			EditorObjectDescription &object_description) const;
		RenderableType get_renderable() const;
		glm::vec3 get_light_color() const;
		void get_light_data(LightData &light) const;
		void set_light_color(const glm::vec3 &color);
		void set_object_selection(const SelectionType selection);
		void set_object_blend(const BlendType blend);
		void set_object_transparency(const float transparency);
		void set_object_materials(const StringVector &materials);
		void set_lights_color(const glm::vec3 &color);
		void set_translation(const glm::vec3 &translation);
		void set_rotation(const glm::vec3 &rotation);
		void set_scale(const glm::vec3 &scale);
		void set_objects_selection(const SelectionType selection);
		void set_objects_blend(const BlendType blend);
		void set_objects_transparency(const float transparency);
		void set_objects_materials(const StringVector &materials);
		void add_object(const String &object);
		void add_light(const float radius);
		void set_dungeon(const bool dungeon);
		bool get_dungeon() const;
		void set_ambient(const glm::vec3 &color);
		const glm::vec3 &get_ambient() const;
		void set_fog(const glm::vec3 &color, const float density);
		void set_terrain_material(const QString &albedo_map,
			const QString &extra_map, const float blend_size,
			const bool use_blend_size_sampler,
			const bool use_blend_size, const bool use_extra_map,
			const int index);
		void get_terrain_material(QString &albedo_map,
			QString &extra_map, float &blend_size,
			bool &use_blend_size_sampler,
			bool &use_blend_size, bool &use_extra_map,
			const int index) const;
		QStringList get_materials() const;
		QStringList get_default_materials(const String &name) const;
		QStringList get_debug_modes() const;
		void export_blend_image(const QString &file_name,
			const QString &codec) const;
		void export_terrain_map(const QString &file_name,
			const QString &codec) const;
		void import_terrain_map(const QString &file_name);
		QString get_terrain_height_map() const;
		QString get_terrain_blend_map() const;
		QString get_terrain_dudv_map() const;
		static void get_codecs(QStringList &codecs);
		static void get_file_extensions_filter(QString &filter);
		static void get_file_extensions_filter(QString &filter,
			QString &default_extension, const QString &codec);
		void set_dirs(const QStringList &dirs);
		void change_terrain_blend_values(const QVector2D &size,
			const float attenuation_size, const float strength,
			const int attenuation, const int shape,
			const int effect, const int layer);
		void ground_tile_edit(const int tile);
		void water_tile_edit(const int water);
		void height_edit(const int height);
		void set_debug_mode(const int value);
		void change_terrain_displacement_values(const QVector3D &data,
			const QVector2D &size, const float attenuation_size,
			const int mask, const int attenuation,
			const int shape, const int effect);
		static QVector3D get_terrain_offset_min();
		static QVector3D get_terrain_offset_max();
		void relax_terrain_uv(
			const AbstractProgressSharedPtr &progress,
			const int count);
		void import_terrain_height_map(const QString &name);
		void import_terrain_blend_map(const QString &name);
		void load_map(const QString &file_name,
			const bool load_2d_objects, const bool load_3d_objects,
			const bool load_lights, const bool load_particles,
			const bool load_materials, const bool load_height_map,
			const bool load_tile_map, const bool load_walk_map,
			const bool load_terrain, const bool load_water);
		bool get_terrain() const;
		int get_terrain_layer_count() const;
		QSize get_map_size() const;
		QVector3D get_map_min() const;
		QVector3D get_map_max() const;
		void get_terrain_layers_usage(QVector<int> &use_layer_pixels,
			int &pixels) const;
		void set_all_copies_of_object_name(const String &name);
		void fill_terrain_blend_layer(const float strength,
			const int effect, const int layer);

		inline Qt::MouseButton get_press_button() const
		{
			return m_press_button;
		}

		inline Qt::MouseButton get_select_rect_button() const
		{
			return m_select_rect_button;
		}

		inline Qt::MouseButton get_click_button() const
		{
			return m_click_button;
		}

		inline Qt::MouseButton get_grab_button() const
		{
			return m_grab_button;
		}

		inline Qt::MouseButton get_view_rotate_button() const
		{
			return m_view_rotate_button;
		}

		inline Qt::KeyboardModifier get_press_key_mod() const
		{
			return m_press_key_mod;
		}

		inline Qt::KeyboardModifier get_select_rect_key_mod() const
		{
			return m_select_rect_key_mod;
		}

		inline Qt::KeyboardModifier get_click_key_mod() const
		{
			return m_click_key_mod;
		}

		inline Qt::KeyboardModifier get_grab_key_mod() const
		{
			return m_grab_key_mod;
		}

		inline Qt::KeyboardModifier get_view_rotate_key_mod() const
		{
			return m_view_rotate_key_mod;
		}

		inline Qt::KeyboardModifier get_wheel_zoom_x10() const
		{
			return m_wheel_zoom_x10;
		}

		inline Qt::Key get_rotate_x_key() const
		{
			return m_rotate_x_key;
		}

		inline Qt::Key get_rotate_y_key() const
		{
			return m_rotate_y_key;
		}

		inline Qt::Key get_rotate_z_key() const
		{
			return m_rotate_z_key;
		}

		inline Qt::Key get_scale_key() const
		{
			return m_scale_key;
		}

		inline bool get_swap_wheel_zoom() const
		{
			return m_swap_wheel_zoom;
		}

		inline bool get_invert_z_rotation() const
		{
			return m_invert_z_rotation;
		}

		inline void set_press_button(const Qt::MouseButton press_button)
		{
			m_press_button = press_button;
		}

		inline void set_select_rect_button(
			const Qt::MouseButton select_rect_button)
		{
			m_select_rect_button = select_rect_button;
		}

		inline void set_click_button(const Qt::MouseButton click_button)
		{
			m_click_button = click_button;
		}

		inline void set_grab_button(const Qt::MouseButton grab_button)
		{
			m_grab_button = grab_button;
		}

		inline void set_view_rotate_button(
			const Qt::MouseButton view_rotate_button)
		{
			m_view_rotate_button = view_rotate_button;
		}

		inline void set_press_key_mod(
			const Qt::KeyboardModifier press_key_mod)
		{
			m_press_key_mod = press_key_mod;
		}

		inline void set_select_rect_key_mod(
			const Qt::KeyboardModifier select_rect_key_mod)
		{
			m_select_rect_key_mod = select_rect_key_mod;
		}

		inline void set_click_key_mod(
			const Qt::KeyboardModifier click_key_mod)
		{
			m_click_key_mod = click_key_mod;
		}

		inline void set_grab_key_mod(
			const Qt::KeyboardModifier grab_key_mod)
		{
			m_grab_key_mod = grab_key_mod;
		}

		inline void set_view_rotate_key_mod(
			const Qt::KeyboardModifier view_rotate_key_mod)
		{
			m_view_rotate_key_mod = view_rotate_key_mod;
		}

		inline void set_wheel_zoom_x10(
			const Qt::KeyboardModifier wheel_zoom_x10)
		{
			m_wheel_zoom_x10 = wheel_zoom_x10;
		}

		inline void set_rotate_x_key(const Qt::Key rotate_x_key)
		{
			m_rotate_x_key = rotate_x_key;
		}

		inline void set_rotate_y_key(const Qt::Key rotate_y_key)
		{
			m_rotate_y_key = rotate_y_key;
		}

		inline void set_rotate_z_key(const Qt::Key rotate_z_key)
		{
			m_rotate_z_key = rotate_z_key;
		}

		inline void set_scale_key(const Qt::Key scale_key)
		{
			m_scale_key = scale_key;
		}

		inline void set_swap_wheel_zoom(const bool swap_wheel_zoom)
		{
			m_swap_wheel_zoom = swap_wheel_zoom;
		}

		inline void set_invert_z_rotation(const bool invert_z_rotation)
		{
			m_invert_z_rotation = invert_z_rotation;
		}

		inline const QStringList &get_dirs() const
		{
			return m_dirs;
		}

		inline const GlobalVarsSharedPtr &get_global_vars() const
		{
			return m_global_vars;
		}

		inline const FileSystemSharedPtr &get_file_system() const
		{
			return m_file_system;
		}

	public slots:
		void remove_object();
		void remove_light();
		void remove_all_copies_of_object();
		void set_light_radius(const double radius);
		void set_lights_radius(const double radius);
		void undo();
		void set_wire_frame(const bool enabled);
		void move_left();
		void move_right();
		void move_up();
		void move_down();
		void rotate_yaw(const int angle);
		void rotate_roll(const int angle);
		void zoom_in();
		void zoom_out();
		void new_map(const int map_size_x, const int map_size_y,
			const int blend_image_size_x,
			const int blend_image_size_y,
			const QStringList textures);
		void new_map(const QString &image,
			const int blend_image_size_x,
			const int blend_image_size_y,
			const QStringList textures);
		void set_terrain_editing(const bool enabled);
		void set_terrain_type_index(const int index);
		void set_terrain_layer_index(const int index);
		void set_random_translation_x(const bool value);
		void set_random_translation_y(const bool value);
		void set_random_translation_z(const bool value);
		void set_random_translation_x_min(const double value);
		void set_random_translation_y_min(const double value);
		void set_random_translation_z_min(const double value);
		void set_random_translation_x_max(const double value);
		void set_random_translation_y_max(const double value);
		void set_random_translation_z_max(const double value);
		void set_random_rotation_x(const bool value);
		void set_random_rotation_y(const bool value);
		void set_random_rotation_z(const bool value);
		void set_random_rotation_x_min(const double value);
		void set_random_rotation_y_min(const double value);
		void set_random_rotation_z_min(const double value);
		void set_random_rotation_x_max(const double value);
		void set_random_rotation_y_max(const double value);
		void set_random_rotation_z_max(const double value);
		void set_random_scale(const bool value);
		void set_random_scale_min(const double value);
		void set_random_scale_max(const double value);
		void disable_object();
		void disable_light();
		void save(const QString &name) const;
		void set_draw_objects(const bool draw_objects);
		void set_draw_terrain(const bool draw_terrain);
		void set_draw_lights(const bool draw_lights);
		void set_draw_light_spheres(const bool draw_light_spheres);
		void set_draw_heights(const bool draw_heights);
		void set_lights_enabled(const bool enabled);
		void init_terrain(const QSize &size, const QString &albedo_map,
			const QString &extra_map,
			const bool use_blend_size_sampler,
			const bool use_extra_map);
		void init_terrain(const QString &height_map, const QSize &size,
			const QString &albedo_map, const QString &extra_map,
			const bool use_blend_size_sampler,
			const bool use_extra_map);
		void set_object_walkable(const bool value);
		void set_objects_walkable(const bool value);
		void get_albedo_map_data(const QString &name,
			const QSize &icon_size, const QSize &image_size,
			QIcon &icon, bool &use_blend_size_sampler, bool &ok);
		void get_extra_map_data(const QString &name,
			const QSize &image_size, bool &ok);
		void get_image_data(const QString &name, QSize &size, bool &ok);
		void update_terrain_dudv();
		void clear_invisible_terrain_layers();
		void pack_terrain_layers();
		void pick_terrain_displacement();
		void pick_terrain_normal();

	signals:
		void update_object(const bool select);
		void update_light(const bool select);
		void update_terrain(const bool enabled);
		void deselect();
		void can_undo(const bool undo);
		void terrain_edit();
		void initialized();
		void changed_camera_yaw(const int yaw);
		void changed_camera_roll(const int roll);
		void terrain_displacement(const QVector3D &displacement);
		void terrain_normal(const QVector3D &normal);

};

#endif	/* UUID_c7da8b8d_d0bd_44ad_a903_16889f710ce5 */
