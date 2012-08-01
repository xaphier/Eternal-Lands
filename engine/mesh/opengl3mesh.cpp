/****************************************************************************
 *            opengl3mesh.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl3mesh.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	class OpenGl3Mesh::VertexArrayObject
	{
		private:
			GLuint m_id;

		public:
			VertexArrayObject();
			~VertexArrayObject() noexcept;
			void bind() const noexcept;
			void unbind() const noexcept;

	};

	OpenGl3Mesh::VertexArrayObject::VertexArrayObject()
	{
		assert(GLEW_VERSION_3_0 || GLEW_ARB_vertex_array_object);
		glGenVertexArrays(1, &m_id);
	}

	OpenGl3Mesh::VertexArrayObject::~VertexArrayObject() noexcept
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void OpenGl3Mesh::VertexArrayObject::bind() const noexcept
	{
		glBindVertexArray(m_id);
	}

	void OpenGl3Mesh::VertexArrayObject::unbind() const noexcept
	{
		glBindVertexArray(0);
	}

	OpenGl3Mesh::OpenGl3Mesh(const HardwareBufferMapperWeakPtr
			&hardware_buffer_mapper, const String &name,
		const bool static_indices, const bool static_vertices,
		const bool static_instances, const bool use_simd):
		OpenGl2Mesh(hardware_buffer_mapper, name, static_indices,
			static_vertices, static_instances, use_simd)
	{
		assert(GLEW_VERSION_3_0 || GLEW_ARB_vertex_array_object);
	}

	OpenGl3Mesh::~OpenGl3Mesh() noexcept
	{
	}

	void OpenGl3Mesh::init_vertex_buffers(BitSet32 &used_attributes)
	{
		bind_vertex_buffers(used_attributes);
	}

	void OpenGl3Mesh::init_vertex_buffers(
		const VertexStreamBitset vertex_buffers)
	{
		CHECK_GL_ERROR();

		OpenGl2Mesh::init_vertex_buffers(vertex_buffers);

		CHECK_GL_ERROR_NAME(get_name());

		m_id = boost::make_shared<VertexArrayObject>();

		m_id->bind();

		init_vertex_buffers(m_used_attributes);

		m_id->unbind();

		CHECK_GL_ERROR_NAME(get_name());
	}

	void OpenGl3Mesh::bind(BitSet32 &used_attributes)
	{
		DEBUG_CHECK_GL_ERROR();

		m_id->bind();

		DEBUG_CHECK_GL_ERROR_NAME(get_name());

		bind_index_buffer();

		used_attributes = m_used_attributes;

		DEBUG_CHECK_GL_ERROR_NAME(get_name());
	}

	void OpenGl3Mesh::unbind()
	{
		DEBUG_CHECK_GL_ERROR();

		m_id->unbind();

		DEBUG_CHECK_GL_ERROR_NAME(get_name());

		unbind_index_buffer();

		DEBUG_CHECK_GL_ERROR_NAME(get_name());
	}

	AbstractMeshSharedPtr OpenGl3Mesh::clone(
		const VertexStreamBitset shared_vertex_datas,
		const bool shared_index_data) const
	{
		boost::shared_ptr<OpenGl3Mesh> result;

		CHECK_GL_ERROR();

		result = boost::make_shared<OpenGl3Mesh>(
			get_hardware_buffer_mapper(), get_name(),
			get_static_indices(), get_static_vertices(),
			get_static_instances(), get_use_simd());

		copy_data(*result);
		clone_buffers(shared_vertex_datas, shared_index_data, *result);

		CHECK_GL_ERROR_NAME(get_name());

		return result;
	}

}
