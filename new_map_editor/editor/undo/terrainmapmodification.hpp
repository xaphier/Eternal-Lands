/****************************************************************************
 *            terrainmapmodification.hpp
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

	class TerrainMapModification: public Modification
	{
		private:
			const String m_map;

			virtual bool do_merge(Modification* modification);

		public:
			TerrainMapModification(const String &map,
				const Uint16 index,
				const ModificationType type,
				const Uint32 edit_id);
			virtual ~TerrainMapModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_893b06d8_f2b4_481c_b7ff_ca0c75568bfc */
