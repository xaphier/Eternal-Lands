/****************************************************************************
 *            indexupdatesource.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_e684829c_baaf_441a_9625_50420997442e
#define	UUID_e684829c_baaf_441a_9625_50420997442e

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "readwritememory.hpp"
#include "primitiveutil.hpp"

/**
 * @file
 * @brief The @c class IndexUpdateSource.
 * This file contains the @c class IndexUpdateSource.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for index update source.
	 *
	 * @c class for index update source.
	 */
	class IndexUpdateSource
	{
		private:
			ReadWriteMemory m_indices;
			const SubMeshVector m_sub_meshs;
			const bool m_use_16_bit_indices;
			const bool m_use_restart_index;

		public:
			/**
			 * @brief Default constructor.
			 * Default constructor initializes the index buffer.
			 * @param index_blocks The blocks of indices.
			 * @param use_16_bit_index Use 16 bit index.
			 * @param use_restart_index Use restart index.
			 */
			IndexUpdateSource(const Uint32Vector &indices,
				const SubMeshVector &sub_meshs,
				const bool use_16_bit_indices,
				const bool use_restart_index);

			/**
			 * Default destructor.
			 */
			~IndexUpdateSource() throw();

			/**
			 * @param index_blocks The blocks of indices.
			 */
			Uint32 get_count(const Uint32Set &blocks) const;

			bool get_use_16_bit_indices() const;

			bool get_use_restart_index() const;

			SubMeshVector get_sub_meshs(const Uint32Set &blocks)
				const;

			/**
			 * Write the data to the index buffer.
			 * @param index_blocks The blocks of indices.
			 * @param index_stream The index buffer to use.
			 */
			void write_index_buffer(
				const Uint32Set &blocks,
				const AbstractWriteMemorySharedPtr &buffer)
				const;

	};

}

#endif	/* UUID_e684829c_baaf_441a_9625_50420997442e */

