/****************************************************************************
 *            rstartreeobjectvisitor.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_50847fdd_15be_4ede_bc0c_b8aacf5b5a50
#define	UUID_50847fdd_15be_4ede_bc0c_b8aacf5b5a50

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractrstartreevisitor.hpp"
#include "boundingbox.hpp"

/**
 * @file
 * @brief The @c class RStarTreeObjectVisitor.
 * This file contains the @c class RStarTreeObjectVisitor.
 */
namespace eternal_lands
{

	/**
	 * @brief visitor for objects of RStarTree.
	 *
	 */
	class RStarTreeObjectVisitor: public AbstractRStarTreeVisitor
	{
		private:
			RenderObjectDataVector m_objects;

			void clear();

		public:
			RStarTreeObjectVisitor();
			virtual ~RStarTreeObjectVisitor() throw();
			virtual void operator()(
				const BoundedObjectSharedPtr &bounded_object,
				const SubFrustumsMask mask);
			void sort(const glm::vec3 &position);
			void add(const ObjectSharedPtr &object,
				const SubFrustumsMask mask);
			void add(const ObjectSharedPtr &object,
				const float transparency, const bool blend,
				const SubFrustumsMask mask);
			void next_frame();

			inline const RenderObjectDataVector &get_objects() const
			{
				return m_objects;
			}

	};
	/**
	 * @}
	 */

}
/**
 * @}
 */

#endif	/* UUID_50847fdd_15be_4ede_bc0c_b8aacf5b5a50 */