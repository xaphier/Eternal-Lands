/****************************************************************************
 *            undostack.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "undostack.hpp"

namespace eternal_lands
{

	UndoStack::UndoStack(const Uint32 size): m_buffer(size)
	{
		assert(size > 0);
	}

	void UndoStack::add(ModificationAutoPtr &ptr)
	{
		if (!m_buffer.empty())
		{
			if (m_buffer.back().merge(ptr.get()))
			{
				return;
			}
		}

		m_buffer.push_back(ptr);
	}

	bool UndoStack::undo(EditorMapData &editor)
	{
		bool result;

		if (can_undo())
		{
			result = m_buffer.back().undo(editor);
			m_buffer.pop_back();

			return result;
		}
		else
		{
			return false;
		}
	}

	bool UndoStack::add_needed(const Uint32 edit_id, const Uint32 id,
		const ModificationType type)
	{
		if (m_buffer.size() == 0)
		{
			return true;
		}

		return m_buffer.begin()->add_needed(edit_id, id, type);
	}

}
