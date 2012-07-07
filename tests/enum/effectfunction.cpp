/****************************************************************************
 *            effectfunction.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "node/effectfunctionutil.hpp"
#define BOOST_TEST_MODULE effect_function
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_effect_function_count)
{
	BOOST_CHECK_GT(el::EffectFunctionUtil::get_effect_function_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::EffectFunctionUtil::get_effect_function_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::EffectFunctionUtil::get_str(
			static_cast<el::EffectFunctionType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::EffectFunctionType type;

	count = el::EffectFunctionUtil::get_effect_function_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectFunctionType>(i);

		BOOST_CHECK_EQUAL(
			el::EffectFunctionUtil::get_effect_function(
			el::EffectFunctionUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_effect_function)
{
	Uint32 i, count;
	el::EffectFunctionType type, tmp;

	count = el::EffectFunctionUtil::get_effect_function_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectFunctionType>(i);

		BOOST_CHECK(el::EffectFunctionUtil::get_effect_function(
			el::EffectFunctionUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::EffectFunctionType type, tmp;

	count = el::EffectFunctionUtil::get_effect_function_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::EffectFunctionType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::EffectFunctionUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::EffectFunctionUtil::get_str(
		el::ent_add).get(), "add");
	BOOST_CHECK_EQUAL(el::EffectFunctionUtil::get_str(
		el::ent_atanh).get(), "atanh");
	BOOST_CHECK_EQUAL(el::EffectFunctionUtil::get_str(
		el::ent_fract).get(), "fract");
	BOOST_CHECK_EQUAL(el::EffectFunctionUtil::get_str(
		el::ent_refract).get(), "refract");
}
