/****************************************************************************
 *            abstractreadmemory.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8a38dde6_edb3_4e7e_95bb_1dba5680411a
#define	UUID_8a38dde6_edb3_4e7e_95bb_1dba5680411a

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class AbstractReadMemory.
 * This file contains the @c class AbstractReadMemory.
 */
namespace eternal_lands
{

	class AbstractReadMemory: public boost::noncopyable
	{
		protected:
			/**
			 * Default constructor.
			 */
			AbstractReadMemory();

		public:
			/**
			 * Default destructor.
			 */
			virtual ~AbstractReadMemory() throw();

			/**
			 * @brief Gets the pointer of the memory.
			 *
			 * Gets the pointer of the memory.
			 * @return Returns the pointer of the memory.
			 */
			virtual const void* get_ptr() const = 0;

			/**
			 * @brief Gets the size of the memory.
			 *
			 * Gets the size of the memory.
			 * @return Returns the size of the memory.
			 */
			virtual Uint64 get_size() const = 0;

	};

}

#endif	/* UUID_8a38dde6_edb3_4e7e_95bb_1dba5680411a */
