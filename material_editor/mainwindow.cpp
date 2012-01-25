#include <QtGui> 
#include "mainwindow.hpp"
#include "enginewidget.hpp"

MainWindow::MainWindow(QWidget *parent)
{
	EngineWidget* engine_widget;

	setupUi(this); // this sets up GUI
  
	m_data_dir = "/usr/share/games/EternalLands/";
  
        engine_widget = new EngineWidget(centralwidget);
        engine_widget->setObjectName(QString::fromUtf8("engine"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(25);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(engine_widget->sizePolicy().hasHeightForWidth());
        engine_widget->setSizePolicy(sizePolicy1);
        engine_widget->setMinimumSize(QSize(0, 0));

        horizontalLayout->addWidget(engine_widget);
	// signals/slots mechanism in action
	connect(action_ambient_color, SIGNAL(triggered()), engine_widget,
		SLOT(set_ambient_color()));
	connect(action_sun_color, SIGNAL(triggered()), engine_widget,
		SLOT(set_sun_color()));
	connect(change_diffuse, SIGNAL(clicked()), this,
		SLOT(set_diffuse_map()));
	connect(change_second_diffuse, SIGNAL(clicked()), this,
		SLOT(set_second_diffuse_map()));
	connect(change_normal, SIGNAL(clicked()), this,
		SLOT(set_normal_map()));
	connect(change_specular, SIGNAL(clicked()), this,
		SLOT(set_specular_map()));
	connect(change_emission, SIGNAL(clicked()), this,
		SLOT(set_emission_map()));
	connect(change_blend, SIGNAL(clicked()), this,
		SLOT(set_blend_map()));
	connect(clear_diffuse, SIGNAL(clicked()), diffuse, SLOT(clear()));
	connect(clear_second_diffuse, SIGNAL(clicked()), second_diffuse,
		SLOT(clear()));
	connect(clear_normal, SIGNAL(clicked()), normal, SLOT(clear()));
	connect(clear_specular, SIGNAL(clicked()), specular, SLOT(clear()));
	connect(clear_emission, SIGNAL(clicked()), emission, SLOT(clear()));
	connect(clear_blend, SIGNAL(clicked()), blend, SLOT(clear()));
	connect(engine_widget, SIGNAL(set_names(QStringList)), this,
		SLOT(set_names(QStringList)));
	connect(engine_widget, SIGNAL(set_uv_generation_names(QStringList)),
		this, SLOT(set_uv_generation_names(QStringList)));
	connect(engine_widget, SIGNAL(set_diffuse_mapping_names(QStringList)),
		this, SLOT(set_diffuse_mapping_names(QStringList)));
	connect(engine_widget, SIGNAL(set_normal_mapping_names(QStringList)),
		this, SLOT(set_normal_mapping_names(QStringList)));
	connect(engine_widget, SIGNAL(set_specular_mapping_names(QStringList)),
		this, SLOT(set_specular_mapping_names(QStringList)));
	connect(engine_widget, SIGNAL(set_emission_mapping_names(QStringList)),
		this, SLOT(set_emission_mapping_names(QStringList)));
	connect(engine_widget, SIGNAL(set_diffuse_map(QString)), diffuse,
		SLOT(setText(QString)));
	connect(engine_widget, SIGNAL(set_second_diffuse_map(QString)),
		second_diffuse, SLOT(setText(QString)));
	connect(engine_widget, SIGNAL(set_normal_map(QString)), normal,
		SLOT(setText(QString)));
	connect(engine_widget, SIGNAL(set_specular_map(QString)), specular,
		SLOT(setText(QString)));
	connect(engine_widget, SIGNAL(set_emission_map(QString)), emission,
		SLOT(setText(QString)));
	connect(engine_widget, SIGNAL(set_blend_map(QString)), blend,
		SLOT(setText(QString)));
	connect(name, SIGNAL(currentIndexChanged(QString)), engine_widget,
		SLOT(set_name(QString)));
	connect(engine_widget, SIGNAL(set_receives_shadows(bool)),
		receives_shadows, SLOT(setChecked(bool)));
	connect(engine_widget, SIGNAL(set_lighting(bool)), lighting,
		SLOT(setChecked(bool)));
	connect(engine_widget, SIGNAL(set_uv_generation_name(QString)),
		this, SLOT(set_uv_generation_name(QString)));
	connect(engine_widget, SIGNAL(set_diffuse_mapping_name(QString)),
		this, SLOT(set_diffuse_mapping_name(QString)));
	connect(engine_widget, SIGNAL(set_normal_mapping_name(QString)),
		this, SLOT(set_normal_mapping_name(QString)));
	connect(engine_widget, SIGNAL(set_specular_mapping_name(QString)),
		this, SLOT(set_specular_mapping_name(QString)));
	connect(engine_widget, SIGNAL(set_emission_mapping_name(QString)),
		this, SLOT(set_emission_mapping_name(QString)));
}

void MainWindow::set_diffuse_map()
{
	QString file;

	file = QFileDialog::getOpenFileName(this, tr("Open diffuse image"),
		m_data_dir.path(),
		tr("Image Files (*.dds *.dds.xz *.dds.gz)"));

	file = m_data_dir.relativeFilePath(file);

	diffuse->setText(file);
}

void MainWindow::set_second_diffuse_map()
{
	QString file;

	file = QFileDialog::getOpenFileName(this,
		tr("Open second diffuse image"), m_data_dir.path(),
		tr("Image Files (*.dds *.dds.xz *.dds.gz)"));

	file = m_data_dir.relativeFilePath(file);

	second_diffuse->setText(file);
}

void MainWindow::set_normal_map()
{
	QString file;

	file = QFileDialog::getOpenFileName(this, tr("Open normal image"),
		m_data_dir.path(),
		tr("Image Files (*.dds  *.dds.xz *.dds.gz)"));

	normal->setText(file);
}

void MainWindow::set_specular_map()
{
	QString file;

	file = QFileDialog::getOpenFileName(this, tr("Open specular image"),
		m_data_dir.path(),
		tr("Image Files (*.dds *.dds.xz *.dds.gz)"));

	file = m_data_dir.relativeFilePath(file);

	specular->setText(file);
}

void MainWindow::set_emission_map()
{
	QString file;

	file = QFileDialog::getOpenFileName(this, tr("Open emission image"),
		m_data_dir.path(),
		tr("Image Files (*.dds *.dds.xz *.dds.gz)"));

	file = m_data_dir.relativeFilePath(file);

	emission->setText(file);
}

void MainWindow::set_blend_map()
{
	QString file;

	file = QFileDialog::getOpenFileName(this, tr("Open blend image"),
		m_data_dir.path(),
		tr("Image Files (*.dds *.dds.xz *.dds.gz)"));

	file = m_data_dir.relativeFilePath(file);

	blend->setText(file);
}

void MainWindow::set_names(const QStringList values)
{
	name->clear();
	name->addItems(values);
}

void MainWindow::set_uv_generation_names(const QStringList values)
{
	uv_generation->clear();
	uv_generation->addItems(values);
}

void MainWindow::set_diffuse_mapping_names(const QStringList values)
{
	diffuse_mapping->clear();
	diffuse_mapping->addItems(values);
}

void MainWindow::set_normal_mapping_names(const QStringList values)
{
	normal_mapping->clear();
	normal_mapping->addItems(values);
}

void MainWindow::set_specular_mapping_names(const QStringList values)
{
	specular_mapping->clear();
	specular_mapping->addItems(values);
}

void MainWindow::set_emission_mapping_names(const QStringList values)
{
	emission_mapping->clear();
	emission_mapping->addItems(values);
}

void MainWindow::set_uv_generation_name(const QString value)
{
	int index;

	index = uv_generation->findText(value);

	uv_generation->setCurrentIndex(index);
}

void MainWindow::set_diffuse_mapping_name(const QString value)
{
	int index;

	index = diffuse_mapping->findText(value);

	diffuse_mapping->setCurrentIndex(index);
}

void MainWindow::set_normal_mapping_name(const QString value)
{
	int index;

	index = normal_mapping->findText(value);

	normal_mapping->setCurrentIndex(index);
}

void MainWindow::set_specular_mapping_name(const QString value)
{
	int index;

	index = specular_mapping->findText(value);

	specular_mapping->setCurrentIndex(index);
}

void MainWindow::set_emission_mapping_name(const QString value)
{
	int index;

	index = emission_mapping->findText(value);

	emission_mapping->setCurrentIndex(index);
}
