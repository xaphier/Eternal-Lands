#include "mainwindow.hpp"
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QVector2D>
#include <QMessageBox>
#include <QThreadPool>
#include "newmapdialog.hpp"
#include "lightdata.hpp"
#include "editor/editorobjectdescription.hpp"
#include "editor/editor.hpp"
#include "nodes/qnodeseditor.hpp"
#include "nodes/node.hpp"
#include "nodes/colornode.hpp"
#include "nodes/directionnode.hpp"
#include "nodes/texturenode.hpp"
#include "nodes/valuesnode.hpp"
#include "effect/effectnodes.hpp"
#include "effect/effectnode.hpp"
#include "effect/effectnodeport.hpp"
#include "effect/effectconstant.hpp"
#include "effect/effectfunction.hpp"
#include "effect/effectparameter.hpp"
#include "effect/effecttexture.hpp"
#include "effect/effectoutput.hpp"
#include "shader/samplerparameterutil.hpp"
#include "shader/shadersourceparameter.hpp"
#include "blenddata.hpp"
#include "texturetargetutil.hpp"
#include "logging.hpp"
#include "relaxuv.hpp"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
	setupUi(this);

	init_actions();

	m_material_count = 0;

	m_settings = new SettingsDialog(this);
	m_preferences = new PreferencesDialog(this);
	m_objects = new ObjectsDialog(this);
	m_terrain_textures = new TerrainTexturesDialog(m_terrain_texture_datas,
		this);
	m_all_terrain_textures = new AllTerrainTexturesDialog(m_el_data_dir,
		m_el_extra_data_dir, this);
	m_load_map = new LoadMapDialog(this);
	m_init_terrain = new InitTerrainDialog(this);

	action_time = new QSpinBox(this);
	action_time->setMaximum(359);
	action_time->setWrapping(true);
	action_time->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
	toolbar->insertWidget(action_ground_hemisphere, action_time);

	QObject::connect(action_add_objects, SIGNAL(toggled(bool)), this, SLOT(add_objects(bool)));
	QObject::connect(action_add_lights, SIGNAL(toggled(bool)), this, SLOT(add_lights(bool)));
	QObject::connect(action_wire_frame, SIGNAL(toggled(bool)), el_gl_widget, SLOT(set_wire_frame(bool)));

	QObject::connect(el_gl_widget, SIGNAL(update_object(bool)), this, SLOT(update_object(bool)));
	QObject::connect(el_gl_widget, SIGNAL(update_light(bool)), this, SLOT(update_light(bool)));
	QObject::connect(el_gl_widget, SIGNAL(update_terrain(bool)), this, SLOT(update_terrain(bool)));
	QObject::connect(el_gl_widget, SIGNAL(deselect()), this, SLOT(deselect()));

	QObject::connect(x_translation, SIGNAL(valueChanged(double)), this, SLOT(update_translation()));
	QObject::connect(y_translation, SIGNAL(valueChanged(double)), this, SLOT(update_translation()));
	QObject::connect(z_translation, SIGNAL(valueChanged(double)), this, SLOT(update_translation()));

	QObject::connect(scale_value, SIGNAL(valueChanged(double)), this, SLOT(update_scale()));
	QObject::connect(scale_slider, SIGNAL(valueChanged(int)), this, SLOT(update_scale_slider()));
	QObject::connect(scale_value_x, SIGNAL(valueChanged(double)), this, SLOT(update_scale_x()));
	QObject::connect(scale_value_y, SIGNAL(valueChanged(double)), this, SLOT(update_scale_y()));
	QObject::connect(scale_value_z, SIGNAL(valueChanged(double)), this, SLOT(update_scale_z()));
	QObject::connect(scale_slider_x, SIGNAL(valueChanged(int)), this, SLOT(update_scale_slider_x()));
	QObject::connect(scale_slider_y, SIGNAL(valueChanged(int)), this, SLOT(update_scale_slider_y()));
	QObject::connect(scale_slider_z, SIGNAL(valueChanged(int)), this, SLOT(update_scale_slider_z()));
	QObject::connect(mirror_x, SIGNAL(toggled(bool)), this, SLOT(update_mirror()));
	QObject::connect(mirror_y, SIGNAL(toggled(bool)), this, SLOT(update_mirror()));
	QObject::connect(mirror_z, SIGNAL(toggled(bool)), this, SLOT(update_mirror()));

	m_rotations.push_back(x_rotation);
	m_rotations.push_back(y_rotation);
	m_rotations.push_back(z_rotation);

	m_rotation_mapper = new QSignalMapper(this);
	m_rotation_mapper->setMapping(m_rotations[0], int(0));
	m_rotation_mapper->setMapping(m_rotations[1], int(1));
	m_rotation_mapper->setMapping(m_rotations[2], int(2));

	QObject::connect(m_rotations[0], SIGNAL(valueChanged(double)),
		m_rotation_mapper, SLOT(map()));
	QObject::connect(m_rotations[1], SIGNAL(valueChanged(double)),
		m_rotation_mapper, SLOT(map()));
	QObject::connect(m_rotations[2], SIGNAL(valueChanged(double)),
		m_rotation_mapper, SLOT(map()));
	QObject::connect(m_rotation_mapper, SIGNAL(mapped(const int)),
		this, SLOT(update_rotation(const int)));

	m_rotation_dials.push_back(x_rotation_dial);
	m_rotation_dials.push_back(y_rotation_dial);
	m_rotation_dials.push_back(z_rotation_dial);

	m_rotation_dial_mapper = new QSignalMapper(this);
	m_rotation_dial_mapper->setMapping(m_rotation_dials[0], int(0));
	m_rotation_dial_mapper->setMapping(m_rotation_dials[1], int(1));
	m_rotation_dial_mapper->setMapping(m_rotation_dials[2], int(2));

	QObject::connect(m_rotation_dials[0], SIGNAL(valueChanged(int)),
		m_rotation_dial_mapper, SLOT(map()));
	QObject::connect(m_rotation_dials[1], SIGNAL(valueChanged(int)),
		m_rotation_dial_mapper, SLOT(map()));
	QObject::connect(m_rotation_dials[2], SIGNAL(valueChanged(int)),
		m_rotation_dial_mapper, SLOT(map()));
	QObject::connect(m_rotation_dial_mapper, SIGNAL(mapped(const int)),
		this, SLOT(update_rotation_dial(const int)));

	QObject::connect(radius, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_light_radius(double)));
	QObject::connect(light_color, SIGNAL(clicked()), this, SLOT(change_light_color()));
	QObject::connect(x_position, SIGNAL(valueChanged(double)), this, SLOT(update_position()));
	QObject::connect(y_position, SIGNAL(valueChanged(double)), this, SLOT(update_position()));
	QObject::connect(z_position, SIGNAL(valueChanged(double)), this, SLOT(update_position()));

	QObject::connect(el_gl_widget, SIGNAL(terrain_edit()), this,
		SLOT(terrain_edit()));

	QObject::connect(action_undo, SIGNAL(triggered()), el_gl_widget, SLOT(undo()));
	QObject::connect(el_gl_widget, SIGNAL(can_undo(bool)), action_undo, SLOT(setEnabled(bool)));

	QObject::connect(action_terrain_mode, SIGNAL(toggled(bool)), this, SLOT(terrain_mode(bool)));
	QObject::connect(action_terrain_mode, SIGNAL(toggled(bool)), el_gl_widget, SLOT(set_terrain_editing(bool)));

	QObject::connect(action_delete_mode, SIGNAL(toggled(bool)), this, SLOT(delete_mode(bool)));

	QObject::connect(action_remove, SIGNAL(triggered()), this, SLOT(remove()));
	QObject::connect(action_dungeon, SIGNAL(toggled(bool)), this, SLOT(change_dungeon()));

	QObject::connect(move_l, SIGNAL(clicked()), action_move_l, SLOT(trigger()));
	QObject::connect(move_r, SIGNAL(clicked()), action_move_r, SLOT(trigger()));
	QObject::connect(move_u, SIGNAL(clicked()), action_move_u, SLOT(trigger()));
	QObject::connect(move_d, SIGNAL(clicked()), action_move_d, SLOT(trigger()));
	QObject::connect(camera_yaw, SIGNAL(valueChanged(int)), el_gl_widget,
		SLOT(rotate_yaw(int)));
	QObject::connect(camera_roll, SIGNAL(valueChanged(int)), el_gl_widget,
		SLOT(rotate_roll(int)));
	QObject::connect(zoom_in, SIGNAL(clicked()), action_zoom_in, SLOT(trigger()));
	QObject::connect(zoom_out, SIGNAL(clicked()), action_zoom_out, SLOT(trigger()));

	QObject::connect(action_new, SIGNAL(triggered()), this, SLOT(new_map()));
	QObject::connect(action_open, SIGNAL(triggered()), this, SLOT(open_map()));
	QObject::connect(action_fog, SIGNAL(triggered()), this, SLOT(set_fog()));
	QObject::connect(action_ground_hemisphere, SIGNAL(triggered()), this, SLOT(change_ground_hemisphere()));
