/****************************************************************************
 *            abstractmesh.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractmesh.hpp"
#include "indexupdatesource.hpp"
#include "meshdatatool.hpp"
#include "submesh.hpp"
#include "vertexelement.hpp"
#include "vertexelements.hpp"
#include "vertexformat.hpp"
#include "logging.hpp"
#include "exceptions.hpp"
#include "vertexstream.hpp"

namespace eternal_lands
{

	AbstractMesh::AbstractMesh()
	{
		m_use_16_bit_indices = true;
		m_vertex_count = 0;
		m_index_count = 0;
		m_primitive_type = pt_triangles;
	}

	AbstractMesh::~AbstractMesh() throw()
	{
	}

	void AbstractMesh::init(const VertexFormatSharedPtr &vertex_format,
		const MeshDataToolSharedPtr &source)
	{
		bool use_16_bit_indices;

		use_16_bit_indices = source->get_vertex_count()
			<= std::numeric_limits<Uint16>::max();

		init(use_16_bit_indices, vertex_format, source);
	}

	void AbstractMesh::init(const bool use_16_bit_indices,
		const VertexFormatSharedPtr &vertex_format,
		const MeshDataToolSharedPtr &source)
	{
		AbstractWriteMemoryBufferSharedPtr buffer;
		Uint32 i;

		if (source->get_vertex_count() == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("Vertex count is "
					"zero")));
		}

		if (source->get_use_restart_index() &&
			!get_supports_restart_index())
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_message(UTF8("Restart index used "
					"but not supported")));
		}

		m_use_16_bit_indices = use_16_bit_indices;
		m_vertex_format = vertex_format;
		m_vertex_count = source->get_vertex_count();
		m_index_count = source->get_index_count();
		m_sub_meshs = source->get_sub_meshs();
		m_primitive_type = source->get_primitive_type();
		m_restart_index = source->get_restart_index();
		m_use_restart_index = source->get_use_restart_index();

		LOG_DEBUG(UTF8("use_16_bit_indices: %1%, vertex_count:"
			" %2%, index_count: %3%, sub_mesh_count: %4%"),
			get_use_16_bit_indices() % get_vertex_count() %
			get_index_count() % get_sub_meshs().size());

		init_vertices();

		for (i = 0; i < vertex_stream_count; ++i)
		{
			if (get_vertex_elements(i).get_count() > 0)
			{
				buffer = get_vertex_buffer(i);

				assert(buffer.get() != 0);

				VertexStream stream(m_vertex_format, buffer, i);

				source->write_vertex_stream(stream);
			}
		}

		init_indices();

		if (get_index_count() > 0)
		{
			buffer = get_index_buffer();

			assert(buffer.get() != 0);

			source->write_index_buffer(get_use_16_bit_indices(),
				buffer);
		}
	}

	void AbstractMesh::init_vertex(
		const VertexFormatSharedPtr &vertex_format,
		const MeshDataToolSharedPtr &source)
	{
		AbstractWriteMemoryBufferSharedPtr buffer;
		Uint32 i;

		assert(source != 0);
		assert(source->get_vertex_count() > 0);

		m_vertex_format = vertex_format;
		m_vertex_count = source->get_vertex_count();

		LOG_DEBUG(UTF8("use_16_bit_indices: %1%, vertex_count:"
			" %2%, index_count: %3%, sub_mesh_count: %4%"),
			get_use_16_bit_indices() % get_vertex_count() %
			get_index_count() % get_sub_meshs().size());

		init_vertices();

		for (i = 0; i < 16; ++i)
		{
			if (get_vertex_elements(i).get_count() > 0)
			{
				buffer = get_vertex_buffer(i);

				assert(buffer.get() != 0);

				VertexStream stream(m_vertex_format, buffer, i);

				source->write_vertex_stream(stream);
			}
		}
	}

	void AbstractMesh::update_indices(const Uint32Set &blocks,
		const IndexUpdateSourceSharedPtr &update)
	{
		AbstractWriteMemoryBufferSharedPtr buffer;

		m_use_16_bit_indices = update->get_use_16_bit_indices();
		m_index_count = update->get_count(blocks);
		m_primitive_type = update->get_primitive_type();
		m_sub_meshs = update->get_sub_meshs(blocks);

		init_indices();

		if (get_index_count() > 0)
		{
			buffer = get_index_buffer();

			assert(buffer.get() != 0);

			update->write_index_buffer(blocks, buffer);
		}
	}

	void AbstractMesh::copy_vertex_descriptions(const AbstractMesh &mesh)
	{
		m_vertex_format = mesh.m_vertex_format;
		m_vertex_count = mesh.m_vertex_count;
	}

	void AbstractMesh::get_bounding_box(const glm::mat4x3 &matrix,
		BoundingBox &bounding_box)
	{
		Uint32 i;

		if (get_sub_meshs().size() == 0)
		{
			EL_THROW_EXCEPTION(ArraySizeErrorException()
				<< errinfo_size(0));
		}

		bounding_box = get_sub_meshs()[0].get_bounding_box().transform(
			matrix);

		for (i = 1; i < get_sub_meshs().size(); ++i)
		{
			bounding_box.merge(get_sub_meshs()[i].get_bounding_box(
				).transform(matrix));
		}
	}

	void AbstractMesh::draw(const Uint32 index)
	{
		draw(get_sub_meshs()[index]);
	}

	const VertexElements &AbstractMesh::get_vertex_elements(
		const Uint16 index) const
	{
		return m_vertex_format->get_vertex_elements(index);
	}

}