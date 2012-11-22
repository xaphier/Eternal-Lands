/****************************************************************************
 *            groundhemispheremodification.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "groundhemispheremodification.hpp"
#include "../editormapdata.hpp"

namespace eternal_lands
{

	GroundHemisphereModification::GroundHemisphereModification(
		const glm::vec4 &ground_hemisphere, const Uint32 edit_id):
		Modification(edit_id, 0, mt_scene_ground_hemisphere_changed),
		m_ground_hemisphere(ground_hemisphere)
	{
	}

	GroundHemisphereModification::~GroundHemisphereModification() throw()
	{
	}

	bool GroundHemisphereModification::do_merge(Modification* modification)
	{
		return true;
	}

	bool GroundHemisphereModification::undo(EditorMapData &editor)
	{
		editor.set_ground_hemisphere(m_ground_hemisphere);

		return false;
	}

}
