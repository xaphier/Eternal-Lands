/****************************************************************************
 *            terrainvaluemodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "terrainvaluemodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	namespace
	{

		class CompareTerrainValueIndex
		{
			public:
				inline bool operator() (
					const TerrainValue &terrain_value1,
					const TerrainValue &terrain_value2)
					const
				{
					return terrain_value1.get_index() <
						terrain_value2.get_index();
				}

		};

	}

	TerrainValueModification::TerrainValueModification(
		const TerrainValueVector &terrain_values):
		m_terrain_values(terrain_values)
	{
	}

	TerrainValueModification::~TerrainValueModification() throw()
	{
	}

	ModificationType TerrainValueModification::get_type() const
	{
		return mt_terrain_value_changed;
	}

	bool TerrainValueModification::merge(Modification* modification)
	{
		TerrainValueModification* terrain_value_modification;
		TerrainValueVector::iterator begin, end;
		Uint32 size;
		bool found;

		if (get_type() == modification->get_type())
		{
			terrain_value_modification = dynamic_cast<
				TerrainValueModification*>(modification);

			assert(terrain_value_modification != 0);

			size = m_terrain_values.size();
			begin = m_terrain_values.begin();
			end = begin + size;

			std::sort(begin, end, CompareTerrainValueIndex());

			BOOST_FOREACH(const TerrainValue &terrain_value,
				terrain_value_modification->m_terrain_values)
			{
				found = std::binary_search(begin, end,
					terrain_value,
					CompareTerrainValueIndex());

				if (!found)
				{
					m_terrain_values.push_back(
						terrain_value);
					begin = m_terrain_values.begin();
					end = begin + size;
				}
			}

			return true;
		}
		else
		{
			return false;
		}
	}

	bool TerrainValueModification::undo(EditorMapData &editor)
	{
		switch (get_type())
		{
			case mt_light_added:
			case mt_light_removed:
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
			case mt_object_added:
			case mt_object_removed:
			case mt_object_translation_changed:
			case mt_object_rotation_changed:
			case mt_object_scale_changed:
			case mt_object_blend_changed:
			case mt_object_selection_changed:
			case mt_object_materials_changed:
			case mt_terrain_albedo_map_changed:
			case mt_terrain_blend_map_changed:
			case mt_terrain_height_map_changed:
			case mt_terrain_dudv_map_changed:
			case mt_terrain_scale_offset_changed:
			case mt_tile_texture_changed:
			case mt_scene_ambient_changed:
				return false;
			case mt_terrain_value_changed:
				editor.set_terrain_values(m_terrain_values);
				break;
			case mt_height_changed:
			case mt_blend_values_changed:
				return false;
		}

		return false;
	}

}