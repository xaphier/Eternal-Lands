/****************************************************************************
 *            opengl2mesh.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl2mesh.hpp"
#include "mappedhardwarewritememorybuffer.hpp"
#include "hardwarewritememorybuffer.hpp"
#include "vertexelements.hpp"
#include "submesh.hpp"
#include "vertexstream.hpp"

namespace eternal_lands
{

	OpenGl2Mesh::OpenGl2Mesh()
	{
	}

	OpenGl2Mesh::~OpenGl2Mesh() throw()
	{
	}

	AbstractWriteMemoryBufferSharedPtr OpenGl2Mesh::get_vertex_buffer(
		const Uint16 index)
	{
		AbstractWriteMemoryBufferSharedPtr result;

		assert(index < m_vertex_data.size());

		if (m_vertex_data[index].get() != 0)
		{
			result = boost::make_shared<
				MappedHardwareWriteMemoryBuffer>(
					m_vertex_data[index], hbt_vertex);
		}

		return result;
	}

	void OpenGl2Mesh::set_vertex_buffer(
		const AbstractReadMemoryBufferSharedPtr &buffer,
		const Uint16 index)
	{
		m_vertex_data[index]->set(hbt_vertex, *buffer,
			get_vertices_usage());
	}

	void OpenGl2Mesh::update_vertex_buffer(
		const AbstractReadMemoryBufferSharedPtr &buffer,
		const Uint16 index)
	{
		m_vertex_data[index]->update(hbt_vertex, *buffer);
	}

	AbstractWriteMemoryBufferSharedPtr OpenGl2Mesh::get_index_buffer()
	{
		AbstractWriteMemoryBufferSharedPtr result;

		if (m_index_data.get() != 0)
		{
			result = boost::make_shared<
				MappedHardwareWriteMemoryBuffer>(
					m_index_data, hbt_index);
		}

		return result;
	}

	void OpenGl2Mesh::bind(const VertexElements &vertex_elements,
		const HardwareBufferSharedPtr &buffer)
	{
		GLintptr offset;
		Uint32 i, stride, count;
		GLenum type;
		GLboolean normalized;
		VertexSemanticType semantic;

		if (vertex_elements.get_count() == 0)
		{
			return;
		}

		buffer->bind(hbt_vertex);

		stride = vertex_elements.get_stride();

		for (i = 0; i < vertex_elements.get_count(); ++i)
		{
			offset = vertex_elements.get_offset(i);
			count = vertex_elements.get_count(i);
			type = vertex_elements.get_gl_type(i);
			normalized = vertex_elements.get_gl_normalized(i);
			semantic = vertex_elements.get_semantic(i);

			glVertexAttribPointer(semantic, count, type,
				normalized, stride,
				static_cast<Uint8*>(0) + offset);

			glEnableVertexAttribArray(semantic);
		}
	}

	void OpenGl2Mesh::init_vertices()
	{
		Uint32 i, size, count;

		count = m_vertex_data.size();

		for (i = 0; i < count; ++i)
		{
			if (get_vertex_elements(i).get_count() > 0)
			{
				m_vertex_data[i] =
					boost::make_shared<HardwareBuffer>();

				size = get_vertex_count();
				size *= get_vertex_elements(i).get_stride();
				m_vertex_data[i]->set_size(hbt_vertex, size,
					get_vertices_usage());
			}
			else
			{
				m_vertex_data[i].reset();
			}
		}
	}

	void OpenGl2Mesh::init_indices()
	{
		Uint32 size;

		size = get_index_count();

		if (size == 0)
		{
			m_index_data.reset();

			return;
		}

		if (get_use_16_bit_indices())
		{
			size *= sizeof(Uint16);
		}
		else
		{
			size *= sizeof(Uint32);
		}

		m_index_data = boost::make_shared<HardwareBuffer>();
		m_index_data->set_size(hbt_index, size, get_indices_usage());
	}

	void OpenGl2Mesh::bind_vertex_buffers()
	{
		Uint32 i, count;

		count = m_vertex_data.size();

		for (i = 0; i < count; ++i)
		{
			bind(get_vertex_elements(i), m_vertex_data[i]);
		}
	}

	void OpenGl2Mesh::unbind_vertex_buffers()
	{
		Uint16 i;

		for (i = 0; i < vertex_stream_count; ++i)
		{
			glDisableVertexAttribArray(i);
		}

		HardwareBuffer::unbind(hbt_vertex);
	}

	void OpenGl2Mesh::bind_index_buffers()
	{
		if (m_index_data.get() != 0)
		{
			m_index_data->bind(hbt_index);
		}
	}

	void OpenGl2Mesh::unbind_index_buffers()
	{
		HardwareBuffer::unbind(hbt_index);
	}

	void OpenGl2Mesh::bind()
	{
		bind_vertex_buffers();
		bind_index_buffers();
	}

	void OpenGl2Mesh::unbind()
	{
		unbind_vertex_buffers();
		unbind_index_buffers();
	}

	void OpenGl2Mesh::draw(const MeshDrawData &draw_data)
	{
		if (m_index_data.get() != 0)
		{
			glDrawRangeElements(get_primitive_type(),
				draw_data.get_min_vertex(),
				draw_data.get_max_vertex(),
				draw_data.get_count(), get_index_type(),
				get_index_offset(draw_data.get_offset()) +
					static_cast<Uint8*>(0));
		}
		else
		{
			glDrawArrays(get_primitive_type(),
				draw_data.get_offset(), draw_data.get_count());
		}
	}

	AbstractMeshSharedPtr OpenGl2Mesh::clone_vertex_data()
	{
		boost::shared_ptr<OpenGl2Mesh> result;

		result = boost::make_shared<OpenGl2Mesh>();

		result->m_vertex_data = m_vertex_data;
		result->copy_vertex_descriptions(*this);

		return result;
	}

	bool OpenGl2Mesh::get_supports_restart_index()
	{
		return false;
	}

}
