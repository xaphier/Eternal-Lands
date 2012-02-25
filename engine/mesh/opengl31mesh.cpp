/****************************************************************************
 *            opengl31mesh.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl31mesh.hpp"
#include "meshdrawdata.hpp"

namespace eternal_lands
{

	OpenGl31Mesh::OpenGl31Mesh(const String &name,
		const bool static_indices, const bool static_vertices,
		const bool use_simd): OpenGl3Mesh(name, static_indices,
			static_vertices, use_simd)
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
			get_static_indices(), get_static_vertices(),
			get_use_simd());

		copy_vertex_data(*result);

		return result;
	}

	AbstractMeshSharedPtr OpenGl31Mesh::clone_index_data() const
	{
		boost::shared_ptr<OpenGl31Mesh> result;

		result = boost::make_shared<OpenGl31Mesh>(get_name(),
			get_static_indices(), get_static_vertices(),
			get_use_simd());

		copy_index_data(*result);

		return result;
	}

	bool OpenGl31Mesh::get_supports_restart_index() const
	{
		return true;
	}

	void OpenGl31Mesh::draw(const MeshDrawData &draw_data,
		const Uint32 instances)
	{
		assert(instances > 0);
		assert(draw_data.get_base_vertex() == 0);

		if (get_has_index_data())
		{
			glDrawElementsInstanced(get_primitive_type(),
				draw_data.get_count(), get_index_type(),
				get_index_offset(draw_data.get_offset()) +
					static_cast<Uint8*>(0), instances);
		}
		else
		{
			glDrawArraysInstanced(get_primitive_type(),
				draw_data.get_offset(), draw_data.get_count(),
				instances);
		}
	}

}
