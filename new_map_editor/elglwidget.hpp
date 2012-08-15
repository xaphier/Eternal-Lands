#ifndef	UUID_c7da8b8d_d0bd_44ad_a903_16889f710ce5
#define UUID_c7da8b8d_d0bd_44ad_a903_16889f710ce5

#include "prerequisites.hpp"
#include "editor/editor.hpp"
#include <QtOpenGL/QGLWidget>
#include <QtGui/QKeyEvent>

using namespace eternal_lands;

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
		glm::vec3 m_world_position;
		glm::vec3 m_grab_world_position;
		glm::vec3 m_move_offset;
		glm::uvec4 m_selection_rect;
		glm::uvec2 m_select_pos, m_half_size;
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
		bool m_object;
		bool m_light;
		bool m_mouse_click_action;
		bool m_mouse_move_action;
		bool m_grab_world_position_valid;
		BlendType m_blend;
		Qt::MouseButton m_click_button;
		Qt::KeyboardModifier m_wheel_zoom_x10;
		bool m_swap_wheel_zoom;

		inline bool get_terrain_editing() const
		{
			return m_terrain_editing;
		}

		inline Uint32 get_terrain_type_index() const
		{
			return m_terrain_type_index;
		}

		inline Uint32 get_terrain_layer_index() const
		{
			return m_terrain_layer_index;
		}

		void mouse_click_action();
		void mouse_move_action();
		void update_ortho();

	protected:
		virtual void initializeGL();
		virtual void resizeGL(int width, int height);
		virtual void paintGL();

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
		void set_light_position(const glm::vec3 &position);
		void set_light_color(const glm::vec3 &color);
		void set_object_translation(const glm::vec3 &translation);
		void set_object_rotation(const glm::vec3 &rotation);
		void set_object_scale(const float scale);
		void set_object_selection(const SelectionType selection);
		void set_object_blend(const BlendType blend);
		void set_object_transparency(const float transparency);
		void set_object_materials(const StringVector &materials);
		void add_object(const String &object);
		void add_light(const float radius);
		void set_dungeon(const bool dungeon);
		bool get_dungeon() const;
		void set_ambient(const glm::vec3 &color);
		const glm::vec3 &get_ambient() const;
		void set_fog(const glm::vec3 &color, const float density);
		QString get_blend_image_name() const;
		QStringList get_materials() const;
		QStringList get_default_materials(const String &name) const;
		QStringList get_debug_modes() const;
		void export_blend_image(const QString &file_name, const QString &codec) const;
		void export_terrain_map(const QString &file_name, const QString &codec) const;
		void import_terrain_map(const QString &file_name);
		QStringList get_terrain_albedo_maps() const;
		QString get_terrain_height_map() const;
		QString get_terrain_blend_map() const;
		QString get_terrain_dudv_map() const;
		static void get_codecs(QStringList &codecs);
		static void get_file_extensions_filter(QString &filter);
		static void get_file_extensions_filter(QString &filter,
			QString &default_extension, const QString &codec);
		void set_dirs(const QStringList &dirs);
		QImage get_icon(const QString &name);
		void terrain_layer_edit(const int terrain_layer_index,
			const float strength, const float radius,
			const int brush_type);
		void ground_tile_edit(const int tile);
		void water_tile_edit(const int water);
		void height_edit(const int height);
		void set_debug_mode(const int value);
		void change_terrain_values(const QVector3D &data,
			const QVector2D &size, const float attenuation_size,
			const int mask, const int attenuation,
			const int shape, const int effect);
		float get_terrain_offset_x() const;
		float get_terrain_offset_y() const;
		float get_terrain_offset_z() const;
		float get_terrain_offset_min_x() const;
		float get_terrain_offset_min_y() const;
		float get_terrain_offset_min_z() const;
		float get_terrain_offset_max_x() const;
		float get_terrain_offset_max_y() const;
		float get_terrain_offset_max_z() const;

		inline Qt::MouseButton get_click_button() const
		{
			return m_click_button;
		}

		inline Qt::KeyboardModifier get_wheel_zoom_x10() const
		{
			return m_wheel_zoom_x10;
		}

		inline bool get_swap_wheel_zoom() const
		{
			return m_swap_wheel_zoom;
		}

		inline void set_click_button(const Qt::MouseButton click_button)
		{
			m_click_button = click_button;
		}

		inline void set_wheel_zoom_x10(const Qt::KeyboardModifier wheel_zoom_x10)
		{
			m_wheel_zoom_x10 = wheel_zoom_x10;
		}

		inline void set_swap_wheel_zoom(const bool swap_wheel_zoom)
		{
			m_swap_wheel_zoom = swap_wheel_zoom;
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
		void set_light_radius(const double radius);
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
			const int blend_image_size_x, const int blend_image_size_y,
			const QStringList textures);
		void new_map(const QString &image, const int blend_image_size_x,
			const int blend_image_size_y, const QStringList textures);
		void set_terrain_editing(const bool enabled);
		void set_terrain_type_index(const int index);
		void set_terrain_layer_index(const int index);
		void set_terrain_albedo_map(const QString &name,
			const Uint32 index);
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
		void open_map(const QString &file_name);
		void set_blend_image_name(const QString &blend_image_name);
		void set_game_minute(const int game_minute);
		void set_draw_objects(const bool draw_objects);
		void set_draw_terrain(const bool draw_terrain);
		void set_draw_lights(const bool draw_lights);
		void set_draw_light_spheres(const bool draw_light_spheres);
		void set_lights_enabled(const bool enabled);
		void init_terrain(const int width, const int height);
		void set_object_walkable(const bool value);

	signals:
		void update_object(const bool select);
		void update_light(const bool select);
		void deselect();
		void can_undo(const bool undo);
		void terrain_edit();
		void initialized();

};

#endif	/* UUID_c7da8b8d_d0bd_44ad_a903_16889f710ce5 */
