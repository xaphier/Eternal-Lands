/****************************************************************************
 *            samplerparameter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
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

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_albedo_0).get(), "albedo_sampler_0");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_albedo_3).get(), "albedo_sampler_3");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_emission).get(), "emission_sampler");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_blend).get(), "blend_sampler");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_light_colors).get(), "light_colors_sampler");
	BOOST_CHECK_EQUAL(el::SamplerParameterUtil::get_str(
		el::spt_shadow).get(), "shadow_sampler");
}
