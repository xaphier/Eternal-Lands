#include "mainwindow.hpp"
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include "newmapdialog.hpp"
#include "lightdata.hpp"
#include "editor/editorobjectdata.hpp"
#include "editor/editor.hpp"

MainWindow::MainWindow(QWidget* parent): QMainWindow(parent)
{
	setupUi(this);

	init_actions();

	m_material_count = 0;

	m_settings = new SettingsDialog(this);
	m_preferences = new PreferencesDialog(this);
	m_objects = new ObjectsDialog(this);

	action_time = new QSpinBox(this);
	action_time->setMaximum(359);
	action_time->setWrapping(true);
	action_time->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
	tool_bar->insertWidget(action_ambient, action_time);

	QObject::connect(action_add_object, SIGNAL(triggered(bool)), this, SLOT(add_object(bool)));
	QObject::connect(action_add_light, SIGNAL(triggered(bool)), this, SLOT(add_light(bool)));
	QObject::connect(action_wire_frame, SIGNAL(triggered(bool)), el_gl_widget, SLOT(set_wire_frame(bool)));

	QObject::connect(el_gl_widget, SIGNAL(update_object(bool)), this, SLOT(update_object(bool)));
	QObject::connect(el_gl_widget, SIGNAL(update_light(bool)), this, SLOT(update_light(bool)));
	QObject::connect(el_gl_widget, SIGNAL(deselect()), this, SLOT(deselect()));

	QObject::connect(x_translation, SIGNAL(valueChanged(double)), this, SLOT(update_translation()));
	QObject::connect(y_translation, SIGNAL(valueChanged(double)), this, SLOT(update_translation()));
	QObject::connect(z_translation, SIGNAL(valueChanged(double)), this, SLOT(update_translation()));
	QObject::connect(scale_value, SIGNAL(valueChanged(double)), this, SLOT(update_scale()));
	QObject::connect(x_rotation, SIGNAL(valueChanged(double)), this, SLOT(update_rotation()));
	QObject::connect(y_rotation, SIGNAL(valueChanged(double)), this, SLOT(update_rotation()));
	QObject::connect(z_rotation, SIGNAL(valueChanged(double)), this, SLOT(update_rotation()));

	QObject::connect(radius, SIGNAL(valueChanged(double)), el_gl_widget, SLOT(set_light_radius(double)));
	QObject::connect(light_color, SIGNAL(clicked()), this, SLOT(change_light_color()));
	QObject::connect(x_position, SIGNAL(valueChanged(double)), this, SLOT(update_position()));
	QObject::connect(y_position, SIGNAL(valueChanged(double)), this, SLOT(update_position()));
	QObject::connect(z_position, SIGNAL(valueChanged(double)), this, SLOT(update_position()));

	m_terrain_albedo_map_mapper = new QSignalMapper(this);
	m_terrain_albedo_map_mapper->setMapping(terrain_albedo_map_0, int(0));
	m_terrain_albedo_map_mapper->setMapping(terrain_albedo_map_1, int(1));
	m_terrain_albedo_map_mapper->setMapping(terrain_albedo_map_2, int(2));
	m_terrain_albedo_map_mapper->setMapping(terrain_albedo_map_3, int(3));
	m_terrain_albedo_map_mapper->setMapping(terrain_albedo_map_4, int(4));

	QObject::connect(terrain_albedo_map_0, SIGNAL(currentIndexChanged(int)),
		m_terrain_albedo_map_mapper, SLOT(map()));
	QObject::connect(terrain_albedo_map_1, SIGNAL(currentIndexChanged(int)),
		m_terrain_albedo_map_mapper, SLOT(map()));
	QObject::connect(terrain_albedo_map_2, SIGNAL(currentIndexChanged(int)),
		m_terrain_albedo_map_mapper, SLOT(map()));
	QObject::connect(terrain_albedo_map_3, SIGNAL(currentIndexChanged(int)),
		m_terrain_albedo_map_mapper, SLOT(map()));
	QObject::connect(terrain_albedo_map_4, SIGNAL(currentIndexChanged(int)),
		m_terrain_albedo_map_mapper, SLOT(map()));
	QObject::connect(m_terrain_albedo_map_mapper, SIGNAL(mapped(const int)),
		this, SLOT(set_terrain_albedo_map(const int)));

	QObject::connect(el_gl_widget, SIGNAL(terrain_edit(const int, const int)), this,
		SLOT(terrain_edit(const int, const int)));

	QObject::connect(action_undo, SIGNAL(triggered()), el_gl_widget, SLOT(undo()));
	QObject::connect(el_gl_widget, SIGNAL(can_undo(bool)), action_undo, SLOT(setEnabled(bool)));

	QObject::connect(action_height, SIGNAL(triggered(bool)), this, SLOT(height_mode(bool)));
	QObject::connect(action_height, SIGNAL(triggered(bool)), el_gl_widget, SLOT(set_terrain_editing(bool)));

	QObject::connect(action_remove, SIGNAL(triggered(bool)), this, SLOT(remove()));

	QObject::connect(move_l, SIGNAL(clicked()), action_move_l, SLOT(trigger()));
	QObject::connect(move_r, SIGNAL(clicked()), action_move_r, SLOT(trigger()));
	QObject::connect(move_u, SIGNAL(clicked()), action_move_u, SLOT(trigger()));
	QObject::connect(move_d, SIGNAL(clicked()), action_move_d, SLOT(trigger()));
	QObject::connect(rotate_l, SIGNAL(clicked()), action_rotate_l, SLOT(trigger()));
	QObject::connect(rotate_r, SIGNAL(clicked()), action_rotate_r, SLOT(trigger()));
	QObject::connect(zoom_in, SIGNAL(clicked()), action_zoom_in, SLOT(trigger()));
	QObject::connect(zoom_out, SIGNAL(clicked()), action_zoom_out, SLOT(trigger()));

	QObject::connect(action_new, SIGNAL(triggered(bool)), this, SLOT(new_map()));
	QObject::connect(action_open, SIGNAL(triggered(bool)), this, SLOT(open_map()));
//	QObject::connect(action_settings, SIGNAL(triggered(bool)), this, SLOT(settings()));
	QObject::connect(action_fog, SIGNAL(triggered(bool)), this, SLOT(set_fog()));
	QObject::connect(action_ambient, SIGNAL(triggered(bool)), this, SLOT(change_ambient()));
	QObject::connect(action_blend_image, SIGNAL(triggered(bool)), this,
		SLOT(change_blend_image_name()));
	QObject::connect(action_time, SIGNAL(valueChanged(int)), el_gl_widget,
		SLOT(set_game_minute(int)));
	QObject::connect(action_preferences, SIGNAL(triggered(bool)), this,
		SLOT(change_preferences()));

	QObject::connect(action_lights, SIGNAL(triggered(bool)),
		el_gl_widget, SLOT(set_draw_lights(bool)));
	QObject::connect(action_light_spheres, SIGNAL(triggered(bool)),
		el_gl_widget, SLOT(set_draw_light_spheres(bool)));
	QObject::connect(action_lights_enabled, SIGNAL(triggered(bool)),
		el_gl_widget, SLOT(set_lights_enabled(bool)));

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

	m_object_witdgets.push_back(x_translation);
	m_object_witdgets.push_back(y_translation);
	m_object_witdgets.push_back(z_translation);
	m_object_witdgets.push_back(scale_value);
	m_object_witdgets.push_back(x_rotation);
	m_object_witdgets.push_back(y_rotation);
	m_object_witdgets.push_back(z_rotation);
	m_object_witdgets.push_back(transparency_type_0);
	m_object_witdgets.push_back(transparency_type_1);
	m_object_witdgets.push_back(transparency_type_2);
	m_object_witdgets.push_back(transparency_value);
	m_object_witdgets.push_back(selection_type_0);
	m_object_witdgets.push_back(selection_type_1);
	m_object_witdgets.push_back(selection_type_2);
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

	m_progress_bar = new QProgressBar(status_bar);
	status_bar->addPermanentWidget(m_progress_bar, 1);
	m_progress.reset(new QProgress);

	QObject::connect(m_progress.get(), SIGNAL(set_range(const int, const int)), m_progress_bar, SLOT(setRange(const int, const int)), Qt::QueuedConnection);
	QObject::connect(m_progress.get(), SIGNAL(set_value(const int)), m_progress_bar, SLOT(setValue(const int)), Qt::QueuedConnection);
/*



	BOOST_FOREACH(const QString &str, ground_tile->currentText().toInt())
	{
		
		("3dobjects/tile" + str + ".dds");
	}
*/
	int i, count;

	count = ground_tile->count();

	for (i = 0; i < count; ++i)
	{
		QPixmap pixmap;

		pixmap = QPixmap::fromImage(el_gl_widget->get_icon(
			"3dobjects/tile" + ground_tile->itemText(i) + ".dds"));

		ground_tile->setItemIcon(i, QIcon(pixmap));
	}
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	save_settings();

	QMainWindow::closeEvent(event);
}

