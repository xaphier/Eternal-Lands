/****************************************************************************
 *            decompressutil.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			static ReadWriteMemorySharedPtr decompress_xz(
				const AbstractReadMemorySharedPtr &buffer);
			static ReadWriteMemorySharedPtr decompress_gz(
				const AbstractReadMemorySharedPtr &buffer);
			static AbstractReadMemorySharedPtr decompress(
				const AbstractReadMemorySharedPtr &buffer);

	};

}

#endif	/* UUID_475d2a5a_b423_41f2_a6a8_39cb442293b5 */
