/****************************************************************************
 *            editorobjectdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "editorobjectdata.hpp"

namespace eternal_lands
{

	EditorObjectData::EditorObjectData()
	{
	}

	EditorObjectData::EditorObjectData(const glm::vec3 &translation,
		const glm::vec3 &rotation_angles,
		const StringVector &material_names, const String &name,
		const float transparency, const float scale,
		const Uint32 id, const SelectionType selection,
		const BlendType blend): ObjectData(Transformation(), name,
			transparency, id, selection, blend),
		m_material_names(material_names),
		m_rotation_angles(rotation_angles)
	{
		Transformation m_world_transformation;

		m_world_transformation.set_translation(translation);
		m_world_transformation.set_rotation_angles(rotation_angles);
		m_world_transformation.set_scale(scale);

		set_world_transformation(m_world_transformation);
	}

	EditorObjectData::~EditorObjectData() throw()
	{
	}

	void EditorObjectData::set_translation(const glm::vec3 &translation)
	{
		Transformation m_world_transformation;

		m_world_transformation = get_world_transformation();

		m_world_transformation.set_translation(translation);

		set_world_transformation(m_world_transformation);
	}

	void EditorObjectData::set_rotation_angles(
		const glm::vec3 &rotation_angles)
	{
		Transformation m_world_transformation;

		m_world_transformation = get_world_transformation();

		m_world_transformation.set_rotation_angles(rotation_angles);

		set_world_transformation(m_world_transformation);

		m_rotation_angles = rotation_angles;
	}

	void EditorObjectData::set_scale(const float scale)
	{
		Transformation m_world_transformation;

		m_world_transformation = get_world_transformation();

		m_world_transformation.set_scale(scale);

		set_world_transformation(m_world_transformation);
	}

	glm::vec3 EditorObjectData::get_translation() const
	{
		return get_world_transformation().get_translation();
	}

	float EditorObjectData::get_scale() const
	{
		return get_world_transformation().get_scale();
	}

}
