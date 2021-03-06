/****************************************************************************
 *            heightmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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

	HeightModification::HeightModification(const HeightVector &heights,
		const Uint32 edit_id):
		Modification(edit_id, 0, mt_height_changed), m_heights(heights)
	{
	}

	HeightModification::~HeightModification() throw()
	{
	}

	bool HeightModification::do_merge(Modification* modification)
	{
		HeightModification* height_modification;
		HeightVector::iterator begin, end;
		Uint32 size;
		bool found;

		height_modification = boost::polymorphic_downcast<
			HeightModification*>(modification);

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

	bool HeightModification::undo(EditorMapData &editor)
	{
		switch (get_type())
		{
			case mt_light_added:
			case mt_light_removed:
			case mt_light_position_changed:
			case mt_light_color_changed:
			case mt_light_radius_changed:
			case mt_lights_position_changed:
			case mt_lights_color_changed:
			case mt_lights_radius_changed:
			case mt_object_added:
			case mt_object_removed:
			case mt_object_translation_changed:
			case mt_object_rotation_changed:
			case mt_object_scale_changed:
			case mt_object_blend_changed:
			case mt_object_transparency_changed:
			case mt_object_glow_changed:
			case mt_object_selection_changed:
			case mt_object_materials_changed:
			case mt_object_name_changed:
			case mt_object_description_changed:
			case mt_object_id_changed:
			case mt_objects_removed:
			case mt_objects_translation_changed:
			case mt_objects_rotation_changed:
			case mt_objects_scale_changed:
			case mt_objects_blend_changed:
			case mt_objects_transparency_changed:
			case mt_objects_glow_changed:
			case mt_objects_selection_changed:
			case mt_objects_materials_changed:
			case mt_objects_name_changed:
			case mt_objects_description_changed:
			case mt_terrain_material_changed:
			case mt_terrain_scale_offset_changed:
			case mt_terrain_translation_changed:
			case mt_scene_ground_hemisphere_changed:
			case mt_displacement_value_changed:
			case mt_dungeon_changed:
				return false;
			case mt_height_changed:
				editor.set_walk_heights(m_heights);
				return false;
			case mt_tile_value_changed:
			case mt_tile_layer_height_changed:
			case mt_blend_value_changed:
			case mt_blend_values_changed:
			case mt_terrain_blend_layers_swapped:
			case mt_terrain_blend_layer_moved:
				return false;
		}

		return false;
	}

}
