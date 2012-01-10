/****************************************************************************
 *            opengl3mesh.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl3mesh.hpp"
#include "submesh.hpp"

namespace eternal_lands
{

	class OpenGl3Mesh::VertexArrayObject
	{
		private:
			GLuint m_id;

		public:
			VertexArrayObject();
			~VertexArrayObject() throw();
			void bind() const;
			void unbind() const;

	};

	OpenGl3Mesh::VertexArrayObject::VertexArrayObject()
	{
		assert(GLEW_VERSION_3_0 || GLEW_ARB_vertex_array_object);
		glGenVertexArrays(1, &m_id);
	}

	OpenGl3Mesh::VertexArrayObject::~VertexArrayObject() throw()
	{
		glDeleteVertexArrays(1, &m_id);
	}

	void OpenGl3Mesh::VertexArrayObject::bind() const
	{
		glBindVertexArray(m_id);
	}

	void OpenGl3Mesh::VertexArrayObject::unbind() const
	{
		glBindVertexArray(0);
	}

	OpenGl3Mesh::OpenGl3Mesh(const String &name, const bool static_indices,
		const bool static_vertices, const bool use_simd):
		OpenGl2Mesh(name, static_indices, static_vertices, use_simd)
	{
		assert(GLEW_VERSION_3_0 || GLEW_ARB_vertex_array_object);
	}

	OpenGl3Mesh::~OpenGl3Mesh() throw()
	{
	}

	void OpenGl3Mesh::init_vertices()
	{
		m_id.reset();

		OpenGl2Mesh::init_vertices();

		m_id = boost::make_shared<VertexArrayObject>();

		m_id->bind();

		bind_vertex_buffers(m_used_attributes);

		m_id->unbind();
	}

	void OpenGl3Mesh::bind(BitSet32 &used_attributes)
	{
		m_id->bind();
		bind_index_buffers();

		used_attributes = m_used_attributes;
	}

	void OpenGl3Mesh::unbind()
	{
		m_id->unbind();
		unbind_index_buffers();
	}

	void OpenGl3Mesh::copy_vertex_data(OpenGl3Mesh &mesh) const
	{
		OpenGl2Mesh::copy_vertex_data(mesh);
		mesh.m_id = m_id;
	}

	AbstractMeshSharedPtr OpenGl3Mesh::clone_vertex_data() const
	{
		boost::shared_ptr<OpenGl3Mesh> result;

		result = boost::make_shared<OpenGl3Mesh>(get_name(),
			get_static_indices(), get_static_vertices(),
			get_use_simd());

		copy_vertex_data(*result);

		return result;
	}

	AbstractMeshSharedPtr OpenGl3Mesh::clone_index_data() const
	{
		boost::shared_ptr<OpenGl3Mesh> result;

		result = boost::make_shared<OpenGl3Mesh>(get_name(),
			get_static_indices(), get_static_vertices(),
			get_use_simd());

		copy_index_data(*result);

		return result;
	}

}
