/****************************************************************************
 *            editorobjectdescription.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editorobjectdescription.hpp"

namespace eternal_lands
{

	EditorObjectDescription::EditorObjectDescription()
	{
	}

	EditorObjectDescription::EditorObjectDescription(
		const glm::vec3 &translation,
		const glm::vec3 &rotation_angles, const glm::vec3 &scale,
		const StringVector &material_names, const String &name,
		const BitSet64 blend_mask, const String &description,
		const float transparency, const float glow, const Uint32 id,
		const SelectionType selection, const BlendType blend):
		ObjectDescription(Transformation(), material_names, name,
			blend_mask, transparency, glow, id, selection, blend),
			m_rotation_angles(rotation_angles),
			m_description(description)
	{
		Transformation m_world_transformation;

		m_world_transformation.set_translation(translation);
		m_world_transformation.set_rotation_angles(rotation_angles);
		m_world_transformation.set_scale(scale);

		set_world_transformation(m_world_transformation);
	}

	EditorObjectDescription::~EditorObjectDescription() throw()
	{
	}

	void EditorObjectDescription::set_translation(
		const glm::vec3 &translation)
	{
		Transformation m_world_transformation;

		m_world_transformation = get_world_transformation();

		m_world_transformation.set_translation(translation);

		set_world_transformation(m_world_transformation);
	}

	void EditorObjectDescription::set_rotation_angles(
		const glm::vec3 &rotation_angles)
	{
		Transformation m_world_transformation;

		m_world_transformation = get_world_transformation();

		m_world_transformation.set_rotation_angles(rotation_angles);

		set_world_transformation(m_world_transformation);

		m_rotation_angles = rotation_angles;
	}

	void EditorObjectDescription::set_scale(const glm::vec3 &scale)
	{
		Transformation m_world_transformation;

		m_world_transformation = get_world_transformation();

		m_world_transformation.set_scale(scale);

		set_world_transformation(m_world_transformation);
	}

	const glm::vec3 &EditorObjectDescription::get_translation() const
	{
		return get_world_transformation().get_translation();
	}

	const glm::vec3 &EditorObjectDescription::get_scale() const
	{
		return get_world_transformation().get_scale();
	}

}
