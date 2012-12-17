/****************************************************************************
 *            modification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_931b2efb_829b_4b86_9cbd_aaec722b837c
#define	UUID_931b2efb_829b_4b86_9cbd_aaec722b837c

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

namespace eternal_lands
{

	enum ModificationType
	{
		mt_object_added,
		mt_object_removed,
		mt_object_translation_changed,
		mt_object_rotation_changed,
		mt_object_scale_changed,
		mt_object_blend_changed,
		mt_object_transparency_changed,
		mt_object_glow_changed,
		mt_object_selection_changed,
		mt_object_materials_changed,
		mt_object_name_changed,
		mt_object_description_changed,

		mt_objects_removed,
		mt_objects_translation_changed,
		mt_objects_rotation_changed,
		mt_objects_scale_changed,
		mt_objects_blend_changed,
		mt_objects_transparency_changed,
		mt_objects_glow_changed,
		mt_objects_selection_changed,
		mt_objects_materials_changed,
		mt_objects_name_changed,
		mt_objects_description_changed,

		mt_light_added,
		mt_light_removed,
		mt_light_position_changed,
		mt_light_color_changed,
		mt_light_radius_changed,

		mt_lights_position_changed,
		mt_lights_color_changed,
		mt_lights_radius_changed,

		mt_terrain_material_changed,
		mt_terrain_scale_offset_changed,
		mt_terrain_translation_changed,

		mt_tile_value_changed,
		mt_tile_layer_height_changed,
		mt_scene_ground_hemisphere_changed,
		mt_dungeon_changed,
		mt_displacement_value_changed,
		mt_height_changed,
		mt_blend_value_changed,
		mt_blend_values_changed,
	};

	class EditorMapData;

	class Modification
	{
		private:
			const Uint32 m_edit_id;
			const Uint32 m_id;
			const ModificationType m_type;

			virtual bool do_merge(Modification* modification) = 0;

		protected:
			Modification(const Uint32 edit_id,
				const Uint32 id, const ModificationType type);

		public:
			virtual ~Modification() throw();
			virtual bool undo(EditorMapData &editor) = 0;
			bool add_needed(const Uint32 edit_id,
				const Uint32 id, const ModificationType type);
			bool merge(Modification* modification);

			inline ModificationType get_type() const noexcept
			{
				return m_type;
			}

			inline Uint32 get_id() const noexcept
			{
				return m_id;
			}

			inline Uint32 get_edit_id() const noexcept
			{
				return m_edit_id;
			}

	};

	typedef std::auto_ptr<Modification> ModificationAutoPtr;

}

#endif	/* UUID_931b2efb_829b_4b86_9cbd_aaec722b837c */
