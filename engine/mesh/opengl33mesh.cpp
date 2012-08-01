/****************************************************************************
 *            opengl33mesh.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "opengl33mesh.hpp"
#include "vertexelements.hpp"
#include "exceptions.hpp"

namespace eternal_lands
{

	OpenGl33Mesh::OpenGl33Mesh(const HardwareBufferMapperWeakPtr
			&hardware_buffer_mapper, const String &name,
		const bool static_indices, const bool static_vertices,
		const bool static_instances,
		const bool use_simd): OpenGl32Mesh(hardware_buffer_mapper,
			name, static_indices, static_vertices,
			static_instances, use_simd)
	{
		assert(GLEW_VERSION_3_3);
	}

	OpenGl33Mesh::~OpenGl33Mesh() noexcept
	{
	}


	void OpenGl33Mesh::inti_divisor(const VertexElements &vertex_elements)
	{
		Uint32 i, divisor, count;
		VertexSemanticType semantic;

		if (vertex_elements.get_count() == 0)
		{
			return;
		}

		count = vertex_elements.get_count();
		divisor = vertex_elements.get_divisor();

		for (i = 0; i < count; ++i)
		{
			semantic = vertex_elements.get_semantic(i);

			glVertexAttribDivisor(semantic, divisor);
		}
	}

	void OpenGl33Mesh::init_vertex_buffers(BitSet32 &used_attributes)
	{
		Uint32 i;

		OpenGl32Mesh::init_vertex_buffers(used_attributes);

		DEBUG_CHECK_GL_ERROR();

		for (i = 0; i < vertex_stream_count; ++i)
		{
			inti_divisor(get_vertex_elements(i));
		}

		DEBUG_CHECK_GL_ERROR();
	}

	AbstractMeshSharedPtr OpenGl33Mesh::clone(
		const BitSet16 shared_vertex_datas,
		const bool shared_index_data) const
	{
		boost::shared_ptr<OpenGl33Mesh> result;

		CHECK_GL_ERROR();

		result = boost::make_shared<OpenGl33Mesh>(
			get_hardware_buffer_mapper(), get_name(),
			get_static_indices(), get_static_vertices(),
			get_static_instances(), get_use_simd());

		copy_data(*result);
		clone_buffers(shared_vertex_datas, shared_index_data, *result);

		CHECK_GL_ERROR_NAME(get_name());

		return result;
	}

	bool OpenGl33Mesh::get_supports_vertex_attribute_divisor() const
	{
		return true;
	}

}

