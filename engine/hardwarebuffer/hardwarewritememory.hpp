/****************************************************************************
 *            hardwarewritememory.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c0c47815_78b4_49c8_97b8_dfab11e205ad
#define	UUID_c0c47815_78b4_49c8_97b8_dfab11e205ad

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "readwritememory.hpp"
#include "hardwarebuffer.hpp"

/**
 * @file
 * @brief The @c class HardwareWriteMemory.
 * This file contains the @c class HardwareWriteMemory.
 */
namespace eternal_lands
{

	class HardwareWriteMemory: public AbstractWriteMemory
	{
		private:
			ReadWriteMemory m_memory_buffer;
			const HardwareBufferSharedPtr m_buffer;
			const BufferTargetType m_target;

		public:
			/**
			 * Default constructor.
			 */
			HardwareWriteMemory(
				const HardwareBufferSharedPtr &buffer,
				const BufferTargetType target);

			/**
			 * Default destructor.
			 */
			virtual ~HardwareWriteMemory() noexcept;

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

#endif	/* UUID_c0c47815_78b4_49c8_97b8_dfab11e205ad */
