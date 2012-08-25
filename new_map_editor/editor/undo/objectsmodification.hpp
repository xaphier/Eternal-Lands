/****************************************************************************
 *            objectsmodification.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c8849329_b1be_418c_b6ac_a9e64badf26f
#define	UUID_c8849329_b1be_418c_b6ac_a9e64badf26f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "modification.hpp"
#include "../editorobjectdescription.hpp"

namespace eternal_lands
{

	class ObjectsModification: public Modification
	{
		private:
			EditorObjectDescriptionVector m_object_descriptions;

			virtual bool do_merge(Modification* modification);

		public:
			ObjectsModification(
				const EditorObjectDescriptionVector
					&object_descriptions,
				const ModificationType type,
				const Uint32 edit_id);
			virtual ~ObjectsModification() throw();
			virtual bool undo(EditorMapData &editor);

	};

}

#endif	/* UUID_c8849329_b1be_418c_b6ac_a9e64badf26f */
