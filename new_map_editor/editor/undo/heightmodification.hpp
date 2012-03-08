/****************************************************************************
 *            heightmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2fe40d0e_ad13_482d_bb78_4b34cee950a6
#define	UUID_2fe40d0e_ad13_482d_bb78_4b34cee950a6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "../height.hpp"

namespace eternal_lands
{

	class HeightModification: public Modification
	{
		private:
			HeightVector m_heights;
			const Uint16 m_id;
			const ModificationType m_type;

		public:
			HeightModification(const HeightVector &heights,
				const Uint16 id, const ModificationType type);
			virtual ~HeightModification() throw();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_2fe40d0e_ad13_482d_bb78_4b34cee950a6 */
