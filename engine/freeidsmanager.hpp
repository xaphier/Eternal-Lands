/****************************************************************************
 *            freeidsmanager.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef UUID_733b7c13_b16f_4dc6_b58b_e2e7b28738e6
#define	UUID_733b7c13_b16f_4dc6_b58b_e2e7b28738e6

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "freeids.hpp"
#include "exceptions.hpp"

/**
 * @file
 * @brief The @c class FreeIdsManager.
 * This file contains the @c class FreeIdsManager.
 */
namespace eternal_lands
{

	enum IdType
	{
		it_3d_object = 0,
		it_2d_object,
		it_tile_object,
		it_water_object,
		it_dynamic_object,
		it_light_object
	};

	/**
	 * @brief @c class for free ids.
	 *
	 * @c class for free ids.
	 */
	class FreeIdsManager
	{
		private:
			boost::array<FreeIds, 6> m_free_ids;

		public:
			/**
			 * Default constructor.
			 */
			FreeIdsManager();

			/**
			 * Default destructor.
			 */
			~FreeIdsManager() throw();

			void clear();

			void clear(const IdType type);

			Uint32 get_next_free_id(const IdType type);

			void free_id(const Uint32 id);

			Uint32 use_typeless_id(const Uint32 typeless_id,
				const IdType type);

			FreeIds &get_free_ids(const IdType type)
			{
				return m_free_ids[type];
			}

	};

}

#endif	/* UUID_733b7c13_b16f_4dc6_b58b_e2e7b28738e6 */
