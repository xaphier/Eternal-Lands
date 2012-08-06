#include "mainwindow.hpp"

#include <QGraphicsScene>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

#include "qnodeseditor.hpp"
#include "node.hpp"
#include "colornode.hpp"
#include "directionnode.hpp"
#include "texturenode.hpp"
#include "valuesnode.hpp"
#include "../engine/node/effectnodes.hpp"
#include "../engine/node/effectnode.hpp"
#include "../engine/node/effectnodeport.hpp"
#include "../engine/node/effectconstant.hpp"
#include "../engine/node/effectfunction.hpp"
#include "../engine/node/effectparameter.hpp"
#include "../engine/node/effecttexture.hpp"
#include "../engine/node/effectoutput.hpp"
#include "../engine/shader/samplerparameterutil.hpp"
#include "../engine/shader/shadersourceparameter.hpp"
#include "../engine/texturetargetutil.hpp"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
	QAction* action;
	int i, count;

	setupUi(this);

	m_changed = false;

	QGraphicsScene *s = new QGraphicsScene();
	graphicsView->setScene(s);
	graphicsView->setRenderHint(QPainter::Antialiasing);
	s->setFont(graphicsView->font());
	// graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

	m_nodes_editor = new QNodesEditor(this);
	m_nodes_editor->install(s);

	connect(constant_button, SIGNAL(clicked()), this, SLOT(add_constant()));
	connect(function_button, SIGNAL(clicked()), this, SLOT(add_function()));
	connect(texture_button, SIGNAL(clicked()), this, SLOT(add_texture()));
	connect(parameter_button, SIGNAL(clicked()), this,
		SLOT(add_parameter()));
	connect(output_button, SIGNAL(clicked()), this, SLOT(add_output()));
	connect(color_button, SIGNAL(clicked()), this, SLOT(add_color()));
	connect(direction_button, SIGNAL(clicked()), this,
		SLOT(add_direction()));

	m_buttons.push_back(texture_unit_0);
	m_buttons.push_back(texture_unit_1);
	m_buttons.push_back(texture_unit_2);
	m_buttons.push_back(texture_unit_3);
	m_buttons.push_back(texture_unit_4);
	m_buttons.push_back(texture_unit_5);
	m_buttons.push_back(texture_unit_6);
	m_buttons.push_back(texture_unit_7);

	m_texture_unit_mapper = new QSignalMapper(this);

	count = m_buttons.size();

	for (i = 0; i < count; ++i)
	{
		m_targets.push_back(el::ttt_texture_2d);
		m_file_names.push_back(QStringList());
		m_texture_unit_mapper->setMapping(m_buttons[i], i);
		m_buttons[i]->setText(QString::fromUtf8(
			el::SamplerParameterUtil::get_str(
				static_cast<el::SamplerParameterType>(i)).get(
					).c_str()));
		m_names << m_buttons[i]->text();

		QObject::connect(m_buttons[i], SIGNAL(clicked(bool)),
			m_texture_unit_mapper, SLOT(map()));
	}

	QObject::connect(m_texture_unit_mapper, SIGNAL(mapped(const int)),
		this, SLOT(change_texture(const int)));

	m_texture_dialog = new TextureDialog(this);
	m_texture_unit_dialog = new TextureUnitDialog(this);

	connect(m_texture_dialog->texture_unit,
		SIGNAL(currentIndexChanged(int)), this,
		SLOT(texture_unit_changed(int)));

	m_effect_nodes = boost::make_shared<el::EffectNodes>(el::String());

	QObject::connect(action_new, SIGNAL(triggered()), this, SLOT(do_new()));
	QObject::connect(action_open, SIGNAL(triggered()), this, SLOT(load()));
	QObject::connect(action_save, SIGNAL(triggered()), this, SLOT(save()));
	QObject::connect(action_save_as, SIGNAL(triggered()), this,
		SLOT(save_as()));
	QObject::connect(action_generate, SIGNAL(triggered()), this,
		SLOT(generate()));

	action_new->setIcon(QIcon::fromTheme("document-new"));
	action_open->setIcon(QIcon::fromTheme("document-open"));
	action_save->setIcon(QIcon::fromTheme("document-save"));
	action_save_as->setIcon(QIcon::fromTheme("document-save-as"));
	action_exit->setIcon(QIcon::fromTheme("application-exit"));

	foreach(QDockWidget* dock_widget, findChildren<QDockWidget*>())
	{
		action = dock_widget->toggleViewAction();
		action->setIcon(dock_widget->windowIcon());
		action->setText(dock_widget->windowTitle());
		menu_windows->addAction(action);
	}
}

