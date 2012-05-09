/****************************************************************************
 *            abstractboundedobjectvisitor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_a4596af8_e295_4247_bad1_2b7c43c009f0
#define	UUID_a4596af8_e295_4247_bad1_2b7c43c009f0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class AbstractBoundedObjectVisitor.
 * This file contains the @c class AbstractBoundedObjectVisitor.
 */
namespace eternal_lands
{

	/**
	 * @brief abstract visitor for RStarTree.
	 *
	 */
	class AbstractBoundedObjectVisitor: public boost::noncopyable
	{
		protected:
			AbstractBoundedObjectVisitor();

		public:
			virtual ~AbstractBoundedObjectVisitor() noexcept;

			virtual void operator()(
				const BoundedObjectSharedPtr &bounded_object,
				const SubFrustumsMask mask) = 0;

	};
	/**
	 * @}
	 */

}
/**
 * @}
 */

#endif	/* UUID_a4596af8_e295_4247_bad1_2b7c43c009f0 */
