/****************************************************************************
 *            modification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
		mt_object_selection_changed,
		mt_object_materials_changed,
		mt_light_added,
		mt_light_removed,
		mt_light_position_changed,
		mt_light_color_changed,
		mt_light_radius_changed,
		mt_terrain_albedo_map_changed,
		mt_terrain_blend_map_changed,
		mt_terrain_vector_map_changed,
		mt_terrain_dudv_map_changed,
		mt_terrain_scale_offset_changed,
		mt_tile_texture_changed,
		mt_scene_ambient_changed,
		mt_terrain_value_changed,
		mt_height_changed,
		mt_blend_values_changed,
	};

	class EditorMapData;

	class Modification
	{
		protected:
			Modification();

		public:
			virtual ~Modification() throw();
			virtual ModificationType get_type() const = 0;
			virtual bool merge(Modification* modification) = 0;
			virtual bool undo(EditorMapData &editor) = 0;

	};

	typedef std::auto_ptr<Modification> ModificationAutoPtr;

}

#endif	/* UUID_931b2efb_829b_4b86_9cbd_aaec722b837c */
