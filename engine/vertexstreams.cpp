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
#include "memorybuffer.hpp"

namespace eternal_lands
{

	VertexStreams::VertexStreams(const VertexFormatSharedPtr &format,
		const AbstractWriteMemoryBufferSharedPtrArray &memory,
		const Uint32 vertex_count, const bool use_simd):
		m_format(format), m_vertex_count(vertex_count)
	{
		Uint32 i, count;

		count = memory.size();

		for (i = 0; i < count; i++)
		{
			if (memory[i].get() != 0)
			{
				m_streams.push_back(new VertexStream(format,
					memory[i], i, use_simd));
			}
		}
	}

	VertexStreams::VertexStreams(const VertexFormatSharedPtr &format,
		const MemoryBufferSharedPtrVector &buffers,
		const Uint32 vertex_count, const bool use_simd):
		m_format(format), m_vertex_count(vertex_count)
	{
		Uint32 i, count;

		count = buffers.size();

		for (i = 0; i < count; i++)
		{
			if (buffers[i].get() != 0)
			{
				m_streams.push_back(new VertexStream(format,
					buffers[i], i, use_simd));
			}
		}
	}

	VertexStreams::~VertexStreams() throw()
	{
	}

	void VertexStreams::set(const VertexSemanticType semantic,
		const glm::vec4 &data)
	{
		BOOST_FOREACH(VertexStream &stream, get_streams())
		{
			stream.set(semantic, data);
		}
	}

	void VertexStreams::reset()
	{
		BOOST_FOREACH(VertexStream &stream, get_streams())
		{
			stream.reset();
		}
	}

	void VertexStreams::push_vertex()
	{
		BOOST_FOREACH(VertexStream &stream, get_streams())
		{
			stream.push_vertex();
		}
	}

}
