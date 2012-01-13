/****************************************************************************
 *            renderobjectdata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "renderobjectdata.hpp"
#include <boost/random.hpp>
#define BOOST_TEST_MODULE render_object_data
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(render_object_data_creation)
{
	el::RenderObjectData render_object_data;
	el::ObjectSharedPtr object;

	BOOST_CHECK_EQUAL(render_object_data.get_object().use_count(), 0);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x1);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data = el::RenderObjectData(object, 0.1f, true);

	BOOST_CHECK_EQUAL(render_object_data.get_object().use_count(), 0);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.1f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x1);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), true);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data = el::RenderObjectData(object, 0.7f, true, 0x0);

	BOOST_CHECK_EQUAL(render_object_data.get_object().use_count(), 0);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.7f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x0);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), true);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);
}

BOOST_AUTO_TEST_CASE(render_object_data_transparency)
{
	el::RenderObjectData render_object_data;

	render_object_data.set_transparency(0.7f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.7f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_transparency(0.3f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.3f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_transparency(0.0f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_transparency(1.0f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 1.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);
}

BOOST_AUTO_TEST_CASE(render_object_data_distance)
{
	el::RenderObjectData render_object_data;

	render_object_data.set_distance(0.7f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.7f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_distance(0.3f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.3f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_distance(0.0f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_distance(1.0f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 1.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_distance(234576.4543f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 234576.4543f,
		0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_distance(45.5346f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 45.5346f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);
}

BOOST_AUTO_TEST_CASE(render_object_data_blend)
{
	el::RenderObjectData render_object_data;

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_blend(true);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), true);

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	render_object_data.set_blend(false);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);
}

BOOST_AUTO_TEST_CASE(render_object_data_sub_frustums_mask)
{
	el::RenderObjectData render_object_data;
	el::Uint16 i;

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	for (i = 0; i < 16; ++i)
	{
		render_object_data.set_sub_frustums_mask(i);
		BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(),
			i);
	}
}

BOOST_AUTO_TEST_CASE(render_object_data_layers)
{
	el::RenderObjectData render_object_data;
	el::Uint16 i;

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_sub_frustums_mask(), 0x01);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);

	for (i = 0; i < 4; ++i)
	{
		render_object_data.set_sub_frustums_mask(1 << i);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], i);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 0);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);
		BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f,
			0.001);
		BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);

		render_object_data.set_sub_frustums_mask(0xF - (1 << i));
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[0],
			i <= 0 ? 1 : 0);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[1],
			i <= 1 ? 2 : 1);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[2],
			i <= 2 ? 3 : 2);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);
		BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f,
			0.001);
		BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	}

	for (i = 0; i < 3; ++i)
	{
		render_object_data.set_sub_frustums_mask(3 << i);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], i);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], i + 1);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);
		BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f,
			0.001);
		BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	}

	for (i = 0; i < 2; ++i)
	{
		render_object_data.set_sub_frustums_mask(7 << i);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], i);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], i + 1);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], i + 2);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);
		BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f,
			0.001);
		BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);

		render_object_data.set_sub_frustums_mask(5 << i);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], i);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], i + 2);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 0);
		BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 0);
		BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f,
			0.001);
		BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f,
			0.001);
		BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
	}

	render_object_data.set_sub_frustums_mask(0xF);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[0], 0);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[1], 1);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[2], 2);
	BOOST_CHECK_EQUAL(render_object_data.get_layers()[3], 3);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), false);
}
