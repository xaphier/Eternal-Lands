/****************************************************************************
 *            vertexstream.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "vertexstream.hpp"
#include "abstractwritememory.hpp"
#include "vertexformat.hpp"
#include "exceptions.hpp"
#include "alignedvec4array.hpp"

namespace eternal_lands
{

	class VertexStream::PackData
	{
		private:
			PackFormatType m_pack_format;
			Uint32 m_offset;

		public:
			PackData();
			PackData(const PackFormatType pack_format,
				const Uint32 offset);
			~PackData() throw();

			inline PackFormatType get_pack_format() const
			{
				return m_pack_format;
			}

			inline Uint32 get_offset() const
			{
				return m_offset;
			}
	};

	VertexStream::PackData::PackData(): m_pack_format(pft_float_4),
		m_offset(0)
	{
	}

	VertexStream::PackData::PackData(const PackFormatType pack_format,
		const Uint32 offset): m_pack_format(pack_format),
		m_offset(offset)
	{
	}

	VertexStream::PackData::~PackData() throw()
	{
	}

	VertexStream::VertexStream(const VertexFormatSharedPtr &format,
		const AbstractWriteMemorySharedPtr &buffer, const Uint16 index,
		const bool use_simd): m_format(format), m_buffer(buffer),
		m_offset(0), m_index(index), m_use_simd(use_simd)
	{
		BOOST_FOREACH(const VertexElement &element,
			m_format->get_vertex_elements(
				index).get_vertex_elements())
		{
			m_pack_datas[element.get_semantic()] =
				PackData(element.get_pack_format(),
					element.get_offset());
		}

		m_stride = m_format->get_vertex_elements(index).get_stride();
	}

	VertexStream::~VertexStream() throw()
	{
	}

	void VertexStream::set(const VertexSemanticType semantic,
		const glm::vec4 &data)
	{
		VertexSemanticTypePackDataMap::const_iterator found;

		found = m_pack_datas.find(semantic);

		if (found == m_pack_datas.end())
		{
			return;
		}

		try
		{
			PackTool::pack(data, get_offset() +
				found->second.get_offset(),
				found->second.get_pack_format(), *m_buffer);
		}
		catch (boost::exception &exception)
		{
			BoostFormat format_string(UTF8("Range error while "
				"writing to vertex format '%1%' at semantic "
				"%2% of type %3%."));

			BOOST_FOREACH(const VertexElement &element,
				get_format()->get_vertex_elements(
					get_index()).get_vertex_elements())
			{
				if (element.get_semantic() == semantic)
				{
					format_string % get_format()->get_name()
						% semantic % element.get_type();
					break;
				}
			}

			exception << errinfo_message(format_string.str());

			throw;
		}
	}

	void VertexStream::set(const VertexSemanticType semantic,
		const Vec4Vector &data)
	{
		VertexSemanticTypePackDataMap::const_iterator found;
		Uint32 count, offset;
		PackFormatType pack_format;

		found = m_pack_datas.find(semantic);

		if (found == m_pack_datas.end())
		{
			return;
		}

		count = data.size();
		offset = found->second.get_offset() + get_offset();
		pack_format = found->second.get_pack_format();

		try
		{
			PackTool::pack(data, offset, get_stride(), count,
				pack_format, *m_buffer);
		}
		catch (boost::exception &exception)
		{
			BoostFormat format_string(UTF8("Range error while "
				"writing to vertex format '%1%' at semantic "
				"%2% of type %3%."));

			BOOST_FOREACH(const VertexElement &element,
				get_format()->get_vertex_elements(
					get_index()).get_vertex_elements())
			{
				if (element.get_semantic() == semantic)
				{
					format_string % get_format()->get_name()
						% semantic % element.get_type();
					break;
				}
			}

			exception << errinfo_message(format_string.str());

			throw;
		}
	}

	void VertexStream::set(const VertexSemanticType semantic,
		const AlignedVec4Array &data)
	{
		VertexSemanticTypePackDataMap::const_iterator found;
		Uint32 count, offset;
		PackFormatType pack_format;

		found = m_pack_datas.find(semantic);

		if (found == m_pack_datas.end())
		{
			return;
		}

		assert(get_offset() < m_buffer->get_size());

		count = data.size();
		offset = found->second.get_offset() + get_offset();
		pack_format = found->second.get_pack_format();

		try
		{
			PackTool::pack(data, offset, get_stride(), count,
				pack_format, get_use_simd(), *m_buffer);
		}
		catch (boost::exception &exception)
		{
			BoostFormat format_string(UTF8("Range error while "
				"writing to vertex format '%1%' at semantic "
				"%2% of type %3%."));

			BOOST_FOREACH(const VertexElement &element,
				get_format()->get_vertex_elements(
					get_index()).get_vertex_elements())
			{
				if (element.get_semantic() == semantic)
				{
					format_string % get_format()->get_name()
						% semantic % element.get_type();
					break;
				}
			}

			exception << errinfo_message(format_string.str());

			throw;
		}
	}

	const VertexElements &VertexStream::get_vertex_elements() const
	{
		return m_format->get_vertex_elements(get_index());
	}

	Uint16 VertexStream::get_vertex_elements_count() const
	{
		return get_vertex_elements().get_count();
	}

	void VertexStream::push_vertex()
	{
		assert(get_offset() < m_buffer->get_size());
		m_offset += get_stride();
	}

}
