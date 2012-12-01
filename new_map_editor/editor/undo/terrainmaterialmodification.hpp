/****************************************************************************
 *            terrainmaterialmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_893b06d8_f2b4_481c_b7ff_ca0c75568bfc
#define	UUID_893b06d8_f2b4_481c_b7ff_ca0c75568bfc

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"

namespace eternal_lands
{

	class TerrainMaterialModification: public Modification
	{
		private:
			const String m_albedo_map;
			const String m_specular_map;
			const String m_gloss_map;
			const String m_height_map;
			const glm::vec3 m_default_specular;
			const float m_default_gloss;
			const float m_default_height;
			const float m_blend_size;
			const bool m_use_blend_size_texture;
			const bool m_use_specular_map;
			const bool m_use_gloss_map;
			const bool m_use_height_map;
			const Uint16 m_index;

			virtual bool do_merge(Modification* modification);

		public:
			TerrainMaterialModification(const String &albedo_map,
				const String &specular_map,
				const String &gloss_map,
				const String &height_map,
				const glm::vec3 &default_specular,
				const float default_gloss,
				const float default_height,
				const float blend_size,
				const bool use_blend_size_texture,
				const bool use_specular_map,
				const bool use_gloss_map,
				const bool use_height_map,
				const Uint16 index,
				const ModificationType type,
				const Uint32 edit_id);
			virtual ~TerrainMaterialModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_893b06d8_f2b4_481c_b7ff_ca0c75568bfc */
