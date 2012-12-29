/****************************************************************************
 *            terrainblendlayermovemodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_18ea0fd1_f23a_4b84_b0e2_a7ea0a151f0e
#define	UUID_18ea0fd1_f23a_4b84_b0e2_a7ea0a151f0e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"

namespace eternal_lands
{

	class TerrainBlendLayerMoveModification: public Modification
	{
		private:
			const Uint16 m_idx0;
			const Uint16 m_idx1;

			virtual bool do_merge(Modification* modification);

		public:
			TerrainBlendLayerMoveModification(const Uint16 idx0,
				const Uint16 idx1, const Uint32 edit_id);
			virtual ~TerrainBlendLayerMoveModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_18ea0fd1_f23a_4b84_b0e2_a7ea0a151f0e */
