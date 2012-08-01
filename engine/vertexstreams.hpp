/****************************************************************************
 *            vertexstreams.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b6f80d31_e6f9_4983_a728_af871374c025
#define	UUID_b6f80d31_e6f9_4983_a728_af871374c025

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "vertexelement.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class VertexStreams.
 * This file contains the @c class VertexStreams.
 */
namespace eternal_lands
{

	class VertexStreams: public boost::noncopyable
	{
		private:
			const VertexFormatSharedPtr m_format;
			VertexStreamVector m_streams;
			const Uint32 m_vertex_count;

		public:
			VertexStreams(const VertexFormatSharedPtr &format,
				const AbstractWriteMemorySharedPtrArray
					&memory, const Uint32 vertex_count,
					const bool use_simd);
			VertexStreams(const VertexFormatSharedPtr &format,
				const AbstractReadWriteMemorySharedPtrVector
					&buffers, const Uint32 vertex_count,
				const bool use_simd);
			~VertexStreams() noexcept;
			void set(const VertexSemanticType semantic,
				const glm::vec4 &data) noexcept;
			void reset() noexcept;
			void push_vertex() noexcept;

			inline const VertexFormatSharedPtr &get_format() const
				noexcept
			{
				return m_format;
			}

			inline const VertexStreamVector &get_streams() const
				noexcept
			{
				return m_streams;
			}

			inline VertexStreamVector &get_streams() noexcept
			{
				return m_streams;
			}

			inline const VertexStream &get_stream(
				const Uint32 index) const
			{
				RANGE_CECK_MAX(index, m_streams.size(),
					UTF8("index value too big"));

				return m_streams[index];
			}

			inline VertexStream &get_streams(const Uint32 index)
			{
				RANGE_CECK_MAX(index, m_streams.size(),
					UTF8("index value too big"));

				return m_streams[index];
			}

			inline Uint32 get_vertex_count() const noexcept
			{
				return m_vertex_count;
			}

	};

}

#endif	/* UUID_b6f80d31_e6f9_4983_a728_af871374c025 */
