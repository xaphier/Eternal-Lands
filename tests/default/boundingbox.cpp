/****************************************************************************
 *            boundingbox.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "boundingbox.hpp"
#define BOOST_TEST_MODULE bounding_box
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::BoundingBox box(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	BOOST_CHECK_CLOSE(box.get_min()[0], 23.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_min()[1], -1234.56f, 0.001);
	BOOST_CHECK_CLOSE(box.get_min()[2], -34.0f, 0.001);

	BOOST_CHECK_CLOSE(box.get_max()[0], 34.5f, 0.001);
	BOOST_CHECK_CLOSE(box.get_max()[1], 214.012f, 0.001);
	BOOST_CHECK_CLOSE(box.get_max()[2], 0.234f, 0.001);
}

BOOST_AUTO_TEST_CASE(min_max)
{
	el::BoundingBox box(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box.set_min_max(glm::vec3(1.0f, 2.0f, 3.0f),
		glm::vec3(4.0f, 5.0f, 6.0f));

	BOOST_CHECK_CLOSE(box.get_min()[0], 1.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_min()[1], 2.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_min()[2], 3.0f, 0.001);

	BOOST_CHECK_CLOSE(box.get_max()[0], 4.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_max()[1], 5.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_max()[2], 6.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(half_size)
{
	el::BoundingBox box(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box.set_half_size(glm::vec3(1.0f, 2.0f, 3.0f));

	BOOST_CHECK_CLOSE(box.get_half_size()[0], 1.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_half_size()[1], 2.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_half_size()[2], 3.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(center)
{
	el::BoundingBox box(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box.set_center(glm::vec3(1.0f, 2.0f, 3.0f));

	BOOST_CHECK_CLOSE(box.get_center()[0], 1.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_center()[1], 2.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_center()[2], 3.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(volume)
{
	el::BoundingBox box(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box.set_half_size(glm::vec3(1.0f, 2.0f, 3.0f));

	BOOST_CHECK_CLOSE(box.get_volume(), 48.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(margin)
{
	el::BoundingBox box(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box.set_half_size(glm::vec3(1.0f, 2.0f, 3.0f));

	BOOST_CHECK_CLOSE(box.get_margin(), 12.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(scale)
{
	el::BoundingBox box;

	box.set_center(glm::vec3(23.0f, -1234.56f, -34.0f));
	box.set_half_size(glm::vec3(34.5f, 214.012f, 0.234f));

	box.scale(2.0f);

	BOOST_CHECK_CLOSE(box.get_center()[0], 23.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_center()[1], -1234.56f, 0.001);
	BOOST_CHECK_CLOSE(box.get_center()[2], -34.0f, 0.001);

	BOOST_CHECK_CLOSE(box.get_half_size()[0], 69.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_half_size()[1], 428.024f, 0.001);
	BOOST_CHECK_CLOSE(box.get_half_size()[2], 0.468f, 0.001);
}

BOOST_AUTO_TEST_CASE(extend)
{
	el::BoundingBox box;

	box.set_center(glm::vec3(23.0f, -1234.56f, -34.0f));
	box.set_half_size(glm::vec3(34.5f, 214.012f, 0.234f));

	box.extend(2.0f);

	BOOST_CHECK_CLOSE(box.get_center()[0], 23.0f, 0.001);
	BOOST_CHECK_CLOSE(box.get_center()[1], -1234.56f, 0.001);
	BOOST_CHECK_CLOSE(box.get_center()[2], -34.0f, 0.001);

	BOOST_CHECK_CLOSE(box.get_half_size()[0], 36.5f, 0.001);
	BOOST_CHECK_CLOSE(box.get_half_size()[1], 216.012f, 0.001);
	BOOST_CHECK_CLOSE(box.get_half_size()[2], 2.234f, 0.001);
}

BOOST_AUTO_TEST_CASE(merge)
{
	el::BoundingBox box0, box1(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box0.set_center(glm::vec3(23.0f, -1234.56f, -34.0f));
	box0.set_half_size(glm::vec3(34.5f, 214.012f, 0.234f));

	box0.merge(box1);

	BOOST_CHECK_CLOSE(box0.get_center()[0], 23.0f, 0.001);
	BOOST_CHECK_CLOSE(box0.get_center()[1], -617.28f, 0.001);
	BOOST_CHECK_CLOSE(box0.get_center()[2], -17.0f, 0.001);

	BOOST_CHECK_CLOSE(box0.get_half_size()[0], 34.5f, 0.001);
	BOOST_CHECK_CLOSE(box0.get_half_size()[1], 831.292f, 0.001);
	BOOST_CHECK_CLOSE(box0.get_half_size()[2], 17.234f, 0.001);
}

BOOST_AUTO_TEST_CASE(clamp)
{
	el::BoundingBox box0, box1(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box0.set_center(glm::vec3(23.0f, -1234.56f, -34.0f));
	box0.set_half_size(glm::vec3(34.5f, 214.012f, 0.234f));

	box0.clamp(box1);

	BOOST_CHECK_CLOSE(box0.get_center()[0], 28.75f, 0.001);
	BOOST_CHECK_CLOSE(box0.get_center()[1], -1127.554f, 0.001);
	BOOST_CHECK_CLOSE(box0.get_center()[2], -33.883f, 0.001);

	BOOST_CHECK_CLOSE(box0.get_half_size()[0], 5.75f, 0.001);
	BOOST_CHECK_CLOSE(box0.get_half_size()[1], 107.006f, 0.001);
	BOOST_CHECK_CLOSE(box0.get_half_size()[2], 0.117f, 0.001);
}

BOOST_AUTO_TEST_CASE(overlap)
{
	el::BoundingBox box0, box1(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box0.set_center(glm::vec3(23.0f, -1234.56f, -34.0f));
	box0.set_half_size(glm::vec3(34.5f, 214.012f, 0.234f));

	BOOST_CHECK_CLOSE(box0.overlap(box1), 1727.713f, 0.001);
	BOOST_CHECK_CLOSE(box1.overlap(box0), 1727.713f, 0.001);
}

BOOST_AUTO_TEST_CASE(contains)
{
	el::BoundingBox box0, box1(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box0.set_center(glm::vec3(23.0f, -1234.56f, -34.0f));
	box0.set_half_size(glm::vec3(34.5f, 214.012f, 0.234f));

	BOOST_CHECK(!box0.contains(box1));
	BOOST_CHECK(!box1.contains(box0));

	box0.set_center(glm::vec3(29.0f, -234.56f, -4.0f));
	box0.set_half_size(glm::vec3(4.5f, 214.012f, 0.234f));

	BOOST_CHECK(!box0.contains(box1));
	BOOST_CHECK(box1.contains(box0));
}

BOOST_AUTO_TEST_CASE(enlargement)
{
	el::BoundingBox box0, box1(glm::vec3(23.0f, -1234.56f, -34.0f),
		glm::vec3(34.5f, 214.012f, 0.234f));

	box0.set_center(glm::vec3(23.0f, -1234.56f, -34.0f));
	box0.set_half_size(glm::vec3(34.5f, 214.012f, 0.234f));

	BOOST_CHECK_CLOSE(box0.enlargement(box1), 3940288.75f, 0.001);
	BOOST_CHECK_CLOSE(box1.enlargement(box0), 3383820.75f, 0.001);
}
