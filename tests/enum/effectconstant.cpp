/****************************************************************************
 *            effectconstant.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "node/effectconstantutil.hpp"
#define BOOST_TEST_MODULE effect_constant
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_effect_constant_count)
{
	BOOST_CHECK_GT(el::EffectConstantUtil::get_effect_constant_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::EffectConstantUtil::get_effect_constant_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::EffectConstantUtil::get_str(
			static_cast<el::EffectConstantType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_count)
{
	Uint32 i, count;
	Uint16 value;

	count = el::EffectConstantUtil::get_effect_constant_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(value = el::EffectConstantUtil::get_count(
			static_cast<el::EffectConstantType>(i)));

		BOOST_CHECK_LE(value, 4);
		BOOST_CHECK_GT(value, 0);
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::EffectConstantType type;

	count = el::EffectConstantUtil::get_effect_constant_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectConstantType>(i);

		BOOST_CHECK_EQUAL(
			el::EffectConstantUtil::get_effect_constant(
			el::EffectConstantUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_effect_constant)
{
	Uint32 i, count;
	el::EffectConstantType type, tmp;

	count = el::EffectConstantUtil::get_effect_constant_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectConstantType>(i);

		BOOST_CHECK(el::EffectConstantUtil::get_effect_constant(
			el::EffectConstantUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::EffectConstantType type, tmp;

	count = el::EffectConstantUtil::get_effect_constant_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::EffectConstantType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::EffectConstantUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::EffectConstantUtil::get_str(
		el::ect_color_rgb).get(), "color_rgb");
	BOOST_CHECK_EQUAL(el::EffectConstantUtil::get_str(
		el::ect_direction_xyz).get(), "direction_xyz");
	BOOST_CHECK_EQUAL(el::EffectConstantUtil::get_str(
		el::ect_float).get(), "float");
	BOOST_CHECK_EQUAL(el::EffectConstantUtil::get_str(
		el::ect_vec4).get(), "vec4");

	BOOST_CHECK_EQUAL(el::EffectConstantUtil::get_count(
		el::ect_color_rgba), 4);
	BOOST_CHECK_EQUAL(el::EffectConstantUtil::get_count(
		el::ect_direction_xy), 2);
	BOOST_CHECK_EQUAL(el::EffectConstantUtil::get_count(el::ect_float), 1);
	BOOST_CHECK_EQUAL(el::EffectConstantUtil::get_count(el::ect_vec3), 3);
}
