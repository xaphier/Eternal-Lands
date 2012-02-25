/****************************************************************************
 *            objectdata.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "objectdata.hpp"
#include "utf.hpp"
#include <glm/gtx/epsilon.hpp>
#include <boost/random.hpp>
#define BOOST_TEST_MODULE object_data
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(default_creation)
{
	el::ObjectData object_data(el::Transformation(),
		el::String(UTF8("3sfd23")), 0.54f, 543549564, el::st_player,
		true, true, 244);

	BOOST_CHECK_EQUAL(UTF8(object_data.get_name()), UTF8("3sfd23"));

	BOOST_CHECK_CLOSE(object_data.get_transparency(), 0.54f, 0.001);

	BOOST_CHECK_EQUAL(object_data.get_id(), 543549564);

	BOOST_CHECK_EQUAL(object_data.get_selection(), el::st_player);

	BOOST_CHECK_EQUAL(object_data.get_state_blend(), true);

	BOOST_CHECK_EQUAL(object_data.get_state_stencil(), true);

	BOOST_CHECK_EQUAL(object_data.get_stencil_value(), 244);
}

BOOST_AUTO_TEST_CASE(name)
{
	el::ObjectData object_data;

	object_data.set_name(el::String(UTF8("object")));
	BOOST_CHECK_EQUAL(object_data.get_name(), UTF8("object"));

	object_data.set_name(el::String(UTF8("sdgstgr345")));
	BOOST_CHECK_EQUAL(object_data.get_name(), UTF8("sdgstgr345"));

	object_data.set_name(el::String(UTF8("1246546")));
	BOOST_CHECK_EQUAL(object_data.get_name(), UTF8("1246546"));
}

BOOST_AUTO_TEST_CASE(transparency)
{
	el::ObjectData object_data;

	object_data.set_transparency(1.0f);
	BOOST_CHECK_CLOSE(object_data.get_transparency(), 1.0f, 0.001);

	object_data.set_transparency(0.75f);
	BOOST_CHECK_CLOSE(object_data.get_transparency(), 0.75f, 0.001);

	object_data.set_transparency(0.5f);
	BOOST_CHECK_CLOSE(object_data.get_transparency(), 0.5f, 0.001);

	object_data.set_transparency(0.25f);
	BOOST_CHECK_CLOSE(object_data.get_transparency(), 0.25f, 0.001);

	object_data.set_transparency(0.0f);
	BOOST_CHECK_CLOSE(object_data.get_transparency(), 0.0f, 0.001);
}

BOOST_AUTO_TEST_CASE(state_blend)
{
	el::ObjectData object_data;

	object_data.set_state_blend(true);
	BOOST_CHECK_EQUAL(object_data.get_state_blend(), true);

	object_data.set_state_blend(false);
	BOOST_CHECK_EQUAL(object_data.get_state_blend(), false);
}

BOOST_AUTO_TEST_CASE(state_stencil)
{
	el::ObjectData object_data;

	object_data.set_state_stencil(true);
	BOOST_CHECK_EQUAL(object_data.get_state_stencil(), true);

	object_data.set_state_stencil(false);
	BOOST_CHECK_EQUAL(object_data.get_state_stencil(), false);
}

BOOST_AUTO_TEST_CASE(selection)
{
	el::ObjectData object_data;

	object_data.set_selection(el::st_none);
	BOOST_CHECK_EQUAL(object_data.get_selection(), el::st_none);

	object_data.set_selection(el::st_select);
	BOOST_CHECK_EQUAL(object_data.get_selection(), el::st_select);

	object_data.set_selection(el::st_harvest);
	BOOST_CHECK_EQUAL(object_data.get_selection(), el::st_harvest);

	object_data.set_selection(el::st_pick);
	BOOST_CHECK_EQUAL(object_data.get_selection(), el::st_pick);

	object_data.set_selection(el::st_enter);
	BOOST_CHECK_EQUAL(object_data.get_selection(), el::st_enter);

	object_data.set_selection(el::st_npc);
	BOOST_CHECK_EQUAL(object_data.get_selection(), el::st_npc);

	object_data.set_selection(el::st_player);
	BOOST_CHECK_EQUAL(object_data.get_selection(), el::st_player);

	object_data.set_selection(el::st_animal);
	BOOST_CHECK_EQUAL(object_data.get_selection(), el::st_animal);
}

BOOST_AUTO_TEST_CASE(id)
{
	el::ObjectData object_data;

	object_data.set_id(8977);
	BOOST_CHECK_EQUAL(object_data.get_id(), 8977);

	object_data.set_id(4566067);
	BOOST_CHECK_EQUAL(object_data.get_id(), 4566067);
}
