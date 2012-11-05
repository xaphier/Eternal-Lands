#include "terraintexturesdialog.hpp"

TerrainTexturesDialog::TerrainTexturesDialog(
	const QStringTerrainTextureDataQMap &terrain_texture_datas,
	QWidget *parent): QDialog(parent),
	m_terrain_texture_datas(terrain_texture_datas)
{
	setupUi(this);

	QObject::connect(terrain_textures,
		SIGNAL(itemDoubleClicked(QListWidgetItem*)), this,
		SLOT(accept()));
}

TerrainTexturesDialog::~TerrainTexturesDialog()
{
}

void TerrainTexturesDialog::texture_clicked(QListWidgetItem* item)
{
	blend_size->setEnabled(item != 0);
	blend_size_value->setEnabled(item != 0);
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
		item = new QListWidgetItem();

		item->setIcon(it.value().get_icon());
		item->setToolTip(it.value().get_albedo_map());
		item->setData(Qt::UserRole,
			it.value().get_use_blend_size_sampler());

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

		if (item->toolTip() == texture)
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

	return terrain_textures->selectedItems()[0]->toolTip();
}

void TerrainTexturesDialog::set_label(const QString &label)
{
	layer_label->setText(label);
}

void TerrainTexturesDialog::set_use_blend_size(const bool enabled)
{
	blend_size->setChecked(enabled);
}

void TerrainTexturesDialog::set_blend_size(const float value)
{
	blend_size_value->setValue(value);
}


bool TerrainTexturesDialog::get_use_blend_size() const
{
	return blend_size->isChecked();
}

float TerrainTexturesDialog::get_blend_size() const
{
	return blend_size_value->value();
}
