/****************************************************************************
 *            blendvaluesmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "blendvaluesmodification.hpp"
#include "../editormapdata.hpp"
#include "image.hpp"

namespace eternal_lands
{

	namespace
	{

		class CompareImageValuesIndex
		{
			public:
				inline bool operator() (
					const ImageValues &image_value1,
					const ImageValues &image_value2) const
				{
					return image_value1.get_index() <
						image_value2.get_index();
				}

		};

	}

	BlendValuesModification::BlendValuesModification(
		const ImageValuesVector &blend_values, const Uint32 edit_id):
		Modification(edit_id, 0, mt_blend_values_changed),
		m_blend_values(blend_values)
	{
	}

	BlendValuesModification::~BlendValuesModification() throw()
	{
	}

	bool BlendValuesModification::do_merge(Modification* modification)
	{
		BlendValuesModification* blend_values_modification;
		ImageValuesVector::iterator begin, end;
		Uint32 size;
		bool found;

		blend_values_modification = boost::polymorphic_downcast<
			BlendValuesModification*>(modification);

		assert(blend_values_modification != 0);

		size = m_blend_values.size();
		begin = m_blend_values.begin();
		end = begin + size;

		std::sort(begin, end, CompareImageValuesIndex());

		BOOST_FOREACH(const ImageValues &image_value,
			blend_values_modification->m_blend_values)
		{
			found = std::binary_search(begin, end, image_value,
				CompareImageValuesIndex());

			if (!found)
			{
				m_blend_values.push_back(image_value);
				begin = m_blend_values.begin();
				end = begin + size;
			}
		}

		return true;
	}

	bool BlendValuesModification::undo(EditorMapData &editor)
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
			case mt_object_id_changed:		
			case mt_object_description_changed:
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
			case mt_height_changed:
			case mt_tile_value_changed:
			case mt_tile_layer_height_changed:
			case mt_blend_value_changed:
			case mt_terrain_blend_layers_swapped:
			case mt_terrain_blend_layer_moved:
				return false;
			case mt_blend_values_changed:
				editor.set_terrain_blend_values(m_blend_values);
				return false;
		}

		return false;
	}

}
