/***************************************************************************
 *            boundedobject.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a1abf3c2_3378_4c9a_affd_d4ae14a29394
#define	UUID_a1abf3c2_3378_4c9a_affd_d4ae14a29394

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "boundingbox.hpp"

/**
 * @file
 * @brief The @c class BoundedObject.
 * This file contains the @c class BoundedObject.
 */
namespace eternal_lands
{

	/**
	 * @brief .
	 *
	 * .
	 */
	class BoundedObject
	{
		private:
			BoundingBox m_bounding_box;

		protected:
			inline void set_bounding_box(
				const BoundingBox &bounding_box) noexcept
			{
				m_bounding_box = bounding_box;
			}

		public:
			/**
			 * Default constructor.
			 */
			BoundedObject();

			/**
			 * Default destructor.
			 */
			virtual ~BoundedObject() noexcept;

			inline const BoundingBox &get_bounding_box() const
				noexcept
			{
				return m_bounding_box;
			}

	};

}

#endif	/* UUID_a1abf3c2_3378_4c9a_affd_d4ae14a29394 */
