/****************************************************************************
 *            hardwarewritememorybuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_c0c47815_78b4_49c8_97b8_dfab11e205ad
#define	UUID_c0c47815_78b4_49c8_97b8_dfab11e205ad

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "memorybuffer.hpp"
#include "hardwarebuffer.hpp"

/**
 * @file
 * @brief The @c class HardwareWriteMemoryBuffer.
 * This file contains the @c class HardwareWriteMemoryBuffer.
 */
namespace eternal_lands
{

	class HardwareWriteMemoryBuffer: public AbstractWriteMemoryBuffer
	{
		private:
			MemoryBuffer m_memory_buffer;
			HardwareBufferSharedPtr m_buffer;
			const HardwareBufferType m_type;

		public:
			/**
			 * Default constructor.
			 */
			HardwareWriteMemoryBuffer(
				const HardwareBufferSharedPtr &buffer,
				const HardwareBufferType type);

			/**
			 * Default destructor.
			 */
			virtual ~HardwareWriteMemoryBuffer() throw();

			/**
			 * @brief Gets the pointer of the memory.
			 *
			 * Gets the pointer of the memory.
			 * @return Returns the pointer of the memory.
			 */
			virtual void* get_ptr();

			/**
			 * @brief Gets the size of the memory.
			 *
			 * Gets the size of the memory.
			 * @return Returns the size of the memory.
			 */
			virtual Uint64 get_size() const;

			inline HardwareBufferType get_type() const
			{
				return m_type;
			}

	};

}

#endif	/* UUID_c0c47815_78b4_49c8_97b8_dfab11e205ad */