void MainWindow::save_settings()
{
	QSettings settings;

	settings.beginGroup("window");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.setValue("icon size", tool_bar->iconSize());
	settings.endGroup();
	save_shortcuts(settings);
	save_mouse_settings(settings);
	save_dirs_settings(settings);
	save_textures_settings(settings);
}

void MainWindow::load_settings()
{
	QSettings settings;

	settings.beginGroup("window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	tool_bar->setIconSize(settings.value("icon size", QSize(24, 24)).value<QSize>());
	settings.endGroup();

	load_shortcuts(settings);
	load_mouse_settings(settings);
	load_dirs_settings(settings);
	load_textures_settings(settings);
}

void MainWindow::update_object()
{
	QStringList default_materials, materials, object_materials;
	QString material;
	EditorObjectData object_data;
	unsigned int id, i;
	int index, j;

	el_gl_widget->get_object_data(object_data);

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

	x_translation->setValue(object_data.get_world_transformation(
		).get_translation()[0]);
	y_translation->setValue(object_data.get_world_transformation(
		).get_translation()[1]);
	z_translation->setValue(object_data.get_world_transformation(
		).get_translation()[2]);

	x_rotation->setValue(object_data.get_rotation_angles()[0]);
	y_rotation->setValue(object_data.get_rotation_angles()[1]);
	z_rotation->setValue(object_data.get_rotation_angles()[2]);

	scale_value->setValue(object_data.get_world_transformation(
		).get_scale() * 100.0f);

	set_blend(object_data.get_blend());
	set_selection(object_data.get_selection());

	mesh_name->setText(QString::fromUtf8(object_data.get_name().get().c_str()));

	id = object_data.get_id();

	object_id->setText(QVariant(id).toString());

	default_materials = el_gl_widget->get_default_materials(
		object_data.get_name());

	object_materials = default_materials;

	for (j = 0; j < default_materials.size(); ++j)
	{
		if (object_data.get_material_names().size() > j)
		{
			material = QString::fromUtf8(
				object_data.get_material_names()[j].get(
					).c_str());
		}

		if (!material.isEmpty())
		{
			object_materials[j] = material;
		}
	}

	m_material_count = object_materials.size();

	m_material_count = std::min(m_material_count,
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

void MainWindow::set_terrain_albedo_map(const QString &str, const int index)
{
	QComboBox* combobox;
	int idx;

	combobox = dynamic_cast<QComboBox*>(m_terrain_albedo_map_mapper->mapping(index));

	idx = combobox->findText(str);

	if (idx == -1)
	{
		add_item(str, terrain_albedo_map_0);
		add_item(str, terrain_albedo_map_1);
		add_item(str, terrain_albedo_map_2);
		add_item(str, terrain_albedo_map_3);
		add_item(str, terrain_albedo_map_4);
		idx = combobox->findText(str);
	}

	combobox->setCurrentIndex(idx);
}

void MainWindow::update_terrain()
{
	QStringList terrain_albedo_maps;
	int i;

	for (i = 0; i < 5; i++)
	{
		m_terrain_albedo_map_mapper->mapping(i)->blockSignals(true);
	}

	terrain_albedo_maps = el_gl_widget->get_terrain_albedo_maps();

	for (i = 0; i < std::min(terrain_albedo_maps.size(), 5); i++)
	{
		set_terrain_albedo_map(terrain_albedo_maps[i], i);
	}

	height_map->setText(el_gl_widget->get_terrain_height_map());
	blend_map->setText(el_gl_widget->get_terrain_blend_map());
	dudv_map->setText(el_gl_widget->get_terrain_dudv_map());

	for (i = 0; i < 5; i++)
	{
		m_terrain_albedo_map_mapper->mapping(i)->blockSignals(false);
	}
}

void MainWindow::update_object(const bool select)
{
	Uint16 renderable;
	int i;

	if (select && action_delete->isChecked())
	{
		el_gl_widget->remove_object();
		deselect();
	}
	else
	{
		i = -1;
		renderable = el_gl_widget->get_renderable();

/*		if (renderable == rt_terrain)
		{
			i = 1;
			update_terrain();
		}
		else
*/		{
			if (renderable == rt_object)
			{
				i = 0;
				update_object();
			}
			else
			{
				return;
			}
		}

		if (select)
		{
			properties->setCurrentIndex(i);
		}

		action_remove->setEnabled(properties->currentIndex() == i);
	}
}

void MainWindow::update_light(const bool select)
{
	LightData light;
	unsigned int id;

	if (select && action_delete->isChecked())
	{
		el_gl_widget->remove_light();
		deselect();
	}
	else
	{
		if (select)
		{
			properties->setCurrentIndex(2);
		}

		action_remove->setEnabled(properties->currentIndex() == 2);

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
}

void MainWindow::deselect()
{
	set_default_mode();
	action_remove->setEnabled(false);
}

void MainWindow::update_translation()
{
	glm::vec3 translation;

	translation[0] = x_translation->value();
	translation[1] = y_translation->value();
	translation[2] = z_translation->value();

	el_gl_widget->set_object_translation(translation);
}

void MainWindow::update_rotation()
{
	glm::vec3 rotation;

	rotation[0] = x_rotation->value();
	rotation[1] = y_rotation->value();
	rotation[2] = z_rotation->value();

	el_gl_widget->set_object_rotation(rotation);
}

void MainWindow::update_scale()
{
	el_gl_widget->set_object_scale(scale_value->value() * 0.01f);
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

void MainWindow::update_position()
{
	glm::vec3 position;

	position[0] = x_position->value();
	position[1] = y_position->value();
	position[2] = z_position->value();

	el_gl_widget->set_light_position(position);
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

void MainWindow::add_object(const bool value)
{
	if (value)
	{
		if (m_objects->exec() == QDialog::Accepted)
		{
			/*
			el_gl_widget->add_object(glm::vec4(1.0f, 1.0f, 1.0f, 0.0f),
				m_objects->get_type(), m_objects->get_server_id(),
				m_objects->get_object());
			*/
		}
		else
		{
			action_add_object->setChecked(false);
			el_gl_widget->disable_object();
		}
	}
	else
	{
		el_gl_widget->disable_object();
	}
}

void MainWindow::add_light(const bool value)
{
	if (value)
	{
		el_gl_widget->add_light();
	}
	else
	{
		el_gl_widget->disable_light();
	}
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
		case rt_none:
			return;
	}

	deselect();
}

void MainWindow::height_mode(const bool checked)
{
	if (checked)
	{
		action_remove->setEnabled(false);
		properties->setCurrentIndex(3);
	}
	else
	{
		set_default_mode();
	}
}

void MainWindow::set_terrain_albedo_map(const int index)
{
	QString file_name;

	file_name = dynamic_cast<QComboBox*>(
		m_terrain_albedo_map_mapper->mapping(index))->currentText();

	el_gl_widget->set_terrain_albedo_map(file_name, index);
}

void MainWindow::set_fog()
{
	el_gl_widget->set_fog(glm::vec3(1.0f), 2.5f);
}

void MainWindow::open_map()
{
	QString file_name;

	file_name = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
		tr("EL-map (*.elm *.elm.gz *.elm.xz); All files (*.*)"));

	if (!file_name.isEmpty())
	{
		el_gl_widget->open_map(file_name);
	}
}

void MainWindow::set_default_mode()
{
	properties->setCurrentIndex(4);
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

void MainWindow::change_ambient()
{
	QColor color;
	glm::vec3 ambient_color;

	ambient_color = el_gl_widget->get_ambient();

	color = QColor::fromRgbF(ambient_color[0], ambient_color[1],
		ambient_color[2]);

	color = QColorDialog::getColor(color, this, tr("Scene ambient color"));

	if (color.isValid())
	{
		ambient_color[0] = color.redF();
		ambient_color[1] = color.greenF();
		ambient_color[2] = color.blueF();

		el_gl_widget->set_ambient(ambient_color);
	}
}

void MainWindow::change_blend_image_name()
{
	QString blend_image_name;
	bool ok;

	blend_image_name = el_gl_widget->get_blend_image_name();

	blend_image_name = QInputDialog::getText(this, tr("Blend image"), tr("File name:"),
		QLineEdit::Normal, blend_image_name, &ok);

	if (ok && !blend_image_name.isEmpty())
	{
		el_gl_widget->set_blend_image_name(blend_image_name);
	}
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
	new_map->set_diffuse_textures(m_textures);

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
	m_preferences->set_click_button(el_gl_widget->get_click_button());
	m_preferences->set_wheel_zoom_x10(el_gl_widget->get_wheel_zoom_x10());
	m_preferences->set_swap_wheel_zoom(el_gl_widget->get_swap_wheel_zoom());
	m_preferences->set_toolbar_icon_size(tool_bar->iconSize());
	m_preferences->set_dirs(el_gl_widget->get_dirs());
	m_preferences->set_textures(m_textures);

	if (m_preferences->exec() == QDialog::Accepted)
	{
		el_gl_widget->set_click_button(m_preferences->get_click_button());
		el_gl_widget->set_wheel_zoom_x10(m_preferences->get_wheel_zoom_x10());
		el_gl_widget->set_swap_wheel_zoom(m_preferences->get_swap_wheel_zoom());
		tool_bar->setIconSize(m_preferences->get_toolbar_icon_size());
		el_gl_widget->set_dirs(m_preferences->get_dirs());
		m_objects->set_dirs(m_preferences->get_dirs());
		set_textures(m_preferences->get_textures());
		save_settings();
	}
}

void MainWindow::init_actions()
{
	QAction* action;
	QIcon icon;

	icon.addFile(QString::fromUtf8(":/icons/gtk-execute.png"), QSize(), QIcon::Normal,
		QIcon::Off);

	action = tool_bar->toggleViewAction();
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

	action_move_l->setShortcutContext(Qt::ApplicationShortcut);
	action_move_r->setText("Move Right");
	action_move_r->setIcon(move_r->icon());

	action_move_u = new QAction(this);

	action_move_l->setShortcutContext(Qt::ApplicationShortcut);
	action_move_u->setText("Move Up");
	action_move_u->setIcon(move_u->icon());

	action_move_d = new QAction(this);

	action_move_l->setShortcutContext(Qt::ApplicationShortcut);
	action_move_d->setText("Move Down");
	action_move_d->setIcon(move_d->icon());

	action_rotate_l = new QAction(this);

	action_move_l->setShortcutContext(Qt::ApplicationShortcut);
	action_rotate_l->setText("Rotate Left");
	action_rotate_l->setIcon(rotate_l->icon());

	action_rotate_r = new QAction(this);

	action_move_l->setShortcutContext(Qt::ApplicationShortcut);
	action_rotate_r->setText("Rotate Right");
	action_rotate_r->setIcon(rotate_r->icon());

	action_zoom_in = new QAction(this);

	action_move_l->setShortcutContext(Qt::ApplicationShortcut);
	action_zoom_in->setText("Zoom In");
	action_zoom_in->setIcon(zoom_in->icon());

	action_zoom_out = new QAction(this);

	action_move_l->setShortcutContext(Qt::ApplicationShortcut);
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

	QObject::connect(action_move_l, SIGNAL(triggered()), el_gl_widget, SLOT(move_left()));
	QObject::connect(action_move_r, SIGNAL(triggered()), el_gl_widget, SLOT(move_right()));
	QObject::connect(action_move_u, SIGNAL(triggered()), el_gl_widget, SLOT(move_up()));
	QObject::connect(action_move_d, SIGNAL(triggered()), el_gl_widget, SLOT(move_down()));
	QObject::connect(action_rotate_l, SIGNAL(triggered()), el_gl_widget, SLOT(rotate_left()));
	QObject::connect(action_rotate_r, SIGNAL(triggered()), el_gl_widget, SLOT(rotate_right()));
	QObject::connect(action_zoom_in, SIGNAL(triggered()), el_gl_widget, SLOT(zoom_in()));
	QObject::connect(action_zoom_out, SIGNAL(triggered()), el_gl_widget, SLOT(zoom_out()));
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
					action->shortcut().toString(QKeySequence::NativeText));
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
					action->data().toString(), QKeySequence::mnemonic(
					action->text()).toString()).toString()));
			}
		}
	}

	settings.endGroup();
}

