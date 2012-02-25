/****************************************************************************
 *            heightmodification.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_2fe40d0e_ad13_482d_bb78_4b34cee950a6
#define	UUID_2fe40d0e_ad13_482d_bb78_4b34cee950a6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "height.hpp"

namespace eternal_lands
{

	class HeightModification: public Modification
	{
		private:
			HeightVector m_heights;
			Uint32 m_id;
			Uint16 m_page_id;
			bool m_terrain;

		public:
			HeightModification(const HeightVector &heights,
				const Uint32 id, const Uint16 page_id,
				const bool terrain);
			virtual ~HeightModification();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(Editor &editor);

	};

}

#endif	/* UUID_2fe40d0e_ad13_482d_bb78_4b34cee950a6 */
