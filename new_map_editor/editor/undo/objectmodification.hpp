/****************************************************************************
 *            objectmodification.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_706df60b_9c5f_4f35_9d7b_625b14544002
#define	UUID_706df60b_9c5f_4f35_9d7b_625b14544002

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "mesh/meshobjectdata.hpp"
#include "globalid.hpp"

namespace eternal_lands
{

	class ObjectModification: public Modification
	{
		private:
			MeshObjectData m_data;
			const GlobalId m_id;
			const ModificationType m_type;

		public:
			ObjectModification(const MeshObjectData &data, const GlobalId id,
				const ModificationType type);
			virtual ~ObjectModification();
			virtual ModificationType get_type() const;
			virtual bool merge(Modification* modification);
			virtual bool undo(Editor &editor);

	};

}

#endif	/* UUID_706df60b_9c5f_4f35_9d7b_625b14544002 */
