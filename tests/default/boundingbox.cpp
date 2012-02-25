/****************************************************************************
 *            boundingbox.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "boundingbox.hpp"
#include <glm/gtx/epsilon.hpp>
#include <boost/random.hpp>
#include <boost/exception/diagnostic_information.hpp>
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
