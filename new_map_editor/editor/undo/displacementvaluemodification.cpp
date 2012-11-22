/****************************************************************************
 *            displacementvaluemodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "displacementvaluemodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	namespace
	{

		class CompareDisplacementValueIndex
		{
			public:
				inline bool operator()(const DisplacementValue
					&displacement_value1,
					const DisplacementValue
						&displacement_value2) const
				{
					return displacement_value1.get_index() <
						displacement_value2.get_index();
				}

		};

	}

	DisplacementValueModification::DisplacementValueModification(
		const DisplacementValueVector &displacement_values,
		const Uint32 edit_id):
		Modification(edit_id, 0, mt_displacement_value_changed),
		m_displacement_values(displacement_values)
	{
	}

	DisplacementValueModification::~DisplacementValueModification() throw()
	{
	}

	bool DisplacementValueModification::do_merge(Modification* modification)
	{
		DisplacementValueModification* displacement_value_modification;
		DisplacementValueVector::iterator begin, end;
		Uint32 size;
		bool found;

		displacement_value_modification = boost::polymorphic_downcast<
			DisplacementValueModification*>(modification);

		size = m_displacement_values.size();
		begin = m_displacement_values.begin();
		end = begin + size;

		std::sort(begin, end, CompareDisplacementValueIndex());

		BOOST_FOREACH(const DisplacementValue &displacement_value,
			displacement_value_modification->m_displacement_values)
		{
			found = std::binary_search(begin, end,
				displacement_value,
				CompareDisplacementValueIndex());

			if (!found)
			{
				m_displacement_values.push_back(
					displacement_value);
				begin = m_displacement_values.begin();
				end = begin + size;
			}
		}

		return true;
	}

	bool DisplacementValueModification::undo(EditorMapData &editor)
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
			case mt_object_name_changed:
			case mt_objects_removed:
			case mt_objects_translation_changed:
			case mt_objects_rotation_changed:
			case mt_objects_scale_changed:
			case mt_objects_blend_changed:
			case mt_objects_transparency_changed:
			case mt_objects_selection_changed:
			case mt_objects_materials_changed:
			case mt_objects_walkable_changed:
			case mt_objects_name_changed:
			case mt_terrain_material_changed:
			case mt_terrain_scale_offset_changed:
			case mt_tile_texture_changed:
			case mt_scene_ground_hemisphere_changed:
			case mt_dungeon_changed:
				return false;
			case mt_displacement_value_changed:
				editor.set_terrain_displacement_values(
					m_displacement_values);
				return false;
			case mt_height_changed:
			case mt_blend_values_changed:
				return false;
		}

		return false;
	}

}
