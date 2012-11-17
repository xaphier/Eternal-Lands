#include "allterraintexturesdialog.hpp"
#include <QFileDialog>
#include <QMessageBox>

AllTerrainTexturesDialog::AllTerrainTexturesDialog(const QString &el_data_dir,
	const QString &el_extra_data_dir, QWidget *parent): QDialog(parent),
	m_el_data_dir(el_data_dir), m_el_extra_data_dir(el_extra_data_dir)
{
	setupUi(this);

	QObject::connect(textures_remove, SIGNAL(clicked()), this,
		SLOT(remove_texture()));
	QObject::connect(textures_add, SIGNAL(clicked()), this,
		SLOT(add_texture()));
	QObject::connect(textures_clear, SIGNAL(clicked()), this,
		SLOT(clear_textures()));
	QObject::connect(textures_remove_extra, SIGNAL(clicked()), this,
		SLOT(remove_extra_texture()));
	QObject::connect(textures_change_extra, SIGNAL(clicked()), this,
		SLOT(change_extra_texture()));

	QObject::connect(textures_remove, SIGNAL(clicked()), this,
		SLOT(update_buttons_enabled()));
	QObject::connect(textures_add, SIGNAL(clicked()), this,
		SLOT(update_buttons_enabled()));
	QObject::connect(textures_clear, SIGNAL(clicked()), this,
		SLOT(update_buttons_enabled()));
	QObject::connect(textures_remove_extra, SIGNAL(clicked()), this,
		SLOT(update_buttons_enabled()));
	QObject::connect(textures_change_extra, SIGNAL(clicked()), this,
		SLOT(update_buttons_enabled()));

	QObject::connect(terrain_textures, SIGNAL(currentItemChanged(
		QListWidgetItem*, QListWidgetItem*)), this,
			SLOT(update_buttons_enabled()));
}

AllTerrainTexturesDialog::~AllTerrainTexturesDialog()
{
}

void AllTerrainTexturesDialog::set_terrain_texture_datas(
	const QStringTerrainTextureDataQMap &terrain_texture_datas)
{
	m_terrain_texture_datas = terrain_texture_datas;

	update_terrain_texture_datas();
}

void AllTerrainTexturesDialog::update_terrain_texture_datas()
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

		item->setData(tr_albedo_map,
			it.value().get_albedo_map());
		item->setData(tr_extra_map,
			it.value().get_extra_map());
		item->setData(tr_use_blend_size_sampler,
			it.value().get_use_blend_size_sampler());
		item->setData(tr_use_extra_map,
			it.value().get_use_extra_map());

		terrain_textures->insertItem(index, item);

		terrain_textures->setCurrentItem(item);

		index++;
	}
}

void AllTerrainTexturesDialog::update_buttons_enabled()
{
	QListWidgetItem* item;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		textures_remove->setEnabled(false);
		textures_remove_extra->setEnabled(false);
		textures_change_extra->setEnabled(false);

		return;
	}

	textures_remove->setEnabled(true);
	textures_remove_extra->setEnabled(item->data(tr_use_extra_map).toBool());
	textures_change_extra->setEnabled(true);
}

void AllTerrainTexturesDialog::add_texture()
{
	QDir el_data_dir, el_extra_data_dir;
	QString name, tmp, albedo_map;
	QIcon icon;
	bool use_blend_size_sampler, ok;

	name = QFileDialog::getOpenFileName(this, "Add texture",
		m_el_data_dir,
		tr("Images (*.dds *.dds.xz *.png *.jpeg *.jpg)"));

	if (name.isEmpty())
	{
		return;
	}

	el_data_dir = QDir(m_el_data_dir);
	el_extra_data_dir = QDir(m_el_extra_data_dir);

	tmp = el_extra_data_dir.relativeFilePath(name);

	tmp = el_extra_data_dir.cleanPath(tmp);

	if (QDir::isRelativePath(tmp) && (tmp.indexOf("..") == -1))
	{
		albedo_map = tmp;
	}
	else
	{
		tmp = el_data_dir.relativeFilePath(name);

		tmp = el_data_dir.cleanPath(tmp);

		if (QDir::isRelativePath(tmp) && (tmp.indexOf("..") == -1))
		{
			albedo_map = tmp;
		}
	}

	if (albedo_map.isEmpty())
	{
		return;
	}

	ok = false;

	emit get_albedo_map_data(albedo_map, m_icon_size, m_image_size, icon,
		use_blend_size_sampler, ok);

	if (!ok)
	{
		return;
	}

	m_terrain_texture_datas[albedo_map] = TerrainTextureData(icon,
		albedo_map, QString(), use_blend_size_sampler, false);

	update_terrain_texture_datas();
}

void AllTerrainTexturesDialog::remove_texture()
{
	QListWidgetItem* item;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	m_terrain_texture_datas.remove(item->data(tr_albedo_map).toString());

	terrain_textures->removeItemWidget(item);
}

void AllTerrainTexturesDialog::remove_extra_texture()
{
	QListWidgetItem* item;
	QString albedo_map;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(tr_albedo_map).toString();

	m_terrain_texture_datas[albedo_map].set_extra_map(QString());
	m_terrain_texture_datas[albedo_map].set_use_extra_map(false);

	item->setData(tr_extra_map, QString());
	item->setData(tr_use_extra_map, false);
}

void AllTerrainTexturesDialog::change_extra_texture()
{
	QListWidgetItem* item;
	QDir el_data_dir, el_extra_data_dir;
	QString name, tmp, albedo_map, extra_map;
	QIcon icon;
	bool ok;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(tr_albedo_map).toString();

	name = QFileDialog::getOpenFileName(this, "Add texture",
		m_el_data_dir,
		tr("Images (*.dds *.dds.xz *.png *.jpeg *.jpg)"));

	if (name.isEmpty())
	{
		return;
	}

	el_data_dir = QDir(m_el_data_dir);
	el_extra_data_dir = QDir(m_el_extra_data_dir);

	tmp = el_extra_data_dir.relativeFilePath(name);

	tmp = el_extra_data_dir.cleanPath(tmp);

	if (QDir::isRelativePath(tmp) && (tmp.indexOf("..") == -1))
	{
		extra_map = tmp;
	}
	else
	{
		tmp = el_data_dir.relativeFilePath(name);

		tmp = el_data_dir.cleanPath(tmp);

		if (QDir::isRelativePath(tmp) && (tmp.indexOf("..") == -1))
		{
			extra_map = tmp;
		}
	}

	if (extra_map.isEmpty())
	{
		return;
	}

	ok = false;

	emit get_extra_map_data(extra_map, m_image_size, ok);

	if (!ok)
	{
		return;
	}

	m_terrain_texture_datas[albedo_map].set_extra_map(extra_map);
	m_terrain_texture_datas[albedo_map].set_use_extra_map(true);

	item->setData(tr_extra_map, extra_map);
	item->setData(tr_use_extra_map, true);
}

void AllTerrainTexturesDialog::clear_textures()
{
	m_terrain_texture_datas.clear();

	terrain_textures->clear();
}

void AllTerrainTexturesDialog::set_icon_size(const QSize &size)
{
	m_icon_size = size;

	terrain_textures->setIconSize(size);
}

void AllTerrainTexturesDialog::set_image_size(const QSize &size)
{
	m_image_size = size;
}
