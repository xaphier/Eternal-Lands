/****************************************************************************
 *            lightvisitor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#ifndef	UUID_5aeeb371_f944_4460_a5a1_f42c07e8ffd0
#define	UUID_5aeeb371_f944_4460_a5a1_f42c07e8ffd0

#ifndef	__cplusplus
#error	"Including C++ header in C translation unit!"
#endif	/* __cplusplus */

#include "prerequisites.hpp"
#include "abstractboundedobjectvisitor.hpp"

/**
 * @file
 * @brief The @c class RStarTreeLightVisitor.
 * This file contains the @c class RStarTreeLightVisitor.
 */
namespace eternal_lands
{

	/**
	 * @brief visitor for objects of RStarTree.
	 *
	 */
	class LightVisitor: public AbstractBoundedObjectVisitor
	{
		private:
			RenderLightDataVector m_lights;

		public:
			LightVisitor();
			virtual ~LightVisitor() noexcept;
			virtual void operator()(
				const BoundedObjectSharedPtr &bounded_object);
			void sort(const glm::vec3 &position);
			void update_camera(const glm::vec3 &camera);

			inline RenderLightDataVector &get_lights() noexcept
			{
				return m_lights;
			}

			inline const RenderLightDataVector &get_lights() const
				noexcept
			{
				return m_lights;
			}

	};
	/**
	 * @}
	 */

}
/**
 * @}
 */

#endif	/* UUID_5aeeb371_f944_4460_a5a1_f42c07e8ffd0 */
