/****************************************************************************
 *            heightmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "heightmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	class CompareHeightIndex
	{
		public:
			inline bool operator() (const Height &height1,
				const Height &height2) const
			{
				return height1.get_index() <
					height2.get_index();
			}

	};

	HeightModification::HeightModification(const HeightVector &heights):
		m_heights(heights)
	{
	}

	HeightModification::~HeightModification() throw()
	{
	}

	ModificationType HeightModification::get_type() const
	{
		return mt_height_changed;
	}

	bool HeightModification::merge(Modification* modification)
	{
		HeightModification* height_modification;
		HeightVector::iterator begin, end;
		Uint32 size;
		bool found;

		if (get_type() == modification->get_type())
		{
			height_modification = dynamic_cast<
				HeightModification*>(modification);

			assert(height_modification != 0);

			size = m_heights.size();
			begin = m_heights.begin();
			end = begin + size;

			std::sort(begin, end, CompareHeightIndex());

			BOOST_FOREACH(const Height &height,
				height_modification->m_heights)
			{
				found = std::binary_search(begin, end, height,
					CompareHeightIndex());

				if (!found)
				{
					m_heights.push_back(height);
					begin = m_heights.begin();
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

	bool HeightModification::undo(EditorMapData &editor)
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
			case mt_terrain_value_changed:
				return false;
			case mt_height_changed:
				editor.set_heights(m_heights);
			case mt_blend_values_changed:
				return false;
		}

		return false;
	}

}
