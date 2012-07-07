/****************************************************************************
 *            effectchange.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "node/effectchangeutil.hpp"
#define BOOST_TEST_MODULE effect_change
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_effect_change_count)
{
	BOOST_CHECK_GT(el::EffectChangeUtil::get_effect_change_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::EffectChangeUtil::get_effect_change_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::EffectChangeUtil::get_str(
			static_cast<el::EffectChangeType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::EffectChangeType type;

	count = el::EffectChangeUtil::get_effect_change_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectChangeType>(i);

		BOOST_CHECK_EQUAL(
			el::EffectChangeUtil::get_effect_change(
			el::EffectChangeUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_effect_change)
{
	Uint32 i, count;
	el::EffectChangeType type, tmp;

	count = el::EffectChangeUtil::get_effect_change_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectChangeType>(i);

		BOOST_CHECK(el::EffectChangeUtil::get_effect_change(
			el::EffectChangeUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::EffectChangeType type, tmp;

	count = el::EffectChangeUtil::get_effect_change_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::EffectChangeType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::EffectChangeUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::EffectChangeUtil::get_str(
		el::ect_undefined).get(), "undefined");
	BOOST_CHECK_EQUAL(el::EffectChangeUtil::get_str(
		el::ect_constant).get(), "constant");
	BOOST_CHECK_EQUAL(el::EffectChangeUtil::get_str(
		el::ect_vertex).get(), "vertex");
	BOOST_CHECK_EQUAL(el::EffectChangeUtil::get_str(
		el::ect_fragment).get(), "fragment");
}
