/****************************************************************************
 *            opengl3mappedhardwarewritememory.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_ed536052_3d29_4289_965c_4bc3cb99bb25
#define	UUID_ed536052_3d29_4289_965c_4bc3cb99bb25

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractwritememory.hpp"
#include "hardwarebuffer.hpp"

/**
 * @file
 * @brief The @c class Opengl3HardwareWriteMemory.
 * This file contains the @c class Opengl3HardwareWriteMemory.
 */
namespace eternal_lands
{

	class Opengl3HardwareWriteMemory: public AbstractWriteMemory
	{
		private:
			const HardwareBufferSharedPtr m_buffer;
			void* m_ptr;
			const BufferTargetType m_target;

		public:
			/**
			 * Default constructor.
			 */
			Opengl3HardwareWriteMemory(
				const HardwareBufferSharedPtr &buffer,
				const BufferTargetType target);

			/**
			 * Default destructor.
			 */
			virtual ~Opengl3HardwareWriteMemory() noexcept;

			/**
			 * @brief Gets the pointer of the memory.
			 *
			 * Gets the pointer of the memory.
			 * @return Returns the pointer of the memory.
			 */
			virtual void* get_ptr() noexcept;

			/**
			 * @brief Gets the size of the memory.
			 *
			 * Gets the size of the memory.
			 * @return Returns the size of the memory.
			 */
			virtual Uint64 get_size() const noexcept;

	};

}

#endif	/* UUID_ed536052_3d29_4289_965c_4bc3cb99bb25 */
