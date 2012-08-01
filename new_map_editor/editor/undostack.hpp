/****************************************************************************
 *            undostack.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_72af5ec0_10b0_4ca7_8b27_83275c3a4b2e
#define	UUID_72af5ec0_10b0_4ca7_8b27_83275c3a4b2e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "undo/modification.hpp"
#include <boost/ptr_container/ptr_circular_buffer.hpp>

namespace eternal_lands
{

	class UndoStack
	{
		private:
			boost::ptr_circular_buffer<Modification> m_buffer;

		public:
			UndoStack(const Uint32 size);
			void add(ModificationAutoPtr &ptr);
			bool undo(EditorMapData &editor);
			bool add_needed(const Uint32 edit_id, const Uint32 id,
				const ModificationType type);

			inline void clear()
			{
				return m_buffer.clear();
			}

			inline bool can_undo() const
			{
				return !m_buffer.empty();
			}

	};

}

#endif	/* UUID_72af5ec0_10b0_4ca7_8b27_83275c3a4b2e */
