/****************************************************************************
 *            opengl2mesh.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl2mesh.hpp"
#include "hardwarebuffer/hardwarebuffermapper.hpp"
#include "vertexelements.hpp"
#include "meshdrawdata.hpp"
#include "vertexstream.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	OpenGl2Mesh::OpenGl2Mesh(const HardwareBufferMapperConstWeakPtr
			&hardware_buffer_mapper, const String &name,
		const bool static_indices, const bool static_vertices,
		const bool static_instances, const bool use_simd):
		AbstractMesh(name, static_indices, static_vertices,
			static_instances, use_simd),
		m_hardware_buffer_mapper(hardware_buffer_mapper)
	{
	}

	OpenGl2Mesh::~OpenGl2Mesh() noexcept
	{
	}

	AbstractWriteMemorySharedPtr OpenGl2Mesh::get_vertex_buffer(
		const Uint16 index)
	{
		AbstractWriteMemorySharedPtr result;

		assert(index < m_vertex_data.size());

		if (m_vertex_data[index].get() != nullptr)
		{
			result = get_hardware_buffer_mapper(
				)->write_map_hardware_buffer(
					m_vertex_data[index]);
		}

		return result;
	}

	void OpenGl2Mesh::set_vertex_buffer(
		const AbstractReadMemoryConstSharedPtr &buffer,
		const Uint16 index)
	{
		m_vertex_data[index]->bind(btt_vertex);
		m_vertex_data[index]->set(btt_vertex, *buffer,
			get_vertices_usage());
	}

	void OpenGl2Mesh::update_vertex_buffer(
		const AbstractReadMemoryConstSharedPtr &buffer,
		const Uint16 index)
	{
		m_vertex_data[index]->bind(btt_vertex);
		m_vertex_data[index]->update(btt_vertex, *buffer);
	}

	AbstractWriteMemorySharedPtr OpenGl2Mesh::get_index_buffer()
	{
		AbstractWriteMemorySharedPtr result;

		if (m_index_data.get() != nullptr)
		{
			result = get_hardware_buffer_mapper(
				)->write_map_hardware_buffer(m_index_data);
		}

		return result;
	}

	void OpenGl2Mesh::bind(const VertexElements &vertex_elements,
		const HardwareBufferSharedPtr &buffer,
		BitSet32 &used_attributes)
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

		buffer->bind(btt_vertex);

		stride = vertex_elements.get_stride();

		for (i = 0; i < vertex_elements.get_count(); ++i)
		{
			offset = vertex_elements.get_offset(i);
			count = vertex_elements.get_count(i);
			type = vertex_elements.get_gl_type(i);
			normalized = vertex_elements.get_gl_normalized(i);
			semantic = vertex_elements.get_semantic(i);

			used_attributes[semantic] = true;

			glVertexAttribPointer(semantic, count, type,
				normalized, stride,
				static_cast<Uint8*>(0) + offset);
		}
	}

	void OpenGl2Mesh::init_vertex_buffers(
		const VertexStreamBitset vertex_buffers)
	{
		Uint32 i, size, count;

		CHECK_GL_ERROR();

		count = m_vertex_data.size();

		for (i = 0; i < count; ++i)
		{
			if (!vertex_buffers[i])
			{
				continue;
			}

			m_vertex_data[i].reset();

			assert((get_vertex_elements(i).get_divisor() == 0) ||
				get_supports_vertex_attribute_divisor());

			if ((get_vertex_elements(i).get_count() > 0) &&
				((get_vertex_elements(i).get_divisor() == 0) ||
				get_supports_vertex_attribute_divisor()))
			{
				m_vertex_data[i] =
					boost::make_shared<HardwareBuffer>();

				if (get_vertex_elements(i).get_divisor() > 0)
				{
					size = get_instance_count();
				}
				else
				{
					size = get_vertex_count();
				}

				size *= get_vertex_elements(i).get_stride();

				assert(size > 0);

				m_vertex_data[i]->bind(btt_vertex);
				m_vertex_data[i]->set_size(btt_vertex, size,
					get_vertices_usage());
			}
		}

		CHECK_GL_ERROR_NAME(get_name());
	}

	void OpenGl2Mesh::init_index_buffer()
	{
		Uint32 size;

		CHECK_GL_ERROR();

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
		m_index_data->bind(btt_index);
		m_index_data->set_size(btt_index, size, get_indices_usage());

		CHECK_GL_ERROR_NAME(get_name());
	}

	void OpenGl2Mesh::bind_vertex_buffers(BitSet32 &used_attributes) const
	{
		BitSet32 attributes;
		Uint32 i;

		DEBUG_CHECK_GL_ERROR();

		for (i = 0; i < vertex_stream_count; ++i)
		{
			bind(get_vertex_elements(i), m_vertex_data[i],
				attributes);
		}

		DEBUG_CHECK_GL_ERROR_NAME(get_name());

		for (i = 0; i < vertex_stream_count; ++i)
		{
			if (used_attributes[i] && !attributes[i])
			{
				glDisableVertexAttribArray(i);
			}

			if (!used_attributes[i] && attributes[i])
			{
				glEnableVertexAttribArray(i);
			}
		}

		used_attributes = attributes;

		DEBUG_CHECK_GL_ERROR_NAME(get_name());
	}

	void OpenGl2Mesh::unbind_vertex_buffers() const
	{
		Uint16 i;

		DEBUG_CHECK_GL_ERROR();

		for (i = 0; i < vertex_stream_count; ++i)
		{
			glDisableVertexAttribArray(i);
		}

		DEBUG_CHECK_GL_ERROR_NAME(get_name());

		HardwareBuffer::unbind(btt_vertex);

		DEBUG_CHECK_GL_ERROR_NAME(get_name());
	}

	void OpenGl2Mesh::bind_index_buffer() const
	{
		if (m_index_data.get() != nullptr)
		{
			m_index_data->bind(btt_index);
		}
	}

	void OpenGl2Mesh::unbind_index_buffer() const
	{
		HardwareBuffer::unbind(btt_index);
	}

	void OpenGl2Mesh::bind(BitSet32 &used_attributes) const
	{
		bind_vertex_buffers(used_attributes);
		bind_index_buffer();
	}

	void OpenGl2Mesh::unbind() const
	{
		unbind_vertex_buffers();
		unbind_index_buffer();
	}

	void OpenGl2Mesh::draw(const MeshDrawData &draw_data,
		const Uint32 instances, const PrimitiveType primitive) const
	{
		assert(instances == 1);
		assert(draw_data.get_base_vertex() == 0);

		if (get_has_index_data())
		{
			glDrawRangeElements(primitive,
				draw_data.get_min_vertex(),
				draw_data.get_max_vertex(),
				draw_data.get_count(), get_index_type(),
				get_index_offset(draw_data.get_offset()) +
					static_cast<Uint8*>(0));
		}
		else
		{
			glDrawArrays(primitive, draw_data.get_offset(),
				draw_data.get_count());
		}
	}

	void OpenGl2Mesh::clone_buffers(const BitSet16 shared_vertex_datas,
		const bool shared_index_data, OpenGl2Mesh &mesh) const
	{
		Uint16 i;

		for (i = 0; i < vertex_stream_count; ++i)
		{
			if (shared_vertex_datas[i])
			{
				mesh.m_vertex_data[i] = m_vertex_data[i];
			}
		}

		if (shared_vertex_datas != get_used_vertex_buffers())
		{
			mesh.init_vertex_buffers(~shared_vertex_datas);
		}

		if (shared_index_data)
		{
			mesh.m_index_data = m_index_data;
		}
		else
		{
			mesh.init_index_buffer();
		}
	}

	AbstractMeshSharedPtr OpenGl2Mesh::clone(
		const VertexStreamBitset shared_vertex_datas,
		const bool shared_index_data) const
	{
		boost::shared_ptr<OpenGl2Mesh> result;

		CHECK_GL_ERROR();

		result = boost::make_shared<OpenGl2Mesh>(
			get_hardware_buffer_mapper(), get_name(),
			get_static_indices(), get_static_vertices(),
			get_static_instances(), get_use_simd());

		copy_data(*result);
		clone_buffers(shared_vertex_datas, shared_index_data, *result);

		CHECK_GL_ERROR_NAME(get_name());

		return result;
	}

	bool OpenGl2Mesh::get_supports_base_vertex() const
	{
		return false;
	}

	bool OpenGl2Mesh::get_supports_restart_index() const
	{
		return false;
	}

	bool OpenGl2Mesh::get_supports_vertex_attribute_divisor() const
	{
		return false;
	}

}

