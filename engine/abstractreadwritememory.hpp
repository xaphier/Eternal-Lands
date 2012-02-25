/****************************************************************************
 *            abstractreadwritememory.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_f7b831fb_bde4_414a_9d54_2c06e8048fcb
#define	UUID_f7b831fb_bde4_414a_9d54_2c06e8048fcb

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractreadmemory.hpp"
#include "abstractwritememory.hpp"

/**
 * @file
 * @brief The @c class AbstractReadWriteMemory.
 * This file contains the @c class AbstractReadWriteMemory.
 */
namespace eternal_lands
{

	class AbstractReadWriteMemory: public AbstractReadMemory,
		public AbstractWriteMemory
	{
		protected:
			/**
			 * Default constructor.
			 */
			AbstractReadWriteMemory();

		public:
			/**
			 * Default destructor.
			 */
			virtual ~AbstractReadWriteMemory() throw();

	};

}

#endif	/* UUID_f7b831fb_bde4_414a_9d54_2c06e8048fcb */
