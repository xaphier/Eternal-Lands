/****************************************************************************
 *            tilevaluemodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5291787c_a28e_44ee_b6af_39f51472517f
#define	UUID_5291787c_a28e_44ee_b6af_39f51472517f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "../imagevalue.hpp"

namespace eternal_lands
{

	class TileValueModification: public Modification
	{
		private:
			ImageValueVector m_tile_values;
			const Uint16 m_layer;

			virtual bool do_merge(Modification* modification);

		public:
			TileValueModification(
				const ImageValueVector &tile_values,
				const Uint16 layer, const Uint32 edit_id);
			virtual ~TileValueModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_5291787c_a28e_44ee_b6af_39f51472517f */
