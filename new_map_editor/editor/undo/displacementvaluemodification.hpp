/****************************************************************************
 *            displacementvaluemodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_204fe654_d409_49e9_b4ad_48b9a370e686
#define	UUID_204fe654_d409_49e9_b4ad_48b9a370e686

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "../displacementvalue.hpp"

namespace eternal_lands
{

	class DisplacementValueModification: public Modification
	{
		private:
			DisplacementValueVector m_displacement_values;

			virtual bool do_merge(Modification* modification);

		public:
			DisplacementValueModification(
				const DisplacementValueVector
					&displacement_values,
				const Uint32 edit_id);
			virtual ~DisplacementValueModification() throw();
			virtual bool undo(EditorMapData &editor);
			bool add_needed(const Uint32 edit_id);

	};

}

#endif	/* UUID_204fe654_d409_49e9_b4ad_48b9a370e686 */
