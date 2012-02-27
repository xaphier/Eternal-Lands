#include "settingsdialog.hpp"
#include <QColorDialog>
#include <QFileDialog>

SettingsDialog::SettingsDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);

	QObject::connect(ambient, SIGNAL(clicked()), this, SLOT(select_ambient_color()));
	QObject::connect(save, SIGNAL(clicked()), this, SLOT(select_blend_image()));
}

void SettingsDialog::set_ambient_color(const QColor &ambient_color)
{
	QPixmap pixmap(150, 15);

	m_ambient_color = ambient_color;

	pixmap.fill(m_ambient_color);

	ambient->setIcon(QIcon(pixmap));
}

void SettingsDialog::select_ambient_color()
{
	QColor ambient_color;

	ambient_color = QColorDialog::getColor(get_ambient_color(), this);

	if (ambient_color.isValid())
	{
		set_ambient_color(ambient_color);
	}
}

void SettingsDialog::select_blend_image()
{
	QString file_name;

	file_name = get_blend_image_name();

	file_name = QFileDialog::getSaveFileName(this, tr("Save blend image"), file_name,
		tr("DDS (*.dds)"), 0, 0);

	if (!file_name.isEmpty())
	{
		set_blend_image_name(file_name);
	}
}
