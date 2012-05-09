/****************************************************************************
 *            abstractnodevisitor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5997c46d_cd63_415c_ab83_1670ae547005
#define	UUID_5997c46d_cd63_415c_ab83_1670ae547005

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"

/**
 * @file
 * @brief The @c class AbstractNodeVisitor.
 * This file contains the @c class AbstractNodeVisitor.
 */
namespace eternal_lands
{

	/**
	 * @brief abstract visitor for RStarTreeNodes.
	 *
	 */
	class AbstractNodeVisitor: public boost::noncopyable
	{
		protected:
			AbstractNodeVisitor();

		public:
			virtual ~AbstractNodeVisitor() noexcept;
			virtual bool operator()(
				const BoundedObjectSharedPtr &node) = 0;

	};
	/**
	 * @}
	 */

}
/**
 * @}
 */

#endif	/* UUID_5997c46d_cd63_415c_ab83_1670ae547005 */
