/****************************************************************************
 *            tilelayerheightmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8f843765_4ddd_4404_b348_4fb41caab267
#define	UUID_8f843765_4ddd_4404_b348_4fb41caab267

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"

namespace eternal_lands
{

	class TileLayerHeightModification: public Modification
	{
		private:
			float m_value;
			const Uint16 m_layer;

			virtual bool do_merge(Modification* modification);

		public:
			TileLayerHeightModification(const float value,
				const Uint16 layer, const Uint32 edit_id);
			virtual ~TileLayerHeightModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_8f843765_4ddd_4404_b348_4fb41caab267 */
