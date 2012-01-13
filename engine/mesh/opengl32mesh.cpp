/****************************************************************************
 *            opengl32mesh.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl32mesh.hpp"
#include "meshdrawdata.hpp"

namespace eternal_lands
{

	OpenGl32Mesh::OpenGl32Mesh(const String &name,
		const bool static_indices, const bool static_vertices,
		const bool use_simd): OpenGl3Mesh(name, static_indices,
			static_vertices, use_simd)
	{
		assert(GLEW_VERSION_3_2);
	}

	OpenGl32Mesh::~OpenGl32Mesh() throw()
	{
	}

	AbstractMeshSharedPtr OpenGl32Mesh::clone_vertex_data() const
	{
		boost::shared_ptr<OpenGl32Mesh> result;

		result = boost::make_shared<OpenGl32Mesh>(get_name(),
			get_static_indices(), get_static_vertices(),
			get_use_simd());

		copy_vertex_data(*result);

		return result;
	}

	AbstractMeshSharedPtr OpenGl32Mesh::clone_index_data() const
	{
		boost::shared_ptr<OpenGl32Mesh> result;

		result = boost::make_shared<OpenGl32Mesh>(get_name(),
			get_static_indices(), get_static_vertices(),
			get_use_simd());

		copy_index_data(*result);

		return result;
	}

	bool OpenGl32Mesh::get_supports_base_vertex() const
	{
		return true;
	}

	void OpenGl32Mesh::draw(const MeshDrawData &draw_data,
		const Uint32 instances)
	{
		assert(instances > 0);

		if (get_has_index_data())
		{
			glDrawElementsInstancedBaseVertex(get_primitive_type(),
				draw_data.get_count(), get_index_type(),
				get_index_offset(draw_data.get_offset()) +
					static_cast<Uint8*>(0), instances,
				draw_data.get_base_vertex());
		}
		else
		{
			glDrawArraysInstanced(get_primitive_type(),
				draw_data.get_offset(), draw_data.get_count(),
				instances);
		}
	}

}
