/****************************************************************************
 *            lightdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "lightdata.hpp"
#define BOOST_TEST_MODULE light_data
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::LightData light_data(glm::vec3(43.0f, 1234.56f, -34.0f),
		glm::vec3(34.5f, -214.012f, 0.234f), 10.9546f, 5435495);

	BOOST_CHECK_CLOSE(light_data.get_position()[0], 43.0f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_position()[1], 1234.56f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_position()[2], -34.0f, 0.001);

	BOOST_CHECK_CLOSE(light_data.get_color()[0], 34.5f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_color()[1], -214.012f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_color()[2], 0.234f, 0.001);

	BOOST_CHECK_CLOSE(light_data.get_radius(), 10.9546f, 0.001);

	BOOST_CHECK_EQUAL(light_data.get_id(), 5435495);
}

BOOST_AUTO_TEST_CASE(position)
{
	el::LightData light_data(glm::vec3(43.0f, 1234.56f, -34.0f),
		glm::vec3(34.5f, -214.012f, 0.234f), 10.9546f, 5435495);

	light_data.set_position(glm::vec3(1.0f, 2.0f, 3.0f));
	BOOST_CHECK_CLOSE(light_data.get_position()[0], 1.0f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_position()[1], 2.0f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_position()[2], 3.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(color)
{
	el::LightData light_data(glm::vec3(43.0f, 1234.56f, -34.0f),
		glm::vec3(34.5f, -214.012f, 0.234f), 10.9546f, 5435495);

	light_data.set_color(glm::vec3(1.0f, 2.0f, 3.0f));
	BOOST_CHECK_CLOSE(light_data.get_color()[0], 1.0f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_color()[1], 2.0f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_color()[2], 3.0f, 0.001);

	light_data.set_color(glm::vec3(341.0f, -2655.0f, 7893.0f));
	BOOST_CHECK_CLOSE(light_data.get_color()[0], 341.0f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_color()[1], -2655.0f, 0.001);
	BOOST_CHECK_CLOSE(light_data.get_color()[2], 7893.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(radius)
{
	el::LightData light_data(glm::vec3(43.0f, 1234.56f, -34.0f),
		glm::vec3(34.5f, -214.012f, 0.234f), 10.9546f, 5435495);

	light_data.set_radius(-897.0f);
	BOOST_CHECK_CLOSE(light_data.get_radius(), -897.0f, 0.001);

	light_data.set_radius(1345.0f);
	BOOST_CHECK_CLOSE(light_data.get_radius(), 1345.0f, 0.001);

	light_data.set_radius(0.0078f);
	BOOST_CHECK_CLOSE(light_data.get_radius(), 0.0078f, 0.001);

	light_data.set_radius(676897.0f);
	BOOST_CHECK_CLOSE(light_data.get_radius(), 676897.0f, 0.001);
}