//	QObject::connect(action_time, SIGNAL(valueChanged(int)), el_gl_widget,
//		SLOT(set_game_minute(int)));
	QObject::connect(action_preferences, SIGNAL(triggered()), this,
		SLOT(change_preferences()));
	QObject::connect(action_terrain_textures, SIGNAL(triggered()), this,
		SLOT(change_terrain_textures()));

	QObject::connect(action_objects, SIGNAL(toggled(bool)),
		el_gl_widget, SLOT(set_draw_objects(bool)));
	QObject::connect(action_terrain, SIGNAL(toggled(bool)),
		el_gl_widget, SLOT(set_draw_terrain(bool)));
	QObject::connect(action_lights, SIGNAL(toggled(bool)),
		el_gl_widget, SLOT(set_draw_lights(bool)));
	QObject::connect(action_light_spheres, SIGNAL(toggled(bool)),
		el_gl_widget, SLOT(set_draw_light_spheres(bool)));
	QObject::connect(action_lights_enabled, SIGNAL(toggled(bool)),
		el_gl_widget, SLOT(set_lights_enabled(bool)));

	QObject::connect(action_heights, SIGNAL(toggled(bool)),
		el_gl_widget, SLOT(set_draw_heights(bool)));

	m_object_selection_mapper = new QSignalMapper(this);
	m_object_selection_mapper->setMapping(selection_type_0, int(0));
	m_object_selection_mapper->setMapping(selection_type_1, int(1));
	m_object_selection_mapper->setMapping(selection_type_2, int(2));

	QObject::connect(selection_type_0, SIGNAL(clicked()),
		m_object_selection_mapper, SLOT(map()));
	QObject::connect(selection_type_1, SIGNAL(clicked()),
		m_object_selection_mapper, SLOT(map()));
	QObject::connect(selection_type_2, SIGNAL(clicked()),
		m_object_selection_mapper, SLOT(map()));
	QObject::connect(m_object_selection_mapper, SIGNAL(mapped(const int)),
		this, SLOT(set_object_selection(const int)));

	m_object_transparency_mapper = new QSignalMapper(this);
	m_object_transparency_mapper->setMapping(transparency_type_0, int(0));
	m_object_transparency_mapper->setMapping(transparency_type_1, int(1));
	m_object_transparency_mapper->setMapping(transparency_type_2, int(2));

	QObject::connect(transparency_type_0, SIGNAL(clicked()),
		m_object_transparency_mapper, SLOT(map()));
	QObject::connect(transparency_type_1, SIGNAL(clicked()),
		m_object_transparency_mapper, SLOT(map()));
	QObject::connect(transparency_type_2, SIGNAL(clicked()),
		m_object_transparency_mapper, SLOT(map()));
	QObject::connect(m_object_transparency_mapper,
		SIGNAL(mapped(const int)), this,
		SLOT(set_object_blend(const int)));

	QObject::connect(walkable, SIGNAL(toggled(const bool)), el_gl_widget,
		SLOT(set_object_walkable(const bool)));

	QObject::connect(transparency_value, SIGNAL(valueChanged(int)), this,
		SLOT(set_object_transparency(const int)));

	QObject::connect(material_0, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));
	QObject::connect(material_1, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));
	QObject::connect(material_2, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));
	QObject::connect(material_3, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));
	QObject::connect(material_4, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));
	QObject::connect(material_5, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));
	QObject::connect(material_6, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));
	QObject::connect(material_7, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));
	QObject::connect(material_8, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));
	QObject::connect(material_9, SIGNAL(currentIndexChanged(int)), this,
		SLOT(update_materials()));

	QObject::connect(translate_x_group, SIGNAL(clicked(bool)),
		el_gl_widget, SLOT(set_random_translation_x(bool)));
	QObject::connect(translate_y_group, SIGNAL(clicked(bool)),
		el_gl_widget, SLOT(set_random_translation_y(bool)));
	QObject::connect(translate_z_group, SIGNAL(clicked(bool)),
		el_gl_widget, SLOT(set_random_translation_z(bool)));
	QObject::connect(translate_x_min, SIGNAL(valueChanged(double)),
		el_gl_widget, SLOT(set_random_translation_x_min(double)));
	QObject::connect(translate_y_min, SIGNAL(valueChanged(double)),
		el_gl_widget, SLOT(set_random_translation_y_min(double)));
	QObject::connect(translate_z_min, SIGNAL(valueChanged(double)),
		el_gl_widget, SLOT(set_random_translation_z_min(double)));
	QObject::connect(translate_x_max, SIGNAL(valueChanged(double)),
		el_gl_widget, SLOT(set_random_translation_x_max(double)));
	QObject::connect(translate_y_max, SIGNAL(valueChanged(double)),
		el_gl_widget, SLOT(set_random_translation_y_max(double)));
	QObject::connect(translate_z_max, SIGNAL(valueChanged(double)),
		el_gl_widget, SLOT(set_random_translation_z_max(double)));

	QObject::connect(rotate_x_group, SIGNAL(clicked(bool)), el_gl_widget, SLOT(set_random_rotation_x(bool)));
	QObject::connect(rotate_y_group, SIGNAL(clicked(bool)), el_gl_widget, SLOT(set_random_rotation_y(bool)));
	QObject::connect(rotate_z_group, SIGNAL(clicked(bool)), el_gl_widget, SLOT(set_random_rotation_z(bool)));
	QObject::connect(rotate_x_min, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_random_rotation_x_min(double)));
	QObject::connect(rotate_y_min, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_random_rotation_y_min(double)));
	QObject::connect(rotate_z_min, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_random_rotation_z_min(double)));
	QObject::connect(rotate_x_max, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_random_rotation_x_max(double)));
	QObject::connect(rotate_y_max, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_random_rotation_y_max(double)));
	QObject::connect(rotate_z_max, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_random_rotation_z_max(double)));

	QObject::connect(scale_group, SIGNAL(clicked(bool)), el_gl_widget, SLOT(set_random_scale(bool)));
	QObject::connect(scale_min, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_random_scale_min(double)));
	QObject::connect(scale_max, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_random_scale_max(double)));

	QObject::connect(action_save, SIGNAL(triggered()), this, SLOT(save()));
	QObject::connect(action_save_as, SIGNAL(triggered()), this, SLOT(save_as()));
	QObject::connect(action_info, SIGNAL(triggered()), this, SLOT(about_el()));
	QObject::connect(action_qt_info, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	QObject::connect(action_debug, SIGNAL(toggled(bool)), this, SLOT(set_debug(bool)));

	m_object_witdgets.push_back(x_translation);
	m_object_witdgets.push_back(y_translation);
	m_object_witdgets.push_back(z_translation);
	m_object_witdgets.push_back(scale_value);
	m_object_witdgets.push_back(scale_slider);
	m_object_witdgets.push_back(x_rotation);
	m_object_witdgets.push_back(y_rotation);
	m_object_witdgets.push_back(z_rotation);
	m_object_witdgets.push_back(x_rotation_dial);
	m_object_witdgets.push_back(y_rotation_dial);
	m_object_witdgets.push_back(z_rotation_dial);
	m_object_witdgets.push_back(transparency_type_0);
	m_object_witdgets.push_back(transparency_type_1);
	m_object_witdgets.push_back(transparency_type_2);
	m_object_witdgets.push_back(transparency_value);
	m_object_witdgets.push_back(selection_type_0);
	m_object_witdgets.push_back(selection_type_1);
	m_object_witdgets.push_back(selection_type_2);
	m_object_witdgets.push_back(walkable);
	m_object_witdgets.push_back(material_0);
	m_object_witdgets.push_back(material_1);
	m_object_witdgets.push_back(material_2);
	m_object_witdgets.push_back(material_3);
	m_object_witdgets.push_back(material_4);
	m_object_witdgets.push_back(material_5);
	m_object_witdgets.push_back(material_6);
	m_object_witdgets.push_back(material_7);
	m_object_witdgets.push_back(material_8);
	m_object_witdgets.push_back(material_9);

	m_material_witdgets.push_back(material_0);
	m_material_witdgets.push_back(material_1);
	m_material_witdgets.push_back(material_2);
	m_material_witdgets.push_back(material_3);
	m_material_witdgets.push_back(material_4);
	m_material_witdgets.push_back(material_5);
	m_material_witdgets.push_back(material_6);
	m_material_witdgets.push_back(material_7);
	m_material_witdgets.push_back(material_8);
	m_material_witdgets.push_back(material_9);

	m_material_label_witdgets.push_back(material_label_0);
	m_material_label_witdgets.push_back(material_label_1);
	m_material_label_witdgets.push_back(material_label_2);
	m_material_label_witdgets.push_back(material_label_3);
	m_material_label_witdgets.push_back(material_label_4);
	m_material_label_witdgets.push_back(material_label_5);
	m_material_label_witdgets.push_back(material_label_6);
	m_material_label_witdgets.push_back(material_label_7);
	m_material_label_witdgets.push_back(material_label_8);
	m_material_label_witdgets.push_back(material_label_9);

	m_light_witdgets.push_back(x_position);
	m_light_witdgets.push_back(y_position);
	m_light_witdgets.push_back(z_position);
	m_light_witdgets.push_back(radius);
	m_light_witdgets.push_back(light_color);

	set_light_color(glm::vec3(0.0f));

	load_settings();

	m_progress_bar = new QProgressBar(statusbar);
	statusbar->addPermanentWidget(m_progress_bar, 1);
	m_progress.reset(new QProgress());

	QObject::connect(m_progress.get(),
		SIGNAL(set_range(const int, const int)), m_progress_bar,
		SLOT(setRange(const int, const int)), Qt::QueuedConnection);
	QObject::connect(m_progress.get(), SIGNAL(set_value(const int)),
		m_progress_bar, SLOT(setValue(const int)), Qt::QueuedConnection);

	QObject::connect(el_gl_widget, SIGNAL(initialized()), this,
		SLOT(initialized()), Qt::QueuedConnection);

	QVector3D min, max;

	min = el_gl_widget->get_terrain_offset_min();
	max = el_gl_widget->get_terrain_offset_max();

	vector_brush_add_x->setMinimum(min.x());
	vector_brush_add_x->setMaximum(max.x());
	vector_brush_add_y->setMinimum(min.y());
	vector_brush_add_y->setMaximum(max.y());
	vector_brush_add_z->setMinimum(min.z());
	vector_brush_add_z->setMaximum(max.z());

	vector_brush_set_x->setMinimum(min.x());
	vector_brush_set_x->setMaximum(max.x());
	vector_brush_set_y->setMinimum(min.y());
	vector_brush_set_y->setMaximum(max.y());
	vector_brush_set_z->setMinimum(min.z());
	vector_brush_set_z->setMaximum(max.z());

	m_changed_nodes = false;

	QGraphicsScene *s = new QGraphicsScene();
	graphicsView->setScene(s);
	graphicsView->setRenderHint(QPainter::Antialiasing);
	s->setFont(graphicsView->font());
	// graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

	m_nodes_editor = new QNodesEditor(this);
	m_nodes_editor->install(s);

	m_effect_nodes = boost::make_shared<el::EffectNodes>();

	connect(constant_button, SIGNAL(clicked()), this, SLOT(add_constant()));
	connect(function_button, SIGNAL(clicked()), this, SLOT(add_function()));
	connect(geometric_function_button, SIGNAL(clicked()), this,
		SLOT(add_geometric_function()));
	connect(trigonemetric_function_button, SIGNAL(clicked()), this,
		SLOT(add_trigonemetric_function()));
	connect(texture_button, SIGNAL(clicked()), this, SLOT(add_texture()));
	connect(parameter_button, SIGNAL(clicked()), this,
		SLOT(add_parameter()));
	connect(output_button, SIGNAL(clicked()), this, SLOT(add_output()));
	connect(color_button, SIGNAL(clicked()), this, SLOT(add_color()));
	connect(direction_button, SIGNAL(clicked()), this,
		SLOT(add_direction()));

	connect(terrain_layers, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
		this, SLOT(layers_double_clicked(QListWidgetItem*)));

	connect(this, SIGNAL(get_albedo_map_data(const QString&, const QSize&,
		const QSize&, QIcon&, bool&, bool&)), el_gl_widget,
		SLOT(get_albedo_map_data(const QString&, const QSize&,
			const QSize&, QIcon&, bool&, bool&)));

	connect(m_all_terrain_textures, SIGNAL(get_albedo_map_data(
		const QString&, const QSize&, const QSize&, QIcon&, bool&,
		bool&)), el_gl_widget, SLOT(get_albedo_map_data(const QString&,
			const QSize&, const QSize&, QIcon&, bool&, bool&)));

	connect(this, SIGNAL(get_extra_map_data(const QString&, const QSize&,
		bool&)), el_gl_widget, SLOT(get_extra_map_data(
			const QString&, const QSize&, bool&)));

	connect(m_all_terrain_textures, SIGNAL(get_extra_map_data(
		const QString&, const QSize&, bool&)), el_gl_widget,
		SLOT(get_extra_map_data(const QString&, const QSize&, bool&)));

	connect(action_import_terrain_height_map, SIGNAL(triggered()), this,
		SLOT(import_terrain_height_map()));

	connect(action_import_terrain_blend_map, SIGNAL(triggered()), this,
		SLOT(import_terrain_blend_map()));

	connect(action_relax_terrain_uv, SIGNAL(triggered()), this,
		SLOT(relax_terrain_uv()));

	connect(action_remove_all_copies_of_object, SIGNAL(triggered()), this,
		SLOT(remove_all_copies_of_object()));
	connect(action_replace_all_copies_of_object, SIGNAL(triggered()), this,
		SLOT(replace_all_copies_of_object()));

	m_progress_dialog = new QProgressDialog(this);

	m_progress_dialog->setAutoClose(true);
	m_progress_dialog->setAutoReset(true);
	m_progress_dialog->setWindowModality(Qt::WindowModal);

	m_el_progress.reset(new QProgress());

	connect(m_el_progress.get(), SIGNAL(set_range(const int, const int)),
		m_progress_dialog, SLOT(setRange(const int, const int)));
	connect(m_el_progress.get(), SIGNAL(set_value(const int)),
		m_progress_dialog, SLOT(setValue(const int)));
	connect(m_el_progress.get(), SIGNAL(set_string(const QString&)),
		m_progress_dialog, SLOT(setLabelText(const QString&)));
	connect(m_el_progress.get(), SIGNAL(start()), m_progress_dialog,
		SLOT(show()));

	connect(m_progress_dialog, SIGNAL(canceled()), m_el_progress.get(),
		SLOT(cancel()));

	connect(m_init_terrain, SIGNAL(get_height_map_data(
		const QString&, QSize&, bool&)), el_gl_widget,
		SLOT(get_image_data(const QString&, QSize&, bool&)));

	connect(el_gl_widget, SIGNAL(changed_camera_yaw(const int)),
		camera_yaw, SLOT(setValue(int)));

	connect(el_gl_widget, SIGNAL(changed_camera_roll(const int)),
		camera_roll, SLOT(setValue(int)));

	connect(action_show_used_terrain_layers, SIGNAL(triggered()),
		this, SLOT(show_used_terrain_layers()));
	connect(action_clear_invisible_terrain_layers, SIGNAL(triggered()),
		el_gl_widget, SLOT(clear_invisible_terrain_layers()));
	connect(action_pack_terrain_layers, SIGNAL(triggered()),
		el_gl_widget, SLOT(pack_terrain_layers()));

	pick_displacement->setDefaultAction(action_pick_displacement);
	pick_normal->setDefaultAction(action_pick_normal);

	connect(action_pick_displacement, SIGNAL(triggered()), el_gl_widget,
		SLOT(pick_terrain_displacement()));
	connect(action_pick_normal, SIGNAL(triggered()), el_gl_widget,
		SLOT(pick_terrain_normal()));

	connect(el_gl_widget, SIGNAL(terrain_displacement(const QVector3D&)),
		this, SLOT(terrain_displacement(const QVector3D&)));
	connect(el_gl_widget, SIGNAL(terrain_normal(const QVector3D&)), this,
		SLOT(terrain_normal(const QVector3D&)));

	connect(action_fill_terrain_blend_layer, SIGNAL(triggered()), this,
		SLOT(fill_terrain_blend_layer()));

	connect(action_terrain_normal_mapping, SIGNAL(toggled(bool)),
		el_gl_widget, SLOT(set_terrain_normal_mapping(const bool)));

	object_dock->widget()->setEnabled(false);
	materials_dock->widget()->setEnabled(false);
	transformation_dock->widget()->setEnabled(false);
	light_dock->widget()->setEnabled(false);
	terrain_dock->widget()->setEnabled(false);
	randomize_dock->widget()->setEnabled(false);
}

