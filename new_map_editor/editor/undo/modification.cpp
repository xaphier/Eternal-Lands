/****************************************************************************
 *            modification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "modification.hpp"

namespace eternal_lands
{

	Modification::Modification(const Uint32 edit_id): m_edit_id(edit_id)
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

}
