#include "terraintexturedata.hpp"

TerrainTextureData::TerrainTextureData(): m_use_blend_size_sampler(false),
	m_use_extra_map(false)
{
}

TerrainTextureData::TerrainTextureData(const QIcon &icon,
	const QString &albedo_map, const QString &extra_map,
	const bool use_blend_size_sampler, const bool use_extra_map):
	m_icon(icon), m_albedo_map(albedo_map), m_extra_map(extra_map),
	m_use_blend_size_sampler(use_blend_size_sampler),
	m_use_extra_map(use_extra_map)
{
}

TerrainTextureData::~TerrainTextureData()
{
}
