/****************************************************************************
 *            renderobjectdata.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@gmail.com>
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
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data = el::RenderObjectData(object, el::all_bits_set,
		0.1f, el::bt_additive);

	BOOST_CHECK_EQUAL(render_object_data.get_object().use_count(), 0);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.1f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_additive);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data = el::RenderObjectData(object, 0x0, 0.7f,
		el::bt_alpha_transparency_source_value);

	BOOST_CHECK_EQUAL(render_object_data.get_object().use_count(), 0);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.7f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(), 0x0);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(),
		el::bt_alpha_transparency_source_value);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data = el::RenderObjectData(object, 0x0, 0.7f,
		el::bt_alpha_transparency_value);

	BOOST_CHECK_EQUAL(render_object_data.get_object().use_count(), 0);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.7f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(), 0x0);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(),
		el::bt_alpha_transparency_value);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);
}

BOOST_AUTO_TEST_CASE(render_object_data_transparency)
{
	el::RenderObjectData render_object_data;

	render_object_data.set_transparency(0.7f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.7f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_transparency(0.3f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.3f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_transparency(0.0f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_transparency(1.0f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 1.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);
}

BOOST_AUTO_TEST_CASE(render_object_data_distance)
{
	el::RenderObjectData render_object_data;

	render_object_data.set_distance(0.7f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.7f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_distance(0.3f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.3f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_distance(0.0f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_distance(1.0f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 1.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_distance(234576.4543f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 234576.4543f,
		0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_distance(45.5346f);
	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 45.5346f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);
}

BOOST_AUTO_TEST_CASE(render_object_data_blend)
{
	el::RenderObjectData render_object_data;

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_blend(el::bt_alpha_transparency_source_value);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(),
		el::bt_alpha_transparency_source_value);

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_blend(el::bt_alpha_transparency_value);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(),
		el::bt_alpha_transparency_value);

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_blend(el::bt_additive);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_additive);

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);

	render_object_data.set_blend(el::bt_disabled);
	BOOST_CHECK_EQUAL(render_object_data.get_blend(), el::bt_disabled);

	BOOST_CHECK_CLOSE(render_object_data.get_transparency(), 0.0f, 0.001);
	BOOST_CHECK_CLOSE(render_object_data.get_distance(), 0.0f, 0.001);
	BOOST_CHECK_EQUAL(render_object_data.get_visibility_mask(),
		el::all_bits_set);
	BOOST_CHECK_EQUAL(render_object_data.get_lod(), 0);
}
