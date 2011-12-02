/****************************************************************************
 *            abstractrstartreevisitor.hpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
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
 * @brief The @c class AbstractRStarTreeVisitor.
 * This file contains the @c class AbstractRStarTreeVisitor.
 */
namespace eternal_lands
{

	/**
	 * @brief abstract visitor for RStarTree.
	 *
	 */
	class AbstractRStarTreeVisitor: public boost::noncopyable
	{
		protected:
			AbstractRStarTreeVisitor();

		public:
			virtual ~AbstractRStarTreeVisitor() throw();

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

