/****************************************************************************
 *            samplerparameter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@gmail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/samplerparameterutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE sampler_parameter
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_shader_texture_count)
{
	BOOST_CHECK_GT(el::SamplerParameterUtil::get_sampler_parameter_count(),
		0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::SamplerParameterUtil::get_sampler_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::SamplerParameterUtil::get_str(
			static_cast<el::SamplerParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::SamplerParameterType type;

	count = el::SamplerParameterUtil::get_sampler_parameter_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::SamplerParameterType>(i);

		BOOST_CHECK_EQUAL(
			el::SamplerParameterUtil::get_sampler_parameter(
				el::SamplerParameterUtil::get_str(type)),
			type);
	}
}

BOOST_AUTO_TEST_CASE(get_parameter_qualifier)
{
	Uint32 i, count;
	el::SamplerParameterType type, tmp;

	count = el::SamplerParameterUtil::get_sampler_parameter_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::SamplerParameterType>(i);

		BOOST_CHECK(el::SamplerParameterUtil::get_sampler_parameter(
			el::SamplerParameterUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::SamplerParameterType type, tmp;

	count = el::SamplerParameterUtil::get_sampler_parameter_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::SamplerParameterType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::SamplerParameterUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_effect_0).get(), "effect_sampler_0");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_effect_4).get(), "effect_sampler_4");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_effect_8).get(), "effect_sampler_8");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_effect_11).get(), "effect_sampler_11");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_effect_13).get(), "effect_sampler_13");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_effect_15).get(), "effect_sampler_15");
}
