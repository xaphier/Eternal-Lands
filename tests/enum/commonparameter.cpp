/****************************************************************************
 *            commonparameter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "shader/commonparameterutil.hpp"
#define BOOST_TEST_MODULE common_parameter
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_common_parameter_count)
{
	BOOST_CHECK_GT(el::CommonParameterUtil::get_common_parameter_count(),
		0);
}

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i, count;

	count = el::CommonParameterUtil::get_common_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(el::CommonParameterUtil::get_str(
			static_cast<el::CommonParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_parameter_type)
{
	Uint32 i, count;
	el::ParameterType type;

	count = el::CommonParameterUtil::get_common_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(type = el::CommonParameterUtil::get_type(
			static_cast<el::CommonParameterType>(i)));

		BOOST_CHECK(!el::ParameterUtil::get_sampler(type));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i, count;
	el::CommonParameterType type;

	count = el::CommonParameterUtil::get_common_parameter_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::CommonParameterType>(i);

		BOOST_CHECK_EQUAL(
			el::CommonParameterUtil::get_common_parameter(
			el::CommonParameterUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_common_parameter)
{
	Uint32 i, count;
	el::CommonParameterType type, tmp;

	count = el::CommonParameterUtil::get_common_parameter_count();

	for (i = 0; i < count; i++)
	{
		type = static_cast<el::CommonParameterType>(i);

		BOOST_CHECK(el::CommonParameterUtil::get_common_parameter(
			el::CommonParameterUtil::get_str(type), tmp));

		BOOST_CHECK_EQUAL(type, tmp);
	}
}

BOOST_AUTO_TEST_CASE(get_type)
{
	Uint32 i, count;
	el::ParameterType type;

	count = el::CommonParameterUtil::get_common_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_NO_THROW(type = el::CommonParameterUtil::get_type(
			static_cast<el::CommonParameterType>(i)));

		BOOST_CHECK_NO_THROW(el::ParameterUtil::get_str(type));

		BOOST_CHECK(!el::ParameterUtil::get_sampler(type));
	}
}

BOOST_AUTO_TEST_CASE(get_scale)
{
	Uint32 i, count;

	count = el::CommonParameterUtil::get_common_parameter_count();

	for (i = 0; i < count; i++)
	{
		BOOST_CHECK_GT(el::CommonParameterUtil::get_scale(
			static_cast<el::CommonParameterType>(i)), 0);
	}
}

BOOST_AUTO_TEST_CASE(stream)
{
	Uint32 i, count;
	el::CommonParameterType type, tmp;

	count = el::CommonParameterUtil::get_common_parameter_count();

	for (i = 0; i < count; i++)
	{
		el::StringStream str;

		type = static_cast<el::CommonParameterType>(i);

		BOOST_CHECK_NO_THROW(str << type);

		BOOST_CHECK_EQUAL(str.str(),
			el::CommonParameterUtil::get_str(type).get());

		BOOST_CHECK_NO_THROW(str >> tmp);

		BOOST_CHECK_EQUAL(tmp, type);
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_str(
		el::cpt_diffuse_color).get(), "diffuse_color");

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_str(
		el::cpt_world_tangent).get(), "world_tangent");

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_str(
		el::cpt_shadow_map_data).get(), "shadow_map_data");

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_str(
		el::cpt_world_uv_ddx_ddy).get(), "world_uv_ddx_ddy");

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_str(
		el::cpt_fragment_color).get(), "fragment_color");

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(
		el::cpt_light_position), el::pt_vec4);

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(
			el::cpt_view_position), el::pt_vec3);

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(el::cpt_shadow),
		el::pt_float);

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(
		el::cpt_shadow_map_data), el::pt_vec3);

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(
		el::cpt_world_uv_ddx_ddy), el::pt_vec4);

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(
		el::cpt_fragment_color), el::pt_vec3);
}
