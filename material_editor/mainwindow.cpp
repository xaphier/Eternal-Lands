#include "mainwindow.hpp"

#include "qnodeseditor.hpp"
#include "qneblock.hpp"
#include "qneport.hpp"

#include <QGraphicsScene>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
   	setupUi(this);

	QGraphicsScene *s = new QGraphicsScene();
	graphicsView->setScene(s);
	graphicsView->setRenderHint(QPainter::Antialiasing);
	// graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

	m_nodes_editor = new QNodesEditor(this);
	m_nodes_editor->install(s);

	connect(constant_button, SIGNAL(toggled(bool)), this, SLOT(add_constant()));
}

MainWindow::~MainWindow()
{
}


void MainWindow::add_constant()
{
	QNEBlock *b = new QNEBlock(0, ui->graphicsView->scene());
	static const char* names[] = {"Vin", "Voutsadfasdf", "Imin", "Imax", "mul", "add", "sub", "div", "Conv", "FFT"};
	for (int i = 0; i < 4 + rand() % 3; i++)
	{
		b->addPort(names[rand() % 10], rand() % 2, 0, 0);
		b->setPos(graphicsView->sceneRect().center().toPoint());
	}
}
