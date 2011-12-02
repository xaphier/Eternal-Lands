/****************************************************************************
 *            commonparameter.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "prerequisites.hpp"
#include "exceptions.hpp"
#include "shader/commonparameterutil.hpp"
#include "utf.hpp"
#include <glm/gtx/epsilon.hpp>
#include <boost/random.hpp>
#include <boost/exception/diagnostic_information.hpp>
#define BOOST_TEST_MODULE pack
#include <boost/test/unit_test.hpp>

namespace el = eternal_lands;

BOOST_AUTO_TEST_CASE(get_str)
{
	Uint32 i;

	for (i = 0; i < el::CommonParameterUtil::get_common_parameter_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::CommonParameterUtil::get_str(
			static_cast<el::CommonParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(convert)
{
	Uint32 i;
	el::CommonParameterType type;

	for (i = 0; i < el::CommonParameterUtil::get_common_parameter_count(); i++)
	{
		type = static_cast<el::CommonParameterType>(i);

		BOOST_CHECK_EQUAL(
			el::CommonParameterUtil::get_common_parameter(
			el::CommonParameterUtil::get_str(type)), type);
	}
}

BOOST_AUTO_TEST_CASE(get_type)
{
	Uint32 i;

	for (i = 0; i < el::CommonParameterUtil::get_common_parameter_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::CommonParameterUtil::get_type(
			static_cast<el::CommonParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_size)
{
	Uint32 i;

	for (i = 0; i < el::CommonParameterUtil::get_common_parameter_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::CommonParameterUtil::get_size(
			static_cast<el::CommonParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_scale)
{
	Uint32 i;

	for (i = 0; i < el::CommonParameterUtil::get_common_parameter_count(); i++)
	{
		BOOST_CHECK_NO_THROW(el::CommonParameterUtil::get_scale(
			static_cast<el::CommonParameterType>(i)));
	}
}

BOOST_AUTO_TEST_CASE(get_str_value)
{
	BOOST_CHECK_EQUAL(el::string_to_utf8(el::CommonParameterUtil::get_str(
		el::cpt_diffuse_color)),
		"diffuse_color");

	BOOST_CHECK_EQUAL(el::string_to_utf8(el::CommonParameterUtil::get_str(
		el::cpt_world_tangent)), "world_tangent");

	BOOST_CHECK_EQUAL(el::string_to_utf8(el::CommonParameterUtil::get_str(
		el::cpt_shadow_map_data)), "shadow_map_data");

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(
		el::cpt_light_position), el::pt_vec4);

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(
			el::cpt_view_position), el::pt_vec3);

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(el::cpt_shadow),
		el::pt_float);

	BOOST_CHECK_EQUAL(el::CommonParameterUtil::get_type(
		el::cpt_shadow_map_data), el::pt_vec3);
}
