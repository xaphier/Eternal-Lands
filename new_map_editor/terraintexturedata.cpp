#include "terraintexturedata.hpp"

TerrainTextureData::TerrainTextureData(): m_default_specular(0, 0, 0), 
	m_default_gloss(0.0f), m_default_height(0.0f), m_blend_size(0.05f),
	m_use_blend_size_texture(false), m_use_specular_map(false),
	m_use_gloss_map(false), m_use_height_map(false)
{
}
TerrainTextureData::TerrainTextureData(const QIcon &icon,
	const QString &albedo_map, const QString &specular_map,
	const QString &gloss_map, const QString &height_map,
	const QColor &default_specular, const float default_gloss,
	const float default_height, const float blend_size,
	const bool use_blend_size_texture, const bool use_specular_map,
	const bool use_gloss_map, const bool use_height_map): m_icon(icon),
	m_albedo_map(albedo_map), m_specular_map(specular_map),
	m_gloss_map(gloss_map), m_height_map(height_map),
	m_default_specular(default_specular), m_default_gloss(default_gloss),
	m_default_height(default_height), m_blend_size(blend_size),
	m_use_blend_size_texture(use_blend_size_texture),
	m_use_specular_map(use_specular_map), m_use_gloss_map(use_gloss_map),
	m_use_height_map(use_height_map)
{
}

TerrainTextureData::~TerrainTextureData()
{
}

void TerrainTextureData::set_list_widget_item(QListWidgetItem* item) const
{
	item->setIcon(get_icon());
	item->setToolTip(get_tool_tip());
	item->setData(ttr_albedo_map, get_albedo_map());
	item->setData(ttr_height_map, get_height_map());
	item->setData(ttr_specular_map, get_specular_map());
	item->setData(ttr_gloss_map, get_gloss_map());
	item->setData(ttr_default_specular, get_default_specular());
	item->setData(ttr_default_gloss, get_default_gloss());
	item->setData(ttr_default_height, get_default_height());
	item->setData(ttr_blend_size, get_blend_size());
	item->setData(ttr_use_blend_size_texture, get_use_blend_size_texture());
	item->setData(ttr_use_specular_map, get_use_specular_map());
	item->setData(ttr_use_gloss_map, get_use_gloss_map());
	item->setData(ttr_use_height_map, get_use_height_map());
}

QString TerrainTextureData::get_tool_tip() const
{
	QString result;

	result = QString("albedo: %1\n").arg(get_albedo_map());

	if (get_use_specular_map())
	{
		result += QString("specular: %1\n").arg(get_specular_map());
	}
	else
	{
		result += QString("specular: %1\n").arg(
			get_default_specular().name());
	}

	if (get_use_gloss_map())
	{
		result += QString("gloss: %1\n").arg(get_gloss_map());
	}
	else
	{
		result += QString("gloss: %1\n").arg(get_default_gloss(), 0,
			'g', 2);
	}

	if (get_use_height_map())
	{
		result += QString("height: %1\n").arg(get_height_map());
	}
	else
	{
		result += QString("height: %1\n").arg(get_default_height(), 0,
			'g', 2);
	}

	if (get_use_blend_size_texture())
	{
		result += QString("blend size: %1\n").arg(get_blend_size(), 0,
			'g', 2);
	}

	return result;
}
