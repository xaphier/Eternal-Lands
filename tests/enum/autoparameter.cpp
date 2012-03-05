/****************************************************************************
 *            autoparameter.cpp
 *
 * Author: 2010-2012  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/autoparameterutil.hpp"
#include "utf.hpp"
#define BOOST_TEST_MODULE auto_parameter
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i;

	for (i = 0; i < el::AutoParameterUtil::get_auto_parameter_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::AutoParameterUtil::get_str(
			static_cast<el::AutoParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i;
	el::AutoParameterType type;

	for (i = 0; i < el::AutoParameterUtil::get_auto_parameter_count(); i++)
	{
		type = static_cast<el::AutoParameterType>(i);

		BOOST_CHECK_EQUAL(
			el::AutoParameterUtil::get_auto_parameter(
			el::AutoParameterUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_type)
{
	Uint32 i;

	for (i = 0; i < el::AutoParameterUtil::get_auto_parameter_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::AutoParameterUtil::get_type(
			static_cast<el::AutoParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_size)
{
	Uint32 i;

	for (i = 0; i < el::AutoParameterUtil::get_auto_parameter_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::AutoParameterUtil::get_size(
			static_cast<el::AutoParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_scale)
{
	Uint32 i;

	for (i = 0; i < el::AutoParameterUtil::get_auto_parameter_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::AutoParameterUtil::get_scale(
			static_cast<el::AutoParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_str(
		el::apt_world_transformation).get(), "world_transformation");

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_str(
		el::apt_view_matrix).get(), "view_matrix");

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_str(
		el::apt_projection_matrix).get(), "projection_matrix");

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_str(
		el::apt_projection_view_matrix).get(),
		"projection_view_matrix");

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_str(
		el::apt_reflection_matrix).get(), "reflection_matrix");

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_str(
		el::apt_shadow_texture_matrix).get(), "shadow_texture_matrix");

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_str(
		el::apt_light_colors).get(), "light_colors");

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_str(
		el::apt_split_distances).get(), "split_distances");

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_type(el::apt_view_matrix),
		el::pt_mat4x4);

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_type(el::apt_ambient),
		el::pt_vec4);

	BOOST_CHECK_EQUAL(el::AutoParameterUtil::get_type(
		el::apt_shadow_camera), el::pt_vec4);
}
