/****************************************************************************
 *            terrainheightscalemodification.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f367e214_6055_45ee_94d9_6f927c1b4fc2
#define	UUID_f367e214_6055_45ee_94d9_6f927c1b4fc2

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"

namespace eternal_lands
{

	class TerrainHeightScaleModification: public Modification
	{
		private:
			float m_scale;

		public:
			TerrainHeightScaleModification(const float scale);
			virtual ~TerrainHeightScaleModification();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(Editor &editor);

	};

}

#endif	/* UUID_f367e214_6055_45ee_94d9_6f927c1b4fc2 */
