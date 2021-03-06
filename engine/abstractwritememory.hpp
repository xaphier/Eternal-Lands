/****************************************************************************
 *            abstractwritememory.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_24e315c8_034b_49d7_8199_45c39961e71f
#define	UUID_24e315c8_034b_49d7_8199_45c39961e71f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class AbstractWriteMemory.
 * This file contains the @c class AbstractWriteMemory.
 */
namespace eternal_lands
{

	class AbstractWriteMemory: public boost::noncopyable
	{
		protected:
			/**
			 * Default constructor.
			 */
			AbstractWriteMemory();

		public:
			/**
			 * Default destructor.
			 */
			virtual ~AbstractWriteMemory() noexcept;

			/**
			 * @brief Gets the pointer of the memory.
			 *
			 * Gets the pointer of the memory.
			 * @return Returns the pointer of the memory.
			 */
			virtual void* get_ptr() noexcept = 0;

			/**
			 * @brief Gets the size of the memory.
			 *
			 * Gets the size of the memory.
			 * @return Returns the size of the memory.
			 */
			virtual Uint64 get_size() const noexcept = 0;

	};

}

#endif	/* UUID_24e315c8_034b_49d7_8199_45c39961e71f */
