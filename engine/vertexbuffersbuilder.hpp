/****************************************************************************
 *            vertexbuffersbuilder.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_55464512_eb38_47cf_aec3_a75fec9fae72
#define	UUID_55464512_eb38_47cf_aec3_a75fec9fae72

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class MeshCache.
 * This file contains the @c class MeshCache.
 */
namespace eternal_lands
{


	class VertexBuffersBuilder: public boost::noncopyable
	{
		public:
			static VertexBuffersSharedPtr get_vertex_buffers(
				const VertexFormatSharedPtr &vertex_format,
				const Uint32 vertex_count, const bool use_simd);

	};

}

#endif	/* UUID_55464512_eb38_47cf_aec3_a75fec9fae72 */
