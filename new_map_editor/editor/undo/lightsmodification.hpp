/****************************************************************************
 *            lightmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_62f91d2d_3ce9_4d7b_abdb_c72e704b2c45
#define	UUID_62f91d2d_3ce9_4d7b_abdb_c72e704b2c45

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "lightdata.hpp"

namespace eternal_lands
{

	class LightsModification: public Modification
	{
		private:
			LightDataVector m_datas;

			virtual bool do_merge(Modification* modification);

		public:
			LightsModification(const LightDataVector &datas,
				const ModificationType type,
				const Uint32 edit_id);
			virtual ~LightsModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_62f91d2d_3ce9_4d7b_abdb_c72e704b2c45 */
