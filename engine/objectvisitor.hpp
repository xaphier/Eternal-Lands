/****************************************************************************
 *            objectvisitor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_50847fdd_15be_4ede_bc0c_b8aacf5b5a50
#define	UUID_50847fdd_15be_4ede_bc0c_b8aacf5b5a50

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractboundedobjectvisitor.hpp"
#include "blendutil.hpp"

/**
 * @file
 * @brief The @c class ObjectVisitor.
 * This file contains the @c class ObjectVisitor.
 */
namespace eternal_lands
{

	/**
	 * @brief visitor for objects.
	 *
	 */
	class ObjectVisitor: public AbstractBoundedObjectVisitor
	{
		private:
			RenderObjectDataVector m_objects;

			void clear() noexcept;

		public:
			ObjectVisitor();
			virtual ~ObjectVisitor() noexcept;
			virtual void operator()(
				const BoundedObjectSharedPtr &bounded_object,
				const SubFrustumsMask mask);
			void sort(const glm::vec3 &position);
			void add(const ObjectSharedPtr &object,
				const SubFrustumsMask mask);
			void add(const ObjectSharedPtr &object,
				const float transparency, const BlendType blend,
				const SubFrustumsMask mask);
			void next_frame() noexcept;

			inline RenderObjectDataVector &get_objects() noexcept
			{
				return m_objects;
			}

			inline const RenderObjectDataVector &get_objects() const
				noexcept
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
