/****************************************************************************
 *            vertexbuffers.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			const VertexFormatConstSharedPtr m_format;
			AbstractReadWriteMemorySharedPtrVector m_buffers;

		public:
			VertexBuffers(const VertexFormatConstSharedPtr &format,
				const AbstractReadWriteMemorySharedPtrVector
					&buffers, const Uint32 vertex_count,
				const bool use_simd);
			~VertexBuffers() noexcept;

			inline const AbstractReadWriteMemorySharedPtrVector
				&get_buffers() const noexcept
			{
				return m_buffers;
			}

			inline const AbstractReadWriteMemorySharedPtr
				&get_buffer(const Uint32 index)
			{
				RANGE_CECK_MAX(index, m_buffers.size(),
					UTF8("index value too big"));

				return m_buffers[index];
			}

	};

}

#endif	/* UUID_1b61f044_3a02_4edf_be16_6b5cdb9bd76d */
