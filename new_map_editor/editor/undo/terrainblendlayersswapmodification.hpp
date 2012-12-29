/****************************************************************************
 *            terrainblendlayersswapmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c65616cc_bcf3_4fa1_a696_5720ad62e9fb
#define	UUID_c65616cc_bcf3_4fa1_a696_5720ad62e9fb

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"

namespace eternal_lands
{

	class TerrainBlendLayersSwapModification: public Modification
	{
		private:
			const Uint16 m_idx0;
			const Uint16 m_idx1;

			virtual bool do_merge(Modification* modification);

		public:
			TerrainBlendLayersSwapModification(const Uint16 idx0,
				const Uint16 idx1, const Uint32 edit_id);
			virtual ~TerrainBlendLayersSwapModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_c65616cc_bcf3_4fa1_a696_5720ad62e9fb */
