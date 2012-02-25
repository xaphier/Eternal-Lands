/****************************************************************************
 *            submesh.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "submesh.hpp"

namespace eternal_lands
{

	SubMesh::SubMesh(const BoundingBox &bounding_box, const Uint32 offset,
		const Uint32 count, const Uint32 min_vertex,
		const Uint32 max_vertex, const bool packed):
		MeshDrawData(offset, count, min_vertex, max_vertex),
		m_bounding_box(bounding_box), m_packed(packed)
	{
	}

	SubMesh::SubMesh(const BoundingBox &bounding_box, const Uint32 offset,
		const Uint32 count, const Uint32 min_vertex,
		const Uint32 max_vertex, const Uint32 base_vertex,
		const bool packed): MeshDrawData(offset, count, min_vertex,
			max_vertex, base_vertex), m_bounding_box(bounding_box),
				m_packed(packed)
	{
	}

	SubMesh::SubMesh(): MeshDrawData(), m_packed(true)
	{
	}

	SubMesh::~SubMesh() throw()
	{
	}

}
