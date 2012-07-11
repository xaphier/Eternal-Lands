#include "mainwindow.hpp"

#include <QGraphicsScene>
#include <QInputDialog>
#include <QFileDialog>

#include "qnodeseditor.hpp"
#include "node.hpp"
#include "colornode.hpp"
#include "directionnode.hpp"
#include "valuesnode.hpp"
#include "../engine/node/effectconstant.hpp"
#include "../engine/node/effectfunction.hpp"
#include "../engine/node/effectparameter.hpp"
#include "../engine/node/effecttexture.hpp"
#include "../engine/node/effectoutput.hpp"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
	setupUi(this);

	QGraphicsScene *s = new QGraphicsScene();
	graphicsView->setScene(s);
	graphicsView->setRenderHint(QPainter::Antialiasing);
	s->setFont(graphicsView->font());
	// graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

	m_nodes_editor = new QNodesEditor(this);
	m_nodes_editor->install(s);

	connect(constant_button, SIGNAL(clicked()), this, SLOT(add_constant()));
	connect(function_button, SIGNAL(clicked()), this, SLOT(add_function()));
	connect(parameter_button, SIGNAL(clicked()), this,
		SLOT(add_parameter()));
	connect(output_button, SIGNAL(clicked()), this, SLOT(add_output()));
	connect(texture_button, SIGNAL(clicked()), this, SLOT(edit_direction()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::edit_direction()
{
	DirectionDialog* dialog;

	dialog = new DirectionDialog(this);

	dialog->show();
}

void MainWindow::add_constant()
{
	QNEBlock* node;
	QStringList constants;
	QString tmp;
	el::String constant;
	el::EffectConstantType type;
	Uint32 i;
	bool ok;

	for (i = 0; i < el::EffectConstantUtil::get_effect_constant_count();
		++i)
	{
		constants << QString::fromUtf8(el::EffectConstantUtil::get_str(
			static_cast<el::EffectConstantType>(i)).get().c_str());
	}

	tmp = QInputDialog::getItem(this, "Select constant", "constant",
		constants, 0, false, &ok);

	if (!ok)
	{
		return;
	}

	constant = el::String(tmp.toUtf8());

	if (!el::EffectConstantUtil::get_effect_constant(constant, type))
	{
		return;
	}

	switch (type)
	{
		case el::ect_direction_xy:
			node = new DirectionNode(new el::EffectConstant(
				el::String("Direction"), type, m_generator),
				"Direction", 0, graphicsView->scene());
			break;
		case el::ect_color_rgb:
			node = new ColorNode(new el::EffectConstant(
				el::String("Color"), type, m_generator),
				"Color", 0, graphicsView->scene());
			break;
		case el::ect_float:
			node = new ValuesNode(new el::EffectConstant(
				el::String("Constant"), type, m_generator),
				"Constant", 1, 0, graphicsView->scene());
			break;
		case el::ect_vec2:
			node = new ValuesNode(new el::EffectConstant(
				el::String("Constant"), type, m_generator),
				"Constant", 2, 0, graphicsView->scene());
			break;
		case el::ect_vec3:
			node = new ValuesNode(new el::EffectConstant(
				el::String("Constant"), type, m_generator),
				"Constant", 3, 0, graphicsView->scene());
			break;
		case el::ect_vec4:
			node = new ValuesNode(new el::EffectConstant(
				el::String("Constant"), type, m_generator),
				"Constant", 4, 0, graphicsView->scene());
			break;
		default:
			return;
	};

	node->setPos(graphicsView->sceneRect().center().toPoint());
}

void MainWindow::add_function()
{
	Node* node;
	QStringList functions;
	QString tmp;
	el::String function;
	el::EffectFunctionType type;
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

	node = new Node(new el::EffectFunction(function, type, m_generator),
		"Function", 0, graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());
}

void MainWindow::add_parameter()
{
	Node* node;
	QStringList parameters;
	QString tmp;
	el::String parameter;
	el::EffectParameterType type;
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

	node = new Node(new el::EffectParameter(parameter, type), "Parameter",
		0, graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());
}

void MainWindow::add_output()
{
	Node* node;

	node = new Node(new el::EffectOutput(el::String("Output")),
		"Output", 0, graphicsView->scene());

	node->setPos(graphicsView->sceneRect().center().toPoint());
}
