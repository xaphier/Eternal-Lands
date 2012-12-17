/****************************************************************************
 *            abstractmesh.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "abstractmesh.hpp"
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
		const bool static_instances, const bool use_simd): m_name(name),
		m_index_count(0), m_vertex_count(0), m_instance_count(0),
		m_primitive(pt_triangles),
		m_use_16_bit_indices(true), m_use_restart_index(false),
		m_static_indices(static_indices),
		m_static_vertices(static_vertices),
		m_static_instances(static_instances), m_use_simd(use_simd)
	{
		assert(!get_name().get().empty());
	}

	AbstractMesh::~AbstractMesh() noexcept
	{
	}

	void AbstractMesh::init(const VertexFormatSharedPtr &vertex_format,
		const MeshDataToolSharedPtr &source,
		const Uint32 instance_count, const bool static_indices,
		const bool static_vertices, const bool static_instances)
	{
		AbstractWriteMemorySharedPtr buffer;
		VertexStreamBitset used_vertex_buffers;
		Uint32 i;

		if (source->get_vertex_count() == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_name(get_name())
				<< errinfo_message(UTF8("Vertex count is "
					"zero")));
		}

		if (source->get_use_restart_index() &&
			!get_supports_restart_index())
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_name(get_name())
				<< errinfo_message(UTF8("Restart index used "
					"but not supported")));
		}

		m_use_16_bit_indices = source->get_16_bit_indices();
		m_vertex_format = vertex_format;
		m_index_count = source->get_index_count();
		m_vertex_count = source->get_vertex_count();
		m_instance_count = instance_count;
		m_sub_meshs = source->get_sub_meshs();
		m_min_max_boxes = source->get_min_max_boxes();
		m_primitive = source->get_primitive();
		m_use_restart_index = source->get_use_restart_index();
		m_static_indices = static_indices;
		m_static_vertices = static_vertices;
		m_static_instances = static_instances;

		LOG_DEBUG(lt_mesh, UTF8("Mesh '%1%' use_16_bit_indices: %2%, "
			"vertex_count: %3%, index_count: %4%, sub_mesh_count:"
			" %5%"), get_name() % get_use_16_bit_indices() %
			get_vertex_count() % get_index_count() %
			get_sub_meshs().size());

		try
		{
			used_vertex_buffers = get_used_vertex_buffers();

			init_vertex_buffers(used_vertex_buffers);

			for (i = 0; i < vertex_stream_count; ++i)
			{
				if (used_vertex_buffers[i])
				{
					buffer = get_vertex_buffer(i);

					assert(buffer.get() != nullptr);

					VertexStream stream(m_vertex_format,
						buffer, get_vertex_count(), i,
						get_use_simd());

					source->write_vertex_stream(stream);
				}
			}

			init_index_buffer();

			if (get_index_count() > 0)
			{
				buffer = get_index_buffer();

				assert(buffer.get() != nullptr);

				source->write_index_buffer(
					get_use_16_bit_indices(), buffer);
			}
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(get_name());
			throw;
		}
	}

	void AbstractMesh::init(const VertexFormatSharedPtr &vertex_format,
		const Uint32 index_count, const Uint32 vertex_count,
		const Uint32 instance_count, const bool use_16_bit_indices,
		const bool static_indices, const bool static_vertices,
		const bool static_instances)
	{
		if (vertex_count == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_name(get_name())
				<< errinfo_message(UTF8("Vertex count is "
					"zero")));
		}

		m_use_16_bit_indices = use_16_bit_indices;
		m_vertex_format = vertex_format;
		m_index_count = index_count;
		m_vertex_count = vertex_count;
		m_instance_count = instance_count;
		m_static_indices = static_indices;
		m_static_vertices = static_vertices;
		m_static_instances = static_instances;

		LOG_DEBUG(lt_mesh, UTF8("Mesh '%1%' use_16_bit_indices: %2%, "
			"vertex_count: %3%, index_count: %4%"), get_name() %
			get_use_16_bit_indices() % get_vertex_count() %
			get_index_count());

		try
		{
			init_vertex_buffers(get_used_vertex_buffers());
			init_index_buffer();
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(get_name());
			throw;
		}
	}

	void AbstractMesh::init(const VertexFormatSharedPtr &vertex_format,
		const Uint32 vertex_count, const Uint32 instance_count,
		const bool static_vertices, const bool static_instances)
	{
		if (vertex_count == 0)
		{
			EL_THROW_EXCEPTION(InvalidParameterException()
				<< errinfo_name(get_name())
				<< errinfo_message(UTF8("Vertex count is "
					"zero")));
		}

		m_vertex_format = vertex_format;
		m_vertex_count = vertex_count;
		m_instance_count = instance_count;
		m_static_vertices = static_vertices;
		m_static_instances = static_instances;

		LOG_DEBUG(lt_mesh, UTF8("Mesh '%1%' use_16_bit_indices: %2%, "
			"vertex_count: %3%, index_count: %4%"), get_name() %
			get_use_16_bit_indices() % get_vertex_count() %
			get_index_count());

		try
		{
			init_vertex_buffers(get_used_vertex_buffers());
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(get_name());
			throw;
		}
	}

	void AbstractMesh::init(const Uint32 index_count,
		const bool use_16_bit_indices, const bool static_indices)
	{
		m_use_16_bit_indices = use_16_bit_indices;
		m_index_count = index_count;
		m_static_indices = static_indices;

		LOG_DEBUG(lt_mesh, UTF8("Mesh '%1%' use_16_bit_indices: %2%, "
			"vertex_count: %3%, index_count: %4%"), get_name() %
			get_use_16_bit_indices() % get_vertex_count() %
			get_index_count());

		try
		{
			init_index_buffer();
		}
		catch (boost::exception &exception)
		{
			exception << errinfo_name(get_name());
			throw;
		}
	}

	VertexStreamBitset AbstractMesh::get_used_vertex_buffers() const
	{
		VertexStreamBitset result;
		Uint32 i;

		for (i = 0; i < vertex_stream_count; ++i)
		{
			result[i] = get_vertex_elements(i).get_count() > 0;
		}

		return result;
	}

	void AbstractMesh::copy_data(AbstractMesh &mesh) const
	{
		mesh.m_vertex_format = m_vertex_format;
		mesh.m_vertex_count = m_vertex_count;
		mesh.m_index_count = m_index_count;
		mesh.m_instance_count = m_instance_count;
		mesh.m_primitive = m_primitive;
		mesh.m_use_16_bit_indices = m_use_16_bit_indices;
		mesh.m_use_restart_index = m_use_restart_index;
		mesh.m_static_indices = m_static_indices;
		mesh.m_static_vertices = m_static_vertices;
		mesh.m_sub_meshs = m_sub_meshs;
		mesh.m_min_max_boxes = m_min_max_boxes;
	}

	void AbstractMesh::get_bounding_box(
		const Transformation &transformation, BoundingBox &bounding_box)
	{
		Uint32 i;

		if (get_sub_meshs().size() == 0)
		{
			EL_THROW_EXCEPTION(ArraySizeErrorException()
				<< errinfo_name(get_name())
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
		draw(get_sub_meshs()[index], instances, get_primitive());
	}

	const VertexElements &AbstractMesh::get_vertex_elements(
		const Uint16 index) const
	{
		return m_vertex_format->get_vertex_elements(index);
	}

	void AbstractMesh::set_sub_meshs(const SubMeshVector &sub_meshs)
	{
#ifndef	NDEBUG
		BOOST_FOREACH(const SubMesh &sub_mesh, sub_meshs)
		{
			assert((sub_mesh.get_base_vertex() == 0) ||
				get_supports_base_vertex());
		}
#endif	/* NDEBUG */
		m_sub_meshs = sub_meshs;
	}

	VertexStreamSharedPtr AbstractMesh::get_vertex_stream(
		const Uint16 index)
	{
		VertexStreamSharedPtr result;

		result = boost::make_shared<VertexStream>(
			get_vertex_elements(index), get_vertex_buffer(index),
			get_vertex_count(), get_use_simd());

		return result;
	}

}
