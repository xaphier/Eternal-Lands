/****************************************************************************
 *            decompressutil.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_475d2a5a_b423_41f2_a6a8_39cb442293b5
#define	UUID_475d2a5a_b423_41f2_a6a8_39cb442293b5

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 */
namespace eternal_lands
{

	class DecompresszUtil
	{
		public:
			static MemoryBufferSharedPtr decompress_xz(
				const AbstractReadMemoryBufferSharedPtr
					&buffer);
			static MemoryBufferSharedPtr decompress_gz(
				const AbstractReadMemoryBufferSharedPtr
					&buffer);
			static AbstractReadMemoryBufferSharedPtr decompress(
				const AbstractReadMemoryBufferSharedPtr
					&buffer);

	};

}

#endif	/* UUID_475d2a5a_b423_41f2_a6a8_39cb442293b5 */
