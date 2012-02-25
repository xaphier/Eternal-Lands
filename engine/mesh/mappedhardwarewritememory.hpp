/****************************************************************************
 *            mappedhardwarewritememory.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_6d5a0d7a_874e_4cea_b2aa_9c2dcc7ef07f
#define	UUID_6d5a0d7a_874e_4cea_b2aa_9c2dcc7ef07f

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractwritememory.hpp"
#include "hardwarebuffer.hpp"

/**
 * @file
 * @brief The @c class HardwareWriteMemory.
 * This file contains the @c class HardwareWriteMemory.
 */
namespace eternal_lands
{

	class MappedHardwareWriteMemory: public AbstractWriteMemory
	{
		private:
			HardwareBufferSharedPtr m_buffer;
			void* m_ptr;
			const HardwareBufferType m_type;

		public:
			/**
			 * Default constructor.
			 */
			MappedHardwareWriteMemory(
				const HardwareBufferSharedPtr &buffer,
				const HardwareBufferType type);

			/**
			 * Default destructor.
			 */
			virtual ~MappedHardwareWriteMemory() throw();

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

#endif	/* UUID_6d5a0d7a_874e_4cea_b2aa_9c2dcc7ef07f */
