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
		assert(GLEW_VERSION_3_0);
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

	OpenGl3Mesh::OpenGl3Mesh()
	{
		assert(GLEW_VERSION_3_0);
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

		bind_vertex_buffers();

		m_id->unbind();
	}

	void OpenGl3Mesh::bind()
	{
		m_id->bind();
		bind_index_buffers();
	}

	void OpenGl3Mesh::unbind()
	{
		m_id->unbind();
		unbind_index_buffers();
	}

	AbstractMeshSharedPtr OpenGl3Mesh::clone_vertex_data()
	{
		boost::shared_ptr<OpenGl3Mesh> result;

		result = boost::make_shared<OpenGl3Mesh>();

		result->m_id = m_id;
		result->m_vertex_data = m_vertex_data;
		result->copy_vertex_descriptions(*this);

		return result;
	}

}