MainWindow::~MainWindow()
{
}

void MainWindow::initialized()
{
	QStringTerrainTextureDataQMap::iterator it, end;
	QIcon icon;
	int i, count;
	bool use_blend_size_sampler, ok;

	end = m_terrain_texture_datas.end();
	count = m_terrain_texture_datas.size();

	QProgressDialog progress("Updating icons...", "Cancel", 0, count, this);

	progress.setWindowModality(Qt::WindowModal);

	i = 0;

	for (it = m_terrain_texture_datas.begin(); it != end; ++it)
	{
		ok = false;

		emit get_albedo_map_data(it->get_albedo_map(),
			m_preview_icons_size, m_terrain_texture_size, icon,
			use_blend_size_sampler, ok);

		progress.setValue(i);

		i++;

		if (ok)
		{
			it->set_icon(icon);
			it->set_use_blend_size_sampler(use_blend_size_sampler);
		}
	}

	progress.setValue(count);
}

bool MainWindow::get_terrain_texture_data(const QString &name, QIcon &icon,
	QString &albedo_map, QString &extra_map, bool &use_blend_size_sampler,
	bool &use_extra_map) const
{
	QStringTerrainTextureDataQMap::const_iterator found;

	found = m_terrain_texture_datas.find(name);

	if (found == m_terrain_texture_datas.constEnd())
	{
		return false;
	}

	icon = found->get_icon();
	albedo_map = found->get_albedo_map();
	extra_map = found->get_extra_map();
	use_blend_size_sampler = found->get_use_blend_size_sampler();
	use_extra_map = found->get_use_extra_map();

	return true;
}

QIcon MainWindow::get_terrain_texture_icon(const QString &albedo_map,
	const QString &extra_map, const bool use_blend_size_sampler,
	const bool use_extra_map)
{
	QStringTerrainTextureDataQMap::const_iterator found;
	QIcon icon;
	bool new_use_blend_size_sampler, ok;

	found = m_terrain_texture_datas.find(albedo_map);

	if (found == m_terrain_texture_datas.constEnd())
	{
		ok = false;

		emit get_albedo_map_data(albedo_map, m_preview_icons_size,
			m_terrain_texture_size, icon,
			new_use_blend_size_sampler, ok);

		m_terrain_texture_datas[albedo_map] = TerrainTextureData(icon,
			albedo_map, extra_map, use_blend_size_sampler,
			use_extra_map);

		return icon;
	}

	assert(found->get_albedo_map() == albedo_map);

	return found->get_icon();
}

void MainWindow::update_terrain_layers()
{
	QListWidgetItem* item;
	QString name, albedo_map, extra_map;
	float blend_size;
	Uint32 i, count;
	bool use_blend_size_sampler, use_blend_size, use_extra_map;

	count = el_gl_widget->get_terrain_layer_count();

	terrain_layers->clear();

	for (i = 0; i < count; ++i)
	{
		name = QString(tr("layer ")) + QString::number(i);

		item = new QListWidgetItem(name);

		el_gl_widget->get_terrain_material(albedo_map,
			extra_map, blend_size, use_blend_size_sampler,
			use_blend_size, use_extra_map, i);

		item->setIcon(get_terrain_texture_icon(albedo_map, extra_map,
			use_blend_size_sampler, use_extra_map));
		item->setToolTip(albedo_map);
		item->setData(tlr_index, i);
		item->setData(tlr_blend_size, blend_size);
		item->setData(tlr_use_blend_size, use_blend_size);
		item->setData(tlr_use_blend_size_sampler,
			use_blend_size_sampler);
		item->setData(tlr_use_extra_map, use_extra_map);

		if (i == 0)
		{
			item->setFlags(Qt::ItemIsEnabled);
		}
		else
		{
			item->setFlags(Qt::ItemIsSelectable |
				Qt::ItemIsEnabled);
		}

		terrain_layers->insertItem(i, item);
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	save_settings();

	QMainWindow::closeEvent(event);
}

void MainWindow::save_settings()
{
	QSettings settings("settings.conf", QSettings::IniFormat, this);

	settings.beginGroup("window");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.setValue("invert z rotation",
		el_gl_widget->get_invert_z_rotation());
	settings.setValue("toolbar icon size", toolbar->iconSize());
	settings.setValue("texture preview size", m_preview_icons_size);
	settings.setValue("terrain texture size", m_terrain_texture_size);
	settings.endGroup();
	save_shortcuts(settings);
	save_mouse_settings(settings);
	save_dirs_settings(settings);
	save_terrain_textures_settings(settings);
}

void MainWindow::load_settings()
{
	QSettings settings("settings.conf", QSettings::IniFormat, this);

	settings.beginGroup("window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	el_gl_widget->set_invert_z_rotation(settings.value("invert z rotation",
		false).toBool());
	toolbar->setIconSize(settings.value("toolbar icon size", QSize(24, 24)
		).value<QSize>());
	m_preview_icons_size = settings.value("texture preview size",
		QSize(24, 24)).value<QSize>();
	m_terrain_texture_size = settings.value("terrain preview size",
		QSize(512, 512)).value<QSize>();
	terrain_layers->setIconSize(m_preview_icons_size);
	m_terrain_textures->set_icon_size(m_preview_icons_size);
	m_all_terrain_textures->set_icon_size(m_preview_icons_size);
	m_all_terrain_textures->set_image_size(m_terrain_texture_size);
	settings.endGroup();

	load_shortcuts(settings);
	load_mouse_settings(settings);
	load_dirs_settings(settings);
	load_terrain_textures_settings(settings);
}

void MainWindow::update_object()
{
	QStringList default_materials, materials, object_materials;
	QString material;
	EditorObjectDescription object_description;
	unsigned int id, i, index;
	int j;

	el_gl_widget->get_object_description(object_description);

	BOOST_FOREACH(QObject* object_widget, m_object_witdgets)
	{
		object_widget->blockSignals(true);
	}

	BOOST_FOREACH(QComboBox* material_witdget, m_material_witdgets)
	{
		material_witdget->setCurrentIndex(-1);
		material_witdget->setEnabled(false);
	}

	BOOST_FOREACH(QLabel* material_label_witdget,
		m_material_label_witdgets)
	{
		material_label_witdget->setEnabled(false);
	}

	x_translation->setValue(object_description.get_world_transformation(
		).get_translation()[0]);
	y_translation->setValue(object_description.get_world_transformation(
		).get_translation()[1]);
	z_translation->setValue(object_description.get_world_transformation(
		).get_translation()[2]);

	x_rotation->setValue(object_description.get_rotation_angles()[0]);
	y_rotation->setValue(object_description.get_rotation_angles()[1]);
	z_rotation->setValue(object_description.get_rotation_angles()[2]);
	x_rotation_dial->setValue(object_description.get_rotation_angles()[0]);
	y_rotation_dial->setValue(object_description.get_rotation_angles()[1]);
	z_rotation_dial->setValue(object_description.get_rotation_angles()[2]);

	scale_value_x->setValue(std::abs(
		object_description.get_world_transformation().get_scale().x *
		100.0f));
	scale_slider_x->setValue(200.0f * (std::log10(
		scale_value_x->value()) - 1.0f));

	scale_value_y->setValue(std::abs(
		object_description.get_world_transformation().get_scale().y *
		100.0f));
	scale_slider_y->setValue(200.0f * (std::log10(
		scale_value_y->value()) - 1.0f));

	scale_value_z->setValue(std::abs(
		object_description.get_world_transformation().get_scale().z *
		100.0f));
	scale_slider_z->setValue(200.0f * (std::log10(
		scale_value->value()) - 1.0f));

	mirror_x->setChecked(object_description.get_world_transformation(
		).get_scale().x < 0.0f);
	mirror_y->setChecked(object_description.get_world_transformation(
		).get_scale().y < 0.0f);
	mirror_z->setChecked(object_description.get_world_transformation(
		).get_scale().z < 0.0f);

	transparency_value->setValue(object_description.get_transparency() *
		100.0f);

	set_blend(object_description.get_blend());
	set_selection(object_description.get_selection());
	walkable->setChecked(object_description.get_walkable());

	mesh_name->setText(QString::fromUtf8(object_description.get_name(
		).get().c_str()));

	id = object_description.get_id();

	object_id->setText(QVariant(id).toString());

	default_materials = el_gl_widget->get_default_materials(
		object_description.get_name());

	object_materials = default_materials;

	for (j = 0; j < default_materials.size(); ++j)
	{
		if (static_cast<int>(
			object_description.get_material_names().size()) > j)
		{
			material = QString::fromUtf8(
				object_description.get_material_names()[j].get(
					).c_str());
		}

		if (!material.isEmpty())
		{
			object_materials[j] = material;
		}
	}

	m_material_count = object_materials.size();

	m_material_count = std::min(static_cast<int>(m_material_count),
		m_material_witdgets.size());

	materials = el_gl_widget->get_materials();

	for (i = 0; i < m_material_count; ++i)
	{
		material = object_materials[i];
		index = -1;

		m_material_witdgets[i]->clear();

		for (j = 0; j < materials.size(); ++j)
		{
			QColor color;

			m_material_witdgets[i]->addItem(materials[j]);

			if (materials[j] == default_materials[i])
			{
				color = QColor::fromRgbF(0.5f, 0.5f, 0.5f);

				m_material_witdgets[i]->setItemData(j,
					QBrush(color), Qt::BackgroundRole);
			}

			if (materials[j] == material)
			{
				index = j;
			}
		}

		m_material_witdgets[i]->setCurrentIndex(index);

		m_material_witdgets[i]->setEnabled(true);
		m_material_label_witdgets[i]->setEnabled(true);
	}

	BOOST_FOREACH(QObject* object_widget, m_object_witdgets)
	{
		object_widget->blockSignals(false);
	}
}

void MainWindow::add_item(const QString &str, QComboBox* combobox)
{
	QString selection;
	int index;

	selection = combobox->currentText();

	combobox->addItem(str);

	index = combobox->findText(selection);

	combobox->setCurrentIndex(index);
}

void MainWindow::update_terrain(const bool enabled)
{
	action_relax_terrain_uv->setEnabled(enabled);
	action_import_terrain_height_map->setEnabled(enabled);
	action_import_terrain_blend_map->setEnabled(enabled);
	action_show_used_terrain_layers->setEnabled(enabled);
	action_clear_invisible_terrain_layers->setEnabled(enabled);
	action_pack_terrain_layers->setEnabled(enabled);
	action_fill_terrain_blend_layer->setEnabled(enabled);

	if (enabled)
	{
		update_terrain_layers();
	}
}

