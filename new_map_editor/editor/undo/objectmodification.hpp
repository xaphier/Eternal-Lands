/****************************************************************************
 *            objectmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_706df60b_9c5f_4f35_9d7b_625b14544002
#define	UUID_706df60b_9c5f_4f35_9d7b_625b14544002

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "../editorobjectdescription.hpp"

namespace eternal_lands
{

	class ObjectModification: public Modification
	{
		private:
			EditorObjectDescription m_object_description;
			const ModificationType m_type;

			virtual bool do_merge(Modification* modification);

		public:
			ObjectModification(
				const EditorObjectDescription
					&object_description,
				const ModificationType type,
				const Uint32 edit_id);
			virtual ~ObjectModification() throw();
			virtual ModificationType get_type() const;
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_706df60b_9c5f_4f35_9d7b_625b14544002 */
