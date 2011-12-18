/****************************************************************************
 *            vertexstream.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_cbe01508_0b4c_4d0b_9d57_89b5edacb9a5
#define	UUID_cbe01508_0b4c_4d0b_9d57_89b5edacb9a5

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "vertexelement.hpp"

/**
 * @file
 * @brief The @c class VertexStream.
 * This file contains the @c class VertexStream.
 */
namespace eternal_lands
{

	class VertexStream: public boost::noncopyable
	{
		private:
			class PackData;

			typedef std::map<VertexSemanticType, PackData>
				VertexSemanticTypePackDataMap;

			const VertexFormatSharedPtr m_format;
			const AbstractWriteMemoryBufferSharedPtr m_buffer;
			VertexSemanticTypePackDataMap m_pack_datas;
			Uint64 m_offset;
			Uint32 m_stride;
			const Uint16 m_index;

		public:
			VertexStream(const VertexFormatSharedPtr &format,
				const AbstractWriteMemoryBufferSharedPtr
					&buffer, const Uint16 index);
			~VertexStream() throw();
			void set(const VertexSemanticType semantic,
				const glm::vec4 &data);
			void set(const VertexSemanticType semantic,
				const Vec4Vector &data);
			void set(const VertexSemanticType semantic,
				const AlignedVec4Array &data);
			const VertexElements &get_vertex_elements() const;
			Uint16 get_vertex_elements_count() const;

			inline void reset()
			{
				m_offset = 0;
			}

			inline void push_vertex()
			{
				m_offset += get_stride();
			}

			inline VertexFormatSharedPtr get_format() const
			{
				return m_format;
			}

			inline Uint64 get_offset() const
			{
				return m_offset;
			}

			inline Uint32 get_stride() const
			{
				return m_stride;
			}

			inline Uint16 get_index() const
			{
				return m_index;
			}

	};

}

#endif	/* UUID_cbe01508_0b4c_4d0b_9d57_89b5edacb9a5 */
