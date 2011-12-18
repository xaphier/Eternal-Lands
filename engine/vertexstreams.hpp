/****************************************************************************
 *            vertexstreams.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_b6f80d31_e6f9_4983_a728_af871374c025
#define	UUID_b6f80d31_e6f9_4983_a728_af871374c025

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

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
				const AbstractWriteMemoryBufferSharedPtrArray
					&memory, const Uint32 vertex_count);
			~VertexStreams() throw();

			inline const VertexFormatSharedPtr &get_format() const
			{
				return m_format;
			}

			inline const VertexStreamVector &get_streams() const
			{
				return m_streams;
			}

			inline VertexStreamVector &get_streams()
			{
				return m_streams;
			}

			inline Uint32 get_vertex_count() const
			{
				return m_vertex_count;
			}

	};

}

#endif	/* UUID_b6f80d31_e6f9_4983_a728_af871374c025 */
