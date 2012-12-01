#include "allterraintexturesdialog.hpp"
#include <QFileDialog>
#include <QColorDialog>
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
	QObject::connect(textures_remove_height, SIGNAL(clicked()), this,
		SLOT(remove_height_texture()));
	QObject::connect(textures_change_height, SIGNAL(clicked()), this,
		SLOT(change_height_texture()));
	QObject::connect(default_height, SIGNAL(valueChanged(double)),
		this, SLOT(change_height_value(double)));
	QObject::connect(textures_remove_specular, SIGNAL(clicked()), this,
		SLOT(remove_specular_texture()));
	QObject::connect(textures_change_specular, SIGNAL(clicked()), this,
		SLOT(change_specular_texture()));
	QObject::connect(default_specular, SIGNAL(clicked()),
		this, SLOT(change_specular_value()));
	QObject::connect(textures_remove_gloss, SIGNAL(clicked()), this,
		SLOT(remove_gloss_texture()));
	QObject::connect(textures_change_gloss, SIGNAL(clicked()), this,
		SLOT(change_gloss_texture()));
	QObject::connect(default_gloss, SIGNAL(valueChanged(double)),
		this, SLOT(change_gloss_value(double)));
	QObject::connect(blend_size, SIGNAL(valueChanged(double)),
		this, SLOT(change_blend_size_value(double)));

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

		it.value().set_list_widget_item(item);

		terrain_textures->insertItem(index, item);

		terrain_textures->setCurrentItem(item);

		index++;
	}
}

void AllTerrainTexturesDialog::update_buttons_enabled()
{
	QListWidgetItem* item;
	QPixmap pixmap(24, 24);
	QColor color;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		textures_remove->setEnabled(false);
		textures_change_height->setEnabled(false);
		textures_remove_height->setEnabled(false);
		label_height->setEnabled(false);
		default_height->setEnabled(false);
		textures_change_specular->setEnabled(false);
		textures_remove_specular->setEnabled(false);
		label_specular->setEnabled(false);
		default_specular->setEnabled(false);
		textures_change_gloss->setEnabled(false);
		textures_remove_gloss->setEnabled(false);
		label_gloss->setEnabled(false);
		default_gloss->setEnabled(false);
		label_blend_size->setEnabled(false);
		blend_size->setEnabled(false);

		return;
	}

	textures_remove->setEnabled(true);
	textures_change_height->setEnabled(true);
	textures_remove_height->setEnabled(item->data(ttr_height_map).toBool());
	label_height->setEnabled(!item->data(ttr_height_map).toBool());
	default_height->setEnabled(!item->data(ttr_height_map).toBool());
	textures_change_specular->setEnabled(true);
	textures_remove_specular->setEnabled(item->data(
		ttr_specular_map).toBool());
	label_specular->setEnabled(!item->data(ttr_specular_map).toBool());
	default_specular->setEnabled(!item->data(ttr_specular_map).toBool());
	textures_change_gloss->setEnabled(true);
	textures_remove_gloss->setEnabled(item->data(ttr_gloss_map).toBool());
	label_gloss->setEnabled(!item->data(ttr_gloss_map).toBool());
	default_gloss->setEnabled(!item->data(ttr_gloss_map).toBool());
	label_blend_size->setEnabled(
		item->data(ttr_use_blend_size_texture).toBool());
	blend_size->setEnabled(
		item->data(ttr_use_blend_size_texture).toBool());

	color = item->data(ttr_default_specular).value<QColor>();

	pixmap.fill(color);

	default_specular->setIcon(QIcon(pixmap));

	default_gloss->setValue(item->data(ttr_default_gloss).toFloat());
	default_height->setValue(item->data(ttr_default_height).toFloat());
	blend_size->setValue(item->data(ttr_blend_size).toFloat());
}

void AllTerrainTexturesDialog::add_texture()
{
	QDir el_data_dir, el_extra_data_dir;
	QString name, tmp, albedo_map;
	QIcon icon;
	bool use_blend_size_texture, ok;

	name = QFileDialog::getOpenFileName(this, "Add texture",
		m_el_data_dir, tr("Images (*.dds *.dds.xz)"));

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
		use_blend_size_texture, ok);

	if (!ok)
	{
		return;
	}

	m_terrain_texture_datas[albedo_map] = TerrainTextureData(icon,
		albedo_map, QString(), QString(), QString(), QColor(0, 0, 0),
		0.0f, 0.0f, 0.05f, use_blend_size_texture, false, false, false);

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

	m_terrain_texture_datas.remove(item->data(ttr_albedo_map).toString());

	terrain_textures->removeItemWidget(item);
}

void AllTerrainTexturesDialog::remove_height_texture()
{
	QListWidgetItem* item;
	QString albedo_map;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	m_terrain_texture_datas[albedo_map].set_height_map(QString());
	m_terrain_texture_datas[albedo_map].set_use_height_map(false);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);
}

