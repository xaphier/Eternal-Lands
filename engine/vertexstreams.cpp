/****************************************************************************
 *            vertexstreams.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "vertexstreams.hpp"
#include "abstractwritememorybuffer.hpp"
#include "vertexformat.hpp"
#include "vertexstream.hpp"

namespace eternal_lands
{

	VertexStreams::VertexStreams(const VertexFormatSharedPtr &format,
		const AbstractWriteMemoryBufferSharedPtrArray &memory,
		const Uint32 vertex_count): m_format(format),
		m_vertex_count(vertex_count)
	{
		Uint32 i, count;

		count = m_streams.size();

		for (i = 0; i < count; i++)
		{
			if (memory[i].get() != 0)
			{
				m_streams.push_back(new VertexStream(format,
					memory[i], i));
			}
		}
	}

	VertexStreams::~VertexStreams() throw()
	{
	}

}
