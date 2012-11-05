#include "initterraindialog.hpp"
#include "prerequisites.hpp"
#include "abstractterrain.hpp"
#include <QFileDialog>
#include <QMessageBox>

InitTerrainDialog::InitTerrainDialog(QWidget *parent): QDialog(parent)
{
	setupUi(this);

	QObject::connect(patches_x, SIGNAL(valueChanged(const int)), this,
		SLOT(change_patches()));
	QObject::connect(patches_y, SIGNAL(valueChanged(const int)), this,
		SLOT(change_patches()));
	QObject::connect(height_map_checkbox, SIGNAL(toggled(bool)), this,
		SLOT(change_height_map_toggle(const bool)));
	QObject::connect(height_map_change, SIGNAL(clicked()), this,
		SLOT(change_height_map()));

	change_patches();
}

InitTerrainDialog::~InitTerrainDialog()
{
}

void InitTerrainDialog::change_height_map_toggle(const bool toggle)
{
	if (toggle)
	{
		change_height_map();
		height_map_checkbox->setChecked(
			!height_map_name->text().isEmpty());
	}
	else
	{
		height_map_name->setText(QString());
	}
}

void InitTerrainDialog::change_height_map()
{
	QString name;
	QSize size;
	int x, y;
	bool ok;

	name = QFileDialog::getOpenFileName(this, "Add texture",
		QString(), tr("Images (*.dds *.dds.xz *.png *.jpeg *.jpg)"));

	if (name.isEmpty())
	{
		return;
	}

	ok = false;

	emit get_height_map_data(name, size, ok);

	if (!ok)
	{
		return;
	}

	x = size.width() - 1;
	y = size.height() - 1;
	x += eternal_lands::AbstractTerrain::get_tile_size() - 1;
	y += eternal_lands::AbstractTerrain::get_tile_size() - 1;
	x /= eternal_lands::AbstractTerrain::get_tile_size();
	y /= eternal_lands::AbstractTerrain::get_tile_size();

	patches_x->setValue(x);
	patches_y->setValue(y);

	change_patches();

	height_map_name->setText(name);
}

void InitTerrainDialog::change_patches()
{
	float fx, fy;
	int x, y;

	x = patches_x->value();
	y = patches_y->value();

	x *= eternal_lands::AbstractTerrain::get_tile_size();
	y *= eternal_lands::AbstractTerrain::get_tile_size();

	m_size.setWidth(x + 1);
	m_size.setHeight(y + 1);

	fx = x * eternal_lands::AbstractTerrain::get_patch_scale();
	fy = y * eternal_lands::AbstractTerrain::get_patch_scale();

	size_x->setText(QString::number(fx) + tr("m"));
	size_y->setText(QString::number(fy) + tr("m"));
}

QString InitTerrainDialog::get_height_map() const
{
	return height_map_name->text();
}