void AllTerrainTexturesDialog::change_height_texture()
{
	QListWidgetItem* item;
	QDir el_data_dir, el_extra_data_dir;
	QString name, tmp, albedo_map, height_map;
	QIcon icon;
	bool ok;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	name = QFileDialog::getOpenFileName(this, "Add height texture",
		m_el_data_dir, tr("Images (*.dds *.dds.xz)"));

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
		height_map = tmp;
	}
	else
	{
		tmp = el_data_dir.relativeFilePath(name);

		tmp = el_data_dir.cleanPath(tmp);

		if (QDir::isRelativePath(tmp) && (tmp.indexOf("..") == -1))
		{
			height_map = tmp;
		}
	}

	if (height_map.isEmpty())
	{
		return;
	}

	ok = false;

	emit get_height_map_data(height_map, m_image_size, ok);

	if (!ok)
	{
		return;
	}

	m_terrain_texture_datas[albedo_map].set_height_map(height_map);
	m_terrain_texture_datas[albedo_map].set_use_height_map(true);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);
}

void AllTerrainTexturesDialog::change_height_value(const double value)
{
	QListWidgetItem* item;
	QString albedo_map;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	m_terrain_texture_datas[albedo_map].set_default_height(value);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);
}

void AllTerrainTexturesDialog::remove_specular_texture()
{
	QListWidgetItem* item;
	QString albedo_map;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	m_terrain_texture_datas[albedo_map].set_specular_map(QString());
	m_terrain_texture_datas[albedo_map].set_use_specular_map(false);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);
}

void AllTerrainTexturesDialog::change_specular_texture()
{
	QListWidgetItem* item;
	QDir el_data_dir, el_extra_data_dir;
	QString name, tmp, albedo_map, specular_map;
	QIcon icon;
	bool ok;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	name = QFileDialog::getOpenFileName(this, "Add specular texture",
		m_el_data_dir, tr("Images (*.dds *.dds.xz)"));

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
		specular_map = tmp;
	}
	else
	{
		tmp = el_data_dir.relativeFilePath(name);

		tmp = el_data_dir.cleanPath(tmp);

		if (QDir::isRelativePath(tmp) && (tmp.indexOf("..") == -1))
		{
			specular_map = tmp;
		}
	}

	if (specular_map.isEmpty())
	{
		return;
	}

	ok = false;

	emit get_specular_map_data(specular_map, m_image_size, ok);

	if (!ok)
	{
		return;
	}

	m_terrain_texture_datas[albedo_map].set_specular_map(specular_map);
	m_terrain_texture_datas[albedo_map].set_use_specular_map(true);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);
}

void AllTerrainTexturesDialog::change_specular_value()
{
	QListWidgetItem* item;
	QString albedo_map;
	QColor color;
	QPixmap pixmap(24, 24);

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	color = m_terrain_texture_datas[albedo_map].get_default_specular();

	color = QColorDialog::getColor(color, this);

	if (!color.isValid())
	{
		return;
	}

	m_terrain_texture_datas[albedo_map].set_default_specular(color);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);

	pixmap.fill(color);

	default_specular->setIcon(QIcon(pixmap));
}

void AllTerrainTexturesDialog::remove_gloss_texture()
{
	QListWidgetItem* item;
	QString albedo_map;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	m_terrain_texture_datas[albedo_map].set_gloss_map(QString());
	m_terrain_texture_datas[albedo_map].set_use_gloss_map(false);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);
}

void AllTerrainTexturesDialog::change_gloss_texture()
{
	QListWidgetItem* item;
	QDir el_data_dir, el_extra_data_dir;
	QString name, tmp, albedo_map, gloss_map;
	QIcon icon;
	bool ok;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	name = QFileDialog::getOpenFileName(this, "Add gloss texture",
		m_el_data_dir, tr("Images (*.dds *.dds.xz)"));

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
		gloss_map = tmp;
	}
	else
	{
		tmp = el_data_dir.relativeFilePath(name);

		tmp = el_data_dir.cleanPath(tmp);

		if (QDir::isRelativePath(tmp) && (tmp.indexOf("..") == -1))
		{
			gloss_map = tmp;
		}
	}

	if (gloss_map.isEmpty())
	{
		return;
	}

	ok = false;

	emit get_gloss_map_data(gloss_map, m_image_size, ok);

	if (!ok)
	{
		return;
	}

	m_terrain_texture_datas[albedo_map].set_gloss_map(gloss_map);
	m_terrain_texture_datas[albedo_map].set_use_gloss_map(true);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);
}

void AllTerrainTexturesDialog::change_gloss_value(const double value)
{
	QListWidgetItem* item;
	QString albedo_map;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	m_terrain_texture_datas[albedo_map].set_default_gloss(value);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);
}

void AllTerrainTexturesDialog::change_blend_size_value(const double value)
{
	QListWidgetItem* item;
	QString albedo_map;

	item = terrain_textures->currentItem();

	if (item == 0)
	{
		return;
	}

	albedo_map = item->data(ttr_albedo_map).toString();

	m_terrain_texture_datas[albedo_map].set_blend_size(value);

	m_terrain_texture_datas[albedo_map].set_list_widget_item(item);
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
