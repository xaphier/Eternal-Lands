/****************************************************************************
 *            terraindata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terraindata.hpp"
#include "xmlreader.hpp"
#include "xmlutil.hpp"
#include "xmlwriter.hpp"

namespace eternal_lands
{

	TerrainData::TerrainData(): m_dudv_scale(1.0f)
	{
	}

	TerrainData::TerrainData(const StringArray4 &albedo_maps,
		const String &blend_map, const String &height_map,
		const String &dudv_map, const glm::vec3 &translation,
		const glm::vec2 &dudv_scale): m_albedo_maps(albedo_maps),
		m_blend_map(blend_map), m_height_map(height_map),
		m_dudv_map(dudv_map), m_translation(translation),
		m_dudv_scale(dudv_scale)
	{
	}

	TerrainData::~TerrainData() noexcept
	{
	}

}
