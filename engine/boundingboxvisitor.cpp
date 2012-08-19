/****************************************************************************
 *            boundingboxvisitor.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "boundingboxvisitor.hpp"
#include "boundedobject.hpp"

namespace eternal_lands
{

	BoundingBoxVisitor::BoundingBoxVisitor()
	{
	}

	BoundingBoxVisitor::~BoundingBoxVisitor() noexcept
	{
	}

	void BoundingBoxVisitor::operator()(
		const BoundedObjectSharedPtr &bounded_object)
	{
		m_bounding_box.merge(bounded_object->get_bounding_box());
	}

}
