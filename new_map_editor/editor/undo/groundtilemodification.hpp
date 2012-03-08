/****************************************************************************
 *            groundtilemodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			const Uint16Array2 m_offset;
			const Uint16 m_material;

		public:
			GroundTileModification(const Uint16Array2 &offset,
				const Uint16 material);
			virtual ~GroundTileModification() throw();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_5291787c_a28e_44ee_b6af_39f51472517f */