void MainWindow::update_object(const bool select)
{
	QList<QDockWidget *> dock_widgets;

	if (select && action_delete_mode->isChecked())
	{
		el_gl_widget->remove_object();
		deselect();

		return;
	}

	if (el_gl_widget->get_renderable() != rt_object)
	{
		deselect();

		return;
	}

	if (select)
	{
		object_dock->raise();

		dock_widgets = tabifiedDockWidgets(object_dock);

		if (dock_widgets.lastIndexOf(transformation_dock) == -1)
		{
			transformation_dock->raise();

			if (dock_widgets.lastIndexOf(materials_dock) == -1)
			{
				dock_widgets = tabifiedDockWidgets(
					transformation_dock);

				if (dock_widgets.lastIndexOf(materials_dock) ==
					-1)
				{
					materials_dock->raise();
				}
			}
		}
		else
		{
			if (dock_widgets.lastIndexOf(materials_dock) == -1)
			{
				materials_dock->raise();
			}
		}
	}

	object_dock->widget()->setEnabled(select);
	materials_dock->widget()->setEnabled(select);
	transformation_dock->widget()->setEnabled(select);
	light_dock->widget()->setEnabled(false);
	terrain_dock->widget()->setEnabled(false);
	randomize_dock->widget()->setEnabled(false);

	update_object();

	action_remove->setEnabled(select);
	action_remove_all_copies_of_object->setEnabled(select);
	action_replace_all_copies_of_object->setEnabled(select);
}

void MainWindow::update_light(const bool select)
{
	LightData light;
	unsigned int id;

	if (select && action_delete_mode->isChecked())
	{
		el_gl_widget->remove_light();
		deselect();

		return;
	}

	if (el_gl_widget->get_renderable() != rt_light)
	{
		deselect();

		return;
	}

	if (select)
	{
		light_dock->raise();
	}

	object_dock->widget()->setEnabled(false);
	materials_dock->widget()->setEnabled(false);
	transformation_dock->widget()->setEnabled(false);
	light_dock->widget()->setEnabled(select);
	terrain_dock->widget()->setEnabled(false);
	randomize_dock->widget()->setEnabled(false);

	action_remove->setEnabled(select);
	action_remove_all_copies_of_object->setEnabled(false);
	action_replace_all_copies_of_object->setEnabled(false);

	el_gl_widget->get_light_data(light);

	BOOST_FOREACH(QObject* light_widget, m_light_witdgets)
	{
		light_widget->blockSignals(true);
	}

	x_position->setValue(light.get_position()[0]);
	y_position->setValue(light.get_position()[1]);
	z_position->setValue(light.get_position()[2]);

	radius->setValue(light.get_radius());

	set_light_color(glm::clamp(light.get_color(), 0.0f, 1.0));

	id = light.get_id();

	light_id->setText(QVariant(id).toString());

	BOOST_FOREACH(QObject* light_widget, m_light_witdgets)
	{
		light_widget->blockSignals(false);
	}
}

void MainWindow::deselect()
{
	set_default_mode();
	action_remove->setEnabled(false);
	action_remove_all_copies_of_object->setEnabled(false);
	action_replace_all_copies_of_object->setEnabled(false);

	object_dock->widget()->setEnabled(false);
	materials_dock->widget()->setEnabled(false);
	transformation_dock->widget()->setEnabled(false);
	light_dock->widget()->setEnabled(false);
	terrain_dock->widget()->setEnabled(false);
}

void MainWindow::update_translation()
{
	glm::vec3 translation;

	translation[0] = x_translation->value();
	translation[1] = y_translation->value();
	translation[2] = z_translation->value();

	el_gl_widget->set_translation(translation);
}

void MainWindow::update_position()
{
	glm::vec3 position;

	position[0] = x_position->value();
	position[1] = y_position->value();
	position[2] = z_position->value();

	el_gl_widget->set_translation(position);
}

void MainWindow::update_rotation(const int index)
{
	glm::vec3 rotation;

	m_rotation_dials[index]->blockSignals(true);
	m_rotation_dials[index]->setValue(m_rotations[index]->value());
	m_rotation_dials[index]->blockSignals(false);

	rotation[0] = x_rotation->value();
	rotation[1] = y_rotation->value();
	rotation[2] = z_rotation->value();

	el_gl_widget->set_rotation(rotation);
}

void MainWindow::update_rotation_dial(const int index)
{
	glm::vec3 rotation;

	m_rotations[index]->blockSignals(true);
	m_rotations[index]->setValue(m_rotation_dials[index]->value());
	m_rotations[index]->blockSignals(false);

	rotation[0] = x_rotation->value();
	rotation[1] = y_rotation->value();
	rotation[2] = z_rotation->value();

	el_gl_widget->set_rotation(rotation);
}

void MainWindow::update_scale()
{
	glm::vec3 scale;

	scale_slider->blockSignals(true);
	scale_slider->setValue(200.0f * (std::log10(
		scale_value->value()) - 1.0f));
	scale_slider->blockSignals(false);

	scale.x = scale_value->value() * 0.01f;
	scale.y = scale_value->value() * 0.01f;
	scale.z = scale_value->value() * 0.01f;

	if (mirror_x->isChecked())
	{
		scale.x = -scale.x;
	}

	if (mirror_y->isChecked())
	{
		scale.y = -scale.y;
	}

	if (mirror_z->isChecked())
	{
		scale.z = -scale.z;
	}

	el_gl_widget->set_scale(scale);
}

void MainWindow::update_scale_x()
{
	glm::vec3 scale;

	scale_slider_x->blockSignals(true);
	scale_slider_x->setValue(200.0f * (std::log10(
		scale_value_x->value()) - 1.0f));
	scale_slider_x->blockSignals(false);

	scale.x = scale_value_x->value() * 0.01f;
	scale.y = scale_value_y->value() * 0.01f;
	scale.z = scale_value_z->value() * 0.01f;

	if (mirror_x->isChecked())
	{
		scale.x = -scale.x;
	}

	if (mirror_y->isChecked())
	{
		scale.y = -scale.y;
	}

	if (mirror_z->isChecked())
	{
		scale.z = -scale.z;
	}

	el_gl_widget->set_scale(scale);
}

void MainWindow::update_scale_y()
{
	glm::vec3 scale;

	scale_slider_y->blockSignals(true);
	scale_slider_y->setValue(200.0f * (std::log10(
		scale_value_y->value()) - 1.0f));
	scale_slider_y->blockSignals(false);

	scale.x = scale_value_x->value() * 0.01f;
	scale.y = scale_value_y->value() * 0.01f;
	scale.z = scale_value_z->value() * 0.01f;

	if (mirror_x->isChecked())
	{
		scale.x = -scale.x;
	}

	if (mirror_y->isChecked())
	{
		scale.y = -scale.y;
	}

	if (mirror_z->isChecked())
	{
		scale.z = -scale.z;
	}

	el_gl_widget->set_scale(scale);
}

void MainWindow::update_scale_z()
{
	glm::vec3 scale;

	scale_slider_z->blockSignals(true);
	scale_slider_z->setValue(200.0f * (std::log10(
		scale_value_z->value()) - 1.0f));
	scale_slider_z->blockSignals(false);

	scale.x = scale_value_x->value() * 0.01f;
	scale.y = scale_value_y->value() * 0.01f;
	scale.z = scale_value_z->value() * 0.01f;

	if (mirror_x->isChecked())
	{
		scale.x = -scale.x;
	}

	if (mirror_y->isChecked())
	{
		scale.y = -scale.y;
	}

	if (mirror_z->isChecked())
	{
		scale.z = -scale.z;
	}

	el_gl_widget->set_scale(scale);
}

void MainWindow::update_scale_slider()
{
	glm::vec3 scale;

	scale_value->blockSignals(true);
	scale_value->setValue(std::pow(10.0f, 1.0f +
		scale_slider->value() / 200.0f));
	scale_value->blockSignals(false);

	scale.x = scale_value->value() * 0.01f;
	scale.y = scale_value->value() * 0.01f;
	scale.z = scale_value->value() * 0.01f;

	if (mirror_x->isChecked())
	{
		scale.x = -scale.x;
	}

	if (mirror_y->isChecked())
	{
		scale.y = -scale.y;
	}

	if (mirror_z->isChecked())
	{
		scale.z = -scale.z;
	}

	el_gl_widget->set_scale(scale);
}

void MainWindow::update_scale_slider_x()
{
	glm::vec3 scale;

	scale_value_x->blockSignals(true);
	scale_value_x->setValue(std::pow(10.0f, 1.0f +
		scale_slider_x->value() / 200.0f));
	scale_value_x->blockSignals(false);

	scale.x = scale_value_x->value() * 0.01f;
	scale.y = scale_value_y->value() * 0.01f;
	scale.z = scale_value_z->value() * 0.01f;

	if (mirror_x->isChecked())
	{
		scale.x = -scale.x;
	}

	if (mirror_y->isChecked())
	{
		scale.y = -scale.y;
	}

	if (mirror_z->isChecked())
	{
		scale.z = -scale.z;
	}

	el_gl_widget->set_scale(scale);
}

void MainWindow::update_scale_slider_y()
{
	glm::vec3 scale;

	scale_value_y->blockSignals(true);
	scale_value_y->setValue(std::pow(10.0f, 1.0f +
		scale_slider_y->value() / 200.0f));
	scale_value_y->blockSignals(false);

	scale.x = scale_value_x->value() * 0.01f;
	scale.y = scale_value_y->value() * 0.01f;
	scale.z = scale_value_z->value() * 0.01f;

	if (mirror_x->isChecked())
	{
		scale.x = -scale.x;
	}

	if (mirror_y->isChecked())
	{
		scale.y = -scale.y;
	}

	if (mirror_z->isChecked())
	{
		scale.z = -scale.z;
	}

	el_gl_widget->set_scale(scale);
}

void MainWindow::update_scale_slider_z()
{
	glm::vec3 scale;

	scale_value_z->blockSignals(true);
	scale_value_z->setValue(std::pow(10.0f, 1.0f +
		scale_slider_z->value() / 200.0f));
	scale_value_z->blockSignals(false);

	scale.x = scale_value_x->value() * 0.01f;
	scale.y = scale_value_y->value() * 0.01f;
	scale.z = scale_value_z->value() * 0.01f;

	if (mirror_x->isChecked())
	{
		scale.x = -scale.x;
	}

	if (mirror_y->isChecked())
	{
		scale.y = -scale.y;
	}

	if (mirror_z->isChecked())
	{
		scale.z = -scale.z;
	}

	el_gl_widget->set_scale(scale);
}

void MainWindow::update_mirror()
{
	glm::vec3 scale;

	scale.x = scale_value_x->value() * 0.01f;
	scale.y = scale_value_y->value() * 0.01f;
	scale.z = scale_value_z->value() * 0.01f;

	if (mirror_x->isChecked())
	{
		scale.x = -scale.x;
	}

	if (mirror_y->isChecked())
	{
		scale.y = -scale.y;
	}

	if (mirror_z->isChecked())
	{
		scale.z = -scale.z;
	}

	el_gl_widget->set_scale(scale);
}

void MainWindow::update_materials()
{
	StringVector materials;
	unsigned int i;

	for (i = 0; i < m_material_count; ++i)
	{
		materials.push_back(String(m_material_witdgets[i]->currentText(
			).toUtf8().data()));
	}

	el_gl_widget->set_object_materials(materials);
}

void MainWindow::set_light_color(const glm::vec3 &color)
{
	set_light_color(QColor::fromRgbF(color[0], color[1], color[2]));
}

void MainWindow::set_light_color(const QColor &color)
{
	QPixmap pixmap(100, 15);

	pixmap.fill(color);

	light_color->setIcon(QIcon(pixmap));
}

void MainWindow::change_light_color()
{
	QColor color;
	glm::vec3 light_color;

	light_color = el_gl_widget->get_light_color();
	color = QColor::fromRgbF(light_color[0], light_color[1],
		light_color[2]);
	color = QColorDialog::getColor(color, this);

	if (color.isValid())
	{
		set_light_color(color);

		el_gl_widget->set_light_color(glm::vec3(color.redF(),
			color.greenF(), color.blueF()));
	}
}

