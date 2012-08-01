/****************************************************************************
 *            modification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "modification.hpp"

namespace eternal_lands
{

	Modification::Modification(const Uint32 edit_id, const Uint32 id,
		const ModificationType type): m_edit_id(edit_id), m_id(id),
		m_type(type)
	{
	}

	Modification::~Modification() throw()
	{
	}

	bool Modification::merge(Modification* modification)
	{
		if (get_edit_id() != modification->get_edit_id())
		{
			return false;
		}

		if (get_type() != modification->get_type())
		{
			return false;
		}

		return do_merge(modification);
	}

	bool Modification::add_needed(const Uint32 edit_id, const Uint32 id,
		const ModificationType type)
	{
		return (edit_id != get_edit_id()) || (id != get_id()) ||
			(type != get_type());
	}

}
