/****************************************************************************
 *            vertexstreams.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "vertexstreams.hpp"
#include "abstractwritememory.hpp"
#include "vertexformat.hpp"
#include "vertexstream.hpp"
#include "readwritememory.hpp"

namespace eternal_lands
{

	VertexStreams::VertexStreams(const VertexFormatConstSharedPtr &format,
		const AbstractWriteMemorySharedPtrArray &memory,
		const Uint32 vertex_count, const bool use_simd):
		m_format(format), m_vertex_count(vertex_count)
	{
		Uint32 i, count;

		count = memory.size();

		for (i = 0; i < count; i++)
		{
			if (memory[i].get() != nullptr)
			{
				m_streams.push_back(new VertexStream(format,
					memory[i], i, vertex_count, use_simd));
			}
		}
	}

	VertexStreams::VertexStreams(const VertexFormatConstSharedPtr &format,
		const AbstractReadWriteMemorySharedPtrVector &buffers,
		const Uint32 vertex_count, const bool use_simd):
		m_format(format), m_vertex_count(vertex_count)
	{
		Uint32 i, count;

		count = buffers.size();

		for (i = 0; i < count; i++)
		{
			if (buffers[i].get() != nullptr)
			{
				m_streams.push_back(new VertexStream(format,
					buffers[i], i, vertex_count, use_simd));
			}
		}
	}

	VertexStreams::~VertexStreams() noexcept
	{
	}

	void VertexStreams::set(const VertexSemanticType semantic,
		const glm::vec4 &data) noexcept
	{
		BOOST_FOREACH(VertexStream &stream, get_streams())
		{
			stream.set(semantic, data);
		}
	}

	void VertexStreams::reset() noexcept
	{
		BOOST_FOREACH(VertexStream &stream, get_streams())
		{
			stream.reset();
		}
	}

	void VertexStreams::push_vertex() noexcept
	{
		BOOST_FOREACH(VertexStream &stream, get_streams())
		{
			stream.push_vertex();
		}
	}

}