void MainWindow::add_objects(const bool value)
{
	EditorObjectDescription object_description;

	if (!value)
	{
		el_gl_widget->disable_object();

		return;
	}

	el_gl_widget->get_object_description(object_description);

	m_objects->set_object(QString::fromUtf8(object_description.get_name(
		).get().c_str()));

	if (m_objects->exec() == QDialog::Accepted)
	{
		el_gl_widget->add_object(String(m_objects->get_object().toUtf8(
			)));

		action_add_lights->setChecked(false);
		action_delete_mode->setChecked(false);
		action_terrain_mode->setChecked(false);

		set_default_mode();

		randomize_dock->widget()->setEnabled(true);
		randomize_dock->raise();

		return;
	}

	el_gl_widget->disable_object();

	action_add_objects->setChecked(false);
}

void MainWindow::set_default_mode()
{
	object_dock->widget()->setEnabled(false);
	materials_dock->widget()->setEnabled(false);
	transformation_dock->widget()->setEnabled(false);
	light_dock->widget()->setEnabled(false);
	terrain_dock->widget()->setEnabled(false);
}

void MainWindow::add_lights(const bool value)
{
	float radius;
	bool ok;

	if (!value)
	{
		el_gl_widget->disable_light();

		return;
	}

	radius = QInputDialog::getDouble(this, "Add ligts", "radius", 5.0,
		0.01, 100.0, 2, &ok);

	if (ok)
	{
		el_gl_widget->add_light(radius);

		action_add_objects->setChecked(false);
		action_delete_mode->setChecked(false);
		action_terrain_mode->setChecked(false);

		return;
	}

	el_gl_widget->disable_light();

	action_add_lights->setChecked(false);
}

void MainWindow::remove()
{
	switch (el_gl_widget->get_renderable())
	{
		case rt_light:
			el_gl_widget->remove_light();
			break;
		case rt_object:
			el_gl_widget->remove_object();
			break;
		case rt_particle:
		case rt_decal:
		case rt_lights:
		case rt_objects:
		case rt_particles:
		case rt_decals:
		case rt_none:
			return;
	}

	deselect();
}

void MainWindow::terrain_mode(const bool checked)
{
	QStringTerrainTextureDataQMap::const_iterator it;

	if (!checked)
	{
		set_default_mode();

		return;
	}

	if (!el_gl_widget->get_terrain())
	{
		if (m_terrain_texture_datas.size() < 1)
		{
			QMessageBox::critical(this, tr("Error"), QString(tr(
				"No terrain textures set")));

			action_terrain_mode->setChecked(false);

			return;
		}

		m_init_terrain->set_map_size(el_gl_widget->get_map_size());

		if (m_init_terrain->exec() != QDialog::Accepted)
		{
			action_terrain_mode->setChecked(false);

			return;
		}

		it = m_terrain_texture_datas.constBegin();

		if (m_init_terrain->get_height_map().isEmpty())
		{
			el_gl_widget->init_terrain(m_init_terrain->get_size(),
				it.value().get_albedo_map(),
				it.value().get_extra_map(),
				it.value().get_use_blend_size_sampler(),
				it.value().get_use_extra_map());
		}
		else
		{
			el_gl_widget->init_terrain(
				m_init_terrain->get_height_map(),
				m_init_terrain->get_size(),
				it.value().get_albedo_map(),
				it.value().get_extra_map(),
				it.value().get_use_blend_size_sampler(),
				it.value().get_use_extra_map());
		}
	}

	action_remove->setEnabled(false);
	action_remove_all_copies_of_object->setEnabled(false);
	action_replace_all_copies_of_object->setEnabled(false);

	action_add_objects->setChecked(false);
	action_add_lights->setChecked(false);
	action_delete_mode->setChecked(false);

	terrain_dock->widget()->setEnabled(checked);
	object_dock->widget()->setEnabled(false);
	materials_dock->widget()->setEnabled(false);
	transformation_dock->widget()->setEnabled(false);
	light_dock->widget()->setEnabled(false);
	randomize_dock->widget()->setEnabled(false);
	terrain_dock->raise();
}

void MainWindow::delete_mode(const bool checked)
{
	if (checked)
	{
		action_add_objects->setChecked(false);
		action_add_lights->setChecked(false);
		action_terrain_mode->setChecked(false);

		object_dock->widget()->setEnabled(false);
		materials_dock->widget()->setEnabled(false);
		transformation_dock->widget()->setEnabled(false);
		light_dock->widget()->setEnabled(false);
		terrain_dock->widget()->setEnabled(false);
		randomize_dock->widget()->setEnabled(false);
	}
	else
	{
		set_default_mode();
	}
}

void MainWindow::set_fog()
{
	el_gl_widget->set_fog(glm::vec3(1.0f), 2.5f);
}

void MainWindow::open_map()
{
	QString file_name;

	file_name = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
		tr("All EL-maps (*.elm *.elm.gz *.elm.xz);;All files (*)"));

	if (file_name.isEmpty())
	{
		return;
	}

	if (m_load_map->exec() != QDialog::Accepted)
	{
		return;
	}

	el_gl_widget->load_map(file_name,
		m_load_map->get_load_2d_objects(),
		m_load_map->get_load_3d_objects(),
		m_load_map->get_load_lights(),
		m_load_map->get_load_particles(),
		m_load_map->get_load_materials(),
		m_load_map->get_load_height_map(),
		m_load_map->get_load_tile_map(),
		m_load_map->get_load_walk_map(),
		m_load_map->get_load_terrain(),
		m_load_map->get_load_water());

	action_add_objects->setChecked(false);
	action_add_lights->setChecked(false);
	action_delete_mode->setChecked(false);
	action_terrain_mode->setChecked(false);

	action_dungeon->blockSignals(true);
	action_dungeon->setChecked(el_gl_widget->get_dungeon());
	action_dungeon->blockSignals(false);
}

void MainWindow::do_save()
{
	el_gl_widget->save(m_file_name);
}

void MainWindow::save()
{
	if (m_file_name.isEmpty())
	{
		save_as();
	}
	else
	{
		do_save();
	}
}

void MainWindow::save_as()
{
	QString file_name;

	file_name = QFileDialog::getSaveFileName(this, tr("Save map"), ".",
		tr("Eternal land map files (*.elm)"), 0, 0);

	if (!file_name.isEmpty())
	{
		m_file_name = file_name;
		do_save();
	}
}

void MainWindow::change_ground_hemisphere()
{
	QColor color;
	glm::vec4 ground_hemisphere;

	ground_hemisphere = el_gl_widget->get_ground_hemisphere();

	color = QColor::fromRgbF(ground_hemisphere[0], ground_hemisphere[1],
		ground_hemisphere[2]);

	color = QColorDialog::getColor(color, this,
		tr("Map ground hemisphere"));

	if (color.isValid())
	{
		ground_hemisphere[0] = color.redF();
		ground_hemisphere[1] = color.greenF();
		ground_hemisphere[2] = color.blueF();

		el_gl_widget->set_ground_hemisphere(ground_hemisphere);
	}
}

void MainWindow::change_dungeon()
{
	el_gl_widget->set_dungeon(action_dungeon->isChecked());
}

void MainWindow::new_map()
{
	QStringList textures;
	QString image_name;
	boost::scoped_ptr<NewMapDialog> new_map;
	int map_size_x;
	int map_size_y;
	int blend_image_size_x;
	int blend_image_size_y;

	new_map.reset(new NewMapDialog(this));
//	new_map->set_diffuse_textures(m_textures);

	if (new_map->exec() == QDialog::Accepted)
	{
		map_size_x = new_map->map_width->currentText().toInt();
		map_size_y = new_map->map_height->currentText().toInt();
		blend_image_size_x = new_map->blend_image_width->currentText().toInt();
		blend_image_size_y = new_map->blend_image_height->currentText().toInt();
		image_name = new_map->image_name->text();

		textures.push_back(new_map->terrain_diffuse_texture_0->currentText());
		textures.push_back(new_map->terrain_diffuse_texture_1->currentText());
		textures.push_back(new_map->terrain_diffuse_texture_2->currentText());
		textures.push_back(new_map->terrain_diffuse_texture_3->currentText());
		textures.push_back(new_map->terrain_diffuse_texture_4->currentText());
		textures.push_back(new_map->terrain_normal_texture_0->currentText());
		textures.push_back(new_map->terrain_normal_texture_1->currentText());
		textures.push_back(new_map->terrain_normal_texture_2->currentText());
		textures.push_back(new_map->terrain_normal_texture_3->currentText());
		textures.push_back(new_map->terrain_normal_texture_4->currentText());

		if (image_name.isEmpty())
		{
			el_gl_widget->new_map(map_size_x, map_size_y, blend_image_size_x,
				blend_image_size_y, textures);
		}
		else
		{
			el_gl_widget->new_map(image_name, blend_image_size_x,
				blend_image_size_y, textures);
		}
	}
}

void MainWindow::change_preferences()
{
	m_preferences->set_actions(findChildren<QAction*>());
	m_preferences->disable_all_mouse_button_checks();
	m_preferences->set_select_rect_button(
		el_gl_widget->get_select_rect_button());
	m_preferences->set_click_button(el_gl_widget->get_click_button());
	m_preferences->set_grab_button(el_gl_widget->get_grab_button());
	m_preferences->set_view_rotate_button(
		el_gl_widget->get_view_rotate_button());
	m_preferences->set_select_rect_key_mod(
		el_gl_widget->get_select_rect_key_mod());
	m_preferences->set_click_key_mod(
		el_gl_widget->get_click_key_mod());
	m_preferences->set_grab_key_mod(
		el_gl_widget->get_grab_key_mod());
	m_preferences->set_view_rotate_key_mod(
		el_gl_widget->get_view_rotate_key_mod());
	m_preferences->enable_all_mouse_button_checks();
	m_preferences->set_wheel_zoom_x10(el_gl_widget->get_wheel_zoom_x10());
	m_preferences->set_swap_wheel_zoom(el_gl_widget->get_swap_wheel_zoom());
	m_preferences->set_invert_z_rotation(
		el_gl_widget->get_invert_z_rotation());
	m_preferences->set_toolbar_icon_size(toolbar->iconSize());
	m_preferences->set_texture_preview_size(m_preview_icons_size);
	m_preferences->set_el_data_dir(m_el_data_dir);
	m_preferences->set_el_extra_data_dir(m_el_extra_data_dir);

	if (m_preferences->exec() == QDialog::Accepted)
	{
		el_gl_widget->set_select_rect_button(
			m_preferences->get_select_rect_button());
		el_gl_widget->set_click_button(
			m_preferences->get_click_button());
		el_gl_widget->set_grab_button(
			m_preferences->get_grab_button());
		el_gl_widget->set_view_rotate_button(
			m_preferences->get_view_rotate_button());
		el_gl_widget->set_select_rect_key_mod(
			m_preferences->get_select_rect_key_mod());
		el_gl_widget->set_click_key_mod(
			m_preferences->get_click_key_mod());
		el_gl_widget->set_grab_key_mod(
			m_preferences->get_grab_key_mod());
		el_gl_widget->set_view_rotate_key_mod(
			m_preferences->get_view_rotate_key_mod());
		el_gl_widget->set_wheel_zoom_x10(
			m_preferences->get_wheel_zoom_x10());
		el_gl_widget->set_swap_wheel_zoom(
			m_preferences->get_swap_wheel_zoom());
		el_gl_widget->set_invert_z_rotation(
			m_preferences->get_invert_z_rotation());
		toolbar->setIconSize(m_preferences->get_toolbar_icon_size());
		m_preview_icons_size =
			m_preferences->get_texture_preview_size();
		terrain_layers->setIconSize(m_preview_icons_size);
		m_terrain_textures->set_icon_size(m_preview_icons_size);
		m_all_terrain_textures->set_icon_size(m_preview_icons_size);
		m_el_data_dir = m_preferences->get_el_data_dir();
		m_el_extra_data_dir = m_preferences->get_el_extra_data_dir();
		set_dirs();
		save_settings();
	}
}

void MainWindow::change_terrain_textures()
{
	m_all_terrain_textures->set_terrain_texture_datas(
		m_terrain_texture_datas);

	if (m_all_terrain_textures->exec() == QDialog::Accepted)
	{
		m_terrain_texture_datas =
			m_all_terrain_textures->get_terrain_texture_datas();
		save_settings();
	}
}

