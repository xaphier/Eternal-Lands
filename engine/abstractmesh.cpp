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
#include "vertexbuffers.hpp"
#include "vertexbuffersbuilder.hpp"

namespace eternal_lands
{

	AbstractMesh::AbstractMesh(const String &name,
		const bool static_indices, const bool static_vertices,
		const bool use_simd): m_name(name), m_vertex_count(0),
		m_index_count(0), m_primitive_type(pt_triangles),
		m_use_16_bit_indices(true), m_static_indices(static_indices),
		m_static_vertices(static_vertices), m_use_simd(use_simd)
	{
	}

	AbstractMesh::~AbstractMesh() throw()
	{
	}

	void AbstractMesh::init(const VertexFormatSharedPtr &vertex_format,
		const MeshDataToolSharedPtr &source,
		const bool static_indices, const bool static_vertices)
	{
		bool use_16_bit_indices;

		use_16_bit_indices = source->get_16_bit_indices();

		init(use_16_bit_indices, vertex_format, source, static_indices,
			static_vertices);
	}

	void AbstractMesh::init(const bool use_16_bit_indices,
		const VertexFormatSharedPtr &vertex_format,
		const MeshDataToolSharedPtr &source, const bool static_indices,
		const bool static_vertices)
	{
		AbstractWriteMemorySharedPtr buffer;
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
		m_static_indices = static_indices;
		m_static_vertices = static_vertices;

		LOG_DEBUG(UTF8("use_16_bit_indices: %1%, vertex_count:"
			" %2%, index_count: %3%, sub_mesh_count: %4%"),
			get_use_16_bit_indices() % get_vertex_count() %
			get_index_count() % get_sub_meshs().size());

		init_vertices();

		try
		{
			for (i = 0; i < vertex_stream_count; ++i)
			{
				if (get_vertex_elements(i).get_count() > 0)
				{
					buffer = get_vertex_buffer(i);

					assert(buffer.get() != 0);

					VertexStream stream(m_vertex_format,
						buffer, i, get_use_simd());

					source->write_vertex_stream(stream);
				}
			}
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(get_name());
			throw;
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
		const MeshDataToolSharedPtr &source, const bool static_vertices)
	{
		AbstractWriteMemorySharedPtr buffer;
		Uint32 i;

		assert(source.get() != 0);
		assert(source->get_vertex_count() > 0);
		assert(vertex_format.get() != 0);

		m_vertex_format = vertex_format;
		m_vertex_count = source->get_vertex_count();
		m_static_vertices = static_vertices;

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

				VertexStream stream(m_vertex_format, buffer, i,
					get_use_simd());

				source->write_vertex_stream(stream);
			}
		}
	}

	void AbstractMesh::init_vertex(const VertexBuffersSharedPtr &buffers,
		const bool static_vertices)
	{
		Uint32 i;

		assert(buffers.get() != 0);
		assert(buffers->get_vertex_count() > 0);

		m_vertex_format = buffers->get_format();
		m_vertex_count = buffers->get_vertex_count();
		m_static_vertices = static_vertices;

		LOG_DEBUG(UTF8("use_16_bit_indices: %1%, vertex_count:"
			" %2%, index_count: %3%, sub_mesh_count: %4%"),
			get_use_16_bit_indices() % get_vertex_count() %
			get_index_count() % get_sub_meshs().size());

		init_vertices();

		for (i = 0; i < vertex_stream_count; ++i)
		{
			if (get_vertex_elements(i).get_count() > 0)
			{
				set_vertex_buffer(buffers->get_buffer(i), i);
			}
		}
	}

	void AbstractMesh::init_vertex(
		const VertexFormatSharedPtr &vertex_format,
		const Uint32 vertex_count, const bool static_vertices)
	{
		assert(vertex_format.get() != 0);
		assert(vertex_count > 0);

		m_vertex_format = vertex_format;
		m_vertex_count = vertex_count;
		m_static_vertices = static_vertices;

		LOG_DEBUG(UTF8("use_16_bit_indices: %1%, vertex_count:"
			" %2%, index_count: %3%, sub_mesh_count: %4%"),
			get_use_16_bit_indices() % get_vertex_count() %
			get_index_count() % get_sub_meshs().size());

		init_vertices();
	}

	void AbstractMesh::update_vertex(const VertexBuffersSharedPtr &buffers)
	{
		Uint32 i;

		assert(buffers.get() != 0);
		assert(buffers->get_vertex_count() > 0);

		LOG_DEBUG(UTF8("use_16_bit_indices: %1%, vertex_count:"
			" %2%, index_count: %3%, sub_mesh_count: %4%"),
			get_use_16_bit_indices() % get_vertex_count() %
			get_index_count() % get_sub_meshs().size());

		for (i = 0; i < vertex_stream_count; ++i)
		{
			if (get_vertex_elements(i).get_count() > 0)
			{
				update_vertex_buffer(buffers->get_buffer(i), i);
			}
		}
	}

	VertexBuffersSharedPtr AbstractMesh::get_vertex_buffers(
		const Uint32 vertex_count) const
	{
		return VertexBuffersBuilder::get_vertex_buffers(
			get_vertex_format(), vertex_count, get_use_simd());
	}

	void AbstractMesh::init_indices(const Uint32Set &blocks,
		const IndexUpdateSourceSharedPtr &update,
		const bool static_indices)
	{
		AbstractWriteMemorySharedPtr buffer;

		m_use_16_bit_indices = update->get_use_16_bit_indices();
		m_index_count = update->get_count(blocks);
		m_primitive_type = update->get_primitive_type();
		m_sub_meshs = update->get_sub_meshs(blocks);
		m_static_indices = static_indices;

		init_indices();

		if (get_index_count() > 0)
		{
			buffer = get_index_buffer();

			assert(buffer.get() != 0);

			update->write_index_buffer(blocks, buffer);
		}
	}

	void AbstractMesh::init_indices(const bool use_16_bit_indices,
		const PrimitiveType primitive_type,
		const SubMeshVector &sub_meshs,
		const Uint32Vector &indices, const bool static_indices)
	{
		AbstractWriteMemorySharedPtr buffer;
		Uint32 i, count;

		m_use_16_bit_indices = use_16_bit_indices;
		m_index_count = indices.size();
		m_primitive_type = primitive_type;
		m_sub_meshs = sub_meshs;
		m_static_indices = static_indices;

		init_indices();

		if (get_index_count() > 0)
		{
		}

		buffer = get_index_buffer();

		assert(buffer.get() != 0);

		count = get_index_count();

		if (use_16_bit_indices)
		{
			for (i = 0; i < count; ++i)
			{
				static_cast<Uint16*>(buffer->get_ptr())[i] =
					indices[i];
			}

			return;
		}

		for (i = 0; i < count; ++i)
		{
			static_cast<Uint32*>(buffer->get_ptr())[i] = indices[i];
		}
	}

	void AbstractMesh::copy_vertex_data(AbstractMesh &mesh) const
	{
		mesh.m_vertex_format = m_vertex_format;
		mesh.m_vertex_count = m_vertex_count;
	}

	void AbstractMesh::copy_index_data(AbstractMesh &mesh) const
	{
		mesh.m_index_count = m_index_count;
		mesh.m_restart_index = m_restart_index;
		mesh.m_primitive_type = m_primitive_type;
		mesh.m_use_16_bit_indices = m_use_16_bit_indices;
		mesh.m_use_restart_index = m_use_restart_index;
	}

	void AbstractMesh::get_bounding_box(
		const Transformation &transformation, BoundingBox &bounding_box)
	{
		Uint32 i;

		if (get_sub_meshs().size() == 0)
		{
			EL_THROW_EXCEPTION(ArraySizeErrorException()
				<< errinfo_size(0));
		}

		bounding_box = get_sub_meshs()[0].get_bounding_box().transform(
			transformation);

		for (i = 1; i < get_sub_meshs().size(); ++i)
		{
			bounding_box.merge(get_sub_meshs()[i].get_bounding_box(
				).transform(transformation));
		}
	}

	void AbstractMesh::draw(const Uint32 index, const Uint32 instances)
	{
		draw(get_sub_meshs()[index], instances);
	}

	const VertexElements &AbstractMesh::get_vertex_elements(
		const Uint16 index) const
	{
		return m_vertex_format->get_vertex_elements(index);
	}

}
