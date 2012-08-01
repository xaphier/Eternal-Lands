/****************************************************************************
 *            lz4.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a2c67a90_f4dd_41fc_a048_468ef8a80d43
#define	UUID_a2c67a90_f4dd_41fc_a048_468ef8a80d43

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 */
namespace eternal_lands
{

	class Lz4
	{
		public:
			static ReadWriteMemorySharedPtr decompress(
				const AbstractReadMemorySharedPtr &buffer,
				const Uint32 uncompressed_size);
			static void decompress(
				const AbstractReadMemorySharedPtr &in_buffer,
				const AbstractWriteMemorySharedPtr &out_buffer);
			static ReadWriteMemorySharedPtr compress(
				const AbstractReadMemorySharedPtr &buffer);
			static ReadWriteMemorySharedPtr compress_high(
				const AbstractReadMemorySharedPtr &buffer);

	};

}

#endif	/* UUID_a2c67a90_f4dd_41fc_a048_468ef8a80d43 */
