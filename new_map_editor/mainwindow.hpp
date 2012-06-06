#ifndef	UUID_dcc59537_9e69_4663_8cda_6ae09d1b3722
#define UUID_dcc59537_9e69_4663_8cda_6ae09d1b3722

#include "ui_mapeditor.h"
#include <QSignalMapper>
#include <QSettings>
#include <QProgressBar>
#include <QListWidget>
#include "settingsdialog.hpp"
#include "timedialog.hpp"
#include "preferencesdialog.hpp"
#include "objectsdialog.hpp"
#include "qprogress.hpp"

class MainWindow: public QMainWindow, public Ui::MainWindow
{
	Q_OBJECT

	private:
		std::vector<QObject*> m_object_witdgets;
		std::vector<QObject*> m_light_witdgets;
		std::vector<QObject*> m_terrain_witdgets;
		std::vector<QComboBox*> m_material_witdgets;
		std::vector<QLabel*> m_material_label_witdgets;
		QSignalMapper* m_object_selection_mapper;
		QSignalMapper* m_object_transparency_mapper;
		QSpinBox* action_time;
		SettingsDialog* m_settings;
		PreferencesDialog* m_preferences;
		ObjectsDialog* m_objects;
		QString m_file_name;
		QAction* action_move_l;
		QAction* action_move_r;
		QAction* action_move_u;
		QAction* action_move_d;
		QAction* action_rotate_l;
		QAction* action_rotate_r;
		QAction* action_zoom_in;
		QAction* action_zoom_out;
		QProgressBar* m_progress_bar;
		boost::shared_ptr<QProgress> m_progress;
		QStringList m_textures;
		size_t m_material_count;

		void set_light_color(const glm::vec3 &color);
		void set_light_color(const QColor &color);
		void update_terrain();
		void set_terrain_albedo_map(const QString &str,
			const int index);
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
		void save_textures_settings(QSettings &settings);
		void load_textures_settings(QSettings &settings);
		void set_textures(const QStringList &textures);
		void set_blend(const BlendType value);
		void set_selection(const SelectionType value);
		void save_dirs_settings(QSettings &settings);
		void load_dirs_settings(QSettings &settings);

	private slots:
		void update_object();
		void about_el();
		void set_object_blend(const int);
		void set_object_selection(const int);

	protected:
		virtual void closeEvent(QCloseEvent* event);

	public:
		MainWindow(QWidget* parent = 0);
		virtual ~MainWindow();

	public slots:
		void update_translation();
		void update_rotation();
		void update_scale();
		void update_position();
		void update_materials();
		void update_object(const bool select);
		void update_light(const bool select);
		void deselect();
		void change_light_color();
		void add_object(const bool value);
		void add_light(const bool value);
		void remove();
		void height_mode(const bool checked);
		void set_terrain_albedo_map(const int index);
		void set_fog();
		void open_map();
		void save();
		void save_as();
		void change_ambient();
		void change_blend_image_name();
		void new_map();
		void change_preferences();
		void terrain_edit(const int x, const int y);
		void initialized();

};

#endif	/* UUID_dcc59537_9e69_4663_8cda_6ae09d1b3722 */
