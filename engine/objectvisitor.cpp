/****************************************************************************
 *            objectvisitor.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "objectvisitor.hpp"
#include "object.hpp"
#include "renderobjectdata.hpp"

namespace eternal_lands
{

	namespace
	{

		class ObjectSort
		{
			public:
				inline ObjectSort()
				{
				}

				inline ~ObjectSort() throw()
				{
				}

				inline bool operator()(
					const RenderObjectData &rod0,
					const RenderObjectData &rod1)
				{
					float distance0, distance1;

					distance0 = rod0.get_distance();
					distance1 = rod1.get_distance();

					if (rod0.get_blend())
					{
						if (rod1.get_blend())
						{
							return distance0 >
								distance1;
						}
						else
						{
							return false;
						}
					}
					else
					{
						if (rod1.get_blend())
						{
							return true;
						}
						else
						{
							return distance0 <
								distance1;
						}
					}
				}

		};

	}

	ObjectVisitor::ObjectVisitor()
	{
	}

	ObjectVisitor::~ObjectVisitor() throw()
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

	void ObjectVisitor::add(const ObjectSharedPtr &object,
		const SubFrustumsMask mask)
	{
		m_objects.push_back(RenderObjectData(object, mask));
	}

	void ObjectVisitor::add(const ObjectSharedPtr &object,
		const float transparency, const bool blend,
		const SubFrustumsMask mask)
	{
		m_objects.push_back(RenderObjectData(object,
			transparency, blend, mask));
	}

	void ObjectVisitor::operator()(
		const BoundedObjectSharedPtr &bounded_object,
		const SubFrustumsMask mask)
	{
		ObjectSharedPtr object;

		object = boost::dynamic_pointer_cast<Object>(bounded_object);

		assert(object.get() != 0);

		add(object, mask);
	}

	void ObjectVisitor::next_frame()
	{
		clear();
	}

	void ObjectVisitor::clear()
	{
		m_objects.clear();
	}

}
