/****************************************************************************
 *            freeids.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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
			Uint32Set m_free_typeless_ids;
			Uint32 m_next_free_typeless_id;
			Uint16 m_type;

			static const Uint32 m_typeless_id_mask = 0x00FFFFFF;
			static const Uint32 m_type_shift = 24;

		public:
			/**
			 * Default constructor.
			 */
			FreeIds(const Uint16 type = 0);

			/**
			 * Default destructor.
			 */
			~FreeIds() throw();

			void clear();

			Uint32 get_next_free_id();

			void free_id(const Uint32 id);

			void free_typeless_id(const Uint32 typeless_id);

			void use_id(const Uint32 id);

			inline Uint32 get_id(const Uint32 typeless_id) const
			{
				return (typeless_id & m_typeless_id_mask) |
					(static_cast<Uint32>(m_type) <<
						m_type_shift);
			}

			Uint32 use_typeless_id(const Uint32 typeless_id)
			{
				Uint32 id;

				id = get_id(typeless_id);

				use_id(id);

				return id;
			}

			static inline Uint16 get_type_value(const Uint32 id)
			{
				return id >> m_type_shift;
			}

			static inline Uint32 get_typeless_id(const Uint32 id)
			{
				return id & m_typeless_id_mask;
			}

	};

}

#endif	/* UUID_e45574d7_2427_4327_b6c2_d1b4778579d5 */
