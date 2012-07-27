/****************************************************************************
 *            ambientmodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "ambientmodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	AmbientModification::AmbientModification(const glm::vec3 &color,
		const Uint32 edit_id):
		Modification(edit_id, 0, mt_scene_ambient_changed),
		m_color(color)
	{
	}

	AmbientModification::~AmbientModification() throw()
	{
	}

	bool AmbientModification::do_merge(Modification* modification)
	{
		return true;
	}

	bool AmbientModification::undo(EditorMapData &editor)
	{
		editor.set_ambient(m_color);

		return false;
	}

}
