/****************************************************************************
 *            shadowobject.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "shadowobject.hpp"

namespace eternal_lands
{

	ShadowObject::ShadowObject(): MeshDrawData(), m_mesh_count(0)
	{
	}

	ShadowObject::ShadowObject(const MeshDrawData &mesh_draw,
		const GlslProgramSharedPtr &program, const Uint32 mesh_count):
		MeshDrawData(mesh_draw), m_program(program),
		m_mesh_count(mesh_count)
	{
	}

	ShadowObject::~ShadowObject() noexcept
	{
	}

}
