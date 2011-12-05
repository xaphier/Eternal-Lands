/****************************************************************************
 *            opengl31mesh.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl31mesh.hpp"

namespace eternal_lands
{

	OpenGl31Mesh::OpenGl31Mesh()
	{
		assert(GLEW_VERSION_3_1);
	}

	OpenGl31Mesh::~OpenGl31Mesh() throw()
	{
	}

	void OpenGl31Mesh::bind()
	{
		OpenGl3Mesh::bind();

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

	bool OpenGl31Mesh::get_supports_restart_index()
	{
		return true;
	}

}
