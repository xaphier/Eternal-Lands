/****************************************************************************
 *            vertexbuffers.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_1b61f044_3a02_4edf_be16_6b5cdb9bd76d
#define	UUID_1b61f044_3a02_4edf_be16_6b5cdb9bd76d

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "vertexstreams.hpp"

/**
 * @file
 * @brief The @c class VertexBuffers.
 * This file contains the @c class VertexBuffers.
 */
namespace eternal_lands
{

	class VertexBuffers: public VertexStreams
	{
		private:
			const VertexFormatSharedPtr m_format;
			MemoryBufferSharedPtrVector m_buffers;

		public:
			VertexBuffers(const VertexFormatSharedPtr &format,
				const MemoryBufferSharedPtrVector &buffers,
				const Uint32 vertex_count);
			~VertexBuffers() throw();

			inline const MemoryBufferSharedPtrVector &get_buffers()
				const
			{
				return m_buffers;
			}

			inline const MemoryBufferSharedPtr &get_buffer(
				const Uint32 index) const
			{
				assert(index < m_buffers.size());

				return m_buffers[index];
			}

	};

}

#endif	/* UUID_1b61f044_3a02_4edf_be16_6b5cdb9bd76d */