void MainWindow::init_actions()
{
	QAction* action;
	QIcon icon;

	icon.addFile(QString::fromUtf8(":/icons/gtk-execute.png"), QSize(),
		QIcon::Normal, QIcon::Off);

	action = toolbar->toggleViewAction();
	action->setIcon(icon);
	action->setText("&Toolbar");
	menu_windows->addAction(action);

	foreach(QDockWidget* dock_widget, findChildren<QDockWidget*>())
	{
		action = dock_widget->toggleViewAction();
		action->setIcon(dock_widget->windowIcon());
		action->setText(dock_widget->windowTitle());
		menu_windows->addAction(action);
	}

	action_move_l = new QAction(this);
	action_move_l->setShortcutContext(Qt::ApplicationShortcut);
	action_move_l->setText("Move Left");
	action_move_l->setIcon(move_l->icon());

	action_move_r = new QAction(this);
	action_move_r->setShortcutContext(Qt::ApplicationShortcut);
	action_move_r->setText("Move Right");
	action_move_r->setIcon(move_r->icon());

	action_move_u = new QAction(this);
	action_move_u->setShortcutContext(Qt::ApplicationShortcut);
	action_move_u->setText("Move Up");
	action_move_u->setIcon(move_u->icon());

	action_move_d = new QAction(this);
	action_move_d->setShortcutContext(Qt::ApplicationShortcut);
	action_move_d->setText("Move Down");
	action_move_d->setIcon(move_d->icon());

	action_zoom_in = new QAction(this);
	action_zoom_in->setShortcutContext(Qt::ApplicationShortcut);
	action_zoom_in->setText("Zoom In");
	action_zoom_in->setIcon(zoom_in->icon());

	action_zoom_out = new QAction(this);
	action_zoom_out->setShortcutContext(Qt::ApplicationShortcut);
	action_zoom_out->setText("Zoom Out");
	action_zoom_out->setIcon(zoom_out->icon());

	foreach (QAction* action, findChildren<QAction*>())
	{
		if (action != 0)
		{
			if (!action->isSeparator() && (action->menu() == 0))
			{
				action->setData(action->text().remove('&'));
			}
		}
	}

	addAction(action_move_l);
	addAction(action_move_r);
	addAction(action_move_u);
	addAction(action_move_d);
	addAction(action_rotate_l);
	addAction(action_rotate_r);
	addAction(action_zoom_in);
	addAction(action_zoom_out);
	addAction(action_rotate_l);
	addAction(action_rotate_r);
	addAction(action_roll_u);
	addAction(action_roll_d);

	QObject::connect(action_move_l, SIGNAL(triggered()), el_gl_widget, SLOT(move_left()));
	QObject::connect(action_move_r, SIGNAL(triggered()), el_gl_widget, SLOT(move_right()));
	QObject::connect(action_move_u, SIGNAL(triggered()), el_gl_widget, SLOT(move_up()));
	QObject::connect(action_move_d, SIGNAL(triggered()), el_gl_widget, SLOT(move_down()));
	QObject::connect(action_rotate_l, SIGNAL(triggered()), this, SLOT(rotate_left()));
	QObject::connect(action_rotate_r, SIGNAL(triggered()), this, SLOT(rotate_right()));
	QObject::connect(action_zoom_in, SIGNAL(triggered()), el_gl_widget, SLOT(zoom_in()));
	QObject::connect(action_zoom_out, SIGNAL(triggered()), el_gl_widget, SLOT(zoom_out()));
	QObject::connect(action_roll_u, SIGNAL(triggered()), this, SLOT(roll_up()));
	QObject::connect(action_roll_d, SIGNAL(triggered()), this, SLOT(roll_down()));
}

void MainWindow::save_shortcuts(QSettings &settings)
{
	settings.beginGroup("shortcuts");

	foreach (QAction* action, findChildren<QAction*>())
	{
		if (action != 0)
		{
			if (!action->isSeparator() && (action->menu() == 0))
			{
				settings.setValue(action->data().toString(),
					action->shortcut().toString(
						QKeySequence::NativeText));
			}
		}
	}

	settings.endGroup();
}

void MainWindow::load_shortcuts(QSettings &settings)
{
	settings.beginGroup("shortcuts");

	foreach (QAction* action, findChildren<QAction*>())
	{
		if (action != 0)
		{
			if (!action->isSeparator() && (action->menu() == 0))
			{
				action->setShortcut(QKeySequence(settings.value(
					action->data().toString(),
					QKeySequence::mnemonic(action->text(
						)).toString()).toString()));
			}
		}
	}

	settings.endGroup();
}

void MainWindow::save_mouse_settings(QSettings &settings)
{
	settings.beginGroup("mouse");

	settings.setValue("select rect button",
		PreferencesDialog::mouse_button_to_str(
			el_gl_widget->get_select_rect_button()));
	settings.setValue("click button",
		PreferencesDialog::mouse_button_to_str(
			el_gl_widget->get_click_button()));
	settings.setValue("grab button",
		PreferencesDialog::mouse_button_to_str(
			el_gl_widget->get_grab_button()));
	settings.setValue("view rotate button",
		PreferencesDialog::mouse_button_to_str(
			el_gl_widget->get_view_rotate_button()));
	settings.setValue("select rect key mod",
		PreferencesDialog::key_mod_to_str(
			el_gl_widget->get_select_rect_key_mod()));
	settings.setValue("click key mod",
		PreferencesDialog::key_mod_to_str(
			el_gl_widget->get_click_key_mod()));
	settings.setValue("grab key mod",
		PreferencesDialog::key_mod_to_str(
			el_gl_widget->get_grab_key_mod()));
	settings.setValue("view rotate key mod",
		PreferencesDialog::key_mod_to_str(
			el_gl_widget->get_view_rotate_key_mod()));
	settings.setValue("wheel zoom x10",
		PreferencesDialog::key_mod_to_str(
			el_gl_widget->get_wheel_zoom_x10()));
	settings.setValue("swap wheel zoom",
		el_gl_widget->get_swap_wheel_zoom());
	settings.setValue("rotate x", PreferencesDialog::key_to_str(
		el_gl_widget->get_rotate_x_key()));
	settings.setValue("rotate y", PreferencesDialog::key_to_str(
		el_gl_widget->get_rotate_y_key()));
	settings.setValue("rotate z", PreferencesDialog::key_to_str(
		el_gl_widget->get_rotate_z_key()));
	settings.setValue("scale", PreferencesDialog::key_to_str(
		el_gl_widget->get_scale_key()));

	settings.endGroup();
}

void MainWindow::load_mouse_settings(QSettings &settings)
{
	settings.beginGroup("mouse");

	el_gl_widget->set_select_rect_button(
		PreferencesDialog::str_to_mouse_button(
			settings.value("select rect button",
			PreferencesDialog::mouse_button_to_str(
				Qt::LeftButton)).toString()));
	el_gl_widget->set_click_button(PreferencesDialog::str_to_mouse_button(
		settings.value("click button",
		PreferencesDialog::mouse_button_to_str(
			Qt::LeftButton)).toString()));
	el_gl_widget->set_grab_button(PreferencesDialog::str_to_mouse_button(
		settings.value("grab button",
		PreferencesDialog::mouse_button_to_str(
			Qt::RightButton)).toString()));
	el_gl_widget->set_view_rotate_button(
		PreferencesDialog::str_to_mouse_button(
			settings.value("view rotate button",
			PreferencesDialog::mouse_button_to_str(
				Qt::MiddleButton)).toString()));
	el_gl_widget->set_select_rect_key_mod(
		PreferencesDialog::str_to_key_mod(
			settings.value("select rect key mod",
			PreferencesDialog::key_mod_to_str(
				Qt::ControlModifier)).toString()));
	el_gl_widget->set_click_key_mod(
		PreferencesDialog::str_to_key_mod(
			settings.value("click key mod",
			PreferencesDialog::key_mod_to_str(
				Qt::NoModifier)).toString()));
	el_gl_widget->set_grab_key_mod(
		PreferencesDialog::str_to_key_mod(
			settings.value("grab key mod",
			PreferencesDialog::key_mod_to_str(
				Qt::NoModifier)).toString()));
	el_gl_widget->set_view_rotate_key_mod(
		PreferencesDialog::str_to_key_mod(
			settings.value("view rotate key mod",
			PreferencesDialog::key_mod_to_str(
				Qt::NoModifier)).toString()));
	el_gl_widget->set_wheel_zoom_x10(PreferencesDialog::str_to_key_mod(
		settings.value("wheel zoom x10",
		PreferencesDialog::key_mod_to_str(Qt::ShiftModifier)).toString(
			)));
	el_gl_widget->set_swap_wheel_zoom(settings.value("swap wheel zoom",
		false).toBool());
	el_gl_widget->set_rotate_x_key(PreferencesDialog::str_to_key(
		settings.value("rotate x",
		PreferencesDialog::key_to_str(Qt::Key_X)).toString()));
	el_gl_widget->set_rotate_y_key(PreferencesDialog::str_to_key(
		settings.value("rotate y",
		PreferencesDialog::key_to_str(Qt::Key_Y)).toString()));
	el_gl_widget->set_rotate_z_key(PreferencesDialog::str_to_key(
		settings.value("rotate z",
		PreferencesDialog::key_to_str(Qt::Key_Z)).toString()));
	el_gl_widget->set_scale_key(PreferencesDialog::str_to_key(
		settings.value("scale",
		PreferencesDialog::key_to_str(Qt::Key_R)).toString()));

	settings.endGroup();
}

void MainWindow::save_dirs_settings(QSettings &settings)
{
	settings.beginGroup("dirs");

	settings.setValue("el_data_dir", m_el_data_dir);
	settings.setValue("el_extra_data_dir", m_el_extra_data_dir);

	settings.endGroup();
}

void MainWindow::load_dirs_settings(QSettings &settings)
{
	settings.beginGroup("dirs");

	m_el_data_dir = settings.value("el_data_dir", "/usr/share/games/EternalLands").toString();
	m_el_extra_data_dir = settings.value("el_extra_data_dir",
		"/home/daniel/Develop/el-opengl2/Eternal-Lands/data").toString();

	settings.endGroup();

	set_dirs();
}

void MainWindow::set_dirs()
{
	QStringList dirs;

	if (!m_el_data_dir.isEmpty())
	{
		dirs.append(m_el_data_dir);
	}

	if (!m_el_extra_data_dir.isEmpty())
	{
		dirs.append(m_el_extra_data_dir);
	}

	el_gl_widget->set_dirs(dirs);
	m_objects->set_dirs(dirs);
}

void MainWindow::save_terrain_textures_settings(QSettings &settings)
{
	QStringTerrainTextureDataQMap::iterator it, end;
	QStringList data;
	int index;

	settings.beginGroup("textures");

	end = m_terrain_texture_datas.end();
	index = 0;

	data.append(QString());
	data.append(QString());

	for (it = m_terrain_texture_datas.begin(); it != end; ++it)
	{
		data[0] = it->get_albedo_map();
		data[1] = it->get_extra_map();

		if (!it->get_use_extra_map())
		{
			data[1] = QString();
		}

		settings.setValue(QString("texture_%1").arg(index), data);

		index++;
	}

	settings.endGroup();
}

void MainWindow::load_terrain_textures_settings(QSettings &settings)
{
	QStringList textures;
	QStringList data;

	settings.beginGroup("textures");

	textures = settings.childKeys();

	foreach(const QString &key, textures)
	{
		data = settings.value(key, QStringList()).toStringList();

		if (data.size() == 0)
		{
			continue;
		}

		if (data.size() < 2)
		{
			data.append(QString());
		}

		m_terrain_texture_datas[data[0]] = TerrainTextureData(
			QIcon(), data[0], data[1], false, !data[1].isEmpty());
	}

	settings.endGroup();
}

