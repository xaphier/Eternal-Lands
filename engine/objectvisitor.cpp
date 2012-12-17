/****************************************************************************
 *            objectvisitor.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "objectvisitor.hpp"
#include "object.hpp"
#include "renderobjectdata.hpp"
#include "cpurasterizer.hpp"
#include "abstractmesh.hpp"
#include "submesh.hpp"

namespace eternal_lands
{

	namespace
	{

		class ObjectSort
		{
			public:
				ObjectSort();
				~ObjectSort() noexcept;
				bool operator()(const RenderObjectData &rod0,
					const RenderObjectData &rod1) const
					noexcept;

		};

		ObjectSort::ObjectSort()
		{
		}

		ObjectSort::~ObjectSort() noexcept
		{
		}

		bool ObjectSort::operator()(
			const RenderObjectData &rod0,
			const RenderObjectData &rod1) const noexcept
		{
			float distance0, distance1;

			distance0 = rod0.get_distance();
			distance1 = rod1.get_distance();

			if (rod0.get_blend_mask().any())
			{
				if (!rod1.get_blend_mask().any())
				{
					return false;
				}

				return distance0 > distance1;
			}

			if (rod1.get_blend_mask().any())
			{
				return true;
			}

			if ((rod0.get_object()->get_materials().size() == 0) ||
				(rod1.get_object()->get_materials().size()
					== 0))
			{
				return distance0 < distance1;
			}

			if (rod0.get_object()->get_materials()[0] !=
				rod1.get_object()->get_materials()[0])
			{
				return rod0.get_object()->get_materials()[0] <
					rod1.get_object()->get_materials()[0];
			}

			return distance0 < distance1;
		}

	}

	ObjectVisitor::ObjectVisitor()
	{
	}

	ObjectVisitor::~ObjectVisitor() noexcept
	{
	}

	void ObjectVisitor::sort(const glm::vec3 &position)
	{
		float distance;

		BOOST_FOREACH(RenderObjectData &rod, m_objects)
		{
			distance = rod.get_object()->get_bounding_box(
				).distance(position);
			rod.set_distance(distance);
		}

		std::sort(m_objects.begin(), m_objects.end(),
			ObjectSort());
	}

	void ObjectVisitor::add(const ObjectSharedPtr &object)
	{
		BitSet64 visibility_mask;

		visibility_mask = get_visibility_mask(object);

		if (visibility_mask.any())
		{
			m_objects.push_back(RenderObjectData(object,
				visibility_mask));
		}
	}

	void ObjectVisitor::add(const ObjectSharedPtr &object,
		const BitSet64 blend_mask, const float transparency,
		const BlendType blend, const bool depth_read)
	{
		BitSet64 visibility_mask;

		visibility_mask = get_visibility_mask(object);

		if (visibility_mask.any())
		{
			m_objects.push_back(RenderObjectData(object,
				visibility_mask, blend_mask, transparency,
				blend, depth_read));
		}
	}

	void ObjectVisitor::operator()(
		const BoundedObjectSharedPtr &bounded_object)
	{
		ObjectSharedPtr object;

		object = boost::dynamic_pointer_cast<Object>(bounded_object);

		assert(object.get() != nullptr);

		add(object);
	}

	void ObjectVisitor::next_frame()
	{
		clear();
	}

	void ObjectVisitor::clear()
	{
		m_objects.clear();
	}

	BitSet64 ObjectVisitor::get_visibility_mask(
		const ObjectSharedPtr &object) const
	{
		if (get_cpu_rasterizer().get() == nullptr)
		{
			return all_bits_set;
		}

		return get_cpu_rasterizer()->check_visibility(
			get_projection_view_matrix(),
			object->get_world_transformation().get_matrix(),
			object->get_mesh()->get_min_max_boxes(),
			object->get_mesh()->get_sub_meshs());
	}

}
