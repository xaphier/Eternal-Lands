/****************************************************************************
 *            vertexstream.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_cbe01508_0b4c_4d0b_9d57_89b5edacb9a5
#define	UUID_cbe01508_0b4c_4d0b_9d57_89b5edacb9a5

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "vertexelements.hpp"

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

			const VertexElements m_vertex_elements;
			const AbstractWriteMemorySharedPtr m_buffer;
			VertexSemanticTypePackDataMap m_pack_datas;
			Uint64 m_offset;
			Uint32 m_stride;
			const Uint32 m_vertex_count;
			const bool m_use_simd;

			void init();

		public:
			VertexStream(const VertexFormatConstSharedPtr &format,
				const AbstractWriteMemorySharedPtr &buffer,
				const Uint32 vertex_count, const Uint16 index,
				const bool use_simd);
			VertexStream(const VertexElements &vertex_elements,
				const AbstractWriteMemorySharedPtr &buffer,
				const Uint32 vertex_count, const bool use_simd);
			~VertexStream() noexcept;
			void set(const VertexSemanticType semantic,
				const glm::vec4 &data);
			void set(const VertexSemanticType semantic,
				const Vec4Vector &data);
			void set(const VertexSemanticType semantic,
				const AlignedVec4Array &data);
			void push_vertex();

			inline Uint16 get_vertex_elements_count() const
			{
				return get_vertex_elements().get_count();
			}

			inline void reset() noexcept
			{
				m_offset = 0;
			}

			inline const VertexElements &get_vertex_elements() const
				noexcept
			{
				return m_vertex_elements;
			}

			inline const AbstractWriteMemorySharedPtr &get_buffer()
				const noexcept
			{
				return m_buffer;
			}

			inline Uint64 get_offset() const noexcept
			{
				return m_offset;
			}

			inline Uint32 get_stride() const noexcept
			{
				return m_stride;
			}

			inline Uint32 get_vertex_count() const noexcept
			{
				return m_vertex_count;
			}

			inline bool get_use_simd() const noexcept
			{
				return m_use_simd;
			}

	};

}

#endif	/* UUID_cbe01508_0b4c_4d0b_9d57_89b5edacb9a5 */
