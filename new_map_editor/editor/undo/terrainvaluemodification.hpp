/****************************************************************************
 *            terrainvaluemodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_204fe654_d409_49e9_b4ad_48b9a370e686
#define	UUID_204fe654_d409_49e9_b4ad_48b9a370e686

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "../terrainvalue.hpp"

namespace eternal_lands
{

	class TerrainValueModification: public Modification
	{
		private:
			TerrainValueVector m_terrain_values;

			virtual bool do_merge(Modification* modification);

		public:
			TerrainValueModification(
				const TerrainValueVector &terrain_values,
				const Uint32 id);
			virtual ~TerrainValueModification() throw();
			virtual ModificationType get_type() const;
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_204fe654_d409_49e9_b4ad_48b9a370e686 */
