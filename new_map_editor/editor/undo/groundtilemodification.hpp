/****************************************************************************
 *            groundtilemodification.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5291787c_a28e_44ee_b6af_39f51472517f
#define	UUID_5291787c_a28e_44ee_b6af_39f51472517f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"

namespace eternal_lands
{

	class GroundTileModification: public Modification
	{
		private:
			Uint32Array2 m_offset;
			Uint16 m_page_id;
			Uint8 m_material;

		public:
			GroundTileModification(const Uint16 page_id, const Uint32Array2 &offset,
				const Uint8 material);
			virtual ~GroundTileModification();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(Editor &editor);

	};

}

#endif	/* UUID_5291787c_a28e_44ee_b6af_39f51472517f */
