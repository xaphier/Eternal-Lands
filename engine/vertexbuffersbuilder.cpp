/****************************************************************************
 *            vertexbuffersbuilder.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "vertexbuffersbuilder.hpp"
#include "vertexformat.hpp"
#include "readwritememory.hpp"
#include "vertexbuffers.hpp"

namespace eternal_lands
{

	VertexBuffersSharedPtr VertexBuffersBuilder::get_vertex_buffers(
		const VertexFormatSharedPtr &vertex_format,
		const Uint32 vertex_count, const bool use_simd)
	{
		ReadWriteMemorySharedPtr buffer;
		AbstractReadWriteMemorySharedPtrVector buffers;
		VertexBuffersSharedPtr result;
		Uint32 i;
		Uint64 size;

		for (i = 0; i < vertex_stream_count; ++i)
		{
			if (vertex_format->get_vertex_elements(i).get_count() > 0)
			{
				size = vertex_format->get_vertex_elements(
					i).get_stride() * vertex_count;
				buffer = boost::make_shared<ReadWriteMemory>(
					size);
				buffers.push_back(buffer);
			}
		}

		result = boost::make_shared<VertexBuffers>(vertex_format,
			buffers, vertex_count, use_simd);

		return result;
	}

}