/****************************************************************************
 *            heightmapmodification.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_05186f6c_118b_4729_9af4_58ab27a51e80
#define	UUID_05186f6c_118b_4729_9af4_58ab27a51e80

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "height.hpp"

namespace eternal_lands
{

	class HeightMapModification: public Modification
	{
		private:
			Uint8MultiArray2 m_height_map;
			Uint16 m_page_id;

		public:
			HeightMapModification(const Uint8MultiArray2 &height_map,
				const Uint16 page_id);
			virtual ~HeightMapModification();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(Editor &editor);

	};

}

#endif	/* UUID_05186f6c_118b_4729_9af4_58ab27a51e80 */