void MainWindow::save_mouse_settings(QSettings &settings)
{
	settings.beginGroup("mouse");

	settings.setValue("Click button", PreferencesDialog::mouse_button_to_str(el_gl_widget->get_click_button()));
	settings.setValue("Wheel zoom x10", PreferencesDialog::key_mod_to_str(el_gl_widget->get_wheel_zoom_x10()));
	settings.setValue("Swap wheel zoom", el_gl_widget->get_swap_wheel_zoom());

	settings.endGroup();
}

void MainWindow::load_mouse_settings(QSettings &settings)
{
	settings.beginGroup("mouse");

	el_gl_widget->set_click_button(PreferencesDialog::str_to_mouse_button(settings.value(
		"Click button", PreferencesDialog::mouse_button_to_str(Qt::LeftButton)).toString()));
	el_gl_widget->set_wheel_zoom_x10(PreferencesDialog::str_to_key_mod(settings.value(
		"Wheel zoom x10", PreferencesDialog::key_mod_to_str(Qt::ShiftModifier)).toString()));
	el_gl_widget->set_swap_wheel_zoom(settings.value("Swap wheel zoom", false).toBool());
	
	settings.endGroup();
}

void MainWindow::save_dirs_settings(QSettings &settings)
{
	QStringList dirs;
	int i, size;

	dirs = el_gl_widget->get_dirs();

	size = dirs.size();

	settings.beginWriteArray("dirs");

	for (i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		settings.setValue("dir", dirs.at(i));
	}

	settings.endArray();
}

