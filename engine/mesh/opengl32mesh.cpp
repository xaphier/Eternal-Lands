/****************************************************************************
 *            opengl32mesh.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl32mesh.hpp"
#include "meshdrawdata.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	OpenGl32Mesh::OpenGl32Mesh(const HardwareBufferMapperWeakPtr
			&hardware_buffer_mapper, const String &name,
		const bool static_indices, const bool static_vertices,
		const bool static_instances, const bool use_simd):
		OpenGl31Mesh(hardware_buffer_mapper, name, static_indices,
			static_vertices, static_instances, use_simd)
	{
		assert(GLEW_VERSION_3_2);
	}

	OpenGl32Mesh::~OpenGl32Mesh() noexcept
	{
	}

	AbstractMeshSharedPtr OpenGl32Mesh::clone(
		const BitSet16 shared_vertex_datas,
		const bool shared_index_data) const
	{
		boost::shared_ptr<OpenGl32Mesh> result;

		CHECK_GL_ERROR();

		result = boost::make_shared<OpenGl32Mesh>(
			get_hardware_buffer_mapper(), get_name(),
			get_static_indices(), get_static_vertices(),
			get_static_instances(), get_use_simd());

		copy_data(*result);
		clone_buffers(shared_vertex_datas, shared_index_data, *result);

		CHECK_GL_ERROR_NAME(get_name());

		return result;
	}

	bool OpenGl32Mesh::get_supports_base_vertex() const
	{
		return true;
	}

	void OpenGl32Mesh::draw(const MeshDrawData &draw_data,
		const Uint32 instances, const PrimitiveType primitive)
	{
		assert(instances > 0);

		if (get_has_index_data())
		{
			glDrawElementsInstancedBaseVertex(primitive,
				draw_data.get_count(), get_index_type(),
				get_index_offset(draw_data.get_offset()) +
					static_cast<Uint8*>(0), instances,
				draw_data.get_base_vertex());
		}
		else
		{
			glDrawArraysInstanced(primitive, draw_data.get_offset(),
				draw_data.get_count(), instances);
		}
	}

}

