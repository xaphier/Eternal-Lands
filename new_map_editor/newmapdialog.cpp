#include "newmapdialog.hpp"
#include <QFileDialog>
#include "elglwidget.hpp"

NewMapDialog::NewMapDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);

	QObject::connect(image_load, SIGNAL(clicked(bool)), this, SLOT(load_image()));
	QObject::connect(image, SIGNAL(clicked(bool)), this, SLOT(check_ok()));
}

void NewMapDialog::set_diffuse_textures(const QStringList &textures)
{
	terrain_diffuse_texture_0->addItems(textures);
	terrain_diffuse_texture_1->addItems(textures);
	terrain_diffuse_texture_2->addItems(textures);
	terrain_diffuse_texture_3->addItems(textures);
	terrain_diffuse_texture_4->addItems(textures);
}

void NewMapDialog::set_normal_textures(const QStringList &textures)
{
	terrain_normal_texture_0->addItems(textures);
	terrain_normal_texture_1->addItems(textures);
	terrain_normal_texture_2->addItems(textures);
	terrain_normal_texture_3->addItems(textures);
	terrain_normal_texture_4->addItems(textures);
}

void NewMapDialog::load_image()
{
	QString file_name, filter;

	ELGLWidget::get_file_extensions_filter(filter);

	file_name = QFileDialog::getOpenFileName(this, tr("Import terrain map"), ".",
		filter, 0, 0);

	if (!file_name.isEmpty())
	{
		image_name->setText(file_name);
		ok->setEnabled(true);
	}
}

void NewMapDialog::check_ok()
{
	ok->setEnabled(!image_name->text().isEmpty());
}