void MainWindow::terrain_displacement_edit()
{
	QVector3D data;
	QVector2D size;
	float attenuation_size;
	int mask, attenuation, shape, effect;

	shape = -1;

	if (brush_shape_circle->isChecked())
	{
		size.setX(brush_radius->value());
		size.setY(brush_radius->value());

		shape = 0;
	}

	if (brush_shape_rect->isChecked())
	{
		size.setX(brush_width->value());
		size.setY(brush_height->value());

		shape = 1;
	}

	attenuation_size = brush_attenuation_size->value();

	attenuation = brush_attenuation->currentIndex();

	effect = displacement_effect->currentIndex();

	mask = 0x7;

	switch (effect)
	{
		case 0:
			data.setX(vector_brush_add_x->value());
			data.setY(vector_brush_add_y->value());
			data.setZ(vector_brush_add_z->value());

			break;
		case 1:
			data.setX(vector_brush_set_x->value());
			data.setY(vector_brush_set_y->value());
			data.setZ(vector_brush_set_z->value());

			mask = 0;

			if (vector_brush_set_checkbox_x->isChecked())
			{
				mask |= 1;
			}

			if (vector_brush_set_checkbox_y->isChecked())
			{
				mask |= 2;
			}

			if (vector_brush_set_checkbox_z->isChecked())
			{
				mask |= 4;
			}

			break;
		case 2:
			data.setX(vector_brush_smooth_strength_x->value());
			data.setY(vector_brush_smooth_strength_y->value());
			data.setZ(vector_brush_smooth_strength_z->value());

			data *= 0.01f;

			mask = 0;

			if (vector_brush_smooth_x->isChecked())
			{
				mask |= 1;
			}

			if (vector_brush_smooth_y->isChecked())
			{
				mask |= 2;
			}

			if (vector_brush_smooth_z->isChecked())
			{
				mask |= 4;
			}

			break;
	}

	el_gl_widget->change_terrain_displacement_values(data, size,
		attenuation_size, mask, attenuation, shape, effect);
}

void MainWindow::terrain_blend_edit()
{
	QListWidgetItem* item;
	QVector2D size;
	float attenuation_size, strength;
	int attenuation, shape, effect, layer;

	shape = -1;

	if (brush_shape_circle->isChecked())
	{
		size.setX(brush_radius->value());
		size.setY(brush_radius->value());

		shape = 0;
	}

	if (brush_shape_rect->isChecked())
	{
		size.setX(brush_width->value());
		size.setY(brush_height->value());

		shape = 1;
	}

	attenuation_size = brush_attenuation_size->value();

	attenuation = brush_attenuation->currentIndex();

	effect = blend_effect->currentIndex();

	strength = blend_strength->value() * 0.01f;

	layer = 0;

	item = terrain_layers->currentItem();

	if (item != 0)
	{
		layer = std::max(0, item->data(tlr_index).toInt() - 1);
	}

	el_gl_widget->change_terrain_blend_values(size, attenuation_size,
		strength, attenuation, shape, effect, layer);
}

void MainWindow::terrain_edit()
{
	switch (paint_tool_tabs->currentIndex())
	{
		case 0:
			terrain_displacement_edit();
			break;
		case 1:
			terrain_blend_edit();
			break;
		case 2:
			el_gl_widget->water_tile_edit(0);
			break;
		case 3:
//			el_gl_widget->ground_tile_edit(
//				ground_tile->currentText().toInt());
			break;
		case 4:
			el_gl_widget->height_edit(0);
			break;
	}
}

void MainWindow::about_el()
{

}

void MainWindow::set_object_transparency(const int value)
{
	el_gl_widget->set_object_transparency(value * 0.01f);
}

void MainWindow::set_object_blend(const int value)
{
	switch (value)
	{
		case 0:
			el_gl_widget->set_object_blend(bt_disabled);
			break;
		case 1:
			el_gl_widget->set_object_blend(
				bt_alpha_transparency_value);
			break;
		case 2:
			el_gl_widget->set_object_blend(
				bt_alpha_transparency_source_value);
			break;
	}
}

void MainWindow::set_object_selection(const int value)
{
	switch (value)
	{
		case 0:
			el_gl_widget->set_object_selection(st_select);
			break;
		case 1:
			el_gl_widget->set_object_selection(st_enter);
			break;
		case 2:
			el_gl_widget->set_object_selection(st_harvest);
			break;
	}
}

void MainWindow::set_blend(const BlendType value)
{
	switch (value)
	{
		case bt_alpha_transparency_value:
			transparency_type_1->setChecked(true);
			break;
		case bt_alpha_transparency_source_value:
			transparency_type_2->setChecked(true);
			break;
		default:
			transparency_type_0->setChecked(true);
			break;
	}
}

void MainWindow::set_selection(const SelectionType value)
{
	switch (value)
	{
		case st_enter:
			selection_type_1->setChecked(true);
			break;
		case st_harvest:
			selection_type_2->setChecked(true);
			break;
		default:
			selection_type_0->setChecked(true);
			break;
	}
}

void MainWindow::set_debug(const bool enabled)
{
	QStringList debug_modes;
	QString debug_mode;
	bool ok;

	if (!enabled)
	{
		el_gl_widget->set_debug_mode(-1);

		return;
	}

	debug_modes = el_gl_widget->get_debug_modes();

	debug_mode = QInputDialog::getItem(this, "Select debug mode",
		"debug mode", debug_modes, 0, false, &ok);

	if (!ok)
	{
		el_gl_widget->set_debug_mode(-1);

		return;
	}

	el_gl_widget->set_debug_mode(debug_modes.indexOf(debug_mode));
}

void MainWindow::rotate_left()
{
	camera_yaw->setValue(camera_yaw->value() - 5);
}

void MainWindow::rotate_right()
{
	camera_yaw->setValue(camera_yaw->value() + 5);
}

void MainWindow::roll_up()
{
	camera_roll->setValue(camera_roll->value() + 5);
}

void MainWindow::roll_down()
{
	camera_roll->setValue(camera_roll->value() - 5);
}

bool MainWindow::check_save_nodes()
{
	if (!m_changed_nodes)
	{
		return true;
	}

	switch (QMessageBox::question(this, "The document has been modified",
		"Do you want to save your changes?", QMessageBox::Yes |
		QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes))
	{
		case QMessageBox::Yes:
			save();
			return true;
		case QMessageBox::No:
			return true;
		case QMessageBox::Cancel:
			return false;
		default:
			return false;
	}
}

void MainWindow::new_nodes()
{
	if (!check_save_nodes())
	{
		return;
	}

	m_effect_nodes = boost::make_shared<el::EffectNodes>();
	graphicsView->scene()->clear();

	m_changed_nodes = false;
}

void MainWindow::save_nodes()
{
	if (m_file_name_nodes.isEmpty())
	{
		save_as_nodes();

		return;
	}

	save_nodes(m_file_name_nodes);
}

void MainWindow::save_as_nodes()
{
	QString file_name_nodes;

	file_name_nodes = QFileDialog::getSaveFileName(this, "save", QString(),
		"xml (*.xml)");

	if (!file_name_nodes.isEmpty())
	{
		save_nodes(file_name_nodes);
		m_file_name_nodes = file_name_nodes;
	}
}

void MainWindow::save_nodes(const QString &file_name_nodes)
{
	try
	{
		m_effect_nodes->save_xml(el::String(file_name_nodes.toUtf8()));
	}
	catch (...)
	{
	}

	m_changed_nodes = false;
}

void MainWindow::load_nodes()
{
	QString file_name_nodes;

	if (!check_save_nodes())
	{
		return;
	}

	file_name_nodes = QFileDialog::getOpenFileName(this, "open", QString(),
		"xml (*.xml)");

	if (!file_name_nodes.isEmpty())
	{
		load_nodes(file_name_nodes);
		m_file_name_nodes = file_name_nodes;
	}
}

void MainWindow::load_nodes(const QString &file_name_nodes)
{
	std::map<el::EffectNodePortPtr, QNEPort*> ports;
	std::vector<BasicNode*> basic_nodes;
	BasicNode* new_node;
	el::EffectNodePtr node;
	el::EffectConstant* constant_node;
	el::EffectTexture* texture_node;
	Uint32 i, count;

	graphicsView->scene()->clear();

	try
	{
		m_effect_nodes->load_xml(el::String(file_name_nodes.toUtf8()));
	}
	catch (...)
	{
		return;
	}

	count = m_effect_nodes->get_node_count();

	for (i = 0; i < count; ++i)
	{
		node = m_effect_nodes->get_node(i);

		constant_node = dynamic_cast<el::EffectConstant*>(node);

		if (constant_node != 0)
		{
			switch (constant_node->get_type())
			{
				case el::ect_color_rgb: 
					new_node = new ColorNode(m_effect_nodes,
						constant_node, "Color", 0,
						graphicsView->scene());
					break;
				case el::ect_direction_xy: 
					new_node = new DirectionNode(
						m_effect_nodes, constant_node,
						"Direction", 0,
						graphicsView->scene());
					break;
				case el::ect_float: 
					new_node = new ValuesNode(
						m_effect_nodes, constant_node,
						"Constant", 1, 0,
						graphicsView->scene());
					break;
				case el::ect_vec2: 
					new_node = new ValuesNode(
						m_effect_nodes, constant_node,
						"Constant", 2, 0,
						graphicsView->scene());
					break;
				case el::ect_vec3: 
					new_node = new ValuesNode(
						m_effect_nodes, constant_node,
						"Constant", 3, 0,
						graphicsView->scene());
					break;
				case el::ect_vec4: 
					new_node = new ValuesNode(
						m_effect_nodes, constant_node,
						"Constant", 4, 0,
						graphicsView->scene());
					break;
			}

			basic_nodes.push_back(new_node);

			continue;
		}

		texture_node = dynamic_cast<el::EffectTexture*>(node);

		if (texture_node != 0)
		{
			new_node = new TextureNode(m_effect_nodes, texture_node,
				node->get_name().get().c_str(), 0,
				graphicsView->scene());

			basic_nodes.push_back(new_node);

			continue;
		}

		new_node = new Node(m_effect_nodes, node,
			node->get_name().get().c_str(), 0,
			graphicsView->scene());

		basic_nodes.push_back(new_node);
	}

	m_nodes_editor->fill_ports_map(ports);

	BOOST_FOREACH(BasicNode* basic_node, basic_nodes)
	{
		basic_node->rebuild_connections(ports);
	}

	m_nodes_editor->update_connections();
	m_nodes_editor->update_tool_tips();

	m_changed_nodes = false;
}


void MainWindow::changed_nodes()
{
	m_changed_nodes = true;
}

