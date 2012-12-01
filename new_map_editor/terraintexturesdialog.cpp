#include "terraintexturesdialog.hpp"
#include <QColorDialog>

TerrainTexturesDialog::TerrainTexturesDialog(
	const QStringTerrainTextureDataQMap &terrain_texture_datas,
	QWidget *parent): QDialog(parent),
	m_terrain_texture_datas(terrain_texture_datas)
{
	setupUi(this);

	QObject::connect(terrain_textures,
		SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
		SLOT(accept()));

	QObject::connect(terrain_textures,
		SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
		this, SLOT(texture_clicked(QListWidgetItem*,
			QListWidgetItem*)));

	QObject::connect(default_specular, SIGNAL(clicked()), this,
		SLOT(change_default_specular()));
}

TerrainTexturesDialog::~TerrainTexturesDialog()
{
}

void TerrainTexturesDialog::texture_clicked(QListWidgetItem* item,
	QListWidgetItem* previous)
{
	QColor color;
	QPixmap pixmap(24, 24);

	if (item == 0)
	{
		label_specular->setEnabled(false);
		default_specular->setEnabled(false);
		label_gloss->setEnabled(false);
		default_gloss->setEnabled(false);
		label_height->setEnabled(false);
		default_height->setEnabled(false);
		label_blend_size->setEnabled(false);
		blend_size->setEnabled(false);

		return;
	}

	label_specular->setEnabled(!item->data(ttr_use_specular_map).toBool());
	default_specular->setEnabled(
		!item->data(ttr_use_specular_map).toBool());
	label_gloss->setEnabled(!item->data(ttr_use_gloss_map).toBool());
	default_gloss->setEnabled(!item->data(ttr_use_gloss_map).toBool());
	label_height->setEnabled(!item->data(ttr_use_height_map).toBool());
	default_height->setEnabled(!item->data(ttr_use_height_map).toBool());
	label_blend_size->setEnabled(item->data(
		ttr_use_blend_size_texture).toBool());
	blend_size->setEnabled(item->data(
		ttr_use_blend_size_texture).toBool());

	color = item->data(ttr_default_specular).value<QColor>();

	pixmap.fill(color);

	default_specular->setIcon(QIcon(pixmap));

	default_gloss->setValue(item->data(ttr_default_gloss).toFloat());
	default_height->setValue(item->data(ttr_default_height).toFloat());
	blend_size->setValue(item->data(ttr_blend_size).toFloat());
}

void TerrainTexturesDialog::update_terrain_texture_datas()
{
	QListWidgetItem* item;
	QStringTerrainTextureDataQMap::const_iterator it, end;
	int index;

	terrain_textures->clear();

	end = m_terrain_texture_datas.constEnd();
	index = 0;

	for (it = m_terrain_texture_datas.constBegin(); it != end; ++it)
	{
		item = new QListWidgetItem(it.value().get_albedo_map());

		it.value().set_list_widget_item(item);

		terrain_textures->insertItem(index, item);

		terrain_textures->setCurrentItem(item);

		index++;
	}
}

void TerrainTexturesDialog::set_icon_size(const QSize &size)
{
	terrain_textures->setIconSize(size);
}

void TerrainTexturesDialog::set_texture(const QString &texture)
{
	QListWidgetItem* item;
	int i, count;

	count = terrain_textures->count();

	item = 0;

	for (i = 0; i < count; ++i)
	{
		item = terrain_textures->item(i);

		if (item->data(ttr_albedo_map).toString() == texture)
		{
			terrain_textures->setCurrentItem(item);
		}
	}
}

QString TerrainTexturesDialog::get_texture() const
{
	if (terrain_textures->selectedItems().size() != 1)
	{
		return QString();
	}

	return terrain_textures->selectedItems()[0]->data(
		ttr_albedo_map).toString();
}

void TerrainTexturesDialog::set_label(const QString &label)
{
	layer_label->setText(label);
}

void TerrainTexturesDialog::change_default_specular()
{
	QColor color;

	color = QColorDialog::getColor(m_color, this);

	if (!color.isValid())
	{
		return;
	}

	set_default_specular(color);
}

void TerrainTexturesDialog::set_default_specular(const QColor &value)
{
	QColor color;
	QPixmap pixmap(24, 24);

	m_color = value;

	pixmap.fill(color);

	default_specular->setIcon(QIcon(pixmap));
}

void TerrainTexturesDialog::set_default_gloss(const float value)
{
	default_gloss->setValue(value);
}

void TerrainTexturesDialog::set_default_height(const float value)
{
	default_height->setValue(value);
}

void TerrainTexturesDialog::set_blend_size(const float value)
{
	blend_size->setValue(value);
}

float TerrainTexturesDialog::get_default_gloss() const
{
	return default_gloss->value();
}

float TerrainTexturesDialog::get_default_height() const
{
	return default_height->value();
}

float TerrainTexturesDialog::get_blend_size() const
{
	return blend_size->value();
}
