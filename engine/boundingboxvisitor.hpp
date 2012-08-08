/****************************************************************************
 *            boundingboxvisitor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_8ca56548_6c87_45c6_98fc_90d8d6d58c89
#define	UUID_8ca56548_6c87_45c6_98fc_90d8d6d58c89

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractboundedobjectvisitor.hpp"
#include "boundingbox.hpp"

/**
 * @file
 * @brief The @c class ObjectVisitor.
 * This file contains the @c class ObjectVisitor.
 */
namespace eternal_lands
{

	/**
	 * @brief visitor for bounding boxes.
	 *
	 */
	class BoundingBoxVisitor: public AbstractBoundedObjectVisitor
	{
		private:
			BoundingBox m_bounding_box;

		public:
			BoundingBoxVisitor();
			virtual ~BoundingBoxVisitor() noexcept;
			virtual void operator()(const Frustum &frustum,
				const BoundedObjectSharedPtr &bounded_object,
				const SubFrustumsMask mask);

			inline void clear()
			{
				m_bounding_box = BoundingBox();
			}

			inline const BoundingBox &get_bounding_box() const
				noexcept
			{
				return m_bounding_box;
			}

	};
	/**
	 * @}
	 */

}
/**
 * @}
 */

#endif	/* UUID_8ca56548_6c87_45c6_98fc_90d8d6d58c89 */
