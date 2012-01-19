/****************************************************************************
 *            abstractreadwritememorybuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f7b831fb_bde4_414a_9d54_2c06e8048fcb
#define	UUID_f7b831fb_bde4_414a_9d54_2c06e8048fcb

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractreadmemorybuffer.hpp"
#include "abstractwritememorybuffer.hpp"

/**
 * @file
 * @brief The @c class AbstractReadWriteMemoryBuffer.
 * This file contains the @c class AbstractReadWriteMemoryBuffer.
 */
namespace eternal_lands
{

	class AbstractReadWriteMemoryBuffer: public AbstractReadMemoryBuffer,
		public AbstractWriteMemoryBuffer
	{
		protected:
			/**
			 * Default constructor.
			 */
			AbstractReadWriteMemoryBuffer();

		public:
			/**
			 * Default destructor.
			 */
			virtual ~AbstractReadWriteMemoryBuffer() throw();

	};

}

#endif	/* UUID_f7b831fb_bde4_414a_9d54_2c06e8048fcb */
