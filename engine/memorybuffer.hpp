/****************************************************************************
 *            memorybuffer.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6c2398ea_1b52_4b71_bb17_647ed4d63575
#define	UUID_6c2398ea_1b52_4b71_bb17_647ed4d63575

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractreadmemorybuffer.hpp"
#include "abstractwritememorybuffer.hpp"

/**
 * @file
 * @brief The @c class MemoryBuffer.
 * This file contains the @c class MemoryBuffer.
 */
namespace eternal_lands
{

	class MemoryBuffer: public AbstractReadMemoryBuffer,
		public AbstractWriteMemoryBuffer
	{
		private:
			/**
			 * @brief Pointer to the memory.
			 *
			 * Pointer to the memory. No special alignment is used.
			 */
			Uint8ScopedArray m_ptr;

			/**
			 * @brief The size of the memory.
			 *
			 * The size of the memory in bytes.
			 */
			Uint64 m_size;

		public:
			/**
			 * Default constructor.
			 */
			MemoryBuffer(const Uint64 size = 0);

			/**
			 * Default destructor.
			 */
			virtual ~MemoryBuffer() throw();

			/**
			 * @brief Gets the pointer of the memory.
			 *
			 * Gets the pointer of the memory.
			 * @return Returns the pointer of the memory.
			 */
			void* get_ptr();

			/**
			 * @brief Gets the pointer of the memory.
			 *
			 * Gets the pointer of the memory.
			 * @return Returns the pointer of the memory.
			 */
			const void* get_ptr() const;

			/**
			 * @brief Gets the size of the memory.
			 *
			 * Gets the size of the memory.
			 * @return Returns the size of the memory.
			 */
			Uint64 get_size() const;

			/**
			 * @brief Resizes the memory.
			 *
			 * Resizes the memory.
			 * @param size The new size of the memory.
			 */
			void resize(const Uint64 size);
			void copy(const AbstractReadMemoryBuffer &memory_buffer,
				const Uint64 source_offset = 0,
				const Uint64 dest_offset = 0);
			void copy(const AbstractReadMemoryBuffer &memory_buffer,
				const Uint64 size,
				const Uint64 source_offset = 0,
				const Uint64 dest_offset = 0);

	};

}

#endif	/* UUID_6c2398ea_1b52_4b71_bb17_647ed4d63575 */

