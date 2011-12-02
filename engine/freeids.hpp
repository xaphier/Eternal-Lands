/****************************************************************************
 *            freeids.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef UUID_e45574d7_2427_4327_b6c2_d1b4778579d5
#define	UUID_e45574d7_2427_4327_b6c2_d1b4778579d5

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class FreeIds.
 * This file contains the @c class FreeIds.
 */
namespace eternal_lands
{

	/**
	 * @brief @c class for free ids.
	 *
	 * @c class for free ids.
	 */
	class FreeIds
	{
		private:
			Uint32Set m_free_ids;
			Uint32 m_next_free_id;

		public:
			/**
			 * Default constructor.
			 */
			FreeIds();

			/**
			 * Default destructor.
			 */
			~FreeIds() throw();

			void clear();

			void set_next_free_id(const Uint32 id);

			Uint32 get_next_free_id();

			void free_id(const Uint32 id);

	};

}

#endif	/* UUID_e45574d7_2427_4327_b6c2_d1b4778579d5 */

