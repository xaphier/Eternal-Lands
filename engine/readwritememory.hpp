/****************************************************************************
 *            readwritememory.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6c2398ea_1b52_4b71_bb17_647ed4d63575
#define	UUID_6c2398ea_1b52_4b71_bb17_647ed4d63575

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractreadwritememory.hpp"

/**
 * @file
 * @brief The @c class ReadWriteMemory.
 * This file contains the @c class ReadWriteMemory.
 */
namespace eternal_lands
{

	class ReadWriteMemory: public AbstractReadWriteMemory
	{
		private:
			/**
			 * @brief Pointer to the memory.
			 *
			 * Pointer to the memory. No special alignment is used.
			 */
			Uint8* m_ptr;

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
			ReadWriteMemory(const Uint64 size = 0);

			/**
			 * Default destructor.
			 */
			virtual ~ReadWriteMemory() noexcept;

			/**
			 * @brief Gets the pointer of the memory.
			 *
			 * Gets the pointer of the memory.
			 * @return Returns the pointer of the memory.
			 */
			virtual void* get_ptr() noexcept;

			/**
			 * @brief Gets the pointer of the memory.
			 *
			 * Gets the pointer of the memory.
			 * @return Returns the pointer of the memory.
			 */
			virtual const void* get_ptr() const noexcept;

			/**
			 * @brief Gets the size of the memory.
			 *
			 * Gets the size of the memory.
			 * @return Returns the size of the memory.
			 */
			virtual Uint64 get_size() const noexcept;

			/**
			 * @brief Resizes the memory.
			 *
			 * Resizes the memory.
			 * @param size The new size of the memory.
			 */
			void resize(const Uint64 size);
			void copy(const AbstractReadMemory &memory_buffer,
				const Uint64 source_offset = 0,
				const Uint64 dest_offset = 0);
			void copy(const AbstractReadMemory &memory_buffer,
				const Uint64 size,
				const Uint64 source_offset = 0,
				const Uint64 dest_offset = 0);

	};

}

#endif	/* UUID_6c2398ea_1b52_4b71_bb17_647ed4d63575 */
