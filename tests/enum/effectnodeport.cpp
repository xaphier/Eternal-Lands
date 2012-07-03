/****************************************************************************
 *            effectnodeport.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "node/effectnodeportutil.hpp"
#define BOOST_TEST_MODULE effect_node_port
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_effect_node_port_count)
{
	BOOST_CHECK_GT(el::EffectNodePortUtil::get_effect_node_port_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::EffectNodePortUtil::get_effect_node_port_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::EffectNodePortUtil::get_str(
			static_cast<el::EffectNodePortType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_description)
{
	Uint32 i, count;

	count = el::EffectNodePortUtil::get_effect_node_port_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::EffectNodePortUtil::get_description(
			static_cast<el::EffectNodePortType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_count)
{
	Uint32 i, count;
	Uint16 value;

	count = el::EffectNodePortUtil::get_effect_node_port_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(value = el::EffectNodePortUtil::get_count(
			static_cast<el::EffectNodePortType>(i)));

		BOOST_CHECK_LE(value, 4);
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::EffectNodePortType type;

	count = el::EffectNodePortUtil::get_effect_node_port_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectNodePortType>(i);

		BOOST_CHECK_EQUAL(
			el::EffectNodePortUtil::get_effect_node_port(
			el::EffectNodePortUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_effect_node_port)
{
	Uint32 i, count;
	el::EffectNodePortType type, tmp;

	count = el::EffectNodePortUtil::get_effect_node_port_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectNodePortType>(i);

		BOOST_CHECK(el::EffectNodePortUtil::get_effect_node_port(
			el::EffectNodePortUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_str(
		el::enpt_r).get(), "r");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_str(
		el::enpt_y).get(), "y");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_str(
		el::enpt_rg).get(), "rg");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_str(
		el::enpt_xyz).get(), "xyz");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_str(
		el::enpt_rgba).get(), "rgba");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_str(
		el::enpt_single).get(), "single");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_str(
		el::enpt_general).get(), "general");

	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_count(
		el::enpt_r), 1);
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_count(
		el::enpt_a), 1);
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_count(
		el::enpt_xy), 2);
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_count(
		el::enpt_rgb), 3);
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_count(
		el::enpt_xyzw), 4);
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_count(
		el::enpt_single), 1);
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_count(
		el::enpt_general), 0);

	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_description(
		el::enpt_r).get(), "r");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_description(
		el::enpt_y).get(), "y");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_description(
		el::enpt_rg).get(), "rg");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_description(
		el::enpt_xyz).get(), "xyz");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_description(
		el::enpt_rgba).get(), "rgba");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_description(
		el::enpt_single).get(), "");
	BOOST_CHECK_EQUAL(el::EffectNodePortUtil::get_description(
		el::enpt_general).get(), "");
}