void MainWindow::add_color()
{
	BasicNode* node;
	el::EffectConstant* ptr;

	ptr = boost::polymorphic_downcast<el::EffectConstant*>(
		m_effect_nodes->add_color(el::String("Color")));

	node = new ColorNode(m_effect_nodes, ptr, "Color", 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed_nodes();
}

void MainWindow::add_direction()
{
	BasicNode* node;
	el::EffectConstant* ptr;

	ptr = boost::polymorphic_downcast<el::EffectConstant*>(
		m_effect_nodes->add_direction(el::String("Direction")));

	node = new DirectionNode(m_effect_nodes, ptr, "Direction", 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed_nodes();
}

void MainWindow::add_constant()
{
	BasicNode* node;
	el::EffectConstant* ptr;
	int count;
	bool ok;

	count = QInputDialog::getInt(this, "Select constant count", "count",
		4, 1, 4, 1, &ok);

	if (!ok)
	{
		return;
	}

	ptr = boost::polymorphic_downcast<el::EffectConstant*>(
		m_effect_nodes->add_constant(el::String("Constant"), count));

	node = new ValuesNode(m_effect_nodes, ptr, "Constant", count, 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed_nodes();
}

void MainWindow::add_function()
{
	Node* node;
	QStringList functions;
	QString tmp;
	el::String function;
	el::EffectFunctionType type;
	el::EffectNodePtr ptr;
	Uint32 i;
	bool ok;

	for (i = 0; i < el::EffectFunctionUtil::get_effect_function_count();
		++i)
	{
		type = static_cast<el::EffectFunctionType>(i);

		if (el::EffectFunctionUtil::get_geometric(type) ||
			el::EffectFunctionUtil::get_trigonometric(type))
		{
			continue;
		}

		functions << QString::fromUtf8(el::EffectFunctionUtil::get_str(
			type).get().c_str());
	}

	tmp = QInputDialog::getItem(this, "Select function", "function",
		functions, 0, false, &ok);

	if (!ok)
	{
		return;
	}

	function = el::String(tmp.toUtf8());

	if (!el::EffectFunctionUtil::get_effect_function(function, type))
	{
		return;
	}

	ptr = m_effect_nodes->add_function(function, type);

	node = new Node(m_effect_nodes, ptr, "Function", 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed_nodes();
}

void MainWindow::add_geometric_function()
{
	Node* node;
	QStringList functions;
	QString tmp;
	el::String function;
	el::EffectFunctionType type;
	el::EffectNodePtr ptr;
	Uint32 i;
	bool ok;

	for (i = 0; i < el::EffectFunctionUtil::get_effect_function_count();
		++i)
	{
		type = static_cast<el::EffectFunctionType>(i);

		if (!el::EffectFunctionUtil::get_geometric(type))
		{
			continue;
		}

		functions << QString::fromUtf8(el::EffectFunctionUtil::get_str(
			type).get().c_str());
	}

	tmp = QInputDialog::getItem(this, "Select function", "function",
		functions, 0, false, &ok);

	if (!ok)
	{
		return;
	}

	function = el::String(tmp.toUtf8());

	if (!el::EffectFunctionUtil::get_effect_function(function, type))
	{
		return;
	}

	ptr = m_effect_nodes->add_function(function, type);

	node = new Node(m_effect_nodes, ptr, "Function", 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed_nodes();
}

void MainWindow::add_trigonemetric_function()
{
	Node* node;
	QStringList functions;
	QString tmp;
	el::String function;
	el::EffectFunctionType type;
	el::EffectNodePtr ptr;
	Uint32 i;
	bool ok;

	for (i = 0; i < el::EffectFunctionUtil::get_effect_function_count();
		++i)
	{
		type = static_cast<el::EffectFunctionType>(i);

		if (!el::EffectFunctionUtil::get_trigonometric(type))
		{
			continue;
		}

		functions << QString::fromUtf8(el::EffectFunctionUtil::get_str(
			type).get().c_str());
	}

	tmp = QInputDialog::getItem(this, "Select function", "function",
		functions, 0, false, &ok);

	if (!ok)
	{
		return;
	}

	function = el::String(tmp.toUtf8());

	if (!el::EffectFunctionUtil::get_effect_function(function, type))
	{
		return;
	}

	ptr = m_effect_nodes->add_function(function, type);

	node = new Node(m_effect_nodes, ptr, "Function", 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed_nodes();
}

void MainWindow::add_parameter()
{
	Node* node;
	QStringList parameters;
	QString tmp;
	el::String parameter;
	el::EffectParameterType type;
	el::EffectNodePtr ptr;
	Uint32 i;
	bool ok;

	for (i = 0; i < el::EffectParameterUtil::get_effect_parameter_count();
		++i)
	{
		parameters << QString::fromUtf8(
			el::EffectParameterUtil::get_str(static_cast<
				el::EffectParameterType>(i)).get().c_str());
	}

	tmp = QInputDialog::getItem(this, "Select parameter", "parameter",
		parameters, 0, false, &ok);

	if (!ok)
	{
		return;
	}

	parameter = el::String(tmp.toUtf8());

	if (!el::EffectParameterUtil::get_effect_parameter(parameter, type))
	{
		return;
	}

	ptr = m_effect_nodes->add_parameter(parameter, type);

	node = new Node(m_effect_nodes, ptr, "Parameter", 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed_nodes();
}

void MainWindow::add_texture()
{
	TextureNode* node;
	el::String name;
	el::EffectSamplerType sampler;
	el::EffectTextureType texture;
	el::EffectTexture* ptr;
	Uint16 texture_unit;
	bool project;

	m_texture_dialog->set_texture_units(m_sampler_names);

	if (m_texture_dialog->exec() != QDialog::Accepted)
	{
		return;
	}

	texture_unit = m_texture_dialog->get_texture_units();
/*
	name = el::String(m_names[texture_unit].toUtf8());
*/
	project = m_texture_dialog->get_project();

	sampler = el::est_sampler_2d;
/*
	switch (m_targets[texture_unit])
	{
		case el::ttt_texture_1d:
			if (project)
			{
				sampler = el::est_sampler_1d_project;
			}
			else
			{
				sampler = el::est_sampler_1d;
			}
			break;
		case el::ttt_texture_2d:
			if (project)
			{
				sampler = el::est_sampler_2d_project;
			}
			else
			{
				sampler = el::est_sampler_2d;
			}
			break;
		case el::ttt_texture_3d:
			if (project)
			{
				sampler = el::est_sampler_3d_project;
			}
			else
			{
				sampler = el::est_sampler_3d;
			}
			break;
		case el::ttt_texture_rectangle:
			if (project)
			{
				sampler = el::est_sampler_rectangle_project;
			}
			else
			{
				sampler = el::est_sampler_rectangle;
			}
			break;
		case el::ttt_texture_cube_map:
			sampler = el::est_sampler_cube_map;
			break;
		case el::ttt_texture_1d_array:
			sampler = el::est_sampler_1d_array;
			break;
		case el::ttt_texture_2d_array:
			sampler = el::est_sampler_2d_array;
			break;
		case el::ttt_texture_2d_multisample:
		case el::ttt_texture_2d_multisample_array:
			return;
	}
*/
	texture = el::ett_default;

	switch (m_texture_dialog->get_texture())
	{
		case el::ett_albedo:
			texture = el::ett_albedo;
			break;
		case el::ett_normal:
			texture = el::ett_normal;
			break;
		case el::ett_parallax:
			texture = el::ett_parallax;
			break;
		case el::ett_default:
			texture = el::ett_default;
			break;
		case el::ett_uv_offset:
			texture = el::ett_uv_offset;
			break;
	}

	ptr = boost::polymorphic_downcast<el::EffectTexture*>(
		m_effect_nodes->add_texture(name, sampler, texture,
			texture_unit));

	node = new TextureNode(m_effect_nodes, ptr, "Texture", 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed_nodes();
}

void MainWindow::add_output()
{
	Node* node;
	el::EffectNodePtr ptr;

	ptr = m_effect_nodes->add_output(el::String("Output"));

	node = new Node(m_effect_nodes, ptr, "Output", 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed_nodes();
}

void MainWindow::generate()
{
	el::ShaderSourceParameterVector vertex_parameters, fragment_parameters;
	el::ShaderSourceParameterVector::const_iterator it, end;
	el::StringStream vertex_str, fragment_str;
	el::Uint16StringMap array_layers;

	m_effect_nodes->write(array_layers, el::svt_130, el::eqt_high,
		vertex_parameters, fragment_parameters, vertex_str,
		fragment_str);

	std::cout << "vertex_str: " << vertex_str.str() << std::endl;
	std::cout << "fragment_str: " << fragment_str.str() << std::endl;

	end = vertex_parameters.end();

	for (it = vertex_parameters.begin(); it != end; ++it)
	{
		std::cout << "vertex parameter: " << *it << std::endl;
	}

	end = fragment_parameters.end();

	for (it = fragment_parameters.begin(); it != end; ++it)
	{
		std::cout << "fragment parameter: " << *it << std::endl;
	}
}

void MainWindow::layers_double_clicked(QListWidgetItem* item)
{
	QString name, albedo_map, extra_map;
	QIcon icon;
	float blend_size;
	int index;
	bool use_blend_size_sampler, use_blend_size, use_extra_map;

	m_terrain_textures->set_label(item->text());
	m_terrain_textures->set_blend_size(item->data(
		tlr_blend_size).toFloat());
	m_terrain_textures->set_use_blend_size(item->data(
		tlr_use_blend_size).toBool());
	m_terrain_textures->update_terrain_texture_datas();
	m_terrain_textures->set_texture(item->toolTip());

	if (m_terrain_textures->exec() == QDialog::Accepted)
	{
		name = m_terrain_textures->get_texture();

		if (!get_terrain_texture_data(name, icon, albedo_map,
			extra_map, use_blend_size_sampler, use_extra_map))
		{
			return;
		}

		blend_size = m_terrain_textures->get_blend_size();
		use_blend_size = m_terrain_textures->get_use_blend_size();

		index = item->data(tlr_index).toInt();

		item->setToolTip(name);
		item->setIcon(icon);

		item->setData(tlr_albedo_map, albedo_map);
		item->setData(tlr_extra_map, extra_map);
		item->setData(tlr_blend_size, blend_size);
		item->setData(tlr_use_blend_size_sampler, use_blend_size_sampler);
		item->setData(tlr_use_blend_size, use_blend_size);
		item->setData(tlr_use_extra_map, use_extra_map);

		el_gl_widget->set_terrain_material(albedo_map, extra_map,
			blend_size, use_blend_size_sampler, use_blend_size,
			use_extra_map, index);
	}
}

void MainWindow::import_terrain_height_map()
{
	QString name;

	name = QFileDialog::getOpenFileName(this, "Terrain height map",
		m_el_data_dir,
		tr("Images (*.dds *.dds.xz *.png *.jpeg *.jpg)"));

	if (!name.isEmpty())
	{
		el_gl_widget->import_terrain_height_map(name);
	}
}

void MainWindow::import_terrain_blend_map()
{
	QString name;

	name = QFileDialog::getOpenFileName(this, "Terrain blend map",
		m_el_data_dir,
		tr("Images (*.dds *.dds.xz *.png *.jpeg *.jpg)"));

	if (!name.isEmpty())
	{
		el_gl_widget->import_terrain_blend_map(name);
	}
}

void MainWindow::relax_terrain_uv()
{
	RelaxUV* relax_uv;

	relax_uv = new RelaxUV(el_gl_widget, m_progress_dialog, m_el_progress);

	QObject::connect(relax_uv, SIGNAL(done()), el_gl_widget,
		SLOT(update_terrain_dudv()));

	QThreadPool::globalInstance()->start(relax_uv);
}

void MainWindow::remove_all_copies_of_object()
{
	el_gl_widget->remove_all_copies_of_object();
}

void MainWindow::show_used_terrain_layers()
{
	QVector<int> terrain_layers_usage;
	QString message;
	int i, count, pixels, size;

	el_gl_widget->get_terrain_layers_usage(terrain_layers_usage, pixels);

	count = terrain_layers_usage.size();

	size = roundf(std::log10(pixels) + 0.5f);

	for (i = 0; i < count; ++i)
	{
		message += QString("layer %1: %2(%3%)\n").arg(i).arg(
			terrain_layers_usage[i], size).arg((100.0f *
				terrain_layers_usage[i]) / pixels, 6, 'f', 2);
	}

	QMessageBox::information(this, "Layer usage", message);
}

void MainWindow::terrain_displacement(const QVector3D &displacement)
{
	vector_brush_set_x->setValue(displacement.x());
	vector_brush_set_y->setValue(displacement.y());
	vector_brush_set_z->setValue(displacement.z());
}

void MainWindow::terrain_normal(const QVector3D &normal)
{
	QVector3D value;

	value = normal * scale_picked_normal->value();

	vector_brush_add_x->setValue(value.x());
	vector_brush_add_y->setValue(value.y());
	vector_brush_add_z->setValue(value.z());
}

void MainWindow::replace_all_copies_of_object()
{
	EditorObjectDescription object_description;

	el_gl_widget->get_object_description(object_description);

	m_objects->set_object(QString::fromUtf8(object_description.get_name(
		).get().c_str()));

	if (m_objects->exec() != QDialog::Accepted)
	{
		return;
	}

	el_gl_widget->set_all_copies_of_object_name(String(
		m_objects->get_object().toUtf8()));
}

void MainWindow::fill_terrain_blend_layer()
{
	QListWidgetItem* item;
	QStringList values;
	QString value;
	float strength;
	int effect, layer;
	bool ok;

	layer = 0;

	item = terrain_layers->currentItem();

	if (item != 0)
	{
		layer = std::max(0, item->data(tlr_index).toInt() - 1);
	}

	ok = false;

	values.push_back("set");
	values.push_back("slope");
	values.push_back("inverse slope");

	value = QInputDialog::getItem(this, "Select fill effect",
		"Effect", values, 0, false, &ok);

	if (!ok)
	{
		return;
	}

	effect = values.indexOf(value);

	if (effect == -1)
	{
		return;
	}

	ok = false;

	strength = QInputDialog::getDouble(this, "Select fill strength",
		"Strength", 0.5f, 0.0f, 1.0f, 2, &ok);

	if (!ok)
	{
		return;
	}

	el_gl_widget->fill_terrain_blend_layer(strength, effect, layer);
}
