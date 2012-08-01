/****************************************************************************
 *            vertexbuffers.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "vertexbuffers.hpp"
#include "abstractreadwritememory.hpp"

namespace eternal_lands
{

	VertexBuffers::VertexBuffers(const VertexFormatSharedPtr &format,
		const AbstractReadWriteMemorySharedPtrVector &buffers,
		const Uint32 vertex_count, const bool use_simd):
		VertexStreams(format, buffers, vertex_count, use_simd),
			m_buffers(buffers)
	{
	}

	VertexBuffers::~VertexBuffers() noexcept
	{
	}

}
