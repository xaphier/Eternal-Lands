/****************************************************************************
 *            effectparameter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "effect/effectparameterutil.hpp"
#define BOOST_TEST_MODULE effect_parameter
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_effect_parameter_count)
{
	BOOST_CHECK_GT(el::EffectParameterUtil::get_effect_parameter_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::EffectParameterUtil::get_effect_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::EffectParameterUtil::get_str(
			static_cast<el::EffectParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::EffectParameterType type;

	count = el::EffectParameterUtil::get_effect_parameter_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectParameterType>(i);

		BOOST_CHECK_EQUAL(
			el::EffectParameterUtil::get_effect_parameter(
			el::EffectParameterUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_effect_parameter)
{
	Uint32 i, count;
	el::EffectParameterType type, tmp;

	count = el::EffectParameterUtil::get_effect_parameter_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectParameterType>(i);

		BOOST_CHECK(el::EffectParameterUtil::get_effect_parameter(
			el::EffectParameterUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::EffectParameterType type, tmp;

	count = el::EffectParameterUtil::get_effect_parameter_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::EffectParameterType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::EffectParameterUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::EffectParameterUtil::get_str(
		el::ept_position).get(), "position");
	BOOST_CHECK_EQUAL(el::EffectParameterUtil::get_str(
		el::ept_tangent).get(), "tangent");
	BOOST_CHECK_EQUAL(el::EffectParameterUtil::get_str(
		el::ept_uv).get(), "uv");
	BOOST_CHECK_EQUAL(el::EffectParameterUtil::get_str(
		el::ept_time).get(), "time");
}
