#ifndef	UUID_dcc59537_9e69_4663_8cda_6ae09d1b3722
#define UUID_dcc59537_9e69_4663_8cda_6ae09d1b3722

#include "ui_mapeditor.h"
#include <QSignalMapper>
#include <QSettings>
#include <QProgressBar>
#include <QListWidget>
#include <QProgressDialog>
#include "settingsdialog.hpp"
#include "timedialog.hpp"
#include "preferencesdialog.hpp"
#include "objectsdialog.hpp"
#include "objectsdialog.hpp"
#include "terraintexturesdialog.hpp"
#include "allterraintexturesdialog.hpp"
#include "loadmapdialog.hpp"
#include "initterraindialog.hpp"
#include "terraintranslationdialog.hpp"
#include "terraintexturedata.hpp"
#include "qprogress.hpp"
#include "nodes/texturedialog.hpp"

namespace el = eternal_lands;

class QNodesEditor;

class MainWindow: public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

	private:
		enum TextureLayerRoles
		{
			tlr_index = Qt::UserRole + 128
		};

		QVector<QObject*> m_object_witdgets;
		QVector<QObject*> m_light_witdgets;
		QVector<QObject*> m_terrain_witdgets;
		QVector<QComboBox*> m_material_witdgets;
		QVector<QLabel*> m_material_label_witdgets;
		QVector<QDoubleSpinBox*> m_rotations;
		QVector<QDial*> m_rotation_dials;
		QSignalMapper* m_rotation_mapper;
		QSignalMapper* m_rotation_dial_mapper;
		QSignalMapper* m_object_selection_mapper;
		QSignalMapper* m_object_transparency_mapper;
		QSpinBox* action_time;
		SettingsDialog* m_settings;
		PreferencesDialog* m_preferences;
		ObjectsDialog* m_objects;
		TerrainTexturesDialog* m_terrain_textures;
		AllTerrainTexturesDialog* m_all_terrain_textures;
		LoadMapDialog* m_load_map;
		InitTerrainDialog* m_init_terrain;
		TerrainTranslationDialog* m_terrain_translation;
		QString m_file_name;
		QAction* action_move_l;
		QAction* action_move_r;
		QAction* action_move_u;
		QAction* action_move_d;
		QAction* action_zoom_in;
		QAction* action_zoom_out;
		QButtonGroup* m_grid_layers;
		QProgressBar* m_progress_bar;
		boost::shared_ptr<QProgress> m_progress;
		QProgressDialog* m_progress_dialog;
		boost::shared_ptr<QProgress> m_el_progress;
		QStringTerrainTextureDataQMap m_terrain_texture_datas;
		QSize m_preview_icons_size;
		QSize m_terrain_texture_size;
		size_t m_material_count;
		TextureDialog* m_texture_dialog;
		QSignalMapper* m_texture_unit_mapper;
		QNodesEditor *m_nodes_editor;
		QStringList m_sampler_names;
		el::EffectNodesSharedPtr m_effect_nodes;
		QString m_file_name_nodes;
		QString m_el_data_dir;
		QString m_el_extra_data_dir;
		bool m_changed_nodes;

		void set_light_color(const glm::vec3 &color);
		void set_light_color(const QColor &color);
		void update_terrain();
		void add_item(const QString &str, QComboBox* combobox);
		void set_items(const QStringList &strs, QComboBox* combobox);
		void set_default_mode();
		void do_save();
		void init_actions();
		void save_settings();
		void load_settings();
		void save_shortcuts(QSettings &settings);
		void load_shortcuts(QSettings &settings);
		void save_mouse_settings(QSettings &settings);
		void load_mouse_settings(QSettings &settings);
		void save_terrain_textures_settings(QSettings &settings);
		void load_terrain_textures_settings(QSettings &settings);
		void set_blend(const BlendType value,
			const BitSet64 blend_mask);
		void set_selection(const SelectionType value);
		void save_dirs_settings(QSettings &settings);
		void load_dirs_settings(QSettings &settings);
		void terrain_displacement_edit();
		void terrain_blend_edit();
		void save_nodes(const QString &file_name);
		void load_nodes(const QString &file_name);
		bool check_save_nodes();
		void set_terrain_blend_data();
		void set_dirs();
		bool get_terrain_texture_icon(
			TerrainTextureData &terrain_texture_data);
		bool get_terrain_texture_data(const QString &name,
			TerrainTextureData &terrain_texture_data) const;
		QColor get_light_color() const;
		void update_tile_materials();

	private slots:
		void update_object();
		void about_el();
		void set_object_blend(const int);
		void set_object_selection(const int);
		void update_translation();
		void update_position();
		void update_scale();
		void update_scale_x();
		void update_scale_y();
		void update_scale_z();
		void update_scale_slider();
		void update_scale_slider_x();
		void update_scale_slider_y();
		void update_scale_slider_z();
		void update_mirror();
		void update_materials();
		void update_object(const bool select);
		void update_light(const bool select);
		void update_terrain(const bool enabled);
		void deselect();
		void change_light_color();
		void add_objects(const bool value);
		void add_lights(const bool value);
		void terrain_mode(const bool checked);
		void tile_mode(const bool checked);
		void delete_mode(const bool checked);
		void remove();
		void set_fog();
		void open_map();
		void save();
		void save_as();
		void change_ground_hemisphere();
		void new_map();
		void change_preferences();
		void terrain_edit();
		void update_terrain_layers();
		void set_debug(const bool enabled);
		void set_object_transparency(const int value);
		void update_rotation(const int index);
		void update_rotation_dial(const int index);
		void change_dungeon();
		void rotate_left();
		void rotate_right();
		void roll_up();
		void roll_down();
		void add_color();
		void add_direction();
		void add_constant();
		void add_function();
		void add_geometric_function();
		void add_trigonemetric_function();
		void add_parameter();
		void add_texture();
		void add_output();
		void changed_nodes();
		void new_nodes();
		void load_nodes();
		void save_nodes();
		void save_as_nodes();
		void generate();
		void layers_double_clicked(QListWidgetItem* item);
		void initialized();
		void change_terrain_textures();
		void import_terrain_height_map();
		void import_terrain_blend_map();
		void remove_all_copies_of_object();
		void relax_terrain_uv();
		void show_used_terrain_layers();
		void terrain_displacement(const QVector3D &displacement);
		void terrain_normal(const QVector3D &normal);
		void replace_all_copies_of_object();
		void fill_terrain_blend_layer();
		void change_light_power();
		void change_terrain_translation();
		void remove_terrain();
		void tile_edit();

	protected:
		virtual void closeEvent(QCloseEvent* event);

	public:
		MainWindow(QWidget* parent = 0);
		virtual ~MainWindow();

	signals:
		void get_albedo_map_data(const QString &name,
			const QSize &icon_size,	const QSize &image_size,
			QIcon &icon, bool &use_blend_size_texture, bool &ok);
		void get_specular_map_data(const QString &name,
			const QSize &image_size, bool &ok);
		void get_gloss_map_data(const QString &name,
			const QSize &image_size, bool &ok);
		void get_height_map_data(const QString &name,
			const QSize &image_size, bool &ok);

};

#endif	/* UUID_dcc59537_9e69_4663_8cda_6ae09d1b3722 */