void MainWindow::load_dirs_settings(QSettings &settings)
{
	QStringList dirs;
	int i, size;

	size = settings.beginReadArray("dirs");

	for (i = 0; i < size; ++i)
	{
		settings.setArrayIndex(i);
		dirs.append(settings.value("dir").toString());
	}

	settings.endArray();

	el_gl_widget->set_dirs(dirs);
	m_objects->set_dirs(dirs);
}

void MainWindow::set_textures(const QStringList &textures)
{
	m_textures = textures;

	m_preferences->set_textures(m_textures);

	set_items(m_textures, terrain_albedo_map_0);
	set_items(m_textures, terrain_albedo_map_1);
	set_items(m_textures, terrain_albedo_map_2);
	set_items(m_textures, terrain_albedo_map_3);
}

void MainWindow::set_items(const QStringList &strs, QComboBox* combobox)
{
	QString selection;
	int index;

	combobox->blockSignals(true);

	selection = combobox->currentText();

	combobox->clear();
	combobox->addItems(strs);

	index = combobox->findText(selection);

	combobox->setCurrentIndex(index);

	combobox->blockSignals(false);
}

void MainWindow::save_textures_settings(QSettings &settings)
{
	settings.beginGroup("textures");

	settings.setValue("terrain", m_textures);

	settings.endGroup();
}

