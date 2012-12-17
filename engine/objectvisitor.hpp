/****************************************************************************
 *            objectvisitor.hpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
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
			glm::mat4x4 m_projection_view_matrix;
			CpuRasterizerSharedPtr m_cpu_rasterizer;

			void clear();
			BitSet64 get_visibility_mask(
				const ObjectSharedPtr &object) const;

		public:
			ObjectVisitor();
			virtual ~ObjectVisitor() noexcept;
			virtual void operator()(
				const BoundedObjectSharedPtr &bounded_object);
			void sort(const glm::vec3 &position);
			void add(const ObjectSharedPtr &object);
			void add(const ObjectSharedPtr &object,
				const BitSet64 blend_mask,
				const float transparency, const BlendType blend,
				const bool depth_read = true);
			void next_frame();

			inline void set_projection_view_matrix(
				const glm::mat4x4 &projection_view_matrix)
				noexcept
			{
				m_projection_view_matrix =
					projection_view_matrix;
			}

			inline void set_cpu_rasterizer(
				const CpuRasterizerSharedPtr &cpu_rasterizer)
				noexcept
			{
				m_cpu_rasterizer = cpu_rasterizer;
			}

			inline const glm::mat4x4 &get_projection_view_matrix()
				const noexcept
			{
				return m_projection_view_matrix;
			}

			inline const CpuRasterizerSharedPtr
				&get_cpu_rasterizer() const noexcept
			{
				return m_cpu_rasterizer;
			}

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
