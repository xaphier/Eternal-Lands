/****************************************************************************
 *            effectsampler.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "effect/effectsamplerutil.hpp"
#define BOOST_TEST_MODULE effect_sampler
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_effect_sampler_count)
{
	BOOST_CHECK_GT(el::EffectSamplerUtil::get_effect_sampler_count(), 0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::EffectSamplerUtil::get_effect_sampler_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::EffectSamplerUtil::get_str(
			static_cast<el::EffectSamplerType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::EffectSamplerType type;

	count = el::EffectSamplerUtil::get_effect_sampler_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectSamplerType>(i);

		BOOST_CHECK_EQUAL(
			el::EffectSamplerUtil::get_effect_sampler(
			el::EffectSamplerUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_effect_sampler)
{
	Uint32 i, count;
	el::EffectSamplerType type, tmp;

	count = el::EffectSamplerUtil::get_effect_sampler_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::EffectSamplerType>(i);

		BOOST_CHECK(el::EffectSamplerUtil::get_effect_sampler(
			el::EffectSamplerUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::EffectSamplerType type, tmp;

	count = el::EffectSamplerUtil::get_effect_sampler_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::EffectSamplerType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::EffectSamplerUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::EffectSamplerUtil::get_str(
		el::est_sampler_1d).get(), "sampler_1d");
	BOOST_CHECK_EQUAL(el::EffectSamplerUtil::get_str(
		el::est_sampler_1d_array).get(), "sampler_1d_array");
	BOOST_CHECK_EQUAL(el::EffectSamplerUtil::get_str(
		el::est_sampler_1d_project).get(), "sampler_1d_project");
	BOOST_CHECK_EQUAL(el::EffectSamplerUtil::get_str(
		el::est_sampler_rectangle_project).get(),
		"sampler_rectangle_project");
}