void MainWindow::load_textures_settings(QSettings &settings)
{
	QStringList textures;
	int i;

	textures.append("textures/dirt.dds");
	textures.append("textures/grass.dds");
	textures.append("textures/mud.dds");
	textures.append("textures/stone.dds");

	for (i = 1; i <= 50; i++)
	{
		textures.append("3dobjects/tile" + QString::number(i) + ".dds");
	}

	for (i = 231; i <= 233; i++)
	{
		textures.append("3dobjects/tile" + QString::number(i) + ".dds");
	}

	for (i = 240; i <= 242; i++)
	{
		textures.append("3dobjects/tile" + QString::number(i) + ".dds");
	}

	settings.beginGroup("textures");

	set_textures(settings.value("terrain", textures).toStringList());

	settings.endGroup();
}

void MainWindow::terrain_edit(const int x, const int y)
{
	switch (paint_tool_box->currentIndex())
	{
		case 0:
			el_gl_widget->terrain_height_edit(x, y, height_brush_strength->value()
				* 0.01f, height_brush_radius->value(),
				height_brush_type->currentIndex());
			break;
		case 1:
			el_gl_widget->terrain_layer_edit(x, y, layer_index->value(),
				layer_brush_strength->value() * 0.01f, layer_brush_radius->value(),
				layer_brush_type->currentIndex());
			break;
		case 2:
			el_gl_widget->ground_tile_edit(x, y, ground_tile->currentText().toInt());
			break;
		case 3:
			el_gl_widget->water_tile_edit(x, y, 0);
			break;
		case 4:
			el_gl_widget->height_edit(x, y, 0);
			break;
	}
}

void MainWindow::about_el()
{

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