MainWindow::~MainWindow()
{
}

void MainWindow::changed()
{
	m_changed = true;
}

void MainWindow::do_new()
{
	if (!check_save())
	{
		return;
	}

	m_effect_nodes = boost::make_shared<el::EffectNodes>(el::String());
	graphicsView->scene()->clear();

	m_changed = false;
}

void MainWindow::save()
{
	if (m_file_name.isEmpty())
	{
		save_as();

		return;
	}

	save(m_file_name);
}

void MainWindow::save_as()
{
	QString file_name;

	file_name = QFileDialog::getSaveFileName(this, "save", QString(),
		"xml (*.xml)");

	if (!file_name.isEmpty())
	{
		save(file_name);
		m_file_name = file_name;
	}
}

void MainWindow::save(const QString &file_name)
{
	try
	{
		m_effect_nodes->save_xml(el::String(file_name.toUtf8()));
	}
	catch (...)
	{
	}

	m_changed = false;
}

void MainWindow::load()
{
	QString file_name;

	if (!check_save())
	{
		return;
	}

	file_name = QFileDialog::getOpenFileName(this, "open", QString(),
		"xml (*.xml)");

	if (!file_name.isEmpty())
	{
		load(file_name);
		m_file_name = file_name;
	}
}

void MainWindow::load(const QString &file_name)
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
		m_effect_nodes->load_xml(el::String(file_name.toUtf8()));
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

	m_changed = false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QMessageBox message_box;

	if (!m_changed)
	{
		event->accept();
		return;
	}

	message_box.setText("Closing.");
	message_box.setInformativeText("Do you want to save your changes "
		"before closing?");
	message_box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard
		| QMessageBox::Cancel);
	message_box.setDefaultButton(QMessageBox::Save);

	switch (message_box.exec())
	{
		case QMessageBox::Save:
			save();
			event->accept();
			return;
		case QMessageBox::Discard:
			event->accept();
			return;
		case QMessageBox::Cancel:
			event->ignore();
			return;
		default:
			return;
	}
}

bool MainWindow::check_save()
{
	QMessageBox message_box;

	if (!m_changed)
	{
		return true;
	}

	message_box.setText("The document has been modified.");
	message_box.setInformativeText("Do you want to save your changes?");
	message_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No
		| QMessageBox::Cancel);
	message_box.setDefaultButton(QMessageBox::Yes);

	switch (message_box.exec())
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

void MainWindow::texture_unit_changed(const int value)
{
	m_texture_dialog->set_target(m_targets[value]);

	changed();
}

void MainWindow::change_texture(const int index)
{
	m_texture_unit_dialog->set_name(m_names[index]);
	m_texture_unit_dialog->set_target(m_targets[index]);
	m_texture_unit_dialog->set_file_names(m_file_names[index]);
	m_texture_unit_dialog->set_texture_unit(index);

	if (m_texture_unit_dialog->exec() == QDialog::Accepted)
	{
		m_names[index] = m_texture_unit_dialog->get_name();
		m_buttons[index]->setText(m_names[index]);
		m_targets[index] = m_texture_unit_dialog->get_target();
		m_file_names[index] = m_texture_unit_dialog->get_file_names();

		changed();
	}
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

	changed();
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

	changed();
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

	changed();
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
		functions << QString::fromUtf8(el::EffectFunctionUtil::get_str(
			static_cast<el::EffectFunctionType>(i)).get().c_str());
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

	changed();
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

	changed();
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

	m_texture_dialog->set_texture_units(m_names);

	if (m_texture_dialog->exec() != QDialog::Accepted)
	{
		return;
	}

	texture_unit = m_texture_dialog->get_texture_units();

	name = el::String(m_names[texture_unit].toUtf8());

	project = m_texture_dialog->get_project();

	sampler = el::est_sampler_2d;

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

	changed();
}

void MainWindow::add_output()
{
	Node* node;
	el::EffectNodePtr ptr;

	ptr = m_effect_nodes->add_output(el::String("Output"));

	node = new Node(m_effect_nodes, ptr, "Output", 0,
		graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());

	changed();
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
