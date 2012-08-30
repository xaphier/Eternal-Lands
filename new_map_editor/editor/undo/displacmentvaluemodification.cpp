/****************************************************************************
 *            displacmentvaluemodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "displacmentvaluemodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	namespace
	{

		class CompareDisplacmentValueIndex
		{
			public:
				inline bool operator()(const DisplacmentValue
					&displacment_value1,
					const DisplacmentValue
						&displacment_value2) const
				{
					return displacment_value1.get_index() <
						displacment_value2.get_index();
				}

		};

	}

	DisplacmentValueModification::DisplacmentValueModification(
		const DisplacmentValueVector &displacment_values,
		const Uint32 edit_id):
		Modification(edit_id, 0, mt_displacment_value_changed),
		m_displacment_values(displacment_values)
	{
	}

	DisplacmentValueModification::~DisplacmentValueModification() throw()
	{
	}

	bool DisplacmentValueModification::do_merge(Modification* modification)
	{
		DisplacmentValueModification* displacment_value_modification;
		DisplacmentValueVector::iterator begin, end;
		Uint32 size;
		bool found;

		displacment_value_modification = boost::polymorphic_downcast<
			DisplacmentValueModification*>(modification);

		size = m_displacment_values.size();
		begin = m_displacment_values.begin();
		end = begin + size;

		std::sort(begin, end, CompareDisplacmentValueIndex());

		BOOST_FOREACH(const DisplacmentValue &displacment_value,
			displacment_value_modification->m_displacment_values)
		{
			found = std::binary_search(begin, end,
				displacment_value,
				CompareDisplacmentValueIndex());

			if (!found)
			{
				m_displacment_values.push_back(
					displacment_value);
				begin = m_displacment_values.begin();
				end = begin + size;
			}
		}

		return true;
	}

	bool DisplacmentValueModification::undo(EditorMapData &editor)
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
			case mt_object_selection_changed:
			case mt_object_materials_changed:
			case mt_object_walkable_changed:
			case mt_objects_translation_changed:
			case mt_objects_rotation_changed:
			case mt_objects_scale_changed:
			case mt_objects_blend_changed:
			case mt_objects_transparency_changed:
			case mt_objects_selection_changed:
			case mt_objects_materials_changed:
			case mt_objects_walkable_changed:
			case mt_terrain_albedo_map_changed:
			case mt_terrain_blend_map_changed:
			case mt_terrain_vector_map_changed:
			case mt_terrain_dudv_map_changed:
			case mt_terrain_scale_offset_changed:
			case mt_tile_texture_changed:
			case mt_scene_ambient_changed:
			case mt_dungeon_changed:
				return false;
			case mt_displacment_value_changed:
				editor.set_terrain_displacment_values(
					m_displacment_values);
				break;
			case mt_height_changed:
			case mt_blend_values_changed:
				return false;
		}

		return false;
	}

}
