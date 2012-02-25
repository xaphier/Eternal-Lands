/****************************************************************************
 *            ambientmodification.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "ambientmodification.hpp"
#include "editor.hpp"
#include "scenepagereadonly.hpp"
#include "scenepagereadwrite.hpp"

namespace eternal_lands
{

	AmbientModification::AmbientModification(const Uint16 page_id, const glm::vec4 &color):
		m_color(color), m_page_id(page_id)
	{
	}

	AmbientModification::~AmbientModification()
	{
	}

	ModificationType AmbientModification::get_type() const
	{
		return mt_scene_ambient_changed;
	}

	bool AmbientModification::merge(Modification* modification)
	{
		return get_type() == modification->get_type();
	}

	bool AmbientModification::undo(Editor &editor)
	{
		ScenePageReadWriteIntrusivePtr scene_page_read_write;

		editor.get_scene().get_scene_page_read_write(m_page_id, scene_page_read_write);

		scene_page_read_write->set_ambient_color(m_color);

		return false;
	}

}
