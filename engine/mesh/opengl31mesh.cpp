/****************************************************************************
 *            opengl31mesh.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl31mesh.hpp"

namespace eternal_lands
{

	OpenGl31Mesh::OpenGl31Mesh(const String &name,
		const bool static_indices, const bool static_vertices):
		OpenGl3Mesh(name, static_indices, static_vertices)
	{
		assert(GLEW_VERSION_3_1);
	}

	OpenGl31Mesh::~OpenGl31Mesh() throw()
	{
	}

	void OpenGl31Mesh::bind(BitSet32 &used_attributes)
	{
		OpenGl3Mesh::bind(used_attributes);

		if (get_use_restart_index())
		{
			glEnable(GL_PRIMITIVE_RESTART);
			glPrimitiveRestartIndex(get_restart_index());
		}
		else
		{
			glDisable(GL_PRIMITIVE_RESTART);
		}
	}

	void OpenGl31Mesh::unbind()
	{
		OpenGl3Mesh::unbind();
		glDisable(GL_PRIMITIVE_RESTART);
	}

	AbstractMeshSharedPtr OpenGl31Mesh::clone_vertex_data() const
	{
		boost::shared_ptr<OpenGl31Mesh> result;

		result = boost::make_shared<OpenGl31Mesh>(get_name(),
			get_static_indices(), get_static_vertices());

		copy_vertex_data(*result);

		return result;
	}

	AbstractMeshSharedPtr OpenGl31Mesh::clone_index_data() const
	{
		boost::shared_ptr<OpenGl31Mesh> result;

		result = boost::make_shared<OpenGl31Mesh>(get_name(),
			get_static_indices(), get_static_vertices());

		copy_index_data(*result);

		return result;
	}

	bool OpenGl31Mesh::get_supports_restart_index()
	{
		return true;
	}

}
