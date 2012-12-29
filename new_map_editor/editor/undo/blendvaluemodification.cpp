/****************************************************************************
 *            blendvaluemodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "blendvaluemodification.hpp"
#include "../editormapdata.hpp"
#include "image.hpp"

namespace eternal_lands
{

	namespace
	{

		class CompareImageValueIndex
		{
			public:
				inline bool operator() (
					const ImageValue &image_value1,
					const ImageValue &image_value2) const
				{
					return image_value1.get_index() <
						image_value2.get_index();
				}

		};

	}

	BlendValueModification::BlendValueModification(
		const ImageValueVector &blend_values, const Uint16 layer,
		const Uint32 edit_id):
		Modification(edit_id, layer, mt_blend_value_changed),
		m_blend_values(blend_values), m_layer(layer)
	{
	}

	BlendValueModification::~BlendValueModification() throw()
	{
	}

	bool BlendValueModification::do_merge(Modification* modification)
	{
		BlendValueModification* blend_value_modification;
		ImageValueVector::iterator begin, end;
		Uint32 size;
		bool found;

		blend_value_modification = boost::polymorphic_downcast<
			BlendValueModification*>(modification);

		assert(blend_value_modification != 0);

		if (blend_value_modification->m_layer != m_layer)
		{
			return false;
		}

		size = m_blend_values.size();
		begin = m_blend_values.begin();
		end = begin + size;

		std::sort(begin, end, CompareImageValueIndex());

		BOOST_FOREACH(const ImageValue &image_value,
			blend_value_modification->m_blend_values)
		{
			found = std::binary_search(begin, end, image_value,
				CompareImageValueIndex());

			if (!found)
			{
				m_blend_values.push_back(image_value);
				begin = m_blend_values.begin();
				end = begin + size;
			}
		}

		return true;
	}

	bool BlendValueModification::undo(EditorMapData &editor)
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
			case mt_height_changed:
			case mt_tile_value_changed:
			case mt_tile_layer_height_changed:
			case mt_blend_values_changed:
			case mt_terrain_blend_layers_swapped:
			case mt_terrain_blend_layer_moved:
				return false;
			case mt_blend_value_changed:
				editor.set_terrain_blend_values(m_blend_values,
					m_layer);
				return false;
		}

		return false;
	}

}
