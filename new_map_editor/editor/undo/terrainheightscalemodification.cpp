/****************************************************************************
 *            terrainheightscalemodification.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainheightscalemodification.hpp"
#include "editor.hpp"

namespace eternal_lands
{

	TerrainHeightScaleModification::TerrainHeightScaleModification(const float scale): m_scale(scale)
	{
	}

	TerrainHeightScaleModification::~TerrainHeightScaleModification()
	{
	}

	ModificationType TerrainHeightScaleModification::get_type() const
	{
		return mt_terrain_height_scale_changed;
	}

	bool TerrainHeightScaleModification::merge(Modification* modification)
	{
		return get_type() == modification->get_type();
	}

	bool TerrainHeightScaleModification::undo(Editor &editor)
	{
		editor.get_scene().set_terrain_height_scale(m_scale);

		return false;
	}

}
